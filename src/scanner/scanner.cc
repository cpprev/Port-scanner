#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <memory>
#include <thread>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "scanner.hh"
#include "target.hh"
#include "utils/utils.hh"

#define CONNECT_TIMEOUT 3

namespace scanner
{
    Scanner::Scanner(const std::string& in)
    {
        size_t len = in.size();
        std::string key, value;
        for (size_t i = 0; i < len; ++i)
        {
            char c = in[i];
            if (c == ':')
            {
                value = utils::CopyToNextComma(in, ++i);
                AddCustomAttribute(key, value);

                key.clear();
                value.clear();
            }
            else
            {
                if (c != '"' and c != '{' and c != '}' and c != '[' and c != ']')
                    key += c;
            }
        }
    }

    void Scanner::ParseOptions(const std::string& in)
    {
        size_t len = in.size();
        std::string key, value;
        for (size_t i = 0; i < len; ++i)
        {
            char c = in[i];
            if (c == ':')
            {
                value = utils::CopyToNextComma(in, ++i);
                AddCustomOption(key, utils::RemoveQuotes(value));

                key.clear();
                value.clear();
            }
            else
            {
                if (c != '"' and c != '{' and c != '}' and c != '[' and c != ']')
                    key += c;
            }
        }
    }

    void Scanner::AddCustomAttribute(const std::string& key, const std::string& value)
    {
        if (key == "Targets")
        {
            _targets = Target::ParseTargets(value);
        }
        else if (key == "Options")
        {
            ParseOptions(value);
        }
        else
        {
            throw std::runtime_error("Unknown Scanner class attribute \'" + key + "\'");
        }
    }

    void Scanner::AddCustomOption(const std::string& key, const std::string& value)
    {
        if (key == "Verbose")
        {
            _verbose = value == "true";
        }
        else
        {
            throw std::runtime_error("Unknown Scanner class option \'" + key + "\'");
        }
    }

    void Scan(std::shared_ptr<Target> target, int port)
    {
        std::string ip = target->GetHost();
        struct sockaddr_in address;
        fd_set fdset;
        struct timeval tv;

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(ip.c_str());
        address.sin_port = htons(port);

        int sock = socket(AF_INET, SOCK_STREAM, 0);
        fcntl(sock, F_SETFL, O_NONBLOCK);

        connect(sock, (struct sockaddr *) &address, sizeof(address));

        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);
        tv.tv_sec = CONNECT_TIMEOUT;
        target->SetResults({port, PORT_STATE::FILTERED});

        if (select(sock + 1, nullptr, &fdset, nullptr, &tv) == 1)
        {
            int so_error;
            socklen_t len = sizeof so_error;

            getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);

            if (so_error == 0)
                target->SetResults({port, PORT_STATE::OPENED});
            else
                target->SetResults({port, PORT_STATE::CLOSED});
        }

        close(sock);
    }

    void Scanner::ScanGlobal()
    {
        /// Incase input host is in format : hostname.com, we need to retreive the ip
        for (size_t i = 0; i < _targets.size(); ++i)
        {
            _targets[i]->SetHost(utils::GetIpAddressFromHostname(_targets[i]->GetHost()));
        }

        ScanGlobalMultithread();
    }

    void Scanner::ScanGlobalMultithread()
    {
        for (const auto& target : _targets)
        {
            std::vector<std::shared_ptr<std::thread>> tasks;

            size_t rangeEnd = target->GetRangeEnd();
            size_t iterator = target->GetRangeStart();
            size_t ref = target->GetRangeStart();
            const size_t add = 1000;
            for (; iterator <= ref + add; ++iterator)
            {
                if (iterator == ref + add or iterator > rangeEnd)
                {
                    for (auto& task : tasks)
                    {
                        task->join();
                    }
                    tasks.clear();
                    ref = iterator;

                    if (iterator >= rangeEnd)
                        break;
                }
                else
                {
                    tasks.emplace_back(std::make_shared<std::thread>(Scan, target, iterator));
                }
            }
        }
    }

    void Scanner::ScanGlobalNonMultithread()
    {
        for (const auto& target : _targets)
        {
            for (size_t port = target->GetRangeStart(); port <= target->GetRangeEnd(); ++port)
            {
                Scan(target, port);
            }
        }
    }

    void Scanner::PrintSummary()
    {
        std::string summary;
        for (const auto& target : GetTargets())
        {
            int countOpened = 0, countClosed = 0, countFiltered = 0;
            for (const auto& result : target->GetResults())
            {
                auto state = result.second;

                scanner::IncrementSummary(state, countOpened, countClosed, countFiltered);
                auto stateString = scanner::StateToString(state);
                std::string color = scanner::StateToColor(state);
                if (state == scanner::OPENED)
                {
                    std::cout << color << "Port \033[1;33m" << result.first << color << " is " << stateString
                              << " on : \033[1;33m" << target->GetHost() << "\033[0m" << "\n";
                }
            }

            summary += "\033[1;34m\n[Summary for host : \033[1;33m" + target->GetHost() + "\033[1;34m]\n";
            summary += "Number of ports opened :\t\033[1;33m" + std::to_string(countOpened) + "\033[1;34m\n";
            summary += "Number of ports closed :\t\033[1;33m" + std::to_string(countClosed) + "\033[1;34m\n";
            summary += "Number of ports filtered :\t\033[1;33m" + std::to_string(countFiltered) + "\033[0m" + "\n";
        }
        std::cout << summary;
    }

    void Scanner::PrettyPrint()
    {
        std::cout << "\033[1;35m";
        int i = 1;
        for (const auto& target : _targets)
        {
            std::cout << "Target \033[1;33m[" + std::to_string(i++) + "]\033[1;35m\n";
            std::cout << "Host : " << target->GetHost() << " (with Ip : \033[1;33m" << utils::GetIpAddressFromHostname(target->GetHost()) << "\033[1;35m)" << "\n";
            std::cout << "Port Range : \033[1;33m" << target->GetRangeStart() << "\033[1;35m to \033[1;33m" << target->GetRangeEnd() << "\033[1;35m\n";
            std::cout << "\n";
        }

        std::cout << utils::PrettyPrintOption("Verbose", _verbose) << "\n";

        std::cout << "\033[1;39m" << "______________________________________________" << "\033[0m" << "\n\n";
    }
}
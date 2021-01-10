#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <memory>
#include <thread>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <future>
#include <functional>

#include "scanner.hh"
#include "target.hh"
#include "options.hh"
#include "parsing/input_parsing.hh"
#include "utils/utils.hh"
#include "utils/globals.hh"
#include "utils/get_service_for_port.hh"

namespace scanner
{
    Scanner::Scanner(const std::string& in)
    {
        *this = parsing::ParseScanner(in);
    }

    std::pair<int, PORT_STATE> Scan(const std::string& ip, int port, bool ipv6)
    {
        fd_set fdset;
        struct timeval tv;

        int sock = -1;
        if (not ipv6)
        {
            struct sockaddr_in address;
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = inet_addr(ip.c_str());
            address.sin_port = htons(port);

            sock = socket(AF_INET, SOCK_STREAM, 0);

            fcntl(sock, F_SETFL, O_NONBLOCK);

            connect(sock, (struct sockaddr *) &address, sizeof(address));
        }
        else
        {
            /// TODO Ipv6 (will have to switch to getaddrinfo to support ipv6)
        }

        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);
        tv.tv_sec = g_Scanner->GetOptions().GetTimeout();
        tv.tv_usec = 0;

        std::pair<int, PORT_STATE> res = {};
        if (select(sock + 1, nullptr, &fdset, nullptr, &tv) == 1)
        {
            int so_error;
            socklen_t len = sizeof so_error;

            getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);

            if (so_error == 0)
            {
                res = {port, PORT_STATE::OPENED};
            }
            else
            {
                res = {port, PORT_STATE::CLOSED};
            }
        }
        else
        {
            res = {port, PORT_STATE::FILTERED};
        }

        close(sock);
        return res;
    }

    void Scanner::ScanGlobal()
    {
        /// Incase input host is in format : hostname.com, we need to retreive the ip
        for (size_t i = 0; i < _targets.size(); ++i)
        {
            _targets[i]->SetHost(utils::GetIpAddressFromHostname(_targets[i]->GetHost(), _targets[i]->IsIpv6()));
        }

        if (g_Scanner->GetOptions().IsMultithreadingEnabled())
            ScanGlobalMultithread();
        else
            ScanGlobalNonMultithread();
    }

    void Scanner::ScanGlobalMultithread()
    {
        for (const auto& target : _targets)
        {
            std::vector<std::shared_ptr<std::future<std::pair<int, PORT_STATE>>>> future;

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
                    iterator--;

                    if (iterator >= rangeEnd)
                        break;
                }
                else
                {
                    std::packaged_task<std::pair<int, PORT_STATE>(const std::string&, int, bool)> task(Scan);
                    future.emplace_back(std::make_shared<std::future<std::pair<int, PORT_STATE>>>(task.get_future()));
                    auto thread = std::make_shared<std::thread>(std::move(task), target->GetHost(), iterator, target->IsIpv6());
                    tasks.emplace_back(thread);
                }
            }
            int op = 0, cl = 0, fi = 0;
            std::vector<int> openedPorts;
            for (const auto& e : future)
            {
                auto pair = e->get();
                if (pair.second == OPENED)
                {
                    op++;
                    openedPorts.emplace_back(pair.first);
                }
                if (pair.second == CLOSED)
                {
                    cl++;
                }
                if (pair.second == FILTERED)
                {
                    fi++;
                }
            }
            target->SetResults(openedPorts, op, cl, fi);
        }
    }

    void Scanner::ScanGlobalNonMultithread()
    {
        for (const auto& target : _targets)
        {
            for (size_t port = target->GetRangeStart(); port <= target->GetRangeEnd(); ++port)
            {
                Scan(target->GetHost(), port, target->IsIpv6());
            }
        }
    }

    void Scanner::PrintSummary()
    {
        std::string summary;
        for (const auto& target : GetTargets())
        {
            auto results = target->GetResults();

            auto color = scanner::StateToColor(OPENED);
            auto stateString = scanner::StateToString(OPENED);
            for (const auto& port : results.GetOpenedPorts())
            {
                std::cout << color << "Port \033[1;33m" << port << color << " is " << stateString
                          << " on : \033[1;33m" << target->GetHost() << color << " (service running : " << "\033[1;33m" << utils::GetServiceForPort(port) << color << ")" << "\033[0m" << "\n";
            }

            summary += "\033[1;34m\n[Summary for host : \033[1;33m" + target->GetHost() + "\033[1;34m]\n";
            summary += "Number of ports opened :\t\033[1;33m" + std::to_string(results.GetCountOpened()) + "\033[1;34m\n";
            summary += "Number of ports closed :\t\033[1;33m" + std::to_string(results.GetCountClosed()) + "\033[1;34m\n";
            summary += "Number of ports filtered :\t\033[1;33m" + std::to_string(results.GetCountFiltered()) + "\033[0m" + "\n";
        }
        std::cout << summary;
    }

    void Scanner::PrettyPrint()
    {
        std::string summary;

        summary += "\033[1;35m";
        int i = 1;
        for (const auto& target : _targets)
        {
            summary += "Target \033[1;33m[" + std::to_string(i++) + "]\033[1;35m\n";
            summary += "Host : " + target->GetHost() + " (with Ip : \033[1;33m" + utils::GetIpAddressFromHostname(target->GetHost(), target->IsIpv6()) + "\033[1;35m)" + (target->IsIpv6() ? " (ipv6)" : "") + "\n";
            summary += "Port Range : \033[1;33m" + std::to_string(target->GetRangeStart()) + "\033[1;35m to \033[1;33m" + std::to_string(target->GetRangeEnd()) + "\033[1;35m\n";
            summary += "\n";
        }

        summary += "\033[1;35mScan timeout (in seconds) : \033[1;33m" + std::to_string(_options.GetTimeout()) + "\n";
        summary += "\033[1;35m" + utils::PrettyPrintOption("Multithreading", _options.IsMultithreadingEnabled()) + "\n";
        summary += "\033[1;35m" + utils::PrettyPrintOption("Verbose", _options.IsVerboseEnabled()) + "\n";

        summary += "\033[1;39m______________________________________________\033[0m\n\n";

        std::cout << summary;
    }
}
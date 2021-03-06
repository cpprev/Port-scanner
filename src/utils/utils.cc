#include <stdexcept>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <iostream>
#include <algorithm>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "utils.hh"

namespace utils
{
    void ThrowsIfTrue(bool condition, const std::string& errorMessage)
    {
        if (condition)
            throw std::runtime_error(errorMessage);
    }

    bool IsFile(const std::string& filePath)
    {
        struct stat st;
        if (stat(filePath.c_str(), &st) != 0)
            return false;
        return (st.st_mode & S_IFREG) != 0;
    }

    std::string ReadFile(const std::string& filePath)
    {
        std::ifstream ifs(filePath);
        std::string contents((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
        return contents;
    }

    bool IsWhiteSpace(const char& c)
    {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    std::string RemoveWhiteSpaces(const std::string& in)
    {
        bool ignore = false;
        std::string cleanVersion;
        for (const auto& c : in)
        {
            if (c == '\"')
                ignore = not ignore;
            if (not IsWhiteSpace(c) or ignore)
                cleanVersion += c;
        }
        cleanVersion.erase(cleanVersion.begin());
        cleanVersion.erase(cleanVersion.end() - 1);
        return cleanVersion;
    }

    bool CheckJSONValid(const std::string& in)
    {
        int countCurlyBraces = 0, countBraces = 0, countQuotes = 0;
        bool ignore = false;
        for (const auto& c : in)
        {
            if (c == '"')
            {
                ignore = not ignore;
                ++countQuotes;
            }
            else if (not ignore)
            {
                if (c == '{' or c == '}')
                    ++countCurlyBraces;
                else if (c == '[' or c == ']')
                    ++countBraces;
            }
        }

        return (countQuotes + countBraces + countCurlyBraces) % 2 == 0;
    }

    std::string CopyToNextBrace(const std::string& in, size_t& startIndex, char brace)
    {
        std::string result = std::string(1, brace);
        ++startIndex;

        char openingRef = brace, closingRef = brace == '{' ? '}' : ']';
        int countOpening = 1, countClosing = 0;

        size_t len = in.size();
        for (; startIndex < len; ++startIndex)
        {
            char c = in[startIndex];
            if (c == openingRef)
                ++countOpening;
            else if (c == closingRef)
                ++countClosing;
            result += c;
            if (countClosing == countOpening)
                break;
        }
        return result;
    }

    std::string CopyToNextComma(const std::string& in, size_t& startIndex)
    {
        std::string result;
        size_t len = in.size();
        for (; startIndex < len; ++startIndex)
        {
            char c = in[startIndex];
            if (c == '[' or c == '{')
                result += CopyToNextBrace(in, startIndex, c);
            else if (c == ',' or c == '}' or c == ']')
                break;
            else
                result += c;
        }
        return result;
    }

    std::string RemoveQuotes(std::string in)
    {
        in.erase(std::remove(in.begin(), in.end(), '\"'), in.end());
        return in;
    }

    std::string PrettyPrintOption(const std::string& optionName, bool option)
    {
        if (optionName == "Multithreading" and not option)
        {
            std::cout << "\033[1;31mWarning : Having multithreading disabled makes the program extremely slow, consider enabling it or lowering the port range.\033[0m\n";
        }
        return optionName + std::string( " option : ") + (option ? "\033[1;33menabled" : "\033[1;31mdisabled");
    }


    std::string GetIpAddressFromHostname(const std::string& hostName, bool ipv6)
    {
        /// TODO handle ipv6 (with getaddrinfo since gethostbyname does not support ipv6)
        (void) ipv6;

        struct hostent *hp = gethostbyname(hostName.c_str());

        ThrowsIfTrue(not hp, hostName + " was not resolved\n");

        struct in_addr ip_addr = *(struct in_addr *)(hp->h_addr);
        std::string ip = inet_ntoa(ip_addr);

        return ip;
    }

    bool IsIgnoreJsonChar(char c)
    {
        return c != '"' and c != '{' and c != '}' and c != '[' and c != ']';
    }

    void ThrowIfBoolNotValid(const std::string& in, const std::string& error)
    {
        utils::ThrowsIfTrue(in != "true" and in != "false", error);
    }

    std::string ToLower(const std::string& in)
    {
        std::string copy = in;
        transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
        return copy;
    }

    bool CompareStringsInvariantCase(const std::string& s1, const std::string& s2)
    {
        return ToLower(s1) == ToLower(s2);
    }
}
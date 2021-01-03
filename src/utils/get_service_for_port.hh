#pragma once

#include <string>
#include <map>

namespace utils
{
    std::map<int, std::string> serviceForPort
    {
        { 20, "ftp data transfer" },
        { 21, "ftp command control" },
        { 22, "ssh" },
        { 23, "telnet" },
        { 25, "smtp" },
        { 53, "dns" },
        { 63, "whois" },
        { 80, "http" },
        { 110, "pop3" },
        { 119, "nntp" },
        { 123, "ntp" },
        { 143, "snmp" },
        { 194, "irc" },
        { 443, "https" },
        { 631, "ipp" }
    };

    std::string GetServiceForPort(int port)
    {
        if (serviceForPort.find(port) != serviceForPort.end())
            return serviceForPort[port];
        return "unknown";
    }
}
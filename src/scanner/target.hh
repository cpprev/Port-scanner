#pragma once

#include <string>
#include <vector>

#include "utils/utils.hh"

namespace scanner
{
    class Target
    {
    public:
        Target(const std::string& in);
        static std::vector<Target> ParseTargets(const std::string& in);
        void AddCustomAttribute(const std::string& key, const std::string& value);

        std::string GetHost() const { return _host; }
        void SetHost(const std::string& newHost) { _host = newHost; }
        int GetRangeStart() const { return _rangeStart; }
        int GetRangeEnd() const { return _rangeEnd; }
    private:
        std::string _host;

        int _rangeStart = 1;
        int _rangeEnd = 1000;
    };
}
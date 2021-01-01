#pragma once

#include <string>
#include <vector>
#include <map>

#include "utils/utils.hh"
#include "port_state.hh"

namespace scanner
{
    class Target
    {
    public:
        Target(const std::string& in);
        static std::vector<std::shared_ptr<Target>> ParseTargets(const std::string& in);
        void AddCustomAttribute(const std::string& key, const std::string& value);

        std::string GetHost() const { return _host; }
        void SetHost(const std::string& newHost) { _host = newHost; }
        int GetRangeStart() const { return _rangeStart; }
        int GetRangeEnd() const { return _rangeEnd; }
        void SetResults(const std::pair<int, PORT_STATE> p) { _results[p.first] = p.second; }
        std::map<int, PORT_STATE> GetResults() const { return _results; }
    private:
        std::string _host;

        int _rangeStart = 1;
        int _rangeEnd = 1000;

        std::map<int, PORT_STATE> _results = std::map<int, PORT_STATE>();
    };
}
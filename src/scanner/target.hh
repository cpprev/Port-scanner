#pragma once

#include <string>
#include <vector>
#include <map>

#include "utils/utils.hh"
#include "port_state.hh"
#include "results.hh"

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
        size_t GetRangeStart() const { return _rangeStart; }
        size_t GetRangeEnd() const { return _rangeEnd; }
        void SetResults(std::vector<int> openedPorts, int countOpened, int countClosed, int countFiltered) { _results = Results(openedPorts, countOpened, countClosed, countFiltered); }
        Results GetResults() const { return _results; }
    private:
        std::string _host;

        size_t _rangeStart = 1;
        size_t _rangeEnd = 1000;

        Results _results;
    };
}
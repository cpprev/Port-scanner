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
        Target() = default;
        Target(const std::string& in);

        std::string GetHost() const { return _host; }
        size_t GetRangeStart() const { return _rangeStart; }
        size_t GetRangeEnd() const { return _rangeEnd; }
        Results GetResults() const { return _results; }

        void SetHost(const std::string& newHost) { _host = newHost; }
        void SetRangeStart(size_t rangeStart) { _rangeStart = rangeStart; }
        void SetRangeEnd(size_t rangeEnd) { _rangeEnd = rangeEnd; }
        void SetResults(std::vector<int> openedPorts, int countOpened, int countClosed, int countFiltered) { _results = Results(openedPorts, countOpened, countClosed, countFiltered); }
    private:
        std::string _host;

        size_t _rangeStart = 1;
        size_t _rangeEnd = 1000;

        Results _results;
    };
}
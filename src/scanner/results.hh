#pragma once

#include <utility>
#include <vector>

namespace scanner
{
    class Results
    {
    public:
        Results() = default;
        Results(std::vector<int> openedPorts, int countOpened, int countClosed, int countFiltered)
        : _openedPorts(std::move(openedPorts)),
          _countOpened(countOpened),
          _countClosed(countClosed),
          _countFiltered(countFiltered)
        {}

        int GetCountOpened() const { return _countOpened; }
        int GetCountClosed() const { return _countClosed; }
        int GetCountFiltered() const { return _countFiltered; }
        std::vector<int> GetOpenedPorts() const { return _openedPorts; }
    private:
        std::vector<int> _openedPorts = std::vector<int>();

        int _countOpened = 0;
        int _countClosed = 0;
        int _countFiltered = 0;
    };
}
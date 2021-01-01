#include <string>

#include "port_state.hh"

namespace scanner
{
    std::string StateToString(PORT_STATE state)
    {
        std::string res;
        switch (state)
        {
            case OPENED:
                res = "OPEN";
                break;
            case CLOSED:
                res = "CLOSED";
                break;
            case FILTERED:
                res = "FILTERED";
                break;
            default:
                break;
        }
        return res;
    }

    std::string StateToColor(PORT_STATE state)
    {
        std::string res;
        switch (state)
        {
            case OPENED:
                res = "\033[1;32m";
                break;
            case CLOSED:
                res = "\033[1;31m";
                break;
            case FILTERED:
                res = "\033[1;33m";
                break;
            default:
                break;
        }
        return res;
    }

    void IncrementSummary(PORT_STATE state, int& countOpened , int& countClosed, int& countFiltered)
    {
        switch (state)
        {
            case OPENED:
                ++countOpened;
                break;
            case CLOSED:
                ++countClosed;
                break;
            case FILTERED:
                ++countFiltered;
                break;
            default:
                break;
        }
    }
}
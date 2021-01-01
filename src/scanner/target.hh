#pragma once

#include <string>

namespace scanner
{
    class Target
    {
    public:
    private:
        std::string ip;

        int _rangeStart = 1;
        int _rangeEnd = 1000;
    };
}
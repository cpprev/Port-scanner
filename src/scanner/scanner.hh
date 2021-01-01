#pragma once

#include <vector>

#include "target.hh"

namespace scanner
{
    class Scanner
    {
    public:
        Scanner(const std::string& in);
        void Scan();
    private:
        std::vector<Target> _targets = std::vector<Target>();

        /// Options
        bool _verbose = false;
    };
}
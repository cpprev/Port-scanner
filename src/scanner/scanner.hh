#pragma once

#include <vector>

#include "target.hh"

namespace scanner
{
    class Scanner
    {
    public:
        Scanner(const std::string& in);
        void ParseOptions(const std::string& in);
        void AddCustomAttribute(const std::string& key, const std::string& value);
        void AddCustomOption(const std::string& key, const std::string& value);

        void ScanGlobal();
        void ScanGlobalMultithread();
        void ScanGlobalNonMultithread();

        void PrettyPrint();

        bool IsVerboseEnabled() { return _verbose; }
    private:
        std::vector<Target> _targets = std::vector<Target>();

        bool _verbose = false;
    };
}
#pragma once

#include <vector>
#include <memory>

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

        void PrintSummary();
        void PrettyPrint();

        bool IsVerboseEnabled() { return _verbose; }

        std::vector<std::shared_ptr<Target>> GetTargets() { return _targets; }
    private:
        std::vector<std::shared_ptr<Target>> _targets = std::vector<std::shared_ptr<Target>>();

        bool _verbose = false;
    };
}
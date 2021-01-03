#pragma once

#include <vector>
#include <memory>

#include "target.hh"

namespace scanner
{
    class Scanner
    {
    public:
        Scanner() = default;
        Scanner(const std::string& in);

        void ScanGlobal();
        void ScanGlobalMultithread();
        void ScanGlobalNonMultithread();

        void PrintSummary();
        void PrettyPrint();

        bool IsVerboseEnabled() { return _verbose; }
        std::vector<std::shared_ptr<Target>> GetTargets() { return _targets; }

        void SetTargets(const std::vector<std::shared_ptr<Target>>& targets) { _targets = std::vector<std::shared_ptr<Target>>(targets); }
        void SetVerbose(bool verbose) { _verbose = verbose; }
    private:
        std::vector<std::shared_ptr<Target>> _targets = std::vector<std::shared_ptr<Target>>();

        bool _verbose = false;
    };
}
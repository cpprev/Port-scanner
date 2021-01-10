#pragma once

#include <vector>
#include <memory>

#include "target.hh"
#include "options.hh"

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

        std::vector<std::shared_ptr<Target>> GetTargets() { return _targets; }
        Options GetOptions() { return _options; }

        void SetTargets(const std::vector<std::shared_ptr<Target>>& targets) { _targets = std::vector<std::shared_ptr<Target>>(targets); }
        void SetOptions(const Options& options) { _options = options; }
    private:
        std::vector<std::shared_ptr<Target>> _targets = std::vector<std::shared_ptr<Target>>();

        Options _options;
    };
}
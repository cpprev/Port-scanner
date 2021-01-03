#pragma once

#include <string>

#include "scanner/scanner.hh"

namespace parsing
{
    /// Pre parses + Parses a JSON file
    /// \param inputPath
    /// \return
    scanner::Scanner HandleInput(const std::string& inputPath);

    /// Pre-parses a JSON file
    /// \param inputPath
    /// \return
    std::string PreParseInput(const std::string& inputPath);

    scanner::Scanner ParseScanner(const std::string& in);

    void ParseOptions(scanner::Scanner& scanner, const std::string& in);

    scanner::Target ParseTarget(const std::string& in);

    std::vector<std::shared_ptr<scanner::Target>> ParseTargets(const std::string& in);
}
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

    /// Parses whole scanner input object
    /// \param in
    /// \return
    scanner::Scanner ParseScanner(const std::string& in);

    /// Parse "options" scanner attribute
    /// \param scanner
    /// \param in
    scanner::Options ParseOptions(const std::string& in);

    /// Parses a single target
    /// \param in
    /// \return
    scanner::Target ParseTarget(const std::string& in);

    /// Parses whole "target" input object
    /// \param in
    /// \return
    std::vector<std::shared_ptr<scanner::Target>> ParseTargets(const std::string& in);
}
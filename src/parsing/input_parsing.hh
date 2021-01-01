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

    /// Parses a JSON file
    /// \param in
    /// \return
    scanner::Scanner ParseInput(const std::string& in);
}
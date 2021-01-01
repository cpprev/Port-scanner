#pragma once

#include <string>

namespace parsing
{
    /// Pre-parses a JSON file
    /// \param inputPath
    /// \return
    bool PreParseInput(const std::string& inputPath);
}
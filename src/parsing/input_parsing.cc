#include <iostream>

#include "input_parsing.hh"
#include "scanner/scanner.hh"
#include "utils/utils.hh"

namespace parsing
{
    scanner::Scanner HandleInput(const std::string& inputPath)
    {
        std::string json = PreParseInput(inputPath);
        scanner::Scanner scanner = ParseInput(json);
        return scanner;
    }

    std::string PreParseInput(const std::string& inputPath)
    {
        std::string fileContents = utils::ReadFile(inputPath);

        fileContents = utils::RemoveWhiteSpaces(fileContents);

        std::string errorMsg = "File \'" + inputPath + "\' is not in valid JSON format.";
        utils::ThrowsIfTrue(not utils::CheckJSONValid(fileContents), errorMsg);

        return fileContents;
    }

    scanner::Scanner ParseInput(const std::string& in)
    {
        return scanner::Scanner(in);
    }
}
#include <iostream>

#include "input_parsing.hh"
#include "utils/utils.hh"

namespace parsing
{
    bool PreParseInput(const std::string& inputPath)
    {
        /// Read File
        std::string fileContents = utils::ReadFile(inputPath);

        /// Remove whitespaces
        fileContents = utils::RemoveWhiteSpaces(fileContents);

        std::cout << fileContents << '\n';

        /// Check JSON validity + Check input is correct format (string where string is expected)
        utils::ThrowsIfTrue(not utils::CheckJSONValid(fileContents), "File \'" + inputPath + "\' is not in valid JSON format.");

        /// JSON Convertion to object
        /// TODO

        return true;
    }
}
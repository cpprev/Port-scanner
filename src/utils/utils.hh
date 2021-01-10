#pragma once

#include <string>

namespace utils
{
    /// Throws an exception if the condition is equals to true
    /// \param condition
    /// \param errorMessage
    void ThrowsIfTrue(bool condition, const std::string& errorMessage);

    /// Checks whether a file exists at a given location
    /// \param path
    /// \return
    bool IsFile(const std::string& filePath);

    /// Checks whether a character is considered whitespace
    /// \param c
    /// \return
    bool IsWhiteSpace(const char& c);

    /// Reads a file from a path
    /// \param filePath
    /// \return
    std::string ReadFile(const std::string& filePath);

    /// Removes whitespace characters from a given string
    /// \param in
    /// \return
    std::string RemoveWhiteSpaces(const std::string& in);

    /// Checks whether a string is in JSON format
    /// \param in
    /// \return
    bool CheckJSONValid(const std::string& in);

    /// Copies contents in a string until next brace
    /// \param in
    /// \param startIndex
    /// \param brace
    /// \return
    std::string CopyToNextBrace(const std::string& in, size_t& startIndex, char brace);

    /// Copies contents in a string until next comma
    /// \param in
    /// \param startIndex
    /// \return
    std::string CopyToNextComma(const std::string& in, size_t& startIndex);

    /// Removes the quotes from a string
    /// \param in
    /// \return
    std::string RemoveQuotes(std::string in);

    /// Pretty prints an option
    /// \param optionName
    /// \param option
    /// \return
    std::string PrettyPrintOption(const std::string& optionName, bool option);

    /// Gets the Ip address from a host name
    /// \param hostName
    /// \return
    std::string GetIpAddressFromHostname(const std::string& hostName);

    /// True if c is considered a character to ignore ('"', '{', ...)
    /// \param c
    /// \return
    bool IsIgnoreJsonChar(char c);
}
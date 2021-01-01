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
}
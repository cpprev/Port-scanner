#include <stdexcept>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <iostream>

#include "utils.hh"

namespace utils
{
    void ThrowsIfTrue(bool condition, const std::string& errorMessage)
    {
        if (condition)
            throw std::runtime_error(errorMessage);
    }

    bool IsFile(const std::string& filePath)
    {
        struct stat st;
        if (stat(filePath.c_str(), &st) != 0)
            return false;
        return (st.st_mode & S_IFREG) != 0;
    }

    std::string ReadFile(const std::string& filePath)
    {
        std::ifstream ifs(filePath);
        std::string contents((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
        return contents;
    }

    bool IsWhiteSpace(const char& c)
    {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    std::string RemoveWhiteSpaces(const std::string& in)
    {
        bool ignore = false;
        std::string cleanVersion;
        for (const auto& c : in)
        {
            if (c == '\"')
                ignore = not ignore;
            if (not IsWhiteSpace(c) or ignore)
                cleanVersion += c;
        }
        return cleanVersion;
    }

    bool CheckJSONValid(const std::string& in)
    {
        int countCurlyBraces = 0, countBraces = 0, countQuotes = 0;
        bool ignore = false;
        for (const auto& c : in)
        {
            if (c == '"')
            {
                ignore = not ignore;
                ++countQuotes;
            }
            else if (not ignore)
            {
                if (c == '{' or c == '}')
                    ++countCurlyBraces;
                else if (c == '[' or c == ']')
                    ++countBraces;
            }
        }

        return (countQuotes + countBraces + countCurlyBraces) % 2 == 0;
    }
}
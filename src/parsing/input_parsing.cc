#include <iostream>

#include "input_parsing.hh"
#include "scanner/scanner.hh"
#include "utils/utils.hh"

namespace parsing
{
    scanner::Scanner HandleInput(const std::string& inputPath)
    {
        std::string json = PreParseInput(inputPath);
        scanner::Scanner scanner(json);
        return scanner;
    }

    std::string PreParseInput(const std::string& inputPath)
    {
        std::string fileContents = utils::ReadFile(inputPath);

        std::string errorMsg = "File \'" + inputPath + "\' is not in valid JSON format.";
        utils::ThrowsIfTrue(not utils::CheckJSONValid(fileContents), errorMsg);

        fileContents = utils::RemoveWhiteSpaces(fileContents);

        return fileContents;
    }

    void ScannerAddCustomAttribute(scanner::Scanner& scanner, const std::string& key, const std::string& value)
    {
        if (utils::CompareStringsInvariantCase(key, "Targets"))
        {
            scanner.SetTargets(parsing::ParseTargets(value));
        }
        else if (utils::CompareStringsInvariantCase(key, "Options"))
        {
            auto options = parsing::ParseOptions(value);
            scanner.SetOptions(options);
        }
        else
        {
            throw std::runtime_error("Unknown Scanner class attribute \'" + key + "\'");
        }
    }

    void OptionsAddCustomAttribute(scanner::Options& options, const std::string& key, const std::string& value)
    {
        if (utils::CompareStringsInvariantCase(key, "Verbose"))
        {
            utils::ThrowIfBoolNotValid(value, "Wrong value for 'verbose' option.");
            options.SetVerbose(value == "true");
        }
        else if (utils::CompareStringsInvariantCase(key, "Multithreading"))
        {
            utils::ThrowIfBoolNotValid(value, "Wrong value for 'multithreading' option.");
            options.SetMultithreading(value == "true");
        }
        else if (utils::CompareStringsInvariantCase(key, "Timeout"))
        {
            int timeout = std::atoi(value.c_str());
            utils::ThrowsIfTrue(timeout <= 0 or timeout > 10, "Timeout value must be bound from 0 to 10 seconds.");
            options.SetTimeout(timeout);
        }
        else
        {
            throw std::runtime_error("Unknown Options class attribute \'" + key + "\'");
        }
    }

    void TargetAddCustomAttribute(scanner::Target& target, const std::string& key, const std::string& value)
    {
        if (utils::CompareStringsInvariantCase(key, "Host"))
        {
            target.SetHost(utils::RemoveQuotes(value));
        }
        else if (utils::CompareStringsInvariantCase(key, "PortRange"))
        {
            std::string::size_type ind = value.find('-');
            int rangeStart = std::atoi(value.substr(0, ind).c_str());
            int rangeEnd = std::atoi(value.substr(ind + 1).c_str());
            utils::ThrowsIfTrue(rangeStart < 0 or rangeEnd > 65535, "Port range start must be bound from 0 to 65535.");
            utils::ThrowsIfTrue(rangeEnd < 0 or rangeEnd > 65535, "Port range end must be bound from 0 to 65535.");
            utils::ThrowsIfTrue(rangeEnd < rangeStart, "Port range start must be lower than port range end.");
            target.SetRangeStart(rangeStart);
            target.SetRangeEnd(rangeEnd);
        }
        else
        {
            throw std::runtime_error("Unknown Target class attribute \'" + key + "\'");
        }
    }

    scanner::Options ParseOptions(const std::string& in)
    {
        scanner::Options options;
        size_t len = in.size();
        std::string key, value;
        for (size_t i = 0; i < len; ++i)
        {
            char c = in[i];
            if (c == ':')
            {
                value = utils::CopyToNextComma(in, ++i);

                OptionsAddCustomAttribute(options, key, utils::RemoveQuotes(value));

                key.clear();
                value.clear();
            }
            else
            {
                if (utils::IsIgnoreJsonChar(c))
                    key += c;
            }
        }
        return options;
    }

    scanner::Scanner ParseScanner(const std::string& in)
    {
        scanner::Scanner scanner;
        size_t len = in.size();
        std::string key, value;
        for (size_t i = 0; i < len; ++i)
        {
            char c = in[i];
            if (c == ':')
            {
                value = utils::CopyToNextComma(in, ++i);

                ScannerAddCustomAttribute(scanner, key, utils::RemoveQuotes(value));

                key.clear();
                value.clear();
            }
            else
            {
                if (utils::IsIgnoreJsonChar(c))
                    key += c;
            }
        }
        return scanner;
    }

    scanner::Target ParseTarget(const std::string& in)
    {
        scanner::Target target;
        size_t len = in.size();
        std::string key, value;
        for (size_t i = 0; i < len; ++i)
        {
            char c = in[i];
            if (c == ':')
            {
                value = utils::CopyToNextComma(in, ++i);
                TargetAddCustomAttribute(target, key, utils::RemoveQuotes(value));

                key.clear();
                value.clear();
            }
            else
            {
                if (utils::IsIgnoreJsonChar(c))
                    key += c;
            }
        }
        return target;
    }

    std::vector<std::shared_ptr<scanner::Target>> ParseTargets(const std::string& in)
    {
        std::vector<std::shared_ptr<scanner::Target>> targets;
        size_t len = in.size();
        for (size_t i = 0; i < len; ++i)
        {
            char c = in[i];
            if (c == '{')
            {
                auto target = std::make_shared<scanner::Target>(utils::CopyToNextBrace(in, i, c));
                targets.push_back(target);
            }
        }
        return targets;
    }
}
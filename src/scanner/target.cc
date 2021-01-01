#include <iostream>
#include <memory>

#include "target.hh"
#include "utils/utils.hh"

namespace scanner
{
    Target::Target(const std::string& in)
    {
        size_t len = in.size();
        std::string key, value;
        for (size_t i = 0; i < len; ++i)
        {
            char c = in[i];
            if (c == ':')
            {
                value = utils::CopyToNextComma(in, ++i);
                AddCustomAttribute(key, utils::RemoveQuotes(value));

                key.clear();
                value.clear();
            }
            else
            {
                if (c != '"' and c != '{' and c != '}' and c != '[' and c != ']')
                    key += c;
            }
        }
    }

    void Target::AddCustomAttribute(const std::string& key, const std::string& value)
    {
        if (key == "Host")
        {
            _host = utils::RemoveQuotes(value);
        }
        else if (key == "PortRange")
        {
            std::string::size_type ind = value.find('-');
            _rangeStart = std::atoi(value.substr(0, ind).c_str());
            _rangeEnd = std::atoi(value.substr(ind + 1).c_str());
        }
        else
        {
            throw std::runtime_error("Unknown Target class attribute \'" + key + "\'");
        }
    }

    std::vector<std::shared_ptr<Target>> Target::ParseTargets(const std::string& in)
    {
        std::vector<std::shared_ptr<Target>> targets;
        size_t len = in.size();
        for (size_t i = 0; i < len; ++i)
        {
            char c = in[i];
            if (c == '{')
            {
                auto target = std::make_shared<Target>(utils::CopyToNextBrace(in, i, c));
                targets.push_back(target);
            }
        }
        return targets;
    }
}
#include <iostream>
#include <memory>

#include "target.hh"
#include "utils/utils.hh"
#include "parsing/input_parsing.hh"

namespace scanner
{
    Target::Target(const std::string& in)
    {
        *this = parsing::ParseTarget(in);
    }
}
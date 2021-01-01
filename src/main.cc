#include <iostream>

#include "parsing/input_parsing.hh"
#include "utils/utils.hh"

int main(int argc, char *argv[])
{
    try
    {
        /// Check that args are valid
        utils::ThrowsIfTrue(argc != 2 || not utils::IsFile(argv[1]),
                            "Correct usage : ./scanner {input.json}");

        /// JSON Input pre-parsing
        parsing::PreParseInput(argv[1]);

        /// Port scan
        /// TODO
    }
    catch (const std::exception& exception)
    {
        std::cout << "[Runtime error] " << exception.what() << '\n';
    }

    return 0;
}
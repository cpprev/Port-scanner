#include <iostream>

#include "parsing/input_parsing.hh"
#include "utils/utils.hh"

int main(int argc, char *argv[])
{
    try
    {
        utils::ThrowsIfTrue(argc != 2 || not utils::IsFile(argv[1]),
                            "Correct usage : ./scanner {input.json}");

        scanner::Scanner scanner = parsing::HandleInput(argv[1]);

        if (scanner.IsVerboseEnabled())
            scanner.PrettyPrint();

        scanner.ScanGlobal();
    }
    catch (const std::exception& exception)
    {
        std::cout << "[Runtime error] " << exception.what() << '\n';
    }

    return 0;
}
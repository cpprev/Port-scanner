#include <iostream>

#include "parsing/input_parsing.hh"
#include "utils/utils.hh"
#include "utils/globals.hh"
#include "scanner/scanner.hh"

int main(int argc, char *argv[])
{
    try
    {
        utils::ThrowsIfTrue(argc != 2 || not utils::IsFile(argv[1]),
                            "Correct usage : ./scanner {input.json}");

        g_Scanner = parsing::HandleInput(argv[1]);

        if (g_Scanner->GetOptions().IsVerboseEnabled())
            g_Scanner->PrettyPrint();

        g_Scanner->ScanGlobal();

        g_Scanner->PrintSummary();
    }
    catch (const std::exception& exception)
    {
        std::cout << "[Runtime error] " << exception.what() << '\n';
    }

    return 0;
}
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

        int countOpened = 0, countClosed = 0, countFiltered = 0;
        for (const auto& target : scanner.GetTargets())
        {
            for (const auto& result : target->GetResults())
            {
                auto state = result.second;

                scanner::IncrementSummary(state, countOpened, countClosed, countFiltered);
                auto stateString = scanner::StateToString(state);
                std::string color = scanner::StateToColor(state);
                if (state == scanner::OPENED)
                {
                    std::cout << color << "Port " << result.first << " is " << stateString << "\033[0m" << "\n";
                }
            }
        }
        if (scanner.IsVerboseEnabled())
        {
            std::cout << "\033[1;34m" << "\n[Summary]\n";
            std::cout << "Number of ports opened : " << countOpened << "\n";
            std::cout << "Number of ports closed : " << countClosed << "\n";
            std::cout << "Number of ports filtered : " << countFiltered << "\033[0m" << "\n";
        }
    }
    catch (const std::exception& exception)
    {
        std::cout << "[Runtime error] " << exception.what() << '\n';
    }

    return 0;
}
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

        std::string summary;
        for (const auto& target : scanner.GetTargets())
        {
            int countOpened = 0, countClosed = 0, countFiltered = 0;
            for (const auto& result : target->GetResults())
            {
                auto state = result.second;

                scanner::IncrementSummary(state, countOpened, countClosed, countFiltered);
                auto stateString = scanner::StateToString(state);
                std::string color = scanner::StateToColor(state);
                if (state == scanner::OPENED)
                {
                    std::cout << color << "Port \033[1;33m" << result.first << color << " is " << stateString
                              << " on : \033[1;33m" << target->GetHost() << "\033[0m" << "\n";
                }
            }
            if (scanner.IsVerboseEnabled())
            {
                summary += "\033[1;34m\n[Summary for host : \033[1;33m" + target->GetHost() + "\033[1;34m]\n";
                summary += "Number of ports opened :\t\033[1;33m" + std::to_string(countOpened) + "\033[1;34m\n";
                summary += "Number of ports closed :\t\033[1;33m" + std::to_string(countClosed) + "\033[1;34m\n";
                summary += "Number of ports filtered :\t\033[1;33m" + std::to_string(countFiltered) + "\033[0m" + "\n";
            }
        }
        if (scanner.IsVerboseEnabled())
            std::cout << summary;
    }
    catch (const std::exception& exception)
    {
        std::cout << "[Runtime error] " << exception.what() << '\n';
    }

    return 0;
}
#include <iostream>
#include <string>
#include <sstream>

// clear the terminal screen
void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

// displays the initial OS banner
void displayHeader() {
    std::cout << "Welcome to CSOPESY Marquee Operator!\n";
    std::cout << "Group Developers:\n";
    std::cout << " - Guillermo, Iain\n";
    std::cout << " - Lee, Hannah\n";
    std::cout << " - Lim, Jenrick\n";
    std::cout << "Version Date: 2026-09\n";
}

// displays available commands
void displayHelp() {
    std::cout << "\nAvailable Commands:\n";
    std::cout << "  help          - Displays the commands and their descriptions\n";
    std::cout << "  start_marquee - Starts the marquee animation\n";
    std::cout << "  stop_marquee  - Stops the marquee animation\n";
    std::cout << "  set_text      - Sets custom marquee text (e.g., set_text Hello)\n";
    std::cout << "  set_speed     - Sets refresh rate in milliseconds (e.g., set_speed 100)\n";
    std::cout << "  exit          - Terminates the console\n\n";
}

int main() {
    clearScreen();
    displayHeader();

    std::string input;
    bool isRunning = true;

    std::cout << "\n";

    while (isRunning) {
        std::cout << "Command> ";
        std::getline(std::cin, input);

        std::stringstream ss(input);
        std::string command;
        ss >> command; 

        // lowers the command string just in case 
        for (char &c : command) {
            c = std::tolower(c);
        }

        if (command == "help") {
            displayHelp();
        } else if (command == "start_marquee") {
            
            // to be filled

        } else if (command == "stop_marquee") {

            // to be filled

        } else if (command == "set_text") {

           // to be filled

        } else if (command == "set_speed") {

            // to be filled

        } else if (command == "exit") {
            std::cout << "Exiting Marquee Operator...\n";
            isRunning = false;
        } else if (!command.empty()) {
            std::cout << "Unrecognized command. Type 'help' for commands.\n";
        }
    }

    return 0;
}
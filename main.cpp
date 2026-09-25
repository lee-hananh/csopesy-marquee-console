#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <windows.h>
#include <conio.h>

// initialize variables
std::string marqueeText = "Welcome to CSOPESY Marquee Operator!";
std::atomic <int> marqueeSpeed = 500;
std::atomic <bool> running = false;
std::thread marqueeThread;
std::mutex textMutex;
std::mutex consoleMutex;
const int width = 40;
const int marqueeRow = 0;
const int commandRow = 7;
const int headerRow = 8;

// clear the terminal screen
void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

// set console position
void gotoRC(int row, int col = 0) {
    COORD pos = { (SHORT)col, (SHORT)row };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// displays the initial OS banner
void displayHeader() {
    gotoRC(2, 0);
    std::cout << "Group Developers:";
    gotoRC(3, 0);
    std::cout << " - Guillermo, Iain";
    gotoRC(4, 0);
    std::cout << " - Lee, Hannah";
    gotoRC(5, 0);
    std::cout << " - Lim, Jenrick";
    gotoRC(6, 0);
    std::cout << "Version Date: 2026-09-25";
    gotoRC(marqueeRow, 0);
    std::cout<<marqueeText<< std::flush;
}

// clears a single row
void clearRow(int row) {
    gotoRC(row, 0);
    std::cout << std::string(120, ' ');
    gotoRC(row, 0);
}

// clears the area for the output (help and feedback)
void clearOutput() {
    for (int i = 0; i < 10; i++)
        clearRow(headerRow + 1 + i);

    clearRow(commandRow + 1);
    gotoRC(headerRow + 2, 0);
}

// displays command feedback
void displayFeedback(const std::string& message) {
    std::lock_guard<std::mutex> lock(consoleMutex);
    clearRow(commandRow + 1);
    gotoRC(commandRow + 1, 0);
    std::cout << message << std::flush;
}

// displays available commands
void displayHelp() {
    std::lock_guard<std::mutex> lock(consoleMutex);
    int row = headerRow + 2;
    gotoRC(row++, 0); std::cout << "Available Commands:";
    gotoRC(row++, 0); std::cout << "  help          - Displays the commands and their descriptions";
    gotoRC(row++, 0); std::cout << "  start_marquee - Starts the marquee animation";
    gotoRC(row++, 0); std::cout << "  stop_marquee  - Stops the marquee animation";
    gotoRC(row++, 0); std::cout << "  set_text      - Sets custom marquee text (e.g., set_text Hello)";
    gotoRC(row++, 0); std::cout << "  set_speed     - Sets refresh rate in milliseconds (e.g., set_speed 100)";
    gotoRC(row++, 0); std::cout << "  exit          - Terminates the console";
    std::cout << std::flush;
}

// this makes the text move
void marqueeLoop(){
    while(running){
        std::string text;
        {
            std::lock_guard<std::mutex> lock(textMutex);
            text = marqueeText;
        }
        std::string padded = std::string(width, ' ') + text + std::string(width, ' ');
        for (int i = 0; running && i <= (int)padded.length() - width; i++) {
            {
                std::lock_guard<std::mutex> lock(consoleMutex);
                gotoRC(marqueeRow, 0);
                std::cout << padded.substr(i, width) << std::flush;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(marqueeSpeed));
        }
    }
}

// starts the marquee
void startMarquee(){
    running = true;
    marqueeThread = std::thread(marqueeLoop);
}

// stops the marquee
void stopMarquee(){
    running = false;
    if(marqueeThread.joinable()){
        marqueeThread.join();
    }
}

void setText(const std::string& text){
    std::lock_guard<std::mutex> lock(textMutex);
    marqueeText = text;
}

// sets the speed of the marquee
bool setSpeed(int speed){
    if (speed > 0) {
        marqueeSpeed = speed;
        return true;
    } else {
        displayFeedback("Invalid value. Provide only positive speed values (numbers) in milliseconds (e.g., set_speed 100)");
        return false;
    }
}

// allows continue typing while marquee running
std::string readCommandLine() {
    {
        std::lock_guard<std::mutex> lock(consoleMutex);
        clearRow(commandRow);
        gotoRC(commandRow, 0);
        std::cout << "Command> " << std::flush;
    }
    std::string buffer;
    while (true) {
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 0 || ch == (char)0xE0) {
                _getch(); // disregards extended key code (ex: arrows, F-keys, etc.)
                continue;
            }
            std::lock_guard<std::mutex> lock(consoleMutex);
            gotoRC(commandRow, 9 + (int)buffer.length());
            if (ch == '\r') {
                break;
            } else if (ch == '\b') {
                if (!buffer.empty()) {
                    buffer.pop_back();
                    gotoRC(commandRow, 9 + (int)buffer.length());
                    std::cout << ' ';
                    gotoRC(commandRow, 9 + (int)buffer.length());
                }
            } else if (isprint((unsigned char)ch)) {
                buffer += ch;
                std::cout << ch;
            }
            std::cout << std::flush;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    return buffer;
}

int main() {
    clearScreen();
    displayHeader();

    std::string input;
    bool isRunning = true;

    while (isRunning) {
        input = readCommandLine();

        std::stringstream ss(input);
        std::string command;
        ss >> command; 

        // lowers the command string just in case 
        for (char &c : command) {
            c = std::tolower(c);
        }

        clearOutput();

        if (command == "help") {
            displayHelp();
        } else if (command == "start_marquee") {
            if (running)
                displayFeedback("Marquee is already running.");
            else {
                startMarquee();
                displayFeedback("Marquee has started.");
            }
        } else if (command == "stop_marquee") {
            if (!running)
                displayFeedback("Marquee is already not running.");
            else {
                stopMarquee();
                displayFeedback("Marquee has stopped.");
            }
        } else if (command == "set_text") {
            std::string text;
            std::getline(ss, text);
            if (!text.empty() && text[0] == ' ') text.erase(0, 1);
            if (text.find_first_not_of(' ') == std::string::npos) {     // returns std::string::npos if input is empty or just whitespace
                displayFeedback("Invalid value. Provide a non-empty text (e.g., set_text Hello)");
            } else {
                setText(text);
                displayFeedback("Marquee text has been updated to '" + text + "'. The updated text will show in the next cycle.");
            }
        } else if (command == "set_speed") {
            int speed;
            if (ss >> speed) {
                if (setSpeed(speed))
                    displayFeedback("Marquee speed has been updated to " + std::to_string(speed) + " ms.");
            } else {
                displayFeedback("Invalid value. Provide only positive speed values (numbers) in milliseconds (e.g., set_speed 100)");
            }
        } else if (command == "exit") {
            clearOutput();
            displayFeedback("Exiting Marquee Operator...");
            if (running)
                stopMarquee();
            isRunning = false;
        } else if (!command.empty()) {
            displayFeedback("Unrecognized command. Type 'help' for commands.");
        } else {
            displayFeedback("");
        }
    }

    gotoRC(commandRow + 2, 0);
    return 0;
}
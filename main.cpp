#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <sstream>
#include <windows.h>
#include <conio.h>

//initialize variables
std::string marqueeText = "Welcome to CSOPESY Marquee Operator!";
std::atomic <int> marqueeSpeed = 500;
std::atomic <bool> running = false;
std::thread marqueeThread;
std::mutex textMutex;
const int width = 40;
const int marqueeRow = 0;
const int commandRow = 7;

// clear the terminal screen
void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

//set console position
void gotoRC(int row, int col = 0) {
    COORD pos = { (SHORT)col, (SHORT)row };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// displays the initial OS banner
void displayHeader() {
    std::cout << "\n";
    std::cout << "Group Developers:\n";
    std::cout << " - Guillermo, Iain\n";
    std::cout << " - Lee, Hannah\n";
    std::cout << " - Lim, Jenrick\n";
    std::cout << "Version Date: 2026-09\n";
    gotoRC(marqueeRow, 0);
    std::cout<<marqueeText<< std::flush;
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

//this makes the text move
void marqueeLoop(){
    while(running){
        std::string text;
        {
            std::lock_guard<std::mutex> lock(textMutex);
            text = marqueeText;
        }
        std::string padded = std::string(width, ' ') + text + std::string(width, ' ');
        for (int i = 0; running && i <= (int)padded.length() - width; i++) {
            gotoRC(marqueeRow, 0);
            std::cout << "\r" << padded.substr(i, width) << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(marqueeSpeed));
        }
    }
}

void startMarquee(){
    if(running) return;
    running = true;
    marqueeThread = std::thread(marqueeLoop);
}

void stopMarquee(){
    running = false;
    if(marqueeThread.joinable()){
        marqueeThread.join();
    }
    std::cout<<std::endl;
}

void setText(const std::string& text){
    std::lock_guard<std::mutex> lock(textMutex);
    marqueeText = text;
}

//TO DO: add functions for exit and set speed

//allows continue typing while marquee running
std::string readCommandLine() {
    gotoRC(commandRow, 0);
    std::cout << "Command> " << std::string(60, ' ');
    std::string buffer;
    while (true) {
        if (_kbhit()) {
            char ch = _getch();
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
            } else {
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

    std::cout << "\n";

    while (isRunning) {
        input = readCommandLine();
        std::cout << "\n";

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
            startMarquee(); 
        } else if (command == "stop_marquee") {
            stopMarquee();
        } else if (command == "set_text") {
            std::string text;
            std::getline(ss, text);
            if (!text.empty() && text[0] == ' ') text.erase(0, 1);
            setText(text);
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
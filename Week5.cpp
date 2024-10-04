#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <conio.h>   // For _kbhit() and _getch() in Windows
#include <windows.h>
#include <vector>



class MarqueeConsole{
public:
    MarqueeConsole(const std::string & text, int marqueeHeight, int speed)
        : marqueeText(text), marqueeHeight(marqueeHeight), speed(speed),
          x(0), y(0), xDirection(1), yDirection(1) {}

    void run() {
        while (true) {
            clearConsole();
            displayHeader();
            drawMarquee();
            displayUserInput();

            // Update positions
            updatePositions();

            // Handle keyboard input
            handleKeyboardInput();
        }
    }

private:
    std::string marqueeText;
    int marqueeHeight;
    int speed;
    int x, y;
    int xDirection, yDirection;
    std::string userInput;
    std::vector<std::string> commandList;

    void clearConsole() {
        system("cls");
    }

    void getConsoleWidth(int& width) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        }
    }

    void setCursor(int x = 0, int y = 0) {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coordinates;
        coordinates.X = x;
        coordinates.Y = y;
        SetConsoleCursorPosition(handle, coordinates);
    }

    void displayHeader() {
        std::cout << "*******************************\n";
        std::cout << "* Displaying a marquee console! *\n";
        std::cout << "*******************************\n";
    }

    void drawMarquee() {
        int consoleWidth;
        getConsoleWidth(consoleWidth);

        for (int i = 0; i < marqueeHeight; ++i) {
            if (i == y) {
                std::cout << std::string(x, ' ') << marqueeText;
            }
            std::cout << std::endl;
        }
    }

    void displayUserInput() {
        setCursor(0, 24);
        for (std::string command : commandList) {
            std::cout << "\nCommand processed in MARQUEE CONSOLE: " << command;
        }
        setCursor(0, 23);
        std::cout << "\nInput Command for MARQUEE CONSOLE: " << userInput;
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
    }

    void updatePositions() {
        int consoleWidth;
        getConsoleWidth(consoleWidth);

        x += xDirection;
        y += yDirection;

        // Change direction if at the edge
        if (x + marqueeText.length() >= consoleWidth || x < 0) {
            xDirection *= -1;
        }
        if (y >= marqueeHeight - 1 || y < 0) {
            yDirection *= -1;
        }
    }

    void handleKeyboardInput() {
        if (_kbhit()) {
            char ch = _getch();  // Get the character the user typed
            if (ch == '\r') {    // If Enter key is pressed, process the input
                commandList.push_back(userInput);
                userInput.clear();  // Clear input after processing
            }
            else if (ch == '\b') {  // Handle backspace
                if (!userInput.empty()) {
                    userInput.pop_back();
                }
            }
            else {
                userInput += ch;  // Add character to user input
            }
        }
    }
};

int main() {
    std::string message = "Hello World in Marquee!";
    int marqueeHeight = 20;  // Fixed height for vertical movement of marquee
    int speed = 100;         // Speed in milliseconds

    MarqueeConsole marqueeConsole(message, marqueeHeight, speed);
    marqueeConsole.run();

    std::cout << "Marquee stopped. Exiting the program.\n";
    return 0;
}
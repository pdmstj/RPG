#include "Utilities.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h> // 콘솔 화면 크기 관련

using namespace std;

void printCentered(const string& message) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    else {
        columns = 80; // 기본값
    }

    int padding = (columns - message.length()) / 2;
    if (padding < 0) padding = 0;

    cout << string(padding, ' ') << message << endl;
}

void printCenteredSlowly(const string& message) {
    printCentered("");
    for (char ch : message) {
        cout << ch << flush;
        this_thread::sleep_for(chrono::milliseconds(50));
    }
    cout << endl;
}

void printCenteredASCII(const string& ascii_art) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    else {
        columns = 80;
    }

    istringstream artStream(ascii_art);
    string line;
    while (getline(artStream, line)) {
        int padding = (columns - line.length()) / 2;
        if (padding < 0) padding = 0;
        cout << string(padding, ' ') << line << endl;
    }
}

void waitForEnter() {
    printCentered("Press Enter to continue...");
    cin.ignore();
}

void printEmptyLines(int count) {
    for (int i = 0; i < count; ++i) {
        cout << endl;
    }
}

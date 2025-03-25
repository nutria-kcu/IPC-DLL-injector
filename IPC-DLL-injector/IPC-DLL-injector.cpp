#include <iostream>
#include <Windows.h>
#include <conio.h> // For _getch()
#include "SharedMemory.h"
#include <tchar.h>

using namespace std;

bool isValidNumber(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

int main() {
    int cmd = 0, option = 0;
    char ch;
    cout << "Press 'q' to print 'Welcome message'.\n";
    SharedMemoryHandler* sh = new SharedMemoryHandler(_T("NUTRI-IPC"), 1);
    HANDLE fullEvent = sh->getFullEvent();
    HANDLE emptyEvent = sh->getEmptyEvent();

    while (true) {
        ch = _getch(); // Get character input without pressing Enter

        if (ch == '0') {
            cout << "Exiting...\n";
            delete sh;
            break;
        }
        if (ch == 's') {
            DWORD waitResult = WaitForSingleObject(emptyEvent, INFINITE);

            // Prompt user for cmd input
            cout << "Enter command (number): ";
            string cmdInput;
            getline(cin, cmdInput);  // Read the full input line

            if (cmdInput.empty() || !isValidNumber(cmdInput)) {
                cmd = 0;  // If input is empty or not a valid number, set cmd to 0
            }
            else {
                cmd = stoi(cmdInput);  // Convert string to integer
            }

            // Prompt user for option input
            cout << "Enter option (number): ";
            string optionInput;
            getline(cin, optionInput);  // Read the full input line

            if (optionInput.empty() || !isValidNumber(optionInput)) {
                option = 0;  // If input is empty or not a valid number, set option to 0
            }
            else {
                option = stoi(optionInput);  // Convert string to integer
            }

            cout << "SetMSG with cmd: " << cmd << ", option: " << option << endl;
            bool result = sh->setMessage(cmd, option);
           
            ResetEvent(emptyEvent);
            SetEvent(fullEvent);
            cout << "RESULT: " << result << "\n";
        }
        if (ch == 'g') {
            cout << "SetMSG\n";
        }
       
    }
    return 0;
}
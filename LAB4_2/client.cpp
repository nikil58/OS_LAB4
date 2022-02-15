#include <windows.h>
#include <iostream>
#include "main_menu.h"

using namespace std;

void WINAPI Callback(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
    cout << "Message is received" << endl;
}


void ui () {
    char choice;
    HANDLE e = CreateEventA(nullptr, false, false, nullptr);
    HANDLE namedPipe;
    bool conn = false;
    do{
        choice = input(client_menu);
        switch (choice)
        {
            case '1':
            {
                namedPipe = CreateFile("\\\\.\\pipe\\LAB4_2", GENERIC_READ | GENERIC_WRITE, 0,
                                       nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
                break;
            }
            case '2':
            {
                if (namedPipe) {
                    OVERLAPPED overlp = OVERLAPPED();
                    overlp.hEvent = e;
                    char msg[MAX_PATH];
                    conn = ReadFileEx(namedPipe, msg, 512, &overlp, Callback);
                    if (conn)
                        cout << msg << endl;
                    else
                        cout << "Connection failed: error " << GetLastError() << endl;
                }
                else {
                    cout << "First of all connect to the pipe!" << endl;
                }
                system("pause");
                break;
            }

            case '3':
            {
                if (namedPipe)
                {
                    conn = CloseHandle(namedPipe);
                    if (conn)
                    {
                        cout << "You are have disconnected" << endl;
                    }
                    else
                    {
                        cout << "Some problems with disconnecting: " + GetLastError() << endl;
                    }
                }
                else
                {
                    cout << "First of all need to be connected" << endl;
                }
                system("pause");
                break;
            }

            case '0':
            {
                break;
            }

            default:
            {
                cout << "There is no such paragraph!" << endl;
                system("pause");
                break;
            }
        }
    } while (choice != '0');

}
int main (){
    ui();
    return 0;
}
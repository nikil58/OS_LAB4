#include <iostream>
#include <windows.h>
#include "main_menu.h"

using namespace std;

void ui(){
    char choice;
    HANDLE e = CreateEventA(nullptr, false, false, nullptr);
    HANDLE namedPipe;
    bool conn = false;
    do {
        choice = input (server_menu);
        switch (choice)
        {
            case '1':
            {
                namedPipe = CreateNamedPipe("\\\\.\\pipe\\LAB4_2", PIPE_ACCESS_DUPLEX,
                                              PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                                            PIPE_UNLIMITED_INSTANCES, 512, 512, 0, nullptr);
                break;
            }

            case '2':
            {
                if (namedPipe) {
                    OVERLAPPED overlp = OVERLAPPED();
                    overlp.hEvent = e;
                    cout << "Waiting for client's connection" << endl;
                    conn = ConnectNamedPipe(namedPipe, &overlp);

                    WaitForSingleObject(e, INFINITE);
                    if (conn)
                        cout << "Connected" << endl;
                    else
                        cout << "Connection failed: error " << GetLastError() << endl;
                }
                else {
                    cout << "First of all create a named pipe!" << endl;
                }
                system("pause");
                break;
            }

            case '3':
            {
                if (conn)
                {
                    OVERLAPPED overlapped = OVERLAPPED();
                    cout << "Enter the message" << endl;
                    char msg[MAX_PATH];
                    cin.getline(msg, MAX_PATH);
                    overlapped.hEvent = e;
                    conn = WriteFile(namedPipe, (LPCVOID)msg, 512, nullptr, &overlapped);
                }
                else
                {
                    cout << "First of all need to be connected to the pipe" << endl;
                }
                system("pause");
                break;
            }

            case '4':
            {
                if (conn)
                {
                    conn = DisconnectNamedPipe(namedPipe);
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

int main()
{
    ui();
    return 0;
}

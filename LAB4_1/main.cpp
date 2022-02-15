#include <windows.h>
#include <vector>
#include <string>
#include <iostream>


using namespace std;

HANDLE createProc(string path, string log);

int main()
{
    const int pgCount = 3 + 0 + 7 + 1 + 5;
    const int pgSize = 4096;
    const int numOfProc = 8;
    const int fileSize =  pgSize * pgCount;
    vector <HANDLE> processes(pgCount, nullptr);
    vector <HANDLE> unusedSemaphores (pgCount, nullptr);
    vector <HANDLE> usedSemaphores (pgCount, nullptr);

    for (int i = 0; i < pgCount; i++)
    {
        unusedSemaphores[i] = CreateSemaphore(nullptr, 1, 1, ("unusedSemaphores" +
                                                              to_string(i)).c_str());
        usedSemaphores[i] = CreateSemaphore(nullptr, 0, 1, ("usedSemaphores" + to_string(i)).c_str());
    }

    HANDLE mutex = CreateMutex(nullptr, false, "mutex");
    HANDLE handleF = nullptr;
    HANDLE handleM = nullptr;


    handleF = CreateFileA("handleF.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, 0, nullptr);
    if (handleF == INVALID_HANDLE_VALUE) {
        cout << "Error " << GetLastError() << " occurred" << endl;
        return -1;
    }


    handleM = CreateFileMappingA(handleF, nullptr, PAGE_READWRITE, 0, fileSize, "handleM");
    if (handleM == nullptr) {
        CloseHandle(handleF);
        cout << "Error " << GetLastError() << " occurred" << endl;
        return -1;
    }

    cout << "Start create file" << endl;
    for (int i = 0; i < numOfProc; i++){
        processes[i] = createProc("D:\\Programming\\C++\\5 Semestr\\LAB4_1\\cmake-build-debug\\writer.exe", "D:\\Programming\\C++\\5 Semestr\\LAB4_1\\cmake-build-debug\\WriterLog" +
                to_string(i + 1) + ".txt");
        processes[numOfProc + i] = createProc("D:\\Programming\\C++\\5 Semestr\\LAB4_1\\cmake-build-debug\\reader.exe", "D:\\Programming\\C++\\5 Semestr\\LAB4_1\\cmake-build-debug\\ReaderLog" +
                to_string(i + 1) + ".txt");
    }

    cout << "Wait for their work" << endl;
    WaitForMultipleObjects(pgCount, processes.data(), true, INFINITE);

    for (int i = 0; i < pgCount; i++){
        CloseHandle(processes[i]);
    }
    CloseHandle(handleM);
    CloseHandle(handleF);
    CloseHandle(mutex);

    for (int i = 0; i < pgCount; i++) {
        CloseHandle(unusedSemaphores[i]);
        CloseHandle(usedSemaphores[i]);
    }

    cout << "The end of the program" << endl;
    system("pause");
    return 0;
}


HANDLE createProc(string path, string log){
    STARTUPINFO systemInfo;
    PROCESS_INFORMATION processInfo;
    SECURITY_ATTRIBUTES secureAttributes = { sizeof(secureAttributes), nullptr, true };
    ZeroMemory(&systemInfo, sizeof(systemInfo));

    HANDLE handleLog = CreateFile(log.data(), GENERIC_WRITE, FILE_SHARE_WRITE, &secureAttributes,
                                  OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
   systemInfo.cb = sizeof(systemInfo);
   systemInfo.hStdOutput = handleLog;
   systemInfo.hStdError = nullptr;
   systemInfo.hStdInput = nullptr;
   systemInfo.dwFlags = STARTF_USESTDHANDLES;

    ZeroMemory(&processInfo, sizeof(processInfo));

    int writerProcess = CreateProcess(path.data(), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &systemInfo, &processInfo);

    return (writerProcess != 0 ? processInfo.hProcess : nullptr);
}
#include <windows.h>
#include <vector>
#include <string>

using namespace std;

int main () {
    const int pgCount = 3 + 0 + 7 + 1 + 5;
    const int pgSize = 4096;
    const int fileSize =  pgSize * pgCount;
    string logger = "";
    vector <HANDLE> unusedSemaphores (pgCount, nullptr);
    vector <HANDLE> usedSemaphores (pgCount, nullptr);
    HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, false, "mutex");
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE handleM = OpenFileMappingA(FILE_MAP_WRITE, false, "handleM");
    HANDLE mapView = MapViewOfFile(handleM, FILE_MAP_ALL_ACCESS, 0, 0, fileSize);

    for (int i = 0; i < pgCount; i++)
    {
        unusedSemaphores[i] = OpenSemaphoreA(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, false, ("unusedSemaphores" +
                to_string(i)).c_str());
        usedSemaphores[i] = OpenSemaphoreA(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, false, ("usedSemaphores" + to_string(i)).c_str());
    }

    VirtualLock(mapView, fileSize);

    if (handleM){
        for (int i = 0; i < 3; i++){
            int page = WaitForMultipleObjects(pgCount, unusedSemaphores.data(), false, INFINITE);
            logger = "taking unused semaphores for page " + to_string(page) + " at " + to_string(GetTickCount()) + "\n";
            WriteFile (out, logger.data(), logger.length(), nullptr, nullptr);

            WaitForSingleObject(mutex, INFINITE);
            logger = "taking mutex for page " + to_string(page) + " at " + to_string(GetTickCount()) + "\n";
            WriteFile (out, logger.data(), logger.length(), nullptr, nullptr);

            logger = "starting write at " + to_string(GetTickCount()) + "\n";
            WriteFile (out, logger.data(), logger.length(), nullptr, nullptr);
            SleepEx(rand() % 1000 + 500, false);
            logger = "stop write at " + to_string(GetTickCount()) + "\n";
            WriteFile (out, logger.data(), logger.length(), nullptr, nullptr);

            if (ReleaseMutex(mutex))
            {
                logger = "Getting back mutex at " + to_string(GetTickCount()) + "\n";
                WriteFile(out, logger.data(), logger.length(), nullptr, nullptr);
            }
            else
            {
                logger = to_string(GetLastError()) + " CODE mutex \n";
                WriteFile(out, logger.data(), logger.length(), nullptr, nullptr);
            }

            if (ReleaseSemaphore(usedSemaphores[page], 1, nullptr))
            {
                logger = "Getting back used semaphore at " + to_string(GetTickCount()) + "\n";
                WriteFile(out, logger.data(), logger.length(), nullptr, nullptr);
            }
            else
            {
                logger = to_string(GetLastError()) + " CODE semaphore \n";
                WriteFile(out, logger.data(), logger.length(), nullptr, nullptr);
            }
        }
    }
    else {
        logger = to_string(GetLastError()) + " CODE mapping write \n";
        WriteFile(out, logger.data(), logger.length(), nullptr, nullptr);
    }
    VirtualUnlock(mapView, fileSize);
}
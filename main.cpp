
#include <windows.h>
#include "stdio.h"
#include "lib/WString.h"
#include "src/USART.h"
#include "src/SMH.h"


DWORD WINAPI MyThreadFunction( LPVOID lpParam );

int main() {
    USART Serial;
    SMH.set_printer(&Serial);
    SMH.Start();

    DWORD   dwThreadId;
    CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size
            MyThreadFunction,       // thread function name
            0,          // argument to thread function
            0,                      // use default creation flags
            &dwThreadId);   // returns the thread identifier


    while (1)
    {
        Sleep(1000);
        printf(".");
    }
    return 0;
}

DWORD WINAPI MyThreadFunction( LPVOID lpParam )
{
    HANDLE hStdout;
    while (1)
    {
        Sleep(367);
        printf("!");
    }
}
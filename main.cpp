
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include "stdio.h"
#include "lib/WString.h"
#include "src/USART.h"
#include "src/SMH.h"


USART Serial;

void* PosixThreadMainRoutine(void* data)
{
    // Do some work here.
    while (1)
    {
        usleep(367);
    }

    return NULL;
}

int main() {
    SMH.set_printer(&Serial);
    SMH.Start();

// Create the thread using POSIX routines.
    pthread_attr_t  attr;
    pthread_t       posixThreadID;
    int             returnVal;

    returnVal = pthread_attr_init(&attr);
    assert(!returnVal);
    returnVal = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    assert(!returnVal);

    int     threadError = pthread_create(&posixThreadID, &attr, &PosixThreadMainRoutine, NULL);

    returnVal = pthread_attr_destroy(&attr);
    assert(!returnVal);
    if (threadError != 0)
    {
        // Report an error.
    }

    while (1)
    {
        usleep(1000);
    }
    return 0;
}


#ifndef ASYNCIO_H
#define ASYNCIO_H
/*
 * Includes & defines
*/
#ifdef __linux__
// linux includes
#include <aio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#else
// windows includes
#endif

// default includes
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <list>
#include <string>
#include <fstream>
#include <iostream>

#define BUFFER_SIZE 256
typedef std::pair<aiocb*, char *> Message;

/*
 * Code
*/

// for the best shutdowns ever
#define criticalError(msg) { std::cout << "critical error: "\
                                       << msg << std::endl; \
                             exit(1);}

// todo: delete it
#define debug(msg) {std::cout << "debug: " << msg << std::endl;}

#define assert(condition) {if (!(condition)) { std::cout << "assertion error" << std::endl; exit(1);}}

#define Thread pthread_t
#define ThreadMutex pthread_mutex_t
#define semaphore sem_t *
namespace globals
{
    extern std::queue<std::pair<aiocb *, char *> > messages;
    extern long todoWriteBlockCount;
    extern ThreadMutex countMutex;
    extern ThreadMutex iomutex;
}

void *
writer(void * arg);

std::list<int>
reader(std::list<std::string> names);
#endif // ASYNCIO_H

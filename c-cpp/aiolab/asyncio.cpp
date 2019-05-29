#include "asyncio.h"
using namespace std;

namespace globals
{
    std::queue<std::pair<aiocb *, char *> > messages;
    long todoWriteBlockCount = 0;
    ThreadMutex countMutex;
    ThreadMutex iomutex;
}

void
readHandler(sigval_t sigval)
{
    semaphore writerWaitSem;

    if ((writerWaitSem = sem_open("2282281", O_CREAT, 0644, 0)) == SEM_FAILED)
    {
        debug(errno)
        criticalError("sem failed")
    }

    sem_post(writerWaitSem);

    sem_close(writerWaitSem);
}

void
writeHandler(sigval_t sigval)
{
    pthread_mutex_lock(&globals::countMutex);

    globals::todoWriteBlockCount--;

    pthread_mutex_unlock(&globals::countMutex);
}

void
incBlockCount()
{
    pthread_mutex_lock(&globals::countMutex);

    globals::todoWriteBlockCount++;

    pthread_mutex_unlock(&globals::countMutex);
}

Message
readMessage()
{
    pthread_mutex_lock(&globals::iomutex);

    Message message = globals::messages.front();
    globals::messages.pop();

    pthread_mutex_unlock(&globals::iomutex);

    return message;
}

// writer thread routine
void *
writer(void * arg)
{
    long offset = 0;
    FILE* output = fopen("output.txt", "w");
    long outputDesc = fileno(output);
    assert(outputDesc >= 0);
    list<Message> todoWrite;
    list<Message> todoWriteCopy; // super - crutch

    semaphore end;
    if ((end = sem_open("end13372281", O_CREAT, 0644, 0)) == SEM_FAILED)
    {
        debug(errno)
        criticalError("sem failed")
    }
    semaphore writerWaitSem;

    if ((writerWaitSem = sem_open("2282281", O_CREAT, 0644, 0)) == SEM_FAILED)
    {
        criticalError("sem failed")
    }

    while(true)
    {
        // wait until any op ends
        sem_wait(writerWaitSem);

        // if readers work is done, then end it
        if (globals::messages.empty() && !sem_trywait(end))
        {
            debug(globals::todoWriteBlockCount)
            while(globals::todoWriteBlockCount)
            {
                todoWriteCopy.clear();
                for(Message& mes : todoWrite)
                {
                    if(aio_error(mes.first) == EINPROGRESS)
                    {
                        todoWriteCopy.push_back(mes);
                    }
                    else
                    {
                        free(mes.first);
                        free(mes.second);
                    }
                }
                todoWrite = todoWriteCopy;
            }

            fclose(output);
            sem_close(end);
            sem_close(writerWaitSem);

            return 0;
        }

        // check state of head
        // write to console result
        if(!globals::messages.empty())
        {

            while (aio_error(globals::messages.front().first) != EINPROGRESS)
            {
                Message message = readMessage();

                // redirect to write
                message.first->aio_fildes = outputDesc;
                message.first->aio_offset = offset;
                offset += message.first->aio_nbytes;
                message.first->aio_lio_opcode = LIO_WRITE;
                message.first->aio_sigevent.sigev_notify = SIGEV_THREAD;
                message.first->aio_sigevent.sigev_notify_function = writeHandler;

                aio_write(message.first);
                incBlockCount();
                todoWrite.push_back(message);

                if (globals::messages.empty())
                {
                    break;
                }
            }
        }
    }

    return 0;
}

list<string>
formNamesList(int argc, char* argv[])
{
    // go crazy, go stupid (have no time)
    // better to do it with iterators
    list<string> result;

    if (argc <= 1)
    {
        criticalError("incorrect input, need config file name")
    }

    std::ifstream files(argv[1]);

    if (!files.is_open())
    {
        criticalError("can't open file")
    }

    std::string name;
    while (std::getline(files, name))
    {
        result.push_back(name);
    }

    files.close();

    return result;
}

Message
makeMessage()
{
    // alloc buffer
    long attempt = 10;
    char * bufferPtr;

    while(attempt --> 0)
    {
        if ((bufferPtr = (char*) malloc(BUFFER_SIZE)) != 0)
        {
            break;
        }
    }

    if ((attempt < 0) && (!bufferPtr))
    {
        criticalError("can't alloc buffer")
    }

    // alloc & contstruct aiocb

    attempt = 10;
    aiocb * aiocbPtr;

    while(attempt --> 0)
    {
        if ((aiocbPtr = (aiocb*) malloc(sizeof(aiocb))) != 0)
        {
            break;
        }
    }

    if ((attempt < 0) && (!aiocbPtr))
    {
        criticalError("can't alloc aiocb")
    }

    // setup a callback for notifying
    memset(aiocbPtr, 0, sizeof(aiocb));
    aiocbPtr->aio_sigevent.sigev_notify = SIGEV_THREAD;
    aiocbPtr->aio_sigevent.sigev_notify_function = readHandler;

    // give result pair
    return std::make_pair(aiocbPtr, bufferPtr);
}

void
commitMessage(Message message)
{
    pthread_mutex_lock(&globals::iomutex);

    globals::messages.push(message);

    pthread_mutex_unlock(&globals::iomutex);
}

void
initMutexes()
{
    if (pthread_mutex_init(&globals::iomutex, NULL))
    {
        criticalError("can't create mutex")
    }

    if (pthread_mutex_init(&globals::countMutex, NULL))
    {
        criticalError("can't create mutex")
    }
}


// reader thread routine
list<int>
reader(list<string> names)
{
    list<int>    todoClose;
    long offset, size;

    // for the every file read it asyncroniosly with buffer
    for (string name : names)
    {
        int fileDescriptor = open(name.c_str(), O_RDONLY);
        struct stat st;
        fstat(fileDescriptor, &st);
        size = st.st_size;

        for (offset = 0; offset < (size - BUFFER_SIZE); offset += BUFFER_SIZE)
        {
          // alloc message
          Message message = makeMessage();

          // form aiocb
          message.first->aio_buf    = message.second;
          message.first->aio_fildes = fileDescriptor;
          message.first->aio_offset = offset;
          message.first->aio_nbytes = BUFFER_SIZE;
          message.first->aio_lio_opcode = LIO_READ;

          // read from some of files
          aio_read(message.first);

          commitMessage(message);
        }

        if ((size - offset) > 0)
        {
          // alloc message
          Message message = makeMessage();

          // form aiocb
          message.first->aio_buf    = message.second;
          message.first->aio_fildes = fileDescriptor;
          message.first->aio_offset = offset;
          message.first->aio_nbytes = size - offset;
          message.first->aio_lio_opcode = LIO_READ;

          // read from some of files
          aio_read(message.first);

          commitMessage(message);
        }

        todoClose.push_back(fileDescriptor);
    }

    return todoClose;
}

extern "C"
void
mainLike(int argc, char* argv[])
{
    // init syncronization
    semaphore end;
    if ((end = sem_open("end13372281", O_CREAT, 0644, 0)) == SEM_FAILED)
    {
        debug(errno)
        criticalError("sem failed")
    }

    initMutexes();

    Thread writerDesc;
    // init writer
    if (pthread_create(&writerDesc, 0, writer, 0))
    {
        criticalError("can't create writer")
    }

    // init reader
    list<string> names = formNamesList(argc, argv);
    list<int> todoClose = reader(names);

    sem_post(end);
    sem_post(end);

    semaphore writerWaitSem;

    if ((writerWaitSem = sem_open("2282281", O_CREAT, 0644, 0)) == SEM_FAILED)
    {
        debug(errno)
        criticalError("sem failed")
    }

    sem_post(writerWaitSem);

    sem_close(writerWaitSem);
    pthread_join(writerDesc, 0);
    sem_close(end);

    sem_unlink("2282281");
    sem_unlink("end13372281");

    for (auto& todo : todoClose)
    {
        close(todo);
    }
}

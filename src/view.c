#include "view.h"

int main(int argc, char *argv[]) {

    if (setvbuf(stdin, NULL, _IONBF, BUFFER_SIZE) != 0) {
        errorHandler("Error performing setvbuf in main (view)");
    }

    // Reciving data to initialize
    int shmSize;

    if (argc == 1) {
        char buff[MEMORY_LONG] = {0};
        if(read(STDIN, buff, MEMORY_LONG) == ERROR_CODE) {
            errorHandler("Error reading data for initializing shared memory (view)");
        }
        shmSize = atoi(buff);
    } else if (argc == 2) {
        shmSize = atoi(argv[1]);
    } else {
        errorHandler("Error: invalid amount of arguments (view)");
    }

    shmSize *= BUFFER_SIZE;

    if(shmSize <= 0) {
        errorHandler("Error defining size of shared memory (view)");
    }


    // Opening shared memory and semaphores
    int shmFd;
    void * shMemory;
    sem_t * sem;

    if((shmFd = shm_open(SHM_NAME, O_RDWR, 0666)) == ERROR_CODE) {
        errorHandler("Error opening shared memory (view)");
    }

    if((shMemory = mmap(NULL, shmSize, PROT_READ, MAP_SHARED, shmFd, 0)) == MAP_FAILED) {
        errorHandler("Error mapping shared memory (view)");
    }

    if ((sem = sem_open(SEM_NAME, O_CREAT, 0600, INIT_VAL_SEM)) == SEM_FAILED) {
        errorHandler("Error opening semaphore (view)");
    }
    void *shmPtr = shMemory;


    // Showing results
    handleData(sem,(char*)(shMemory), shmSize);


    // Closing shared memory and semaphores
    closeSemaphore(sem);
    close(shmFd);

    unmapSharedMemory(shmPtr,shmSize);

    return 0;
}

void handleData(sem_t * sem,char * shMemory, int size) {
    int i = 0; 
    int fileNumber = 1;

    while(i < size) {

        waitSemaphore(sem);

        printf("%d %s",  fileNumber, shMemory);
        fileNumber++;
        shMemory += JUMP;
        i += JUMP;
    }
}

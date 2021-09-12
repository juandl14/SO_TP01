#include "view.h"

int main(int argc, char *argv[]) {

    if (setvbuf(stdin, NULL, _IONBF, 0) != 0) {
        errorHandler("Error performing setvbuf in main (view)");
    }

    // Reciving data to initialize
    int shmSize;

    if (argc == 1) {
        if(read(STDIN, &shmSize, sizeof(int)) == ERROR_CODE) {
            errorHandler("Error reading data for initializing shared memory (view)");
        }
    } else if (argc == 2) {
        shmSize = atoi(argv[1]);
    } else {
        errorHandler("Error: invalid amount of arguments (view)");
    }

    if(shmSize <= 0) {
        errorHandler("Error defining size of shared memory (view)");
    }

    // Opening shared memory and semaphores
    int shmFd;
    char * shMemory;
    sem_t * sem;

    // unlinkSemaphore();
    if ((sem = sem_open(SEM_NAME, O_RDWR)) == SEM_FAILED) {
        errorHandler("Error opening semaphore (view)");
    }

    //TODO mode and permissions
    if((shmFd = shm_open(SHM_NAME,O_CREAT | O_RDONLY,0)) == ERROR_CODE) {
        errorHandler("Error opening shared memory (view)");
    }

    // TODO prot
    if((shMemory = (char *) mmap(NULL, shmSize, PROT_READ, MAP_SHARED, shmFd, 0)) == MAP_FAILED) {
        errorHandler("Error mapping shared memory (view)");
    }


    // Showing results
    handleData(sem,shMemory);


    // Closing shared memory and semaphores
    closeSemaphore(sem);
    close(shmFd);

    unmapSharedMemory(shMemory,shmSize);
    unlinkSharedMemory();
}

void handleData(sem_t * sem,char * shMemory) {
    while(1) {
        waitSemaphore(sem);
        if(*shMemory == 0) {
            break;
        }
        int move = printf("%s",shMemory);
        shMemory += (move + 1) * sizeof(*shMemory);
    }
}

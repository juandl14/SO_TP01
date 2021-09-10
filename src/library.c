#include "library.h"

void errorHandler(const char *errorMsg) {
    char finalMsg[ERROR_MSG]={0};
    strcat(finalMsg,errorMsg);
    strcat(finalMsg,"\n");
    perror(finalMsg);
    exit(EXIT_FAILURE);
}

void closeSemaphore(sem_t * sem) {
    if(sem_close(sem) == ERROR_CODE) {
        unlinkSemaphore();
        errorHandler("Error closing semaphore");
    }
}

void unlinkSemaphore() {
    if(sem_close(SEM_NAME) == ERROR_CODE) {
        errorHandler("Error unlinking semaphore");
    }
}

void openSharedMemory(void * shMemory, int * shmFd, int setSize, int size) {
    *shmFd = shm_open(SHM_NAME,O_CREAT | O_RDWR,0); //todo mode
    if(*shmFd == ERROR_CODE) {
        errorHandler("Error opening shared memory");
    }

    if (setSize == TRUE) {
        if(ftruncate(*shmFd,size) == ERROR_CODE) {
            errorHandler("Error setting size to shared memory");
        }
    }

    shMemory = mmap(0,size,PROT_READ | PROT_WRITE,MAP_SHARED,*shmFd,0); // todo prot
    if(shMemory == MAP_FAILED) {
        errorHandler("Error mapping shared memory");
    }

    return;
}

void closeSharedMemory() {

}
#include "library.h"

/*** *** *** ERROR HANDLING *** *** *** *** *** *** ***/

void errorHandler(const char *errorMsg) {
    char finalMsg[ERROR_MSG] = {0};
    strcat(finalMsg,errorMsg);
    strcat(finalMsg,"\n");
    perror(finalMsg);
    exit(EXIT_FAILURE);
}


/*** *** *** SEMAHPHORE HANDLING *** *** *** *** *** *** ***/

void postSemaphore(sem_t *sem) {
    if(sem_post(sem) == ERROR_CODE) {
        errorHandler("Error in sem_post");
    }
}

void waitSemaphore(sem_t * sem) {
  if(sem_wait(sem) == ERROR_CODE) {
      errorHandler("Error: sem_wait has failed");
  }
}

void closeSemaphore(sem_t * sem) {
    if(sem_close(sem) == ERROR_CODE) {
        unlinkSemaphore();
        errorHandler("Error closing semaphore");
    }
}

void unlinkSemaphore() {
    if(sem_unlink(SEM_NAME) == ERROR_CODE) {
        errorHandler("Error unlinking semaphore");
    }
}


/*** *** *** SHARED MEMORY HANDLING *** *** *** *** *** *** ***/

void unmapSharedMemory(void * memory, int size) {
    if(munmap( memory,size) == ERROR_CODE) {
        errorHandler("Error unmapping shared memory");
    }
}

void unlinkSharedMemory() {
    if(shm_unlink(SHM_NAME)==ERROR_CODE) {
        errorHandler("Error unlinking shared memory");
    }
}

void setBuffer(FILE * stream,size_t size) {
        if (setvbuf(stream, NULL, _IONBF, size) != 0) {
        errorHandler("Error setting buffer in main (app)");
    }
}
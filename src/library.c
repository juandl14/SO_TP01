#include <library.h>

void errorHandler(const char *errorMsg) {
    char finalMsg[ERROR_MSG] = {0};
    strcat(finalMsg,errorMsg);
    strcat(finalMsg,"\n");
    perror(finalMsg);
    exit(EXIT_FAILURE);
}

void postSemaphore(sem_t *sem) {
    if(sem_post(sem) == ERROR_CODE) {
        errorHandler("Error in sem_post");
    }
}

void waitSemaphore(sem_t * sem) {
  if(sem_wait(sem) == ERROR_CODE) {
      errorHandler("Error: sem_wait has failed (view)");
  }
}

void closeSemaphore(sem_t * sem) {
    if(sem_close(sem) == ERROR_CODE) {
        // unlinkSemaphore();
        errorHandler("Error closing semaphore");
    }
}

// void unlinkSemaphore() {
//     if(sem_close(SEM_NAME) == ERROR_CODE) {
//         errorHandler("Error unlinking semaphore");
//     }
// }

void unmapSharedMemory(char * memory, int size) {
    if(munmap(memory,size) == ERROR_CODE) {
        errorHandler("Error unmapping shared memory (view)");
    }
}

void unlinkSharedMemory() {
    if(shm_unlink(SHM_NAME)==ERROR_CODE) {
        errorHandler("Error unlinking shared memory");
    }
}

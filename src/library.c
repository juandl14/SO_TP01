#include "library.h"

void errorHandler(const char *errorMsg) {
    char finalMsg[ERROR_MSG]={0};
    strcat(finalMsg,errorMsg);
    strcat(finalMsg,"\n");
    perror(finalMsg);
    exit(EXIT_FAILURE);
}

sem_t *openSemaphore() {
    sem_t *sem;
    if ((sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {
        errorHandler("Error opening semaphore in function openSemaphore (app)");
    }
    return sem;
}

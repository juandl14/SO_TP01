#include "view.h"

int main(int argc, char *argv[]) {

    if (setvbuf(stdin, NULL, _IONBF, BUFFER_SIZE) != 0) {
        errorHandler("Error performing setvbuf in main (view)");
    }

    // Reciving data to initialize
    int shmSize, taskCount;

    // printf("%d\n", argc);

    if (argc == 1) {
        char buff[MEMORY_LONG] = {0};
        int len;
        if(len = read(STDIN, buff, MEMORY_LONG) == ERROR_CODE) {
            errorHandler("Error reading data for initializing shared memory (view)");
        }
        shmSize = atoi(buff);
        // char *dest = malloc(len * sizeof(char));
        // strncpy(dest, buff, len);
        // char *tok = strtok(dest, " ");
        // shmSize = atoi(tok);
        // *tok = strtok(NULL, " ");
        // taskCount = atoi(tok);
        // free(dest);
    } else if (argc == 2) {
        shmSize = atoi(argv[1]);
        taskCount = atoi(argv[2]);
    } else {
        errorHandler("Error: invalid amount of arguments (view)");
    }

    // printf("%d\n", shmSize);

    if(shmSize <= 0) {
        errorHandler("Error defining size of shared memory (view)");
    }

    // Opening shared memory and semaphores
    int shmFd;
    void * shMemory;
    sem_t * sem;

    /*  unlinkSemaphore();
    if ((sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, INIT_VAL_SEM)) == SEM_FAILED) {
        errorHandler("Error opening semaphore (view)");
    }

    if((shmFd = shm_open(SHM_NAME, O_RDWR, 0666)) == ERROR_CODE) { */
    if((shmFd = shm_open(SHM_NAME, O_RDWR, 0666)) == ERROR_CODE) {
        errorHandler("Error opening shared memory (view)");
    }

    if((shMemory = mmap(NULL, shmSize, PROT_READ, MAP_SHARED, shmFd, 0)) == MAP_FAILED) {
        errorHandler("Error mapping shared memory (view)");
    }

    if ((sem = sem_open(SEM_NAME, O_CREAT, 0600, INIT_VAL_SEM)) == SEM_FAILED) {
        errorHandler("Error opening semaphore (view)");
    }

    // Showing results
    handleData(sem,(char*)(shMemory));


    // Closing shared memory and semaphores
    closeSemaphore(sem);
    close(shmFd);

    unmapSharedMemory(shMemory,shmSize);
    unlinkSharedMemory();

    return 0;
}

void handleData(sem_t * sem,char * shMemory) {
    int aux = 1; int var = 1;
    while(aux) {

        waitSemaphore(sem);

        if(*shMemory == 0) {
            aux = 0;
        } else {
            printf("%d %s",  var, shMemory);
            var++;
            shMemory += JUMP;
        }
    }
}

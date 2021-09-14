#include "view.h"

int main(int argc, char *argv[]) {

    if (setvbuf(stdin, NULL, _IONBF, BUFFER_SIZE) != 0) {
        errorHandler("Error performing setvbuf in main (view)");
    }

    // Reciving data to initialize
    int shmSize, taskCount;

    // printf("%d\n", argc);

    if (argc == 1) {
        char buff[MEMORY_LEN] = {0};
        int len;
        if((len = read(STDIN, buff, MEMORY_LEN)) == ERROR_CODE) {
            errorHandler("Error reading data for initializing shared memory (view)");
        }
        char dest[len];
        strncpy(dest, buff, len);
        char *tok = strtok(dest, " ");
        if (tok == NULL) {
            errorHandler("Error in strtok 1 (view)");
        }
        shmSize = atoi(tok);
        tok = strtok(NULL, " ");
        if (tok == NULL) {
            errorHandler("Error in strtok 2 (view)");
        }
        taskCount = atoi(tok);
    } else if (argc == 2) {
        shmSize = atoi(argv[1]);
        taskCount = atoi(argv[2]);
    } else {
        errorHandler("Error: invalid amount of arguments (view)");
    }


    shmSize *= BUFFER_SIZE;
    // printf("%d\n", shmSize);
    /* impl juan: */
    //printf("%d\n", taskCount);

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
    void *shmPtr = shMemory;

    // Showing results
    handleData(sem,(char*)(shMemory), shmSize);
    //handleData(sem,(char*)(shMemory), taskCount);


    // Closing shared memory and semaphores
    closeSemaphore(sem);
    close(shmFd);

    unmapSharedMemory(shmPtr,shmSize);
    //unmapSharedMemory(shMemory,shmSize);

    return 0;
}

//void handleData(sem_t * sem,char * shMemory, int taskCount) {
//    int var = 1;
//    while(var <= taskCount) {
//
//        waitSemaphore(sem);
//        printf("%d %s",  var, shMemory);
//        var++;
//        shMemory += JUMP;
//    }
//}

void handleData(sem_t * sem,char * shMemory, int size) {
    int aux = 0; int var = 1;
    while(aux < size) {

        waitSemaphore(sem);

        printf("%d %s",  var, shMemory);
        var++;
        shMemory += JUMP;
        aux += JUMP;
    }
}

#include "app.h"

int main(int argc, char *argv[]) {
    sem_unlink(SEM_NAME);

    if (argc < 2) {
        errorHandler("Error: incorrect amount of arguments (app)");
    }

    /* If I have less arguments than SLAVE_AMOUNT, I create
    ** as many children as arguments, since I do not want more children than arguments
    */
    int taskCount = argc - 1;
    int slaveAmount =  MIN(SLAVE_AMOUNT,taskCount);
    int filesPerSlave = (taskCount/SLAVE_AMOUNT >= 2)? 2 : 1;

    printf("%d - %d - %d\n", taskCount, slaveAmount, filesPerSlave);

    // FILE *resultFile;

    Tslave slavesArray[slaveAmount];

    /*
    ** Opening shared memory and semaphores
    */
    int shmFd;
    void * shMemory;
    int shmSize = taskCount * BUFFER_SIZE;


    if (setvbuf(stdout, NULL, _IONBF, BUFFER_SIZE) != 0) {
        errorHandler("Error setting buffer in main (app)");
    }

    if ((shmFd = shm_open(SHM_NAME,O_CREAT | O_RDWR,0777)) == ERROR_CODE) {
        errorHandler("Error opening shared memory (app)");
    }

    if (ftruncate(shmFd,shmSize) == ERROR_CODE) {
        errorHandler("Error setting size to shared memory (app)");
    }

    shMemory = mmap(NULL, shmSize, PROT_READ/* | PROT_WRITE*/, MAP_SHARED, shmFd, 0); // todo prot
    if (shMemory == MAP_FAILED) {
        errorHandler("Error mapping shared memory (app)");
    }

    sem_t *sem;
    if ((sem = sem_open(SEM_NAME, O_CREAT | O_RDWR, 0600, INIT_VAL_SEM)) == SEM_FAILED) {
        errorHandler("Error opening semaphore (app)");
    }

    write(STDOUT, &shmSize, sizeof(int));
    sleep(2);
    

    /*
    ** Handling slaves and files
    */
    createChildren(slavesArray, slaveAmount, /*argv + 1,*/ SLAVE_PATH, NULL);

    // Send first files to the slaves
    int tasksInProgress = 0;
    int tasksFinished = 0;

    int initialPaths = filesPerSlave * slaveAmount;
    sendInitFiles(slavesArray, slaveAmount, argv, initialPaths, &tasksInProgress, &tasksFinished);

    char buffer[BUFFER_SIZE] = {0};

    // TEST
    // FILE *test = fopen("test.txt", "w");
    // fprintf(test, "antes del while");


    while(tasksFinished < taskCount) {
        fd_set readFdSet;
        FD_ZERO(&readFdSet);

        // int max = chargeReadSet(&readFdSet, slavesArray, slaveAmount);

        //
        int max = -1;
        int currentFd;

        for(int i = 0; i < slaveAmount; i++) {
            if(slavesArray[i].working) {
                currentFd = slavesArray[i].in;
                max = MAX(max, currentFd);
                FD_SET(currentFd, &readFdSet);
            }
        }

        int ready;
        if((ready = select(max + 1, &readFdSet, NULL, NULL, NULL)) == ERROR_CODE) {
            errorHandler("Error in select (app)");
        }

        for(int i = 0; i < slaveAmount && ready > 0; i++) {
            int fd = slavesArray[i].in;
            if(FD_ISSET(fd, &readFdSet)) {
                // printf("entro");
                int dimRead = read(fd, buffer, BUFFER_SIZE);
                if (dimRead == ERROR_CODE) {
                    errorHandler("Error reading from fdData (app)");
                } else if (dimRead <= 0) {
                    slavesArray[i].working = 0;
                } else {
                    tasksFinished++;
                    ready--;
                    int move = fprintf(shMemory, "%s\n", buffer);
                    shMemory += (move + 1) * sizeof(*shMemory);
                    postSemaphore(sem);
                }

            }

        }

    }

    endChildren(slavesArray, slaveAmount);

    /*
    ** Closing shared memory and semaphores
    */
    closeSemaphore(sem);
    unlinkSemaphore();

    close(shmFd);
    unmapSharedMemory(shMemory,shmSize);
    unlinkSharedMemory();
}

// int chargeReadSet(fd_set *fdReadSet, Tslave slavesArray[], int slaveAmount) {
//     int max = -1;
//     int currentFd;
//
//     for(int i = 0; i < slaveAmount; i++) {
//         if(slavesArray[i].working) {
//             currentFd = slavesArray[i].in;
//
//             FD_SET(currentFd, *fdReadSet);
//             max = MAX(max, currentFd);
//         }
//     }
//     return max;
// }

void createChildren(Tslave slavesArray[], int slaveAmount, char *path, char *const argv[]) {
    pid_t pid;

    for (int i = 0; i < slaveAmount; i++) {
        /* Creating the pipes :
        ** fdPath goes from the master to the slave and
        ** fdData goes from the slave to the master
        */
        int fdPath[2], fdData[2];

        if (pipe(fdPath) == ERROR_CODE || pipe(fdData) == ERROR_CODE) {
            errorHandler("Error creating pipe in function createChildren (app)");
        }

        if ((pid = fork()) != ERROR_CODE) {

            if (pid == 0) {

                close(fdPath[WRITE_FD]);
                close(fdData[READ_FD]);
                // close(READ_FD);
                // close(WRITE_FD);
                // printf("%d %d\n",fdPath[READ_FD],fdData[WRITE_FD]);
                if (dup2(fdPath[READ_FD], 0) == ERROR_CODE || dup2(fdData[WRITE_FD], 1) == ERROR_CODE) {
                    errorHandler("Error performing dup2 in function createChildren (app)");
                }

                close(fdPath[READ_FD]);
                close(fdData[WRITE_FD]);

                if (execv(path, argv) == ERROR_CODE) {
                    errorHandler("Error performing execv in function createChildren (app)");
                }

                // -------------TEST---------------------
                // printf("Soy el hijo. Mi PID es %d y el de mi padre, %d\n", getpid(), getppid());
                // sleep(2);
                // printf("Proceso hijo terminado.\n");
            } else {
                close(fdPath[READ_FD]);
                close(fdData[WRITE_FD]);

                slavesArray[i].pid = pid;
                slavesArray[i].in = fdData[READ_FD];
                slavesArray[i].out = fdPath[WRITE_FD];
                slavesArray[i].working = 1;

                // -------------TEST---------------------
                // printf("Soy el padre. Mi PID es %d y el de mi hijo, %d\n", getpid(), pid);
                // sleep(3);
                // waitpid(pid, NULL, 0);
                // printf("Proceso padre terminado.\n");
            }
        } else {
            errorHandler("Error forking in function createChildren (app)");
        }
    }
}

void endChildren(Tslave slavesArray[], int slaveAmount) {

    for(int i = 0; i < slaveAmount; i++) { //todo

        if(close(slavesArray[i].in) == ERROR_CODE) {
            errorHandler("Error closing read end of fdData (app)");
        }

        if(close(slavesArray[i].out) == ERROR_CODE) {
            errorHandler("Error closing write end of fdData (app)");
        }

    }
}

void sendInitFiles(Tslave slavesArray[], int slaveAmount, char **fileName, int initialPaths, int *tasksInProgress, int *tasksFinished) {
    char fileSent[BUFFER_SIZE];

    for(int currentTask = 0, i = 1; currentTask < initialPaths /*capaz no es esto*/; currentTask++, i++) {
        strcat(fileSent, fileName[i]);
        strcat(fileSent, "\n");
        if(write(slavesArray[currentTask % slaveAmount].out, fileSent, strlen(fileSent)) == ERROR_CODE) {
            errorHandler("Error writing in fdPath (app)");
        }

        // if(write(slavesArray[currentTask % slaveAmount].out, "\n", 1) == ERROR_CODE) {
        //     errorHandler("Error writing in fdPath (app)");
        // }

        (*tasksInProgress)++;
        slavesArray[currentTask % slaveAmount].working++;
        //
    }

}

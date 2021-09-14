#include "app.h"

int main(int argc, char *argv[]) {

    /* ** ** Openers - buffer, shared memory, semaphores ** ** */

    if (argc < 2) {
        errorHandler("Error: incorrect amount of arguments (app)");
    }

    /* If I have less arguments than SLAVE_AMOUNT, I create
    ** as many children as arguments, since I do not want more children than arguments
    */
    int taskCount = argc - 1;
    int slaveAmount =  MIN(SLAVE_AMOUNT,taskCount);
    int filesPerSlave = (taskCount > SLAVE_AMOUNT * 2)? 2 : 1;

    Tslave slavesArray[slaveAmount];

    void *shMemory;
    void *shMemCopy;
    off_t shmSize = taskCount * BUFFER_SIZE;

    FILE *res = fopen("result.txt", "w");
    if (res == NULL) {
        errorHandler("Error opening result file (app)");
    }
    setBuffer(stdout,BUFFER_SIZE);

    int shmFd;
    if ((shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666)) == ERROR_CODE) {
        errorHandler("Error opening shared memory (app)");
    }

    if (ftruncate(shmFd, shmSize) == ERROR_CODE) {
        errorHandler("Error setting size to shared memory (app)");
    }

    shMemory = mmap(NULL, shmSize, PROT_WRITE, MAP_SHARED, shmFd, 0);
    if (shMemory == MAP_FAILED) {
        errorHandler("Error mapping shared memory (app)");
    }
    shMemCopy = shMemory;

    sem_unlink(SEM_NAME);

    sem_t *sem;
    if ((sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, INIT_VAL_SEM)) == SEM_FAILED) {
        errorHandler("Error opening semaphore (app)");
    }

    printf("%d\n", argc - 1);
    sleep(2);


    /*
    ** Handling slaves and files
    */
    createChildren(slavesArray, slaveAmount, SLAVE_PATH, NULL);

    /* Send first files to the slaves */
    int tasksSent = 0;
    int tasksFinished = 0;

    int initialPaths = filesPerSlave * slaveAmount;
    sendInitFiles(slavesArray, slaveAmount, argv, initialPaths, &tasksSent);

    char buffer[BUFFER_SIZE] = {0};

    while(tasksFinished < taskCount) {
        fd_set readFdSet;
        FD_ZERO(&readFdSet);

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
                int dimRead = read(fd, buffer, BUFFER_SIZE);
                if (dimRead == ERROR_CODE) {
                    errorHandler("Error reading from fdData (app)");
                } else if (dimRead <= 0) {
                    slavesArray[i].working = 0;
                } else {
                    tasksFinished++;
                    fprintf(res, "%s\n", buffer);
                    slavesArray[i].fileCount--;
                    buffer[dimRead + 1] = '\0';
                    if (sprintf((char*)(shMemory), "%s\n", buffer) == ERROR_CODE) {
                        errorHandler("Error performing sprintf in function main (app)");
                    }
                    shMemory += JUMP;
                    postSemaphore(sem);
                }

                /* send new files to slaves */
                if(slavesArray[i].fileCount == 0 && tasksSent < taskCount) {
                    char fileToSlave[BUFFER_SIZE] = {0};
                    strcat(fileToSlave, argv[tasksSent + 1]);
                    strcat(fileToSlave, "\n\0");
                    if(write(slavesArray[i].out, fileToSlave, strlen(fileToSlave)) == ERROR_CODE) {
                        errorHandler("Error sending files to slaves (app)");
                    }
                    tasksSent++;
                    slavesArray[i].fileCount++;
                    slavesArray[i].working = 1;
                }
                ready--;
            }

        }

    }

    if (fclose(res) != 0) {
        errorHandler("Error closing result file in main (app)");
    }

    endChildren(slavesArray, slaveAmount);

    /*
    ** Closing shared memory and semaphores
    */
    closeSemaphore(sem);
    unlinkSemaphore();

    close(shmFd);
    unmapSharedMemory(shMemCopy,shmSize);
    unlinkSharedMemory();
}


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

                if (dup2(fdPath[READ_FD], 0) == ERROR_CODE || dup2(fdData[WRITE_FD], 1) == ERROR_CODE) {
                    errorHandler("Error performing dup2 in function createChildren (app)");
                }

                close(fdPath[READ_FD]);
                close(fdData[WRITE_FD]);

                if (execv(path, argv) == ERROR_CODE) {
                    errorHandler("Error performing execv in function createChildren (app)");
                }


            } else {
                close(fdPath[READ_FD]);
                close(fdData[WRITE_FD]);

                slavesArray[i].pid = pid;
                slavesArray[i].in = fdData[READ_FD];
                slavesArray[i].out = fdPath[WRITE_FD];
                slavesArray[i].working = 0;
                slavesArray[i].fileCount = 0;

            }
        } else {
            errorHandler("Error forking in function createChildren (app)");
        }
    }
}

void endChildren(Tslave slavesArray[], int slaveAmount) {

    for(int i = 0; i < slaveAmount; i++) {

        if(close(slavesArray[i].in) == ERROR_CODE) {
            errorHandler("Error closing read end of fdData (app)");
        }

        if(close(slavesArray[i].out) == ERROR_CODE) {
            errorHandler("Error closing write end of fdPath (app)");
        }

    }
}

void sendInitFiles(Tslave slavesArray[], int slaveAmount, char **fileName, int initialPaths, int *tasksSent) {

    for(int currentTask = 0, i = 1; currentTask < initialPaths; currentTask++, i++) {

        char fileSent[BUFFER_SIZE] = {0};
        strcat(fileSent, fileName[i]);
        strcat(fileSent, "\n\0");

        if(write(slavesArray[currentTask % slaveAmount].out, fileSent, strlen(fileSent)) == ERROR_CODE) {
            errorHandler("Error writing in fdPath (app)");
        }

        (*tasksSent)++;
        slavesArray[currentTask % slaveAmount].working = 1;
        slavesArray[currentTask % slaveAmount].fileCount++;

    }

}

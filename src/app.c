#include "app.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        errorHandler("Error: incorrect amount of arguments (app)");
    }

    /* If I have less arguments than SLAVE_AMOUNT, I create
    ** as many children as arguments, since I do not want more children than arguments
    */
    int taskCount = argc - 1;
    int slaveAmount = (SLAVE_AMOUNT > taskCount)? taskCount : SLAVE_AMOUNT;

    FILE *resultFile;

    Tslave slavesArray[slaveAmount];

    // Opening shared memory and semaphores
    int shmFd;
    void * shMemory;
    int shmSize = taskCount*BUFFER_SIZE;

    shmFd = shm_open(SHM_NAME,O_CREAT | O_RDWR,0); //todo mode
    if(shmFd == ERROR_CODE) {
        errorHandler("Error opening shared memory (app)");
    }

    if(ftruncate(shmFd,shmSize) == ERROR_CODE) {
        errorHandler("Error setting size to shared memory (app)");
    }

    shMemory = mmap(0,shmSize,PROT_READ | PROT_WRITE,MAP_SHARED,shmFd,0); // todo prot
    if(shMemory == MAP_FAILED) {
        errorHandler("Error mapping shared memory (app)");
    }

    openSharedMemory(shMemory, &shmFd, TRUE, shmSize);

    sem_t *sem; //TODO see permissions and flags
    if ((sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, INIT_VAL_SEM)) == SEM_FAILED) {
        errorHandler("Error opening semaphore (app)");
    }
    
    write(stdout, &shmSize, sizeof(int));

    //Handling slaves
    createChildren(slavesArray, taskCount, slaveAmount, /*argv + 1,*/ SLAVE_PATH, NULL);

    // Send first files to the slaves
    int tasksInProgress = 0;
    int tasksFinished = 0;

    for(int slaveIndex = 0; slaveIndex < slaveAmount; i++) { //undefined i

        sendInitFiles(slavesArray[slaveIndex], argv[], &tasksInProgress);//todo

    }

    //FALTA (se necesita share memory y semaphore)
    while(tasksFinished < taskCount) {
        fd_set readFdSet;
        FD_ZERO(&readFdSet);

    }

    closeSemaphore(sem);
    unlinkSemaphore();
}

void createChildren(Tslave slavesArray[], int taskCount, int slaveAmount, char *path, char *const argv[]) {
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
                printf("%d %d\n",fdPath[READ_FD],fdData[WRITE_FD]);
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
                slavesArray[i].working = 0;

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

    for(int i = 0; i < slaveAmount;) {//todo

        if(close(slavesArray[i].in) == -1) {
            errorHandler("Error closing read end of fdData (app)");
        }

        if(close(slavesArray[i].out) == -1) {
            errorHandler("Error closing write end of fdData (app)");
        }

    }
}

void sendInitFiles(Tslave slave, char *fileName, int *taskIndex) {

        int fd = slave.out;
        int fileLen = strlen(fileName);

        if(write(fd, fileName, fileLen) == -1) {
            errorHandler("Error writing in fdPath (app)");
        }

        if(write(fd, "/n", 1) == -1) {
            errorHandler("Error writing in fdPath (app)");
        }

        slave.working = 1;
        (*taskIndex)++;

}

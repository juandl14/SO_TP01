#include "app.h"

void createChildren(Tslave slavesArray[], int taskCount, int slaveAmount, char *path, char *const argv[]);
sem_t *openSemaphore();

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

    sem_t *sem = openSemaphore();

    createChildren(slavesArray, taskCount, slaveAmount, /*argv + 1,*/ SLAVE_PATH, NULL);

    // Send first files to the slaves
    int taskIndex = 0;

    for(int slaveIndex = 0; slaveIndex < slaveAmount; i++) {

        sendFiles(slavesArray[slaveIndex], argv[], &taskIndex);

    }
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

void sendFiles(Tslave slave, char *fileName, int *taskIndex) {

    //CORREGIR
    if(!slave.working) {

        fd = slavesArray[slaveIndex].out;
        fileName = files[i];
        fileLen = strlen(files[i]);

        if(write(fd, fileName, fileLen) == -1) {
            errorHandler("Error writing in pathPipe (app)");
        }
        slave.working++;
        (*taskIndex)++;
    }

}

sem_t *openSemaphore() {
    sem_t *sem;
    if ((sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {
        errorHandler("Error opening semaphore in function openSemaphore (app)");
    }
    return sem;
}

#include "app.h"

void createChildren(Tslave slavesArray[], int taskCount, int slaveAmount, char *path, char *const argv[]);

int main(int argc, char const *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "%s\n", "Incorrect amount of arguments");
        exit(EXIT_FAILURE);
    }

    /* If I have less arguments than SLAVE_AMOUNT, I create
    ** as many children as arguments, since I do not want more children than arguments
    */
    int taskCount = argc - 1;
    int slaveAmount = (SLAVE_AMOUNT > taskCount)? taskCount : SLAVE_AMOUNT;

    Tslave slavesArray[slaveAmount];

    createChildren(slavesArray, taskCount, slaveAmount, /*argv + 1,*/ "./slave", NULL);
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
            errorHandler("Error creating pipe in function createChildren");
        }

        if ((pid = fork()) != ERROR_CODE) {
            if (pid == 0) {
                close(fdPath[WRITE_FD]);
                close(fdData[READ_FD]);
                // close(READ_FD);
                // close(WRITE_FD);

                if (dup2(fdPath[READ_FD], 0) == ERROR_CODE || dup2(fdData[WRITE_FD], 1) == ERROR_CODE) {
                    errorHandler("Error performing dup2 in function createChildren (app)");
                }

                close(fdPath[READ_FD]);
                close(fdData[WRITE_FD]);

                if (execv(path, argv) == ERROR_CODE) {
                    errorHandler("Error performing execv in function createChildren");
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

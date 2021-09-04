#include "app.h"
#include "errors.h"

int createChildren(/*int childrenAmount*/);

int main(int argc, char const *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "%s\n", "Incorrect amount of arguments");
        exit(EXIT_FAILURE);
    }

    /* Si tengo menos argumentos que SLAVE_AMOUNT, creo
    ** tantos hijos como argumentos, ya que no quiero tener mas hijos que argumentos
    */
    // int slaveAmount = (SLAVE_AMOUNT > argc - 1)? argc - 1 : SLAVE_AMOUNT;
    createChildren();
}

void createChildren(/*int childrenAmount*/) {
    pid_t pid;
    /* Creo los pipes: fdPath va del master al slave y fdData va del slave al master
    */
    int fdPath[2], fdData[2];

    if (pipe(fdPath) == ERROR_CODE || pipe(fdData) == ERROR_CODE) {
        errorHandler("Error creating pipe in function createChildren");
    }

    if ((pid = fork()) != ERROR_CODE) {
        if (pid == 0) {
            printf("Soy el hijo. Mi PID es %d y el de mi padre, %d\n", getpid(), getppid());
            sleep(2);
            printf("Proceso hijo terminado.\n");
        } else {
            printf("Soy el padre. Mi PID es %d y el de mi hijo, %d\n", getpid(), pid);
            sleep(3);
            waitpid(pid, NULL, 0);
            printf("Proceso padre terminado.\n");
        }
    } else {
        errorHandler("Error forking in function createChildren");
    }
    return 1;
}

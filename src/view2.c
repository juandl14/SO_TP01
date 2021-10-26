#include "view.h"

int main(int argc, char *argv[]) {

    setBuffer(stdin,BUFFER_SIZE);

    int shmSize;

    if (argc == 1) {
        char buff[MEMORY_LEN] = {0};
        if(read(STDIN, buff, MEMORY_LEN) == ERROR_CODE) {
            errorHandler("Error reading data for initializing shared memory (view)");
        }
        shmSize = atoi(buff);
    } else if (argc == 2) {
        shmSize = atoi(argv[1]);
    } else {
        errorHandler("Error: invalid amount of arguments (view)");
    }

    printf("%d\n", shmSize);

    // shmSize *= BUFFER_SIZE;

    if(shmSize <= 0) {
        errorHandler("Error defining size of shared memory (view)");
    }

    int fd1 = open(PIPE_NAME, O_RDONLY);
    // Desp se chequea errores


    handleData(sem,(char*)(shMemory), shmSize);
    close(fd1);
    return 0;
}

void handleData(int fd1, char buff[], int size) {
    int i = 0;

    while(i < size) {
        read(fd1, buff, BUFFER_SIZE);
        printf("%s", buff+i);
        i += JUMP;
    }
}
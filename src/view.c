#include "view.h"

int main(int argc, char *argv[]) {

    // Reciving data to initialize
    int shmSize;

    if (argc == 1) {
        if(read(stdin,&shmSize,sizeof(int)) == ERROR_CODE) {
            errorHandler("Error reading data for initializing shared memory (view)");
        }
    } else if (argc == 2) {
        shmSize = atoi(argv[1]);
    } else {
        errorHandler("Error: invalid amount of arguments (view)");
    }

    if(shmSize <= 0) {
        errorHandler("Error defining size of shared memory (view)");
    }

    // Opening shared memory and semaphores
    int shmFd;
    void * shMemory;
    openSharedMemory(shMemory, &shmFd, FALSE, FALSE);
    
    // Doing task

    // Closing shared memory



    /* Machete de SH M:
    ** shmopen(3)		abrir, no toma tamaño
    ** ftruncate(2)		dar tamaño a la memoria
    ** char * ptr;
    ** ptr = mmap(2)		mapea la memoria en el espacio de direcciones del mapeo,
    ** asignalo en donde quierass (null) pero decime dónde ocn ptr
		** laburamos con la memoria como si fuera un array o un malloc
    ** munmap(2)		sacar el mapa
    ** close(2)		cerrar el file descriptor
    ** shm_unlink(3)		borrar la entrada del file system
    */
}

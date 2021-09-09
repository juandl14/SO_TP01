#include "view.h"

int main(int argc, char *argv[]) {

    // Initializing and defining size for shared memory
    int shmSize;

    if (argc == 1) {

    } else if (argc == 2) {

    } else {
        errorHandler("Error: invalid amount of arguments (view)");
    }

    if(shMemSize <= 0) {
        errorHandler("Error defining size of shared memory (view)");
    }

    // Opening shared memory
    int shMemFd = shm_open(SHM_NAME,O_CREAT | O_RDWR,0); //todo mode
    if(shMemFd == ERROR_CODE) {
        errorHandler("Error opening shared memory (view)");
    }

    void * shMemory = mmap(0,shmSize,PROT_READ | PROT_WRITE,MAP_SHARED,shMemFd,0); // todo prot
    if(shMemory == MAP_FAILED) {
        errorHandler("Error mapping shared memory (view)");
    }
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

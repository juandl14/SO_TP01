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
    char * shMemory;
    sem_t * sem;
    
    if ((sem = sem_open(SEM_NAME, O_RDWR)) == SEM_FAILED) {
        errorHandler("Error opening semaphore (view)");
    }

    //TODO mode and permissions
    if((shmFd = shm_open(SHM_NAME,O_CREAT | O_RDONLY,0)) == ERROR_CODE) {
        errorHandler("Error opening shared memory (view)");
    }

    // TODO prot
    if((shMemory = (char *) mmap(0,shmSize,PROT_READ | PROT_WRITE,MAP_SHARED,shmFd,0)) == MAP_FAILED) {
        errorHandler("Error mapping shared memory (view)");
    }



    // Showing results
    handleData(sem,shMemory);


    // Closing shared memory and semaphores
    closeSemaphore(sem);
    close(shmFd);

    unmapSharedMemory(shMemory,shmSize);
    unlinkSharedMemory();


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

void handleData(sem_t * sem,char * shMemory) {
    while(1) {
        if(sem_wait(sem)==ERROR_CODE) {
            errorHandler("Error: sem_wait has failed (view)");
        }

        if(*shMemory == 0) {
            break;
        }
        int move = printf("%s",shMemory);
        shMemory += move + 1;
    }
}
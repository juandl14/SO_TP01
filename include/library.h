#ifndef LIBRARY_H
#define LIBRARY_H

#define _DEFAULT_SOURCE
#define  _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>           /* For O_* constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/stat.h>        /* For mode constants */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ERROR_CODE -1
#define ERROR_MSG 100

#define TRUE 1
#define FALSE 0

#define INIT_VAL_SEM 1
#define SEM_NAME "/semaphore"
#define SHM_NAME "/shMemory"

#define BUFFER_SIZE 1024

void errorHandler(const char *);
void closeSemaphore(sem_t *);
void unlinkSemaphore();
void openSharedMemory(void *,int *,int,int);

#endif

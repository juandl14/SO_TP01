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

#define INIT_VAL_SEM 0
#define SEM_NAME "/semaphore"
#define SHM_NAME "/shMemory"
#define STDIN 0
#define STDOUT 1
#define READ_FD 0
#define WRITE_FD 1

#define BUFFER_SIZE 1024

#define MAX(a, b) ((a) > (b) ? (a) : (b))

void errorHandler(const char *);
void postSemaphore(sem_t *);
void waitSemaphore(sem_t *);
void closeSemaphore(sem_t *);
void unlinkSemaphore();
void unmapSharedMemory(char *, int );
void unlinkSharedMemory();

#endif

#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */

#define ERROR_CODE -1
#define ERROR_MSG 100
#define SEM_NAME "/sem"
#define SHM_NAME "/shMemory"

void errorHandler(const char *errorMsg);
sem_t *openSemaphore();

#endif

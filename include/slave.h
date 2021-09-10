#ifndef SLAVE_H
#define SLAVE_H

#define _DEFAULT_SOURCE
#define  _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <errors.h>
// #include <semaphore.h>
// #include <fcntl.h>           /* For O_* constants */
// #include <sys/stat.h>        /* For mode constants */
#include "library.h"

#define BUFFER 2048
#define READ_FD 0
#define WRITE_FD 1

void solver(const char *);

#endif

#ifndef SOLVE_H
#define SOLVE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>

#define SLAVE_AMOUNT 8
#define READ_FD 0
#define WRITE_FD 1

typedef struct slave {
    pid_t pid;
    int in;
    int out;
    int working;
} slave;

#endif

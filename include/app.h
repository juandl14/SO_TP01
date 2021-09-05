#ifndef SOLVE_H
#define SOLVE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <errors.h>

#define SLAVE_AMOUNT 8
#define READ_FD 0
#define WRITE_FD 1
#define TASK_MAX_CAP 2

typedef struct slave {
    pid_t pid;
    int in;
    int out;
    int working;
} Tslave;

#endif

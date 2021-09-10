#ifndef SOLVE_H
#define SOLVE_H

#include "library.h"

#define SLAVE_AMOUNT 8
#define SLAVE_PATH "./Slave"
#define READ_FD 0
#define WRITE_FD 1
#define TASK_MAX_CAP 2

typedef struct slave {
    pid_t pid;
    int in;
    int out;
    int working;
} Tslave;

void createChildren(Tslave slavesArray[], int taskCount, int slaveAmount, char *path, char *const argv[]);
void endChildren(Tslave slavesArray[], int slaveAmount);
void sendInitFiles(Tslave slave, char *fileName, int *taskIndex);

#endif

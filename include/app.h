#ifndef SOLVE_H
#define SOLVE_H

#include "library.h"

#define SLAVE_AMOUNT 8
#define SLAVE_PATH "./Slave"
#define TASK_MAX_CAP 2

typedef struct slave {
    pid_t pid;
    int in;
    int out;
    int working;
    int fileCount;
} Tslave;

void createChildren(Tslave slavesArray[], int slaveAmount, char *path, char *const argv[]);
void endChildren(Tslave slavesArray[], int slaveAmount);
void sendInitFiles(Tslave slavesArray[], int slaveAmount, char **fileName, int initialPaths, int *tasksInProgress, int *tasksFinished);
int chargeReadSet(fd_set *fdReadSet, Tslave slavesArray[], int slaveAmount);

#endif

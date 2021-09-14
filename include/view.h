#ifndef VIEW_H
#define VIEW_H

#include "library.h"

#define MEMORY_LEN 50

void handleData(sem_t *,char *, int);
void closingView(sem_t *, int, int , void * );

#endif

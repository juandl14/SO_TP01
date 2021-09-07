#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define ERROR_CODE -1
#define ERROR_MSG 100

void errorHandler(const char *errorMsg);

#endif

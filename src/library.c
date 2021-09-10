#include "library.h"

void errorHandler(const char *errorMsg) {
    char finalMsg[ERROR_MSG]={0};
    strcat(finalMsg,errorMsg);
    strcat(finalMsg,"\n");
    perror(finalMsg);
    exit(EXIT_FAILURE);
}

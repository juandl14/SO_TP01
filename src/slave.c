#include "slave.h"
#include "errors.h"

int main(int argc, char const *argv[]) {

    if(setvbuf(stdout, NULL, _IONBF, 0) != 0) {
        errorHandler("Error performing setvbuf in main (slave)");
    }

    char *fileName = NULL;
    size_t fileNameLen = 0;

    while (getline(&fileName, &fileNameLen, stdin) > 0) {
        fileName[strcspn(fileName, "/n")] = 0;
        solver(fileName);
    }
}

void solver(char const *fileName) {
    char command[BUFFER] = {0};
    sptrintf(command,"minisat %s | /bin/grep -o -e \"Number of.*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\"",fileName);

    FILE * pipeName = popen(command, "r");
    if(pipeName == NULL) {
        errorHandler("Error performing popen solver (slave)");
    }



    pclose(pipeName);
}

#include "slave.h"

int main(int argc, const char *argv[]) {

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

void solver(const char *fileName) {
    char command[BUFFER] = {0};
    sprintf(command,"minisat %s | /bin/grep -o -e \"Number of.*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\"",fileName);

    FILE * fd = popen(command, "r");
    if(fd == NULL) {
        errorHandler("Error performing popen solver (slave)");
    }

    char input[BUFFER/2]={0};
    fread(input,sizeof(char),BUFFER,fd);

    char output[BUFFER];
    sprintf(output,"PID: %d\nFile: %s\n%s\n",getpid(),fileName,input);
    write(WRITE_FD,output,strlen(output)+1);

    if(pclose(fd)==-1) {
        errorHandler("Error performing pclose solver (slave)");
    }
}

#include <slave.h>

int main(int argc, char const *argv[]) {

    if(setvbuf(stdout, NULL, _IONBF, 0) != 0) {
        errorHandler("Error performing setvbuf in function main");
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
}

#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "errors.h"

#define READ_FD 0
#define WRITE_FD 1
#define SHM_NAME "/shMemory"

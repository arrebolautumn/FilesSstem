#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "fsLow.h"
#include <fcntl.h>


typedef int b_io_fd;

int initFAT(uint64_t numberOfBlocks, uint64_t blockSize);

int getFreeBlocks(uint64_t numberOfBlocks);

int writeToFile(void* buffer, int numberOfBlocks, int location);


int readFromFile(void* buffer, int numberOfBlocks, int location);


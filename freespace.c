#include "freespace.h"
#include "fs_entities.h"
#define BYTESIZE 4


int initFAT(uint64_t numberOfBlocks, uint64_t blockSize){
    
    // the number of blocks the freespace map needs
    // the total number of bytes needed to store the whole blocks
    int n = sizeof(int)*numberOfBlocks; 
    int m = blockSize;
    int blocksNeeded =  (n + m - 1 )/ m; // formula to round blocks
    int* fat = malloc( blocksNeeded * blockSize );

    if (!fat) {
        perror("FAT memory allocation failed");
        return -1;
    }

    // a linked list that points to the next free space/block
    for( int i = 1; i < numberOfBlocks-1; i++ ) {
        fat[i] = i+1;
    }
    printf("\nblocks needed: %i\n", blocksNeeded);

    // mark the VCB as used
    fat[0] = 0xFFFFFFFF;
    for (int i = 1; i <= blocksNeeded; i++) {
        fat[i] = 0xFFFFFFFF; // mark FAT blocks as used
    }
    fat[numberOfBlocks-1] = 0xFFFFFFFF; // mark last block as end

    // write data to list, start writing at block 1
    int blocksWritten = LBAwrite(fat, blocksNeeded, 1);
    free(fat);

    if (blocksWritten == -1) {
        perror("Failed to write FAT");
        return -1;
    }
    /*
    ypedef struct VCB
{                                     // The goal of the VCB is to be persistent
    unsigned int totalBlockCnt;       // # of total blocks in volume
    unsigned int blockSize;           // size of each block in bytes
    unsigned int firstFreeBlock;      // Index where free space starts
    unsigned int locFAT;              // location of the FAT
    
    unsigned int rootLoc;             // block index of root directory entry
    unsigned long sig;                // file system signature
    unsigned long freeBlockCnt;       // # of free blocks 


    */

    vcbptr->freeBlockCnt = numberOfBlocks - blocksNeeded;
    // first usable block is after the VCB and freespace map
    vcbptr->firstFreeBlock = blocksNeeded + 1; 
    return blocksNeeded + 1;
}


int getFreeBlocks(uint64_t numberOfBlocks) {
    int* freeBlocks;
    if( numberOfBlocks < 1 ) {
        return -1;
    }
    if( numberOfBlocks > vcbptr->freeBlockCnt ) {
        return -1;
    }

    // first free block in the FAT table
    int head = vcbptr->firstFreeBlock;
    int currBlockLoc = vcbptr->firstFreeBlock;
    int nextBlockLoc = fat[currBlockLoc];
    vcbptr->freeBlockCnt--;
    // iterate thru blocks to find the next available freeblock
    
    int blocksToAllocate = numberOfBlocks - 1;
    while (blocksToAllocate > 0) {
        currBlockLoc = nextBlockLoc;
        nextBlockLoc = fat[currBlockLoc];
        vcbptr->freeBlockCnt--;
        blocksToAllocate--;
    }

    fat[currBlockLoc] = 0xFFFFFFFF;
    vcbptr->firstFreeBlock = nextBlockLoc;

    return head;
}


int writeToFile(void* buffer, int numberOfBlocks, int location){
    int blockSize = vcbptr->blockSize;
    int blocksWritten = 0;
    // loop thru
    for( int i = 0; i < numberOfBlocks; i++ ) {
        if (location != 0xFFFFFFFF ){
            continue;
        }
        int positionToWrite = buffer + i * blockSize;
        int writeToFIle = LBAwrite(positionToWrite, 1, location);
        if( writeToFIle == -1 ) {
            return -1;
        }
        // location is updated from the linked list 
        // since each block points to the next free block in the fat
        location = fat[location];
        blocksWritten++;
    }
    return blocksWritten;
}

int readFromFile(void* buffer, int numberOfBlocks, int location){
    int blockSize = vcbptr->blockSize;
    int blocksRead = 0;
    for( int i = 0; location != 0xFFFFFFFF && i < numberOfBlocks; i++ ) {
        if( LBAread(buffer + blockSize*i, 1, location) == -1) {
            return -1;
        }
        location = fat[location];
        blocksRead++;
    }
    return blocksRead;
}
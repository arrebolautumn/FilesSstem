/**************************************************************
 * Class::  CSC-415-0# Spring 2024
 * Name::
 * Student IDs::
 * GitHub-Name::
 * Group-Name::
 * Project:: Basic File System
 *
 * File:: fsInit.c
 *
 * Description:: Main driver for file system assignment.
 *
 * This file is where you will start and initialize your system
 *
 **************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#include "fsLow.h"
#include "mfs.h"
#include "fs_entities.h"
#include "freespace.h"


int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize)
{
	printf("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */
	VCB *vcbptr = malloc(sizeof(VCB));
	if (vcbptr == NULL)
	{
		printf("Failed to allocate mem for VCB pointer.\n");
		return -1;
	}


	int blockInitialized = LBAread(vcbptr, 1, 0);
	if (blockInitialized != 1)
	{
		printf("Failed to read block\n");
	}
	else if (blockInitialized > 0)
	{
		printf("Volume needs to be initialized.\n");
		vcbptr->sig = 0x6264343230;
		vcbptr->blockSize = blockSize;
		vcbptr->totalBlockCnt = numberOfBlocks;
		vcbptr->freeBlockCnt = numberOfBlocks;
		vcbptr->rootLoc = createDirectory(100, NULL, "/");
		vcbptr->firstFreeBlock = initFAT(numberOfBlocks, blockSize);
		LBAwrite(vcbptr, 1, 0);
	}

/*
struct VCB
{                                     // The goal of the VCB is to be persistent
    unsigned int totalBlockCnt;       // # of total blocks in volume
    unsigned int blockSize;           // size of each block in bytes
    unsigned int firstFreeBlock;      // Index where free space starts
    unsigned int locFAT;              // location of the FAT
    
    unsigned int rootLoc;             // block index of root directory entry
    unsigned long sig;                // file system signature
    unsigned long freeBlockCnt;       // # of free blocks in volume

} VCB;


typedef struct directoryEntry {
    size_t size;              // size of file
    time_t creationTime;       // time file created
    time_t lastModificationTime;   // date&time file was last modified
    time_t lastAccessed;       // date&time file was last accessed
    mode_t permissions;        // determines permissions of file
    int blockLocation;     // starting block of file or directory data
    int isDirectory;           // 1 if directory, 0 if file
    char name[256];           // name of file or directory


} directoryEntry;

directoryEntry *createDirectory(int numberOfEntries, int *parent, char *name);

 */
	return 0;
}

void exitFileSystem()
{
	printf("System exiting\n");
}
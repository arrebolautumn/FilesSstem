#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>

typedef struct VCB
{                                     // The goal of the VCB is to be persistent
    unsigned int totalBlockCnt;       // # of total blocks in volume
    unsigned int blockSize;           // size of each block in bytes
    unsigned int firstFreeBlock;      // Index where free space starts
    unsigned int locFAT;              // location of the FAT
    
    unsigned int rootLoc;             // block index of root directory entry
    unsigned int rootSize;            // size of the root 
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
    long location;
    char name[256];           // name of file or directory


} directoryEntry;

extern struct VCB *vcbptr;
extern struct DE *root;
extern int *fat;

directoryEntry *createDirectory(int numberOfEntries, int *parent, char *name);


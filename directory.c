#include <string.h>
#include "fs_entities.h"

#include <stdlib.h>
#include <unistd.h>

#include "fsLow.h" // define MINBLOCKSIZE
#include "mfs.h"
#include "freespace.h"

directoryEntry *createDirectory(int numberOfEntries, struct directoryEntry * parent, char *name) {

    int bytes;			 // The number of bytes needed for entries
	int blockCount;		 // The total number of blocks needed
	int maxEntryCount;	 // How many entries we can fit into our blocks
	int blocksRequested; // Location of first block for new directory

	/* From # of entries argument determine the maximum number of
	 * entries we can actually fit */
	bytes = sizeof(struct directoryEntry) * numberOfEntries;
	blockCount =
		((bytes + MINBLOCKSIZE - 1) / MINBLOCKSIZE);
	maxEntryCount = (blockCount * MINBLOCKSIZE) / sizeof(struct directoryEntry);

	// Allocate memory for directory
	struct directoryEntry *buffer = malloc(blockCount * MINBLOCKSIZE);
	if (buffer == NULL)
	{
		printf("Error: Could not allocate memory for new directory\n");
		exit(EXIT_FAILURE);
	}

	// Initialize each directory entry in new directory to an unused state
	for (int i = 0; i < maxEntryCount; i++)
		buffer[i].location = -2;

	// Request blocks from freespace system
	blocksRequested = getFreeBlocks(blockCount);

	// Initialize dot and dot dot entries of the new directory
	strncpy(buffer[0].name, ".", 36);
	buffer[0].location = blocksRequested;
	buffer[0].size = maxEntryCount * sizeof(struct directoryEntry);
	buffer[0].isDirectory = 0;

	// If no parent is passed, initialize root directory @UNSAFE
	// Cannot handle if root already exists
	if (parent == NULL)
	{
		printf("Initializing root directory\n");
		vcbptr->rootSize = blockCount;
		vcbptr->rootLoc = blocksRequested;
		buffer[1] = buffer[0];
		strncpy(buffer[1].name, "..", 36);

		/* If a parent directory entry is provided, initialize new directory's
		 * parent and link the new directory entry back to the parent */
	}
	else
	{
		printf("Creating a new directory\n");
		int parentSize = (parent[0].size) / sizeof(struct directoryEntry);
		int directorySlots = 0;
		int slotFound = 0;
		for (; !slotFound && (slotFound < parentSize); directorySlots++)
		{
			if (parent[directorySlots].location == -2)
			{
				parent[directorySlots] = buffer[0];
				strncpy(parent[directorySlots].name, name, 36);
				slotFound = 1;
				fileWrite(parent, parent[0].size / MINBLOCKSIZE,
						  parent[0].location);
			}
		}
		if (slotFound == 0)
			perror("Directory is full");
	}

	// Write newly created directory to disk
	int blocksWritten = fileWrite(buffer, blockCount, blocksRequested);
	return blocksRequested;
}
    
    

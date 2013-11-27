#ifndef DISK_H_
#define DISK_H_

/**
 * Simulates a physical disk. The disk reads and writes blocks of data.
 *
 * @author Thomas Reichherzer
 */

//defines
/** The size of a block on disk in bytes */
#define BLOCK_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "inode.h"

typedef struct Block
{
	int numOfBlock;
	char blockData[BLOCK_SIZE];
	struct Block * nextBlock;
}block_t;


//prototypes
int createDisk(int sizeInKB, char* path );
int readBlock(int blockNumber, char* blockData );
int writeBlock(int blockNumber, char* blockData, int numberOfBytes  );
int getSize();
void closeDiskFile();
void openDiskFile(char* path);
void printErrorDesc();
block_t * initializeBlockList(block_t* Block);

//global
int _sizeInKB;
int _diskLength;
FILE* _physicalDisk;
char errorDesc[256];

#endif /*DISK_H_*/

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

/**
 * Emulates a file system.
 *
 * @author Thomas Reichherzer
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "disk.h"
#include "superBlock.h"

#define NUM_INODES 16 //To be filled in

typedef struct fd
{
	int inodeBlockNum;
	long int fileptr;
} fd_t;




//Prototypes

/**
 * Formats the disk.
 *
 * @param diskSizeInKB - the size of the disk in KB
 * @param path - the path of the virtual disk
 *
 * @returns -1 if an error occurs
*/
int format( int diskSizeInKB, char* path );

/**
 * Opens a file by the specified name and returns a file descriptor.
 *
 * @param name - the name of the file to be opened
 *
 * @returns a file descriptor if the file was successfully opened or NULL, if an error occurred
 */
fd_t* openf(char* name);

/**
 * Closes the file given the file descriptor.
 *
 * @returns -1 if an error occurs
 */
int closef(fd_t *stream);

/**
 * Reads data from a file stream and stores them at the specified location.
 *
 * @param stream - the file stream to be read
 * @param data - the buffer to store data in
 * @param size - the number of bytes to be read
 *
 * @returns -1 if an error occurred and 0 otherwise
 */
int readf(fd_t* stream, char* data, int size);

/**
 * Writes data to a file stream given a file descriptor and data to be written.
 *
 * @param stream - the file stream to append data
 * @param data - the buffer with data to be written to a file
 * @param size - the number of bytes to be written
 *
 * @returns -1, if an error occurred and 0 otherwise
 */
int writef(fd_t* stream, char* data, int size);

/*
 * Initializes the SuperBlock
 */
superBlock_t * intializeSupBlock(superBlock_t* spBlck);

void printSupBlock(superBlock_t* spBlck);

void printInode(inode_t * iNode);

void initializeFreeBlockList();

void clearBuffer(char* buf);

void writeFreeBlocks(int blockNumber);

int getNumInodes();

int getNumBlocks();

int getFirstFreeBlock();

void writeBlockToInode(inode_t* node, int block);

#endif /*FILESYSTEM_H_*/

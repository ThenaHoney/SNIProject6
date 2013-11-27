/*
 * main.c
 *
 *  Created on: Nov 27, 2013
 *      Author: david
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "disk.h"
#include "fileSystem.h"
#include "inode.h"
#include "superBlock.h"

int main(int argc, char* argv[])
{
	int error;
	superBlock_t superBlock;
	superBlock_t* spB = &superBlock;
	char path[25] = "storageDisk";
	if((error = format( 100000, path)))
	{
		printf("There was an error formatting the disk\n");
	}
	printf("\nHello World\n");
	readSuperBlock(spB);
	printSupBlock(spB);
	fd_t* k;
	k = openf("here.txt");
	if(k == NULL)
	{
		printf("Could not open file");
	}
	else
	{
		printf("Could open file\n");
	}
	printFreeBlocks();

	return 0;
}

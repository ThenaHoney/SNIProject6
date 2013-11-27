/**
 * Simulates a physical disk. The disk reads and writes blocks of data.
 *
 * @author Thomas Reichherzer
 */

#include "disk.h"
#include "fileSystem.h"

/**
 * Constructs a disk simulated by a file.
 *
 * @param sizeInKB - the size of the disk in kilo bytes
 * @param path - the path of the file simulating the disk
 *
 * @return -1 if error occurs creating the disk
*/
int createDisk( int sizeInKB, char* path )
{
	if (sizeInKB <= 0)
	{
		strcpy( errorDesc, "Invalid disk size\n" );
		return -1;
	}
	_sizeInKB = sizeInKB;
	_physicalDisk = fopen( path, "w+" );
	if( _physicalDisk == NULL )
	{
		strcpy( errorDesc, "Failed to create file\n" );
		return -1;
	}
	_diskLength = sizeInKB*BLOCK_SIZE;
	return 0;
}

/**
* Reads a data block from the disk.
*
* @param blockNumber - specifies the block to be read
* @param blockData - a block of data the size of BLOCK_SIZE
*
* @return -1 if an error occurs reading a block of data, else returns number of bytes read
*/
int readBlock( int blockNumber, char* blockData )
{
	int error, count;
	if ((blockNumber >= _sizeInKB) || (blockNumber < 0))
	{
		strcpy( errorDesc, "Invalid block number\n" );
		return -1;
	}
	//offset specified number of bytes from the beginning of the disk
	error = fseek(_physicalDisk, blockNumber*BLOCK_SIZE, 0);
	if( error )
	{
		strcpy( errorDesc, "Couldn't seek to specified block\n" );
		return -1;
	}
	//read 1 unit of 1024 bytes from file _physicalDisk to array blockData
	if(_physicalDisk == NULL)
	{
		strcpy(errorDesc, "Disk is NULL");
		return -1;
	}
	//printf("Block Data is %s\n", blockData);
	printf("Block Location is %d\n", blockNumber*BLOCK_SIZE);

	count = fread( blockData,  BLOCK_SIZE, 1, _physicalDisk );
	if(count == 0)
	{
		strcpy( errorDesc, "Couldn't read block\n" );
		return -1;
	}

	return error;

}

/**
 * Writes a data block to the disk.
 *
 * @param blockNumber - specifies the location where the block of data be written
 * @param blockData - the block of data to be written
 * @param numberOfBytes - the number of bytes to write into the block
 *
 * @return -1 if an error occurs writing a block of data, else number of elements written
*/
int writeBlock( int blockNumber, char* blockData, int numberOfBytes  )
{
	int error, count;
	if ((blockNumber >= _sizeInKB) || (blockNumber < 0))
	{
		strcpy( errorDesc, "Invalid block number\n" );
		return -1;
	}
	//offset specified number of bytes from the beginning of the disk
	error = fseek(_physicalDisk, blockNumber*BLOCK_SIZE, 0);
	if( error )
	{
		strcpy( errorDesc, "Couldn't seek to specified block\n" );
		return -1;
	}
	//if number of bytes to write is bigger than a block, write maximum of a block,
	//else write the number of bytes
	int len = (numberOfBytes > BLOCK_SIZE ? BLOCK_SIZE : numberOfBytes);
	//read 1 unit of 1024 bytes from file _physicalDisk to array blockData
	count = fwrite( blockData, len, 1, _physicalDisk );
	if(count == 0)
	{
		strcpy( errorDesc, "Couldn't write block\n" );
		return -1;
	}

	return error;
}

/**
 * Returns the size of the disk.
 *
 * @return the size of the disk in kilo bytes
*/
int getSize()
{
	return _sizeInKB;
}

/*
 * Creates a list of blocks that will hold the data on the disk
 */
block_t * initializeBlockList(block_t * Block)
{
	block_t* currentBlock = NULL;
	block_t* previousBlock = (block_t*)-1;
	int i = 0, t = 0;

	while(i < (MAX_POINTERS * NUM_INODES)) //creates the Data blocks starting from the end and going to the beginning
	{
		currentBlock = (block_t*)malloc((sizeof(block_t))+ 4);
		currentBlock->nextBlock = previousBlock;
		currentBlock->numOfBlock = i;
		for(t =0; t < BLOCK_SIZE; t++)
		{
			currentBlock->blockData[t] = 0;
		}
		previousBlock = currentBlock;
		i++;
	}

	return currentBlock;
}

void closeDiskFile()
{
	int error;
	error = fclose(_physicalDisk);
	if(error == -1)
	{
		fprintf(stderr, "There was a problem closing the file\n");
	}
}
void openDiskFile(char* path)
{
	_physicalDisk = fopen(path, "w+");
	if(_physicalDisk == NULL)
	{
		fprintf(stderr, "There was a problem opening the file\n");
	}
}

void printErrorDesc()
{
	fprintf(stderr, "\nError: %s\n", errorDesc);
}

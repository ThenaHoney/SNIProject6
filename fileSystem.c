/**
 * Emulates a file system.
 *
 * @author Thomas Reichherzer
 */

#include "fileSystem.h"
#include "inode.h"
#include "disk.h"
#include "superBlock.h"

void readSuperBlock( superBlock_t* superBlock );
void writeSuperBlock( superBlock_t * superBlock );
void readInode( inode_t* inode, int blockNumber );
void writeInode( inode_t* inode, int blockNumber );
void printInodes();
void printFreeBlocks();
void intToByteArray( int value, char* buffer, int pos );
int byteArrayToInt( char* buffer, int pos );


/**
 * A test program for testing file system operations.
 *
 * @param args - a list of arguments
*/
int main(int argc, char* argv[])
{
	int error;
	if((error = format( 100, "storageDisk.txt")))
	{
		printf("There was an error formatting the disk\n");
	}
	printf("Hello World\n");
	fd_t* diskFile = openf("random-file");
	if(diskFile != NULL)
	{
		printf("There was an error opening the file");
	}
	if(closef(diskFile))
	{
		printf("There was an error opening the file");
	}

	return 0;
}

int format( int diskSizeInKB, char* path )
{
	int error, i;
    // create a disk
	if((error = createDisk(diskSizeInKB, path)))
	{
		printf("There was an error with creating the disk\n");
	}
	else
	{
    // complete implementation of this function
    // you must use the functions read/writeSuperblock() & read/writeInode()
	// formatting requires that you setup the superblock, write empty inodes,
	// and setup your free list for the remaining blocks
	superBlock_t* spBlck = NULL;
	fd_t* fileTemp  = NULL;
	spBlck = intializeSupBlock(spBlck);
	inode_t iNode[NUM_INODES];
	for(i = 0; i < NUM_INODES; i++)
	{
		initializeInode(&iNode[i]);
	}
	block_t* dataBlock = NULL;
	dataBlock = initializeBlockList(dataBlock);
	fileTemp = openf(path);
	readSuperBlock(spBlck);
	writeSuperBlock(spBlck);



	//read content to data blocks
	//write content to file or print out content
	//test if it prints out the content of the superBlock
	printSupBlock(spBlck);
	printInodesTest(&iNode[0]);
	}


	return error;
}

/**
 * Opens a binary file on the disk for storing data.
 */
fd_t* openf(char* name)
{
	fd_t * temp;
	temp->fileptr = NULL;
	temp->inodeBlockNum = NULL;

	return temp;
}

int closef(fd_t *stream)
{

	return -1;
}

int readf(fd_t* stream, char* data, int size)
{
	return -1;
}

int writef(fd_t* stream, char* data, int size)
{
	return -1;
}

/**
    * Reads the super block from disk.
    *
    * @param - destination to store the disk's super block
    */
void readSuperBlock( superBlock_t* superBlock )
{
    int error;
	char buffer[BLOCK_SIZE];

    // read block and convert it to the superblock
	error = readBlock(0, buffer);
	if(error == -1)
	{
		printf("There was an error reading a block and converting it to the superBlock\n");
		return;
	}

	// read number of disk blocks from block
	int numDiskBlocks = byteArrayToInt(buffer, 0 );

	// read number of inodes from block
	int numInodes =  byteArrayToInt(buffer, 4 );

	// read start of free block to disk

	// finish implementing this function
}

/**
 * Writes the super block to disk.
 *
 * @param superBlock - the disk's super block
 */
void writeSuperBlock(superBlock_t* superBlock )
{
	// setup buffer to be written to disk
	char buffer[BLOCK_SIZE];

	// write number of disk blocks into buffer
	intToByteArray(superBlock->_numberOfDiskBlocks, buffer, 0);

	// write number of inodes into buffer
	intToByteArray(superBlock->_numberOfInodes, buffer, 4);

    // finish this function
}

/**
 * Reads an Inode from disk.
 *
 * @param inode - the inode to read into
 * @param blockNumber - the block number that holds the inode to be read
 *
*/
void readInode(inode_t* inode, int blockNumber)
{
	int error;
	char block[BLOCK_SIZE];
	// read block containing the Inode
    error = readBlock(blockNumber, block);

    // finish implementing this method

}

/**
 * Writes an Inode to disk.
 *
 * @param inode - the inode to be written to disk
 * @param blockNumber - the block number in which the inode will be written
 *
*/
void writeInode(inode_t* inode, int blockNumber)
{
	// implement this function
}

/**
 * Prints the content of the Inodes of the virtual disk to the screen.
 */
void printInodes()
{
	// implement this function
}

/**
 * Prints the block numbers of disk blocks that are free.
 */
void printFreeBlocks()
{
	// implement this function
}


/**
 * Writes an integer to a byte array.
 *
 * @param value - the integer to be written to a byte array
 * @param buffer - the byte array in which the integer will be written
 * @param pos - the position in the byte array where an integer will be written
 */
void intToByteArray( int value, char* buffer, int pos )
{
	int i;

    for (i = 0; i < 4; i++)
	{
        int shift = (4 - 1 - i) * 8;
        buffer[i+pos] = (char) (( (unsigned int) value >> shift) & 0xFF);
    }
}

 /**
  * Reads an integer from a byte array at a specific location.
  *
  * @param buffer - the byte array from which an integer will be read
  * @param pos - the position in the array from where an integer will be read
  *
  * @return an integer read from the byte array
 */
int byteArrayToInt( char* buffer, int pos )
{
	int i, value = 0;

    for (i = 0; i < 4; i++)
	{
		int shift = (4 - 1 - i) * 8;
        value += (buffer[i + pos] & 0x000000FF) << shift;
    }
    return value;
}

superBlock_t * intializeSupBlock(superBlock_t* spBlck)
{
	spBlck = (superBlock_t*)malloc((sizeof(superBlock_t))+ 4);
	spBlck->_numberOfDiskBlocks = MAX_POINTERS * NUM_INODES;
	spBlck->_numberOfInodes = NUM_INODES;
	spBlck->_firstBlockOfFreeList = 0;
	return spBlck;
}

void printSupBlock(superBlock_t* spBlck)
{
	printf("Number of DiskBlocks: %d\n", spBlck->_numberOfDiskBlocks);
	printf("Number of Inodes: %d\n", spBlck->_numberOfInodes);
	printf("First Block Free in the List: %d\n", spBlck->_firstBlockOfFreeList);
}

void printInodesTest(inode_t * iNode)
{
	printf("Flag: %d\n", iNode->_flags);
	printf("Owner: %d\n", iNode->_owner);
	printf("FileSize: %d\n", iNode->_filesize);
	printf("File Name: %s\nNodes:  ", iNode->_filename);
	int i = 0;
	while(iNode->pointer[i] != -1)
	{
		printf("%d ", iNode->pointer[i]);
		i++;
	}
	printf("\n\n");

}

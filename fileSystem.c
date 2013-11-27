/**
 * Emulates a file system.
 *
 * @author Thomas Reichherzer
 */

#include "fileSystem.h"
#include "inode.h"
#include "disk.h"
#include "superBlock.h"

void readSuperBlock(superBlock_t* superBlock );
void writeSuperBlock( superBlock_t * superBlock );
void readInode( inode_t* inode, int blockNumber );
void writeInode( inode_t* inode, int blockNumber );
void printInode();
void printFreeBlocks();
void intToByteArray( int value, char* buffer, int pos );
int byteArrayToInt( char* buffer, int pos );

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
	superBlock_t supBlock;
	superBlock_t* spBlck = &supBlock;
	fd_t* fileTemp  = NULL;
	spBlck = intializeSupBlock(spBlck);
	inode_t* node;
	inode_t iNode[NUM_INODES];
	for(i = 0; i < NUM_INODES; i++)
	{
		initializeInode(&iNode[i]);
	}
	block_t* dataBlock = NULL;
	dataBlock = initializeBlockList(dataBlock);

	//read content to data blocks
	//write content to file or print out content
	//test if it prints out the content of the superBlock
	printSupBlock(spBlck);
	writeSuperBlock(spBlck);
	node = &iNode[0];
	node->_flags = 4;
	node->_owner = 5;
	node->_filesize = 56;
	int i;
	printInode(&iNode[0]);
	for(i = 1; i <= 16; i++)
	{
		node = &iNode[i-1];
		writeInode(node, i);
	}
	for(i = 17; i < 4000; i++)
	{
		writeFreeBlocks(i);
	}
}

	return error;
}

/**
 * Opens a binary file on the disk for storing data.
 */
fd_t* openf(char* name)
{
	fd_t * temp = (fd_t*)malloc((sizeof(fd_t))+ 4);
	superBlock_t* spB = (superBlock_t*)malloc((sizeof(superBlock_t))+ 4);;
	inode_t* nodep = (inode_t*)malloc((sizeof(inode_t))+ 4);
	readSuperBlock(spB);
	int i=0, j=1;
	while (i <= (spB->_numberOfInodes))
	{
		//printSupBlock(spB);
		readInode(nodep, j);
		if((strncmp(name, nodep->_filename, 11)) == 0)  //Exist !! open it!!
		{
			temp->inodeBlockNum = j;
			temp->fileptr = 0;
			return temp;
		}
		i++;
		j++;
	}
	//printInodesTest(nodep);
	// file not found, create one
	if(spB->_numberOfDiskBlocks > 0)
	{
		i = 0, j = 1;
		while(i <= (spB->_numberOfInodes))
		{
			readInode(nodep, j);
			if(nodep->_flags == 0)
			{
				temp->inodeBlockNum = j;
				temp->fileptr = 0;
				return temp;
			}
			i++;
			j++;
		}
	}
	free(spB);
	free(nodep);
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
	int error = 0, block = stream->inodeBlockNum, blckRead = 0;
	inode_t* node = (inode_t*)malloc((sizeof(inode_t))+ 4);
	superBlock_t* supBlock = (superBlock_t*)malloc((sizeof(superBlock_t))+ 4);
	readInode(node, stream->inodeBlockNum);
	readSuperBlock(supBlock);
	long int currentPosition = 0;
	if(node->_flags == 0)
	{
		char buffer[BLOCK_SIZE];
		clearBuffer(buffer);
		int numBlocks = size/1022;
		int sizeRemain = size%1022;
		int wBlock = getFirstFreeBlock();
		writeBlockToInode(node, wBlock);
		while(numBlocks > 0)
		{
			while(blckRead < 1020)
			{
				int temp = byteArrayToInt(data, currentPosition);
				intToByteArray(temp, buffer, blckRead);
				currentPosition = currentPosition + 4;
				blckRead = blckRead + 4;
				stream->fileptr = stream->fileptr + 4;
			}
			numBlocks--;
			if((numBlocks == 0) && (sizeRemain == 0))
			{
				intToByteArray(-1, buffer, 1020);
			}
			else
			{
				intToByteArray(getFirstFreeBlock(), buffer, 1020);
			}
		}
	}
	else
	{
		error =-1;
	}

	free(supBlock);
	free(node);
	return error;
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
		printErrorDesc();
	}

	//read number of the first free block
	int numFirstFree = byteArrayToInt(buffer, 8);
	superBlock->_firstBlockOfFreeList = numFirstFree;

	// read number of inodes from block
	int numInodes =  byteArrayToInt(buffer, 4 );
	superBlock->_numberOfInodes = numInodes;

	// read number of disk blocks from block
	int numDiskBlocks = byteArrayToInt(buffer, 0);
	superBlock->_numberOfDiskBlocks = numDiskBlocks;

	// finish implementing this function
}

/**
 * Writes the super block to disk.
 *
 * @param superBlock - the disk's super block
 */
void writeSuperBlock(superBlock_t* superBlock )
{
	int error = 0;
	// setup buffer to be written to disk
	char buffer[BLOCK_SIZE];
	clearBuffer(buffer);

	// write number of disk blocks into buffer
	intToByteArray(superBlock->_numberOfDiskBlocks, buffer, 0);

	// write number of inodes into buffer
	intToByteArray(superBlock->_numberOfInodes, buffer, 4);

	//write the number of the first free block
	intToByteArray(superBlock->_firstBlockOfFreeList, buffer, 8);

	printf("\nThe size of int: %d\n", (int)sizeof(int));
	printf("The size of char: %d\n", (int)sizeof(char));
	printf("The buffer is: \n");
	int i;
	for(i = 0; i < 12;i++)
	{
		printf("%x ", ((int)buffer[i] & 0xff));
	}
	printf("\n");

	for(i = 12; i < 1024; i++)
	{
		buffer[i] = 0;
	}
	error = writeBlock(0, buffer, BLOCK_SIZE);
	if(error)
	{
		printErrorDesc();
	}

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
	int error, tempNum;
	char block[BLOCK_SIZE];
	// read block containing the Inode
    error = readBlock(blockNumber, block);

    int flag = byteArrayToInt(block, 0);
    int owner = byteArrayToInt(block, 4);
    int fileSize = byteArrayToInt(block, 8);
    char name[12];
    int i, j = 0;
    for(i = 12; i < 24; i++)
    {
    	inode->_filename[i-12] = block[i];
    }
    printf("\nFlag read in is: %d\n", flag);

    inode->_flags  = flag;
    inode->_owner = owner;
    inode->_filesize = fileSize;

    while(j < MAX_POINTERS)
    {
    	tempNum = byteArrayToInt(block, i);
    	inode->pointer[j] = tempNum;
    	j++;
    	i = i + 4;
    }

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
	int error = 0;
	// implement this function
	char buffer[BLOCK_SIZE];
	clearBuffer(buffer);
	//write flags to buffer
	intToByteArray(inode->_flags, buffer, 0);

	//write owner to buffer
	intToByteArray(inode->_owner, buffer, 4);

	//write file size to buffer
	intToByteArray(inode->_filesize, buffer, 8);

	//write file name to buffer
	int i, j = 0;
	for(i = 12; i < 24; i++)
	{
		buffer[i] = inode->_filename[i-12];
	}

	i = 0, j = 24;
	while(i < MAX_POINTERS)
	{
		if(inode->pointer[i] != -1)
		{
			intToByteArray(inode->pointer[i], buffer, j);
		}
		else
		{
			intToByteArray(-1, buffer, j);
		}
		i++;
		j = j + 4;
	}
	//buffer[1023] = 3;
	error = writeBlock(blockNumber, buffer, BLOCK_SIZE);
	if(error)
	{
		printErrorDesc();
	}
}

/**
 * Prints the content of the Inodes of the virtual disk to the screen.
 */
void printInode(inode_t* iNode)
{
	printf("The Inode is:\n");
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
    //printf("\nadding value: %d to position %d", value, pos);
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
	spBlck->_numberOfDiskBlocks = 4000;
	spBlck->_numberOfInodes = NUM_INODES;
	spBlck->_firstBlockOfFreeList = 17;
	return spBlck;
}

void printSupBlock(superBlock_t* spBlck)
{
	printf("Number of DiskBlocks: %d\n", spBlck->_numberOfDiskBlocks);
	printf("Number of Inodes: %d\n", spBlck->_numberOfInodes);
	printf("First Block Free in the List: %d\n", spBlck->_firstBlockOfFreeList);
}

void initializeFreeBlockList()
{
	int i;
	fprintf(stderr, "This runs\n");
	for(i = 2; i < 4; i++)
	{
		char buffer[BLOCK_SIZE];
		int j;
		for(j = 0; j < BLOCK_SIZE; j++)
		{
			buffer[j] = 255;
		}
		writeBlock(i, buffer, BLOCK_SIZE);
	}
}

void clearBuffer(char* buf)
{
	int i;
	for(i = 0; i < BLOCK_SIZE; i++)
	{
		buf[i] = 0;
	}
}

void writeFreeBlocks(int blockNumber)
{
	char buffer[BLOCK_SIZE];
	clearBuffer(buffer);
	writeBlock(blockNumber, buffer,BLOCK_SIZE);
}

int getNumInodes()
{
	char Buffer[BLOCK_SIZE];
	int temp;
	clearBuffer(Buffer);
	readBlock(0, Buffer);
	temp = byteArrayToInt(Buffer, 4);
	return temp;
}

int getNumBlocks()
{
	char Buffer[BLOCK_SIZE];
	int temp;
	clearBuffer(Buffer);
	readBlock(0, Buffer);
	temp = byteArrayToInt(Buffer, 0);
	return temp;
}

int getFirstFreeBlock()
{
	char Buffer[BLOCK_SIZE];
	int temp;
	clearBuffer(Buffer);
	readBlock(0, Buffer);
	temp = byteArrayToInt(Buffer, 8);
	intToByteArray(0, Buffer, 8);
	writeSuperBlock(Buffer);
	return temp;
}

void writeBlockToInode(inode_t* node, int block)
{
	if(node != NULL)
	{
		int i = 0;
		while((node->pointer[i] != -1) && (i < MAX_POINTERS))
		{
			i++;
		}
		if(i < MAX_POINTERS)
		{
			node->pointer[i] = block;
		}
	}
}

void checkNode(inode_t* node)
{
	if(node->pointer == NULL)
	{
		int i;
		for(i = 0; i < MAX_POINTERS; i++)
		{
			node->pointer[i] = -1;
		}
	}
}

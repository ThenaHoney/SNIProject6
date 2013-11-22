#ifndef SUPERBLOCK_H_
#define SUPERBLOCK_H_

/**
 * Models a super block in a file system.
 *
 * @author Thomas Reichherzer
 */

typedef struct superBlock
{
	int _numberOfDiskBlocks;
	int _numberOfInodes;
	int _firstBlockOfFreeList;
} superBlock_t;

#endif /*SUPERBLOCK_H_*/

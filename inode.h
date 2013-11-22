#ifndef INODE_H_
#define INODE_H_

/**
 * Models an inode in a file system.
 *
 * @author Thomas Reichherzer
 */

#define MAX_POINTERS 250

// indicates if the inode points to a file or a directory
typedef struct inode
{
	 // indicates if the inode points to a file or a directory
	 // 0 indicates that the Inode is unused
    int _flags;
    // indicates the owner of the file (an Id)
    int _owner;
    // indicates the size of the file
    int _filesize;
	 // specifies the filename (up to 8 characters plus . plus 3 letter extension is allowed)
	 char _filename[12];
    // a list of direct pointers to data blocks of the file
    int pointer[MAX_POINTERS];
} inode_t;

//Prototypes
void initializeInode(inode_t* inode);

#endif /*INODE_H_*/

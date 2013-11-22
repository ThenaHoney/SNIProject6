/**
 * Models an inode in a file system.
 *
 * @author Thomas Reichherzer
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inode.h"

    // complete this code

void initializeInode(inode_t* inode)
{
	int i;

	inode->_flags = 0;
	inode->_owner = 0;
	inode->_filesize = 0;
	inode->_filename[0] = '0';

    for (i=0; i < MAX_POINTERS; i++)
    {
        inode->pointer[i] = -1;
    }
}

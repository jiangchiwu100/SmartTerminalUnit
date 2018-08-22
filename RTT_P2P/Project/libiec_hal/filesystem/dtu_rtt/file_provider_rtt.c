/*
 *  file_provider_win32.c
 *
 *  Copyright 2014, 2015 Michael Zillgith
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>


#define MAX_PATH 32

#include "hal_filesystem.h"
#include <dfs_posix.h>
#include "libiec61850_platform_includes.h"
#include "stack_config.h"

struct sDirectoryHandle {
	int handle;
    //WIN32_FIND_DATAW findData;
	char utf8Filename[MAX_PATH * 3 + 1];
    bool available;
};


struct sFileHandle {
	int fd;

};

FileHandle
FileSystem_openFile(char* fileName, bool readWrite)
{
    FileHandle newHandle = NULL;
    int fd = 0;
    if (readWrite)
    {
    	fd =  open(fileName,  O_RDWR, 0 );
    }
     else
     {
           fd =  open(fileName,  O_WRONLY |O_RDWR, 0 );
     }
    if (fd > 0)
    {
    	newHandle = GLOBAL_MALLOC(sizeof(struct sFileHandle));
    	if(newHandle)
    	{
    		newHandle->fd =fd;
    	}
    }

    return newHandle;
}

int
FileSystem_readFile(FileHandle handle, uint8_t* buffer, int maxSize)
{
    return  read(handle->fd, (char*)buffer, maxSize);;
}

int
FileSystem_writeFile(FileHandle handle, uint8_t* buffer, int size)
{
	return write(handle->fd, (char*)buffer, size);

}

void
FileSystem_closeFile(FileHandle handle)
{
    close(handle->fd);
    GLOBAL_FREEMEM(handle);
}

bool
FileSystem_getFileInfo(char* filename, uint32_t* fileSize, uint64_t* lastModificationTimestamp)
{
    perror("Unimplent\n");
    return false;
}

DirectoryHandle
FileSystem_openDirectory(char* directoryName)
{
    DirectoryHandle dirHandle = NULL;
    perror("Unimplent\n");

        return dirHandle;
}

static char*
getNextDirectoryEntry(DirectoryHandle directory, bool* isDirectory)
{
	 perror("Unimplent\n");

    return NULL;
    
}


bool
FileSystem_deleteFile(char* filename)
{
	 perror("Unimplent\n");

    if (remove(filename) == 0)
        return true;
    else
        return false;
}

bool
FileSystem_renameFile(char* oldFilename, char* newFilename)
{
	 perror("Unimplent\n");

    if (rename(oldFilename, newFilename) == 0)
        return true;
    else
        return false;
}


char*
FileSystem_readDirectory(DirectoryHandle directory, bool* isDirectory)
{
    
	perror("Unimplent\n");
    return NULL;
}

void
FileSystem_closeDirectory(DirectoryHandle directory)
{
	perror("Unimplent\n");
}


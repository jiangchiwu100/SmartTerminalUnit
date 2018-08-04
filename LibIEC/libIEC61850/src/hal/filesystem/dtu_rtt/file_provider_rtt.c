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



#include "hal_filesystem.h"

#include "libiec61850_platform_includes.h"
#include "stack_config.h"




FileHandle
FileSystem_openFile(char* fileName, bool readWrite)
{
    FileHandle newHandle = NULL;



    return newHandle;
}

int
FileSystem_readFile(FileHandle handle, uint8_t* buffer, int maxSize)
{
    return 0;
}

int
FileSystem_writeFile(FileHandle handle, uint8_t* buffer, int size)
{
    return 0;
}

void
FileSystem_closeFile(FileHandle handle)
{
    fclose((FILE*) handle);
}

bool
FileSystem_getFileInfo(char* filename, uint32_t* fileSize, uint64_t* lastModificationTimestamp)
{
    
    return true;
}

DirectoryHandle
FileSystem_openDirectory(char* directoryName)
{
    DirectoryHandle dirHandle;

   
        return dirHandle;
}

static char*
getNextDirectoryEntry(DirectoryHandle directory, bool* isDirectory)
{
    
    return NULL;
    
}


bool
FileSystem_deleteFile(char* filename)
{
    if (remove(filename) == 0)
        return true;
    else
        return false;
}

bool
FileSystem_renameFile(char* oldFilename, char* newFilename)
{
    if (rename(oldFilename, newFilename) == 0)
        return true;
    else
        return false;
}


char*
FileSystem_readDirectory(DirectoryHandle directory, bool* isDirectory)
{
    

    return NULL;
}

void
FileSystem_closeDirectory(DirectoryHandle directory)
{
    
}


/*****************************************************************************
** $Source: WiiFile.c,v $
**
** $Revision: 1.2 $
**
** $Date: 2006/06/24 02:27:08 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
******************************************************************************
*/
#include "../Arch/ArchFile.h"

#include <windows.h>
#include <sys/stat.h>
#ifdef UNDER_CE
#include <Win32Wrappers.h>
#else
#include <direct.h>
#endif

int archCreateDirectory(const char* pathname)
{
    return CreateDirectoryA(pathname, NULL);
}

const char* archGetCurrentDirectory()
{
#ifndef UNDER_CE
    static char buf[512];
    char *p;
    (void)_getcwd(buf, sizeof(buf));
    p = buf;
    while( *p != '\0' ) {
        if( *p == '\\' ) *p = '/';
        p++;
    }
    p--;
    if( *p == '/' ) {
        *p = '\0';
    }
    return buf;
#else
    return "/User/MSX";
#endif
}

void archSetCurrentDirectory(const char* pathname)
{
#ifndef UNDER_CE
    _chdir(pathname);
#endif
}

int archFileExists(const char* fileName)
{
    struct stat s;
    return stat(fileName, &s) == 0;
}

FILE* archFileOpen(const char *fname, const char *mode)
{
    char fullpath[512];
    if( fname[0] == '/' || fname[0] == '\\' ||
        (fname[0] != '\0' && fname[1] == ':') ) {
        // It's an absolute path, use it
        strcpy(fullpath, fname);
    }else{
        // Relative path, make it an absolute path
        strcpy(fullpath, archGetCurrentDirectory());
        strcat(fullpath, "/");
        strcat(fullpath, fname);
    }
    return fopen(fullpath, mode);
}

/* File dialogs: */
char* archFilenameGetOpenRom(Properties* properties, int cartSlot, RomType* romType) { return NULL; }
char* archFilenameGetOpenDisk(Properties* properties, int drive, int allowCreate) { return NULL; }
char* archFilenameGetOpenHarddisk(Properties* properties, int drive, int allowCreate) { return NULL; }
char* archFilenameGetOpenCas(Properties* properties) { return NULL; }
char* archFilenameGetSaveCas(Properties* properties, int* type) { return NULL; }
char* archFilenameGetOpenState(Properties* properties) { return NULL; }
char* archFilenameGetOpenCapture(Properties* properties) { return NULL; }
char* archFilenameGetSaveState(Properties* properties) { return NULL; }
char* archDirnameGetOpenDisk(Properties* properties, int drive) { return NULL; }
char* archFilenameGetOpenRomZip(Properties* properties, int cartSlot, const char* fname, const char* fileList, int count, int* autostart, int* romType) { return NULL; }
char* archFilenameGetOpenDiskZip(Properties* properties, int drive, const char* fname, const char* fileList, int count, int* autostart) { return NULL; }
char* archFilenameGetOpenCasZip(Properties* properties, const char* fname, const char* fileList, int count, int* autostart) { return NULL; }
char* archFilenameGetOpenAnyZip(Properties* properties, const char* fname, const char* fileList, int count, int* autostart, int* romType) { return NULL; }


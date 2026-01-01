#include <windows.h>
#include <stdio.h>
#include <sys/stat.h>

void* __builtin_frame_address(unsigned int level)
{
    return NULL;
}

FILE* popen(const char* command, const char* mode)
{
    return _popen(command, mode);
}

int pclose(FILE* stream)
{
    return _pclose(stream);
}

#ifndef S_ISDIR
#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif

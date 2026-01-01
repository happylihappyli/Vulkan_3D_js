/*
 * dirent_compat.h - Windows兼容性头文件
 * 提供POSIX dirent.h中定义的函数和结构体
 */

#ifndef DIRENT_COMPAT_H
#define DIRENT_COMPAT_H

#ifdef _WIN32

#include <windows.h>
#include <string.h>
#include <stdio.h>

// POSIX dirent结构体
struct dirent {
    long d_ino;              // inode号（Windows上为0）
    unsigned short d_reclen; // 记录长度
    unsigned short d_namlen; // 文件名长度
    char d_name[MAX_PATH];   // 文件名
};

// DIR结构体
typedef struct {
    HANDLE handle;           // Windows查找句柄
    WIN32_FIND_DATAA find_data; // 查找结果数据
    struct dirent dirent;    // 当前目录项
    int is_first;            // 是否是第一次调用
} DIR;

// 函数声明
DIR* opendir(const char* dirname);
struct dirent* readdir(DIR* dirp);
int closedir(DIR* dirp);
void rewinddir(DIR* dirp);

#else
// 非Windows平台使用标准dirent.h
#include <dirent.h>
#endif

#endif // DIRENT_COMPAT_H

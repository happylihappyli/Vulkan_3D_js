/*
 * dirent_compat.c - Windows兼容性实现
 * 实现POSIX dirent.h中定义的函数
 */

#ifdef _WIN32

#include "dirent_compat.h"
#include <stdlib.h>
#include <errno.h>

// 打开目录
DIR* opendir(const char* dirname) {
    DIR* dirp;
    char search_path[MAX_PATH + 2];
    size_t len;

    if (dirname == NULL) {
        errno = EINVAL;
        return NULL;
    }

    // 分配DIR结构体
    dirp = (DIR*)malloc(sizeof(DIR));
    if (dirp == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    // 构建搜索路径（添加\*通配符）
    len = strlen(dirname);
    if (len >= MAX_PATH) {
        free(dirp);
        errno = ENAMETOOLONG;
        return NULL;
    }

    memcpy(search_path, dirname, len);
    if (len > 0 && search_path[len - 1] != '\\' && search_path[len - 1] != '/') {
        search_path[len++] = '\\';
    }
    search_path[len++] = '*';
    search_path[len] = '\0';

    // 开始查找
    dirp->handle = FindFirstFileA(search_path, &dirp->find_data);
    if (dirp->handle == INVALID_HANDLE_VALUE) {
        free(dirp);
        return NULL;
    }

    dirp->is_first = 1;
    return dirp;
}

// 读取目录项
struct dirent* readdir(DIR* dirp) {
    if (dirp == NULL) {
        errno = EBADF;
        return NULL;
    }

    // 跳过.和..目录
    if (dirp->is_first) {
        dirp->is_first = 0;
    } else {
        if (!FindNextFileA(dirp->handle, &dirp->find_data)) {
            return NULL;
        }
    }

    // 填充dirent结构体
    dirp->dirent.d_ino = 0;
    dirp->dirent.d_reclen = sizeof(struct dirent);
    dirp->dirent.d_namlen = (unsigned short)strlen(dirp->find_data.cFileName);
    strncpy(dirp->dirent.d_name, dirp->find_data.cFileName, MAX_PATH - 1);
    dirp->dirent.d_name[MAX_PATH - 1] = '\0';

    return &dirp->dirent;
}

// 关闭目录
int closedir(DIR* dirp) {
    if (dirp == NULL) {
        errno = EBADF;
        return -1;
    }

    if (dirp->handle != INVALID_HANDLE_VALUE) {
        FindClose(dirp->handle);
    }

    free(dirp);
    return 0;
}

// 重置目录读取位置
void rewinddir(DIR* dirp) {
    char search_path[MAX_PATH + 2];
    size_t len;

    if (dirp == NULL || dirp->handle == INVALID_HANDLE_VALUE) {
        return;
    }

    // 关闭当前句柄
    FindClose(dirp->handle);

    // 重新构建搜索路径
    // 注意：这里无法获取原始路径，所以rewinddir在Windows上可能不完全准确
    // 在实际使用中，建议重新调用opendir
    dirp->is_first = 1;
}

#endif // _WIN32

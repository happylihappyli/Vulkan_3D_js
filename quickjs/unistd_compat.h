/*
 * unistd_compat.h - Windows兼容性头文件
 * 提供POSIX unistd.h中定义的函数和常量
 */

#ifndef UNISTD_COMPAT_H
#define UNISTD_COMPAT_H

#ifdef _WIN32

#include <io.h>
#include <process.h>
#include <direct.h>

// POSIX常量定义
#define F_OK 0
#define X_OK 1
#define W_OK 2
#define R_OK 4

// 文件访问函数
#define access _access

// 文件描述符操作
#define close _close
#define read _read
#define write _write
#define lseek _lseek
#define isatty _isatty
#define unlink _unlink

// 进程相关函数
#define getpid _getpid
#define sleep(x) Sleep((x) * 1000)
#define usleep(x) Sleep((x) / 1000)

// 文件路径相关
#define getcwd _getcwd
#define chdir _chdir
#define mkdir(x) _mkdir(x)
#define rmdir _rmdir

// 管道和重定向
#define dup _dup
#define dup2 _dup2
#define pipe _pipe

// 文件控制
#define open _open
#define creat _creat

// 其他POSIX函数
#define fsync _commit

// 文件锁相关
#define O_RDONLY _O_RDONLY
#define O_WRONLY _O_WRONLY
#define O_RDWR _O_RDWR
#define O_APPEND _O_APPEND
#define O_CREAT _O_CREAT
#define O_TRUNC _O_TRUNC
#define O_EXCL _O_EXCL
#define O_TEXT _O_TEXT
#define O_BINARY _O_BINARY

// 文件权限
#define S_IRUSR _S_IREAD
#define S_IWUSR _S_IWRITE

// 文件类型
#define S_IFREG _S_IFREG
#define S_IFDIR _S_IFDIR
#ifndef S_IFIFO
#define S_IFIFO 0010000
#endif
#ifndef S_IFBLK
#define S_IFBLK 0060000
#endif
#ifndef S_IFCHR
#define S_IFCHR 0020000
#endif
#ifndef S_IFMT
#define S_IFMT  0170000
#endif

// 文件类型检查宏
#ifndef S_ISDIR
#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif
#ifndef S_ISREG
#define S_ISREG(mode) (((mode) & S_IFMT) == S_IFREG)
#endif

// 路径分隔符
#define PATH_SEPARATOR '\\'
#define PATH_SEPARATOR_STRING "\\"

// Windows上缺少的ssize_t类型定义
#ifdef _MSC_VER
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#else
// 非Windows平台使用标准unistd.h
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#endif // UNISTD_COMPAT_H

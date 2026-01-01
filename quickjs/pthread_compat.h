#pragma once

#ifdef _WIN32

#include <windows.h>
#include <process.h>

#define PTHREAD_MUTEX_INITIALIZER { 0 }

typedef CRITICAL_SECTION pthread_mutex_t;
typedef struct {
    HANDLE handle;
    CRITICAL_SECTION cs;
    int initialized;
} pthread_cond_t;

typedef unsigned int pthread_t;
typedef struct {
    int detach_state;
} pthread_attr_t;

#define PTHREAD_CREATE_DETACHED 1

int pthread_mutex_init(pthread_mutex_t* mutex, void* attr);
int pthread_mutex_lock(pthread_mutex_t* mutex);
int pthread_mutex_unlock(pthread_mutex_t* mutex);
int pthread_mutex_destroy(pthread_mutex_t* mutex);

int pthread_cond_init(pthread_cond_t* cond, void* attr);
int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex);
int pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abstime);
int pthread_cond_signal(pthread_cond_t* cond);
int pthread_cond_destroy(pthread_cond_t* cond);

int pthread_attr_init(pthread_attr_t* attr);
int pthread_attr_setdetachstate(pthread_attr_t* attr, int detachstate);
int pthread_attr_destroy(pthread_attr_t* attr);
int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine)(void*), void* arg);

#endif

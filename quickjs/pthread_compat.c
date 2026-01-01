#include "pthread_compat.h"
#include "sys_time_compat.h"
#include <stdlib.h>

#ifdef _WIN32

int pthread_mutex_init(pthread_mutex_t* mutex, void* attr) {
    if (mutex == NULL) {
        return -1;
    }
    InitializeCriticalSection(mutex);
    return 0;
}

int pthread_mutex_lock(pthread_mutex_t* mutex) {
    if (mutex == NULL) {
        return -1;
    }
    EnterCriticalSection(mutex);
    return 0;
}

int pthread_mutex_unlock(pthread_mutex_t* mutex) {
    if (mutex == NULL) {
        return -1;
    }
    LeaveCriticalSection(mutex);
    return 0;
}

int pthread_mutex_destroy(pthread_mutex_t* mutex) {
    if (mutex == NULL) {
        return -1;
    }
    DeleteCriticalSection(mutex);
    return 0;
}

int pthread_cond_init(pthread_cond_t* cond, void* attr) {
    if (cond == NULL) {
        return -1;
    }
    cond->initialized = 0;
    cond->handle = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (cond->handle == NULL) {
        return -1;
    }
    InitializeCriticalSection(&cond->cs);
    cond->initialized = 1;
    return 0;
}

int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex) {
    if (cond == NULL || mutex == NULL || !cond->initialized) {
        return -1;
    }
    LeaveCriticalSection(mutex);
    WaitForSingleObject(cond->handle, INFINITE);
    EnterCriticalSection(mutex);
    return 0;
}

int pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abstime) {
    if (cond == NULL || mutex == NULL || !cond->initialized || abstime == NULL) {
        return -1;
    }
    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    DWORD timeout_ms = (abstime->tv_sec - tv.tv_sec) * 1000 + (abstime->tv_nsec / 1000000 - tv.tv_usec / 1000);
    
    LeaveCriticalSection(mutex);
    DWORD result = WaitForSingleObject(cond->handle, timeout_ms);
    EnterCriticalSection(mutex);
    
    if (result == WAIT_TIMEOUT) {
        return ETIMEDOUT;
    }
    return 0;
}

int pthread_cond_signal(pthread_cond_t* cond) {
    if (cond == NULL || !cond->initialized) {
        return -1;
    }
    SetEvent(cond->handle);
    return 0;
}

int pthread_cond_destroy(pthread_cond_t* cond) {
    if (cond == NULL || !cond->initialized) {
        return -1;
    }
    CloseHandle(cond->handle);
    DeleteCriticalSection(&cond->cs);
    cond->initialized = 0;
    return 0;
}

int pthread_attr_init(pthread_attr_t* attr) {
    if (attr == NULL) {
        return -1;
    }
    attr->detach_state = 0;
    return 0;
}

int pthread_attr_setdetachstate(pthread_attr_t* attr, int detachstate) {
    if (attr == NULL) {
        return -1;
    }
    attr->detach_state = detachstate;
    return 0;
}

int pthread_attr_destroy(pthread_attr_t* attr) {
    if (attr == NULL) {
        return -1;
    }
    return 0;
}

typedef struct {
    void* (*start_routine)(void*);
    void* arg;
} thread_wrapper_arg;

static unsigned __stdcall thread_wrapper(void* arg) {
    thread_wrapper_arg* wrapper = (thread_wrapper_arg*)arg;
    void* (*start_routine)(void*) = wrapper->start_routine;
    void* routine_arg = wrapper->arg;
    free(wrapper);
    start_routine(routine_arg);
    return 0;
}

int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine)(void*), void* arg) {
    if (thread == NULL || start_routine == NULL) {
        return -1;
    }
    
    thread_wrapper_arg* wrapper = (thread_wrapper_arg*)malloc(sizeof(thread_wrapper_arg));
    if (wrapper == NULL) {
        return -1;
    }
    
    wrapper->start_routine = start_routine;
    wrapper->arg = arg;
    
    HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, thread_wrapper, wrapper, 0, NULL);
    if (handle == NULL) {
        free(wrapper);
        return -1;
    }
    
    *thread = (unsigned int)handle;
    return 0;
}

#endif

#include "sys_time_compat.h"

#ifdef _WIN32

int gettimeofday(struct timeval* tv, struct timezone* tz) {
    if (tv == NULL) {
        return -1;
    }
    
    FILETIME ft;
    ULARGE_INTEGER uli;
    
    GetSystemTimeAsFileTime(&ft);
    
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    
    // 转换为微秒数（从1601年1月1日到1970年1月1日的时间差）
    uli.QuadPart -= 116444736000000000ULL;
    
    tv->tv_sec = (long)(uli.QuadPart / 10000000ULL);
    tv->tv_usec = (long)((uli.QuadPart % 10000000ULL) / 10ULL);
    
    if (tz != NULL) {
        tz->tz_minuteswest = _timezone / 60;
        tz->tz_dsttime = _daylight;
    }
    
    printf("[DEBUG] gettimeofday: tv_sec=%ld, tv_usec=%ld\n", tv->tv_sec, tv->tv_usec);
    
    return 0;
}

int clock_gettime(int clock_id, struct timespec* tp) {
    if (tp == NULL) {
        return -1;
    }
    
    FILETIME ft;
    ULARGE_INTEGER uli;
    
    GetSystemTimeAsFileTime(&ft);
    
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    
    // 转换为纳秒数（从1601年1月1日到1970年1月1日的时间差）
    uli.QuadPart -= 116444736000000000ULL;
    
    tp->tv_sec = (long)(uli.QuadPart / 10000000ULL);
    tp->tv_nsec = (long)((uli.QuadPart % 10000000ULL) * 100ULL);
    
    return 0;
}

#endif

#pragma once

#ifdef _WIN32
#include <windows.h>
#include <winsock.h>
#include <time.h>

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

int gettimeofday(struct timeval* tv, struct timezone* tz);

#define CLOCK_REALTIME 0

#ifndef CLOCK_GETTIME_DEFINED
#define CLOCK_GETTIME_DEFINED

int clock_gettime(int clock_id, struct timespec* tp);

#endif

#endif

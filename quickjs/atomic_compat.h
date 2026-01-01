#pragma once

#ifdef _WIN32

#include <windows.h>

#define atomic_thread_fence(x) MemoryBarrier()
#define atomic_signal_fence(x) 

#define ATOMIC_BOOL_LOCK_FREE 2
#define ATOMIC_CHAR_LOCK_FREE 2
#define ATOMIC_CHAR16_T_LOCK_FREE 2
#define ATOMIC_CHAR32_T_LOCK_FREE 2
#define ATOMIC_WCHAR_T_LOCK_FREE 2
#define ATOMIC_SHORT_LOCK_FREE 2
#define ATOMIC_INT_LOCK_FREE 2
#define ATOMIC_LONG_LOCK_FREE 2
#define ATOMIC_LLONG_LOCK_FREE 2
#define ATOMIC_POINTER_LOCK_FREE 2

#define memory_order_relaxed 0
#define memory_order_consume 1
#define memory_order_acquire 2
#define memory_order_release 3
#define memory_order_acq_rel 4
#define memory_order_seq_cst 5

typedef volatile uint8_t _Atomic_uint8_t;
typedef volatile uint16_t _Atomic_uint16_t;
typedef volatile uint32_t _Atomic_uint32_t;
typedef volatile uint64_t _Atomic_uint64_t;
typedef volatile int8_t _Atomic_int8_t;
typedef volatile int16_t _Atomic_int16_t;
typedef volatile int32_t _Atomic_int32_t;
typedef volatile int64_t _Atomic_int64_t;

#define _Atomic(T) volatile T

#define atomic_load_8(ptr) (*(volatile uint8_t*)(ptr))
#define atomic_load_16(ptr) (*(volatile uint16_t*)(ptr))
#define atomic_load_32(ptr) (*(volatile uint32_t*)(ptr))
#define atomic_load_64(ptr) (*(volatile uint64_t*)(ptr))

#define atomic_store_8(ptr, value) (*(volatile uint8_t*)(ptr) = (value))
#define atomic_store_16(ptr, value) (*(volatile uint16_t*)(ptr) = (value))
#define atomic_store_32(ptr, value) (*(volatile uint32_t*)(ptr) = (value))
#define atomic_store_64(ptr, value) (*(volatile uint64_t*)(ptr) = (value))

#define atomic_fetch_add_8(ptr, value) InterlockedExchangeAdd8((volatile char*)(ptr), (char)(value))
#define atomic_fetch_add_16(ptr, value) InterlockedExchangeAdd16((volatile short*)(ptr), (short)(value))
#define atomic_fetch_add_32(ptr, value) InterlockedExchangeAdd((volatile long*)(ptr), (long)(value))
#define atomic_fetch_add_64(ptr, value) InterlockedExchangeAdd64((volatile long long*)(ptr), (long long)(value))

#define atomic_fetch_sub_8(ptr, value) InterlockedExchangeAdd8((volatile char*)(ptr), -(char)(value))
#define atomic_fetch_sub_16(ptr, value) InterlockedExchangeAdd16((volatile short*)(ptr), -(short)(value))
#define atomic_fetch_sub_32(ptr, value) InterlockedExchangeAdd((volatile long*)(ptr), -(long)(value))
#define atomic_fetch_sub_64(ptr, value) InterlockedExchangeAdd64((volatile long long*)(ptr), -(long long)(value))

#define atomic_fetch_and_8(ptr, value) InterlockedAnd8((volatile char*)(ptr), (char)(value))
#define atomic_fetch_and_16(ptr, value) InterlockedAnd16((volatile short*)(ptr), (short)(value))
#define atomic_fetch_and_32(ptr, value) InterlockedAnd((volatile long*)(ptr), (long)(value))
#define atomic_fetch_and_64(ptr, value) InterlockedAnd64((volatile long long*)(ptr), (long long)(value))

#define atomic_fetch_or_8(ptr, value) InterlockedOr8((volatile char*)(ptr), (char)(value))
#define atomic_fetch_or_16(ptr, value) InterlockedOr16((volatile short*)(ptr), (short)(value))
#define atomic_fetch_or_32(ptr, value) InterlockedOr((volatile long*)(ptr), (long)(value))
#define atomic_fetch_or_64(ptr, value) InterlockedOr64((volatile long long*)(ptr), (long long)(value))

#define atomic_fetch_xor_8(ptr, value) InterlockedXor8((volatile char*)(ptr), (char)(value))
#define atomic_fetch_xor_16(ptr, value) InterlockedXor16((volatile short*)(ptr), (short)(value))
#define atomic_fetch_xor_32(ptr, value) InterlockedXor((volatile long*)(ptr), (long)(value))
#define atomic_fetch_xor_64(ptr, value) InterlockedXor64((volatile long long*)(ptr), (long long)(value))

#define atomic_exchange_8(ptr, value) InterlockedExchange8((volatile char*)(ptr), (char)(value))
#define atomic_exchange_16(ptr, value) InterlockedExchange16((volatile short*)(ptr), (short)(value))
#define atomic_exchange_32(ptr, value) InterlockedExchange((volatile long*)(ptr), (long)(value))
#define atomic_exchange_64(ptr, value) InterlockedExchange64((volatile long long*)(ptr), (long long)(value))

static inline char InterlockedCompareExchange8(volatile char* ptr, char desired, char expected) {
    return _InterlockedCompareExchange8(ptr, desired, expected);
}

static inline int atomic_compare_exchange_strong_8(volatile uint8_t* ptr, uint8_t* expected, uint8_t desired) {
    uint8_t old = (uint8_t)InterlockedCompareExchange8((volatile char*)ptr, (char)desired, (char)*expected);
    if (old == *expected) {
        return 1;
    }
    *expected = old;
    return 0;
}

static inline int atomic_compare_exchange_strong_16(volatile uint16_t* ptr, uint16_t* expected, uint16_t desired) {
    uint16_t old = (uint16_t)InterlockedCompareExchange16((volatile short*)ptr, (short)desired, (short)*expected);
    if (old == *expected) {
        return 1;
    }
    *expected = old;
    return 0;
}

static inline int atomic_compare_exchange_strong_32(volatile uint32_t* ptr, uint32_t* expected, uint32_t desired) {
    uint32_t old = (uint32_t)InterlockedCompareExchange((volatile long*)ptr, (long)desired, (long)*expected);
    if (old == *expected) {
        return 1;
    }
    *expected = old;
    return 0;
}

static inline int atomic_compare_exchange_strong_64(volatile uint64_t* ptr, uint64_t* expected, uint64_t desired) {
    uint64_t old = (uint64_t)InterlockedCompareExchange64((volatile long long*)ptr, (long long)desired, (long long)*expected);
    if (old == *expected) {
        return 1;
    }
    *expected = old;
    return 0;
}

#define atomic_load(ptr) \
    ((sizeof(*(ptr)) == 1) ? atomic_load_8(ptr) : \
     (sizeof(*(ptr)) == 2) ? atomic_load_16(ptr) : \
     (sizeof(*(ptr)) == 4) ? atomic_load_32(ptr) : \
     atomic_load_64(ptr))

#define atomic_store(ptr, value) \
    ((sizeof(*(ptr)) == 1) ? atomic_store_8(ptr, value) : \
     (sizeof(*(ptr)) == 2) ? atomic_store_16(ptr, value) : \
     (sizeof(*(ptr)) == 4) ? atomic_store_32(ptr, value) : \
     atomic_store_64(ptr, value))

#define atomic_fetch_add(ptr, value) \
    ((sizeof(*(ptr)) == 1) ? atomic_fetch_add_8(ptr, value) : \
     (sizeof(*(ptr)) == 2) ? atomic_fetch_add_16(ptr, value) : \
     (sizeof(*(ptr)) == 4) ? atomic_fetch_add_32(ptr, value) : \
     atomic_fetch_add_64(ptr, value))

#define atomic_fetch_sub(ptr, value) \
    ((sizeof(*(ptr)) == 1) ? atomic_fetch_sub_8(ptr, value) : \
     (sizeof(*(ptr)) == 2) ? atomic_fetch_sub_16(ptr, value) : \
     (sizeof(*(ptr)) == 4) ? atomic_fetch_sub_32(ptr, value) : \
     atomic_fetch_sub_64(ptr, value))

#define atomic_fetch_and(ptr, value) \
    ((sizeof(*(ptr)) == 1) ? atomic_fetch_and_8(ptr, value) : \
     (sizeof(*(ptr)) == 2) ? atomic_fetch_and_16(ptr, value) : \
     (sizeof(*(ptr)) == 4) ? atomic_fetch_and_32(ptr, value) : \
     atomic_fetch_and_64(ptr, value))

#define atomic_fetch_or(ptr, value) \
    ((sizeof(*(ptr)) == 1) ? atomic_fetch_or_8(ptr, value) : \
     (sizeof(*(ptr)) == 2) ? atomic_fetch_or_16(ptr, value) : \
     (sizeof(*(ptr)) == 4) ? atomic_fetch_or_32(ptr, value) : \
     atomic_fetch_or_64(ptr, value))

#define atomic_fetch_xor(ptr, value) \
    ((sizeof(*(ptr)) == 1) ? atomic_fetch_xor_8(ptr, value) : \
     (sizeof(*(ptr)) == 2) ? atomic_fetch_xor_16(ptr, value) : \
     (sizeof(*(ptr)) == 4) ? atomic_fetch_xor_32(ptr, value) : \
     atomic_fetch_xor_64(ptr, value))

#define atomic_exchange(ptr, value) \
    ((sizeof(*(ptr)) == 1) ? atomic_exchange_8(ptr, value) : \
     (sizeof(*(ptr)) == 2) ? atomic_exchange_16(ptr, value) : \
     (sizeof(*(ptr)) == 4) ? atomic_exchange_32(ptr, value) : \
     atomic_exchange_64(ptr, value))

#define atomic_compare_exchange_strong(ptr, expected, desired) \
    ((sizeof(*(ptr)) == 1) ? atomic_compare_exchange_strong_8((volatile uint8_t*)(ptr), (uint8_t*)(expected), (desired)) : \
     (sizeof(*(ptr)) == 2) ? atomic_compare_exchange_strong_16((volatile uint16_t*)(ptr), (uint16_t*)(expected), (desired)) : \
     (sizeof(*(ptr)) == 4) ? atomic_compare_exchange_strong_32((volatile uint32_t*)(ptr), (uint32_t*)(expected), (desired)) : \
     atomic_compare_exchange_strong_64((volatile uint64_t*)(ptr), (uint64_t*)(expected), (desired)))

#endif

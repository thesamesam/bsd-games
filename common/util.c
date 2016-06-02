// Copyright (c) 2016 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT license.

#include "../config.h"

// The standard bsd checksum, summing with right rotation
uint16_t bsdsum (const void* v, size_t n, uint16_t sum)
{
    const uint8_t* s = (const uint8_t*) v;
    for (; n--; ++s) {
	sum = ror16 (sum, 1);
	sum += *s;
    }
    return sum;
}

// Randomly initializes the random number generator
void srandrand (void)
{
    struct timespec now;
    clock_gettime (CLOCK_REALTIME, &now);
    uint32_t seed = now.tv_sec;
    seed ^= getppid();
    seed = ror32 (seed, 16);
    seed ^= getpid();
    seed ^= now.tv_nsec;
    srand (seed);
}

// Generate a random number in given range
unsigned nrand (unsigned r)
{
    return rand() % r;
}

// Return current time in milliseconds
uint64_t time_ms (void)
{
    struct timespec now;
    clock_gettime (CLOCK_REALTIME, &now);
    return now.tv_sec*1000 + now.tv_nsec/1000000;
}

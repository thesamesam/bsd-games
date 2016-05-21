// This file is free software, distributed under the BSD license.
#include "../config.h"

// The standard bsd checksum, summing with right rotation
uint16_t bsdsum (const void* v, size_t n, uint16_t sum)
{
    const uint8_t* s = (const uint8_t*) v;
    for (; n--; ++s) {
	sum = (sum >> 1)|(sum << 15);
	sum += *s;
    }
    return sum;
}

/// Randomly initializes the random number generator
void srandrand (void)
{
    struct timespec now;
    clock_gettime (CLOCK_REALTIME, &now);
    srand (now.tv_sec ^ now.tv_nsec ^ ((uint32_t)getpid() << 16) ^ getppid());
}

/// Generate a random number in given range
unsigned nrand (unsigned r)
{
    return rand() % r;
}

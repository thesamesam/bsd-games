// Copyright (c) 2016 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT license.

#include "../config.h"
#include <sys/stat.h>

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

// Creates all directories in path
int mkpath (const char* path, mode_t mode)
{
    char pbuf [PATH_MAX], *pbe = pbuf;
    do {
	if (*path == '/' || !*path) {
	    *pbe = '\0';
	    if (pbuf[0] && 0 > mkdir (pbuf, mode) && errno != EEXIST)
		return -1;
	}
	*pbe++ = *path;
    } while (*path++);
    return 0;
}

void StringBuilder_skip (struct StringBuilder* sb, ssize_t n) {
    if (n < 0) {
	sb->s[0] = 0; // snprintf error - ensure terminator is restored
	return;
    }
    assert (n <= sb->n && "StringBuilder buffer full");
    if ((size_t) n > sb->n)
	n = sb->n;
    sb->s += n;
    sb->n -= n;
}

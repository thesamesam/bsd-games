// This file is free software, distributed under the BSD license.

#pragma once
#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif
#define _POSIX_C_SOURCE	200809L
#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// Common function and variable attributes
#if __GNUC__ && !defined(UNUSED)
    #define UNUSED		__attribute__((unused))
    #define NORETURN		__attribute__((noreturn))
    #define PRINTFLIKE(f,a)	__attribute__((__format__(__printf__,f,a)))
#endif

// Returns number of elements in array a
#define ArraySize(a)	(sizeof(a)/sizeof(a[0]))

// Curses transparent color
enum { COLOR_DEFAULT = -1 };

// The standard bsd checksum, summing with right rotation
inline static uint16_t bsdsum (const void* v, size_t n, uint16_t sum)
{
    const uint8_t* s = (const uint8_t*) v;
    for (; n--; ++s) {
	sum = (sum >> 1)|(sum << 15);
	sum += *s;
    }
    return sum;
}

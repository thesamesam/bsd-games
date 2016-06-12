// Copyright (c) 2016 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT license.

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
#include <time.h>

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

// Common utility functions
#ifdef __cplusplus
extern "C" {
#endif

// util.c
uint16_t bsdsum (const void* v, size_t n, uint16_t sum);
unsigned nrand (unsigned r);
void srandrand (void);
uint64_t time_ms (void);

// ui.c
void initialize_curses (void);
void cleanup_curses (void);

// scores.c
bool read_score_file (const char* filename, const char* magic, void* scores, size_t scoresSize);
void write_score_file (const char* filename, const char* magic, const void* scores, size_t scoresSize);

// inlines
inline static uint32_t ror32 (uint32_t v, unsigned n)
    { return (v >> n)|(v << (32-n)); }
inline static uint16_t ror16 (uint16_t v, unsigned n)
    { return (v >> n)|(v << (16-n)); }
inline static int sign (int n)
    { return n > 0 ? 1 : n < 0 ? -1 : 0; }
inline static unsigned absv (int n)
    { return n >= 0 ? n : -n; }
inline static unsigned min_u (unsigned a, unsigned b)
    { return a < b ? a : b; }
inline static int min_i (int a, int b)
    { return a < b ? a : b; }
inline static unsigned max_u (unsigned a, unsigned b)
    { return a < b ? b : a; }
inline static int max_i (int a, int b)
    { return a < b ? b : a; }

#ifdef __cplusplus
} // extern "C"
#endif

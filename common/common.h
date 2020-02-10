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

//{{{ Common function and variable attributes --------------------------
#if __GNUC__ && !defined(UNUSED)
    #define UNUSED		__attribute__((unused))
    #define PRINTFLIKE(f,a)	__attribute__((__format__(__printf__,f,a)))
    #ifndef __cplusplus
	#define noexcept	__attribute__((nothrow))
	#define constexpr	__attribute__((const))
    #endif
#endif
//}}}-------------------------------------------------------------------
//{{{ clang quirks
#if __clang__
    #define atomic_exchange(o,v)	__c11_atomic_exchange(o,v,__ATOMIC_SEQ_CST)
#else
    #include <stdatomic.h>
#endif

//}}}-------------------------------------------------------------------

#include "vector.h"

//{{{ Types and constants ----------------------------------------------

// Curses transparent color
enum { COLOR_DEFAULT = -1 };
// Escape key
enum {
    KEY_ESCAPE = '\e',
    KEY_BKSPACE = '~'+1
};
// Color pair definitions array
struct color_pair {
    int8_t	fg:4;
    int8_t	bg:4;
};

// For building string with multiple append calls
struct StringBuilder {
    char*	s;
    size_t	n;
};

//}}}-------------------------------------------------------------------
//{{{ Extern prototypes from common/*.c

// Common utility functions
#ifdef __cplusplus
extern "C" {
#endif

// util.c
uint16_t bsdsum (const void* v, size_t n, uint16_t sum);
unsigned nrand (unsigned r);
void srandrand (void);
uint64_t time_ms (void);
int mkpath (const char* path, mode_t mode);
void StringBuilder_skip (struct StringBuilder* sb, ssize_t n);

// ui.c
void initialize_curses (void);
void cleanup_curses (void);
void init_pairs (const struct color_pair* cps, size_t ncps);

// scores.c
bool read_score_file (const char* filename, const char* magic, void* scores, size_t scoresSize);
void write_score_file (const char* filename, const char* magic, const void* scores, size_t scoresSize);

//}}}-------------------------------------------------------------------
//{{{ Inline utility functions

inline static uint32_t ror32 (uint32_t v, unsigned n)
    { return (v >> n)|(v << (32-n)); }
inline static uint16_t ror16 (uint16_t v, unsigned n)
    { return (v >> n)|(v << (16-n)); }
inline static int sign (int n)
    { return n > 0 ? 1 : n < 0 ? -1 : 0; }
inline static unsigned absv (int n)
    { return n >= 0 ? n : -n; }
inline static unsigned square (int x)
    { return x*x; }
inline static unsigned min_u (unsigned a, unsigned b)
    { return a < b ? a : b; }
inline static int min_i (int a, int b)
    { return a < b ? a : b; }
inline static unsigned max_u (unsigned a, unsigned b)
    { return a < b ? b : a; }
inline static int max_i (int a, int b)
    { return a < b ? b : a; }
static inline size_t DivRU (size_t n, size_t d)
    { return (n+(d-1))/d; }
static inline size_t Floor (size_t n, size_t grain)
    { return n - n % grain; }
static inline size_t Align (size_t n, size_t grain)
    { return Floor (n+grain-1, grain); }
static inline void swap_u8 (uint8_t* a, uint8_t* b)
    { uint8_t t = *a; *a = *b; *b = t; }
static inline void swap_u16 (uint16_t* a, uint16_t* b)
    { uint16_t t = *a; *a = *b; *b = t; }
static inline void swap_u32 (uint32_t* a, uint32_t* b)
    { uint32_t t = *a; *a = *b; *b = t; }
static inline void iota_u8 (uint8_t* v, size_t n)
    { for (size_t i = 0; i < n; ++i) v[i] = i; }
static inline void random_shuffle_u8 (uint8_t* v, size_t n)
    { for (; n; --n,++v) swap_u8 (v, v+nrand(n)); }

//}}}-------------------------------------------------------------------
//{{{ x86 string functions

static inline const char* scasb (const char* p, size_t* n, char c)
{
#if __i386__ || __x86_64__
    __asm__ ("repnz\tscasb":"+D"(p),"+c"(*n):"a"(c):"memory");
#else
    do { ++p; } while (--*n && p[-1] != c);
#endif
    return p;
}

static inline const char* strnext_r (const char* s, size_t* n)
    { return scasb (s, n, '\0'); }

static inline const char* zstrn (const char* strs, unsigned n, unsigned nstrs)
{
    for (size_t i = min_u(n,nstrs-1)+1,sz=-1; --i;)
	strs = strnext_r(strs,&sz);
    return strs;
}

//}}}-------------------------------------------------------------------
//{{{ StringBuilder

#define StringBuilder_new(buf)	{ &buf[0], ArraySize(buf) }

static inline const char* StringBuilder_ptr (const struct StringBuilder* sb)
    { return sb->s; }
static inline size_t StringBuilder_remaining (const struct StringBuilder* sb)
    { return sb->n; }
static inline const char* StringBuilder_end (const struct StringBuilder* sb)
    { return StringBuilder_ptr(sb) + StringBuilder_remaining(sb); }
static inline void StringBuilder_append (struct StringBuilder* sb, const char* s)
    { StringBuilder_skip (sb, snprintf (sb->s, sb->n, "%s", s)); }
#define StringBuilder_appendf(sb,fmt,...)\
    StringBuilder_skip ((sb), snprintf ((sb)->s, (sb)->n, (fmt), __VA_ARGS__))
static inline void StringBuilder_backspace (struct StringBuilder* sb)
    { *--sb->s = 0; --sb->n; }

#ifdef __cplusplus
} // extern "C"
#endif

//}}}-------------------------------------------------------------------

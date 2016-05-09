// This file is free software, distributed under the BSD license.

#pragma once
#define _GNU_SOURCE
#define _POSIX_C_SOURCE	200809L

#if __GNUC__ && !defined(UNUSED)
    #define UNUSED		__attribute__((unused))
    #define NORETURN		__attribute__((noreturn))
    #define PRINTFLIKE(f,a)	__attribute__((__format__(__printf__,f,a)))
#endif

#define ArraySize(a)	(sizeof(a)/sizeof(a[0]))

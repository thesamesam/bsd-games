// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#include "hack.h"
#include "extern.h"

long* alloc (unsigned lth)
{
    void* ptr = malloc(lth);
    if (!ptr)
	panic ("Cannot get %d bytes", lth);
    return (long*) ptr;
}

long* enlarge (char* ptr, unsigned lth)
{
    void* nptr = realloc (ptr, lth);
    if (!nptr)
	panic ("Cannot reallocate %d bytes", lth);
    return (long*) nptr;
}

// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#include "hack.h"
#include "extern.h"

void* alloc (size_t n)
{
    void* p = calloc (n, 1);
    if (!p)
	panic ("Cannot get %zu bytes", n);
    return p;
}

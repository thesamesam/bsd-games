// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#include "hack.h"
#include "extern.h"

int rn1(int x, int y)
{
    return nrand(x) + y;
}

int rn2(int x)
{
    return nrand(x);
}

int rnd(int x)
{
    return nrand(x) + 1;
}

int d(int n, int x)
{
    int tmp = n;
    while (n--)
	tmp += nrand(x);
    return tmp;
}

// Copyright (c) 1982 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "mille.h"

// This routine rolls ndie nside-sided dice.
int roll (int ndie, int nsides)
{
    int tot = 0;
    while (ndie--)
	tot += rand() % nsides + 1;
    return tot;
}

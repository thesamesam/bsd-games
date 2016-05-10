// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

int roll (int ndie, int nsides)
{
    int tot = 0;
    while (ndie--)
	tot += rand() % nsides + 1;
    return tot;
}

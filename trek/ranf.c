// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "trek.h"

int ranf (int max)
{
    if (max <= 0)
	return 0;
    int t = rand() >> 5;
    return t % max;
}

double franf (void)
{
    double t = rand() & 077777;
    return t / 32767.0;
}

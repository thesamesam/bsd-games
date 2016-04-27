// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

// for beenthere, injuries
int card (const char* array, int size)
{
    const char* end = array + size;
    int i = 0;
    while (array < end)
	if (*array++)
	    ++i;
    return i;
}

int ucard (const unsigned int* array)
{
    int j = 0;
    for (int n = 0; n < NUMOFOBJECTS; ++n)
	if (testbit(array, n))
	    ++j;
    return j;
}

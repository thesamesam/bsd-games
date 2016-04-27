// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include	"trek.h"

// RETRIEVE THE STARSYSTEM NAME
//
// Very straightforward, this routine just gets the starsystem
// name.  It returns zero if none in the specified quadrant
// (which, by the way, is passed it).
//
// This routine knows all about such things as distressed
// starsystems, etc.

const char* systemname (const struct quad* q)
{
    int i = q->qsystemname;
    if (i & Q_DISTRESSED)
	i = Event[i & Q_SYSTEM].systemname;
    i &= Q_SYSTEM;
    if (i == 0)
	return 0;
    return Systemname[i];
}

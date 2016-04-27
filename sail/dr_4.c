// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"
#include <stdlib.h>

void ungrap(struct ship *from, struct ship *to)
{
    int k;
    char friend;

    if ((k = grappled2(from, to)) == 0)
	return;
    friend = capship(from)->nationality == capship(to)->nationality;
    while (--k >= 0) {
	if (friend || dieroll() < 3) {
	    cleangrapple(from, to, 0);
	    makesignal(from, "ungrappling $$", to);
	}
    }
}

void grap(struct ship *from, struct ship *to)
{
    if (capship(from)->nationality != capship(to)->nationality && dieroll() > 2)
	return;
    Write(W_GRAP, from, to->file->index, 0, 0, 0);
    Write(W_GRAP, to, from->file->index, 0, 0, 0);
    makesignal(from, "grappled with $$", to);
}

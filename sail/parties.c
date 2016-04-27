// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"
#include <sys/types.h>

int meleeing(struct ship *from, struct ship *to)
{
    struct BP *p = from->file->OBP;
    struct BP *q = p + NBP;

    for (; p < q; p++)
	if (p->turnsent && p->toship == to)
	    return 1;
    return 0;
}

int boarding(struct ship *from, int isdefense)
{
    struct BP *p = isdefense ? from->file->DBP : from->file->OBP;
    struct BP *q = p + NBP;

    for (; p < q; p++)
	if (p->turnsent)
	    return 1;
    return 0;
}

void unboard(struct ship *ship, struct ship *to, int isdefense)
{
    struct BP *p = isdefense ? ship->file->DBP : ship->file->OBP;
    int n;

    for (n = 0; n < NBP; p++, n++)
	if (p->turnsent && (p->toship == to || isdefense || ship == to))
	    Write(isdefense ? W_DBP : W_OBP, ship, n, 0, 0, 0);
}

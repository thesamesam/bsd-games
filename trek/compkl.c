// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include <math.h>
#include "trek.h"

// compute klingon distances
//
// The klingon list has the distances for all klingons recomputed
// and sorted.  The parameter is a Boolean flag which is set if
// we have just entered a new quadrant.
//
// This routine is used every time the Enterprise or the Klingons
// move.

static void sortkl(void);

// f - set if new quadrant
void compkldist(int f)
{
    int i, dx, dy;
    double d;
    double temp;

    if (Etc.nkling == 0)
	return;
    for (i = 0; i < Etc.nkling; i++) {
	// compute distance to the Klingon
	dx = Ship.sectx - Etc.klingon[i].x;
	dy = Ship.secty - Etc.klingon[i].y;
	d = dx * dx + dy * dy;
	d = sqrt(d);

	// compute average of new and old distances to Klingon
	if (!f) {
	    temp = Etc.klingon[i].dist;
	    Etc.klingon[i].avgdist = 0.5 * (temp + d);
	} else {
	    // new quadrant: average is current
	    Etc.klingon[i].avgdist = d;
	}
	Etc.klingon[i].dist = d;
    }

    // leave them sorted
    sortkl();
}

// sort klingons
// bubble sort on ascending distance
static void sortkl(void)
{
    struct kling t;
    int f, i, m;

    m = Etc.nkling - 1;
    f = 1;
    while (f) {
	f = 0;
	for (i = 0; i < m; i++) {
	    if (Etc.klingon[i].dist > Etc.klingon[i + 1].dist) {
		t = Etc.klingon[i];
		Etc.klingon[i] = Etc.klingon[i + 1];
		Etc.klingon[i + 1] = t;
		f = 1;
	    }
	}
    }
}

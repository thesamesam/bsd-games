// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "trek.h"
#include "getpar.h"

// move under impulse power
void impulse(int v UNUSED)
{
    int course;
    int power;
    double dist, time;
    int percent;

    if (Ship.cond == DOCKED) {
	printf("Scotty: Sorry captain, but we are still docked.\n");
	return;
    }
    if (damaged(IMPULSE)) {
	out(IMPULSE);
	return;
    }
    if (getcodi(&course, &dist))
	return;
    power = 20 + 100 * dist;
    percent = 100 * power / Ship.energy + 0.5;
    if (percent >= 85) {
	printf("Scotty: That would consume %d%% of our remaining energy.\n", percent);
	if (!getynpar("Are you sure that is wise"))
	    return;
	printf("Aye aye, sir\n");
    }
    time = dist / 0.095;
    percent = 100 * time / Now.time + 0.5;
    if (percent >= 85) {
	printf("Spock: That would take %d%% of our remaining time.\n", percent);
	if (!getynpar("Are you sure that is wise"))
	    return;
	printf("(He's finally gone mad)\n");
    }
    Move.time = move(0, course, time, 0.095);
    Ship.energy -= 20 + 100 * Move.time * 0.095;
}

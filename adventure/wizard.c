// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
// Re-coding of advent in C: privileged operations

#include "hdr.h"
#include "extern.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void datime(int *d, int *t)
{
    time_t tvec;
    time(&tvec);
    struct tm* tptr = localtime(&tvec);
    // day since 1977  (mod leap)
    *d = (tptr->tm_yday + 365 * (tptr->tm_year - 77)
	  + (tptr->tm_year - 77) / 4 - (tptr->tm_year - 1) / 100 + (tptr->tm_year + 299) / 400);
    // bug: this will overflow in the year 2066 AD (with 16 bit int)
    // it will be attributed to Wm the C's millenial celebration
    // and minutes since midnite
    *t = tptr->tm_hour * 60 + tptr->tm_min;
}			       // pretty painless

int Start (void)
{
    int d, t;
    datime(&d, &t);
    int delay = (d - saveday) * 1440 + (t - savet);	// good for about a month
    if (delay >= latncy) {
	saved = -1;
	return false;
    }
    printf("This adventure was suspended a mere %d minute%s ago.", delay, delay == 1 ? "" : "s");
    if (delay <= latncy / 3) {
	mspeak(2);
	exit(0);
    }
    mspeak(8);
    if (!wizard()) {
	mspeak(9);
	exit(0);
    }
    saved = -1;
    return false;
}

int wizard (void)
{			       // not as complex as advent/10 (for now)
    if (!yesm(16, 0, 7))
	return false;
    mspeak(17);
    char *word, *x;
    getin(&word, &x);
    static const char magic[6] = "dwarf";
    if (!weq (word, magic)) {
	mspeak(20);
	return false;
    }
    mspeak(19);
    return true;
}

void ciao (void)
{
    printf("What would you like to call the saved version?\n");
    // XXX - should use getline to avoid arbitrary limit
    char *c;
    char fname[80];
    for (c = fname; c < fname + sizeof fname - 1; c++) {
	int ch;
	ch = getchar();
	if (ch == '\n' || ch == EOF)
	    break;
	*c = ch;
    }
    *c = 0;
    if (save(fname) != 0)
	return;		       // Save failed
    printf("To resume, say \"adventure %s\".\n", fname);
    printf("\"With these rooms I might now have been familiarly acquainted.\"\n");
    exit(0);
}

int ran (int range)
{
    return rand() % range;
}

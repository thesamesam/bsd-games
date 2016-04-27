// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
// Re-coding of advent in C: privileged operations

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "hdr.h"
#include "extern.h"

void datime(int *d, int *t)
{
    time_t tvec;
    struct tm *tptr;

    time(&tvec);
    tptr = localtime(&tvec);
    // day since 1977  (mod leap)
    *d = (tptr->tm_yday + 365 * (tptr->tm_year - 77)
	  + (tptr->tm_year - 77) / 4 - (tptr->tm_year - 1) / 100 + (tptr->tm_year + 299) / 400);
    // bug: this will overflow in the year 2066 AD (with 16 bit int)
    // it will be attributed to Wm the C's millenial celebration
    // and minutes since midnite
    *t = tptr->tm_hour * 60 + tptr->tm_min;
}			       // pretty painless

char magic[6];

void poof(void)
{
    strcpy(magic, DECR('d', 'w', 'a', 'r', 'f'));
    latncy = 45;
}

int Start(void)
{
    int d, t, delay;

    datime(&d, &t);
    delay = (d - saveday) * 1440 + (t - savet);	// good for about a
    // month

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

int wizard(void)
{			       // not as complex as advent/10 (for now)
    char *word, *x;
    if (!yesm(16, 0, 7))
	return false;
    mspeak(17);
    getin(&word, &x);
    if (!weq(word, magic)) {
	mspeak(20);
	return false;
    }
    mspeak(19);
    return true;
}

void ciao(void)
{
    char *c;
    char fname[80];

    printf("What would you like to call the saved version?\n");
    // XXX - should use getline to avoid arbitrary limit
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

int ran(int range)
{
    return rand() % range;
}

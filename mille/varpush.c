// Copyright (c) 1982 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "mille.h"
#include <paths.h>

// push variables around via the routine func() on the file
// channel file.  func() is either read or write.
#if defined(__linux__) && !defined(__GLIBC__)
bool varpush (int file, ssize_t(*func) (int, const struct iovec *, size_t))
#else
bool varpush (int file, ssize_t(*func) (int, const struct iovec *, int))
#endif
{
    int temp;
    const struct iovec vec[] = {
	{ &Debug, sizeof Debug},
	{ &Finished, sizeof Finished},
	{ &Order, sizeof Order},
	{ &End, sizeof End},
	{ &On_exit, sizeof On_exit},
	{ &Handstart, sizeof Handstart},
	{ &Numgos, sizeof Numgos},
	{ Numseen, sizeof Numseen},
	{ &Play, sizeof Play},
	{ &Window, sizeof Window},
	{ Deck, sizeof Deck},
	{ &Discard, sizeof Discard},
	{ Player, sizeof Player}
    };

    if (((func) (file, vec, sizeof(vec) / sizeof(vec[0]))) < 0) {
	error(strerror(errno));
	return false;
    }
    if (func == readv) {
	if ((read(file, (void *) &temp, sizeof temp)) < 0) {
	    error(strerror(errno));
	    return false;
	}
	Topcard = &Deck[temp];
#ifndef NDEBUG
	if (Debug) {
	    char buf[80];
	  over:
	    printf("Debug file:");
	    fgets(buf, sizeof(buf), stdin);
	    if ((outf = fopen(buf, "w")) == NULL) {
		warn("%s", buf);
		goto over;
	    }
	    if (strcmp(buf, _PATH_DEVNULL) != 0)
		setbuf(outf, (char *) NULL);
	}
#endif
    } else {
	temp = Topcard - Deck;
	if ((write(file, (void *) &temp, sizeof temp)) < 0) {
	    error(strerror(errno));
	    return false;
	}
    }
    return true;
}

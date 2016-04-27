// Copyright (c) 1992 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

// Tetris input.

#include "input.h"
#include "tetris.h"
#include <sys/types.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <errno.h>
#include <unistd.h>

// return true iff the given timeval is positive
#define	TV_POS(tv) \
	((tv)->tv_sec > 0 || ((tv)->tv_sec == 0 && (tv)->tv_usec > 0))

// subtract timeval `sub' from `res'
#define	TV_SUB(res, sub) \
	(res)->tv_sec -= (sub)->tv_sec; \
	(res)->tv_usec -= (sub)->tv_usec; \
	if ((res)->tv_usec < 0) { \
		(res)->tv_usec += 1000000; \
		(res)->tv_sec--; \
	}

// Do a `read wait': poll for reading from stdin, with timeout *tvp.
// On return, modify *tvp to reflect the amount of time spent waiting.
// It will be positive only if input appeared before the time ran out;
// otherwise it will be zero or perhaps negative.
//
// If tvp is nil, wait forever, but return if poll is interrupted.
//
// Return 0 => no input, 1 => can read() from stdin
int rwait(struct timeval *tvp)
{
    struct pollfd set[1];
    struct timeval starttv, endtv;
    int timeout;
#define	NILTZ ((struct timezone *)0)

    if (tvp) {
	(void) gettimeofday(&starttv, NILTZ);
	endtv = *tvp;
	timeout = tvp->tv_sec * 1000 + tvp->tv_usec / 1000;
    } else
	timeout = -1;
  again:
    set[0].fd = STDIN_FILENO;
    set[0].events = POLLIN;
    switch (poll(set, 1, timeout)) {

	case -1:
	    if (tvp == 0)
		return -1;
	    if (errno == EINTR)
		goto again;
	    stop("poll failed, help");
	    // NOTREACHED

	case 0:	       // timed out
	    tvp->tv_sec = 0;
	    tvp->tv_usec = 0;
	    return 0;
    }
    if (tvp) {
	// since there is input, we may not have timed out
	(void) gettimeofday(&endtv, NILTZ);
	TV_SUB(&endtv, &starttv);
	TV_SUB(tvp, &endtv);   // adjust *tvp by elapsed time
    }
    return 1;
}

// sleep for the current turn time.
// Eat any input that might be available.
void tsleep(void)
{
    struct timeval tv;
    char c;
    tv.tv_sec = 0;
    tv.tv_usec = fallrate;
    while (TV_POS(&tv))
	if (rwait(&tv) && read(0, &c, 1) != 1)
	    break;
}

// getchar with timeout.
int tgetchar(void)
{
    static struct timeval timeleft;
    char c;

    // Reset timeleft to fallrate whenever it is not positive.
    // In any case, wait to see if there is any input.  If so,
    // take it, and update timeleft so that the next call to
    // tgetchar() will not wait as long.  If there is no input,
    // make timeleft zero or negative, and return -1.
    // Most of the hard work is done by rwait().
    if (!TV_POS(&timeleft)) {
	faster();	       // go faster
	timeleft.tv_sec = 0;
	timeleft.tv_usec = fallrate;
    }
    if (!rwait(&timeleft))
	return -1;
    if (read(0, &c, 1) != 1)
	stop("end of file, help");
    return (int)(unsigned char) c;
}

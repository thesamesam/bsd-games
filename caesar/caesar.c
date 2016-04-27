// Copyright (c) 1982 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

enum { LINELENGTH = 2048 };

// letter frequencies (taken from some unix(tm) documentation)
// (unix is a trademark of Bell Laboratories)
static double stdf[26] = {
    7.97, 1.35, 3.61, 4.78, 12.37, 2.01, 1.46, 4.49, 6.39, 0.04,
    0.42, 3.81, 2.69, 5.92, 6.96, 2.91, 0.08, 6.63, 8.77, 9.68,
    2.62, 0.81, 1.88, 0.23, 2.07, 0.06,
};

static void printit(const char *) NORETURN;
static char rotateit (char ch, char perm);

int main (int argc, char **argv)
{
    int ch, i, nread;
    double dot, winnerdot;
    char *inbuf;
    int obs[26], try, winner;

    // revoke setgid privileges
    setregid(getgid(), getgid());

    winnerdot = 0;
    size_t prognamelen = strlen(argv[0]);
    if (prognamelen >= 5 && 0 == strcmp(&argv[0][prognamelen - 5], "rot13"))
	printit("13");
    if (argc > 1)
	printit(argv[1]);

    if (!(inbuf = malloc(LINELENGTH)))
	err(1, NULL);

    // adjust frequency table to weight low probs REAL low
    for (i = 0; i < 26; ++i)
	stdf[i] = log(stdf[i]) + log(26.0 / 100.0);

    // zero out observation table
    memset(obs, 0, 26 * sizeof(int));

    if ((nread = read(STDIN_FILENO, inbuf, LINELENGTH)) < 0)
	err(1, "reading from stdin");
    for (i = nread; i--;) {
	ch = inbuf[i];
	if (islower(ch))
	    ++obs[ch - 'a'];
	else if (isupper(ch))
	    ++obs[ch - 'A'];
    }

    // now "dot" the freqs with the observed letter freqs
    // and keep track of best fit
    for (try = winner = 0; try < 26; ++try) {	// += 13) {
	dot = 0;
	for (i = 0; i < 26; i++)
	    dot += obs[i] * stdf[(i + try) % 26];
	// initialize winning score
	if (try == 0)
	    winnerdot = dot;
	if (dot > winnerdot) {
	    // got a new winner!
	    winner = try;
	    winnerdot = dot;
	}
    }

    for (;;) {
	for (i = 0; i < nread; ++i) {
	    ch = inbuf[i];
	    putchar(rotateit(ch, winner));
	}
	if (nread < LINELENGTH)
	    break;
	if ((nread = read(STDIN_FILENO, inbuf, LINELENGTH)) < 0)
	    err(1, "reading from stdin");
    }
    exit(0);
}

static char rotateit (char ch, char perm)
{
    if (ch >= 'A' && ch <= 'Z')
	return 'A' + (unsigned)(ch - 'A' + perm) % 26;
    else if (ch >= 'a' && ch <= 'z')
	return 'a' + (unsigned)(ch - 'a' + perm) % 26;
    return ch;
}

static void printit(const char *arg)
{
    int ch, rot;
    if ((rot = atoi(arg)) < 0)
	errx(1, "bad rotation value.");
    while ((ch = getchar()) != EOF)
	putchar(rotateit(ch, rot));
    exit(0);
}

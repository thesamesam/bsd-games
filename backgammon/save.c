// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include <errno.h>

#include "back.h"

static const char confirm[] = "Are you sure you want to leave now?";
static const char prompt[] = "Enter a file name:  ";
static const char exist1[] = "The file '";
static const char exist2[] = "' already exists.\nAre you sure you want to use this file?";
static const char cantuse[] = "\nCan't use ";
static const char saved[] = "This game has been saved on the file '";
static const char type[] = "'.\nType \"backgammon ";
static const char rec[] = "\" to recover your game.\n\n";
static const char cantrec[] = "Can't recover file:  ";

void save(int n)
{
    int fdesc;
    char *fs;
    char fname[50];

    if (n) {
	if (tflag) {
	    curmove(20, 0);
	    clend();
	} else
	    writec('\n');
	writel(confirm);
	if (!yorn(0))
	    return;
    }
    cflag = 1;
    for (;;) {
	writel(prompt);
	fs = fname;
	while ((*fs = readc()) != '\n') {
	    if (*fs == old.c_cc[VERASE]) {
		if (fs > fname) {
		    fs--;
		    if (tflag)
			curmove(curr, curc - 1);
		    else
			writec(*fs);
		} else
		    writec('\007');
		continue;
	    }
	    writec(*fs++);
	}
	*fs = '\0';
	if ((fdesc = open(fname, O_RDWR)) == -1 && errno == ENOENT) {
	    if ((fdesc = creat(fname, 0600)) != -1)
		break;
	}
	if (fdesc != -1) {
	    if (tflag) {
		curmove(18, 0);
		clend();
	    } else
		writec('\n');
	    writel(exist1);
	    writel(fname);
	    writel(exist2);
	    cflag = 0;
	    close(fdesc);
	    if (yorn(0)) {
		unlink(fname);
		fdesc = creat(fname, 0600);
		break;
	    } else {
		cflag = 1;
		continue;
	    }
	}
	writel(cantuse);
	writel(fname);
	writel(".\n");
	close(fdesc);
	cflag = 1;
    }
    write(fdesc, board, sizeof board);
    write(fdesc, off, sizeof off);
    write(fdesc, in, sizeof in);
    write(fdesc, dice, sizeof dice);
    write(fdesc, &cturn, sizeof cturn);
    write(fdesc, &dlast, sizeof dlast);
    write(fdesc, &pnum, sizeof pnum);
    write(fdesc, &rscore, sizeof rscore);
    write(fdesc, &wscore, sizeof wscore);
    write(fdesc, &gvalue, sizeof gvalue);
    write(fdesc, &raflag, sizeof raflag);
    close(fdesc);
    if (tflag)
	curmove(18, 0);
    writel(saved);
    writel(fname);
    writel(type);
    writel(fname);
    writel(rec);
    if (tflag)
	clend();
    getout(0);
}

void recover(const char *s)
{
    int fdesc;

    if ((fdesc = open(s, O_RDONLY)) == -1)
	norec(s);
    read(fdesc, board, sizeof board);
    read(fdesc, off, sizeof off);
    read(fdesc, in, sizeof in);
    read(fdesc, dice, sizeof dice);
    read(fdesc, &cturn, sizeof cturn);
    read(fdesc, &dlast, sizeof dlast);
    read(fdesc, &pnum, sizeof pnum);
    read(fdesc, &rscore, sizeof rscore);
    read(fdesc, &wscore, sizeof wscore);
    read(fdesc, &gvalue, sizeof gvalue);
    read(fdesc, &raflag, sizeof raflag);
    close(fdesc);
    rflag = 1;
}

void norec(const char *s)
{
    const char *c;

    tflag = 0;
    writel(cantrec);
    c = s;
    while (*c != '\0')
	writec(*c++);
    getout(0);
}

// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "trek.h"
#include <err.h>

// THIS CONSTANT MUST CHANGE AS THE DATA SPACES CHANGE
enum { VERSION = 2 };

struct dump {
    void* area;
    int count;
};

static int readdump(int);

struct dump Dump_template[] = {
    {&Ship, sizeof(Ship)},
    {&Now, sizeof(Now)},
    {&Param, sizeof(Param)},
    {&Etc, sizeof(Etc)},
    {&Game, sizeof(Game)},
    {Sect, sizeof(Sect)},
    {Quad, sizeof(Quad)},
    {&Move, sizeof(Move)},
    {Event, sizeof(Event)},
    {NULL, 0}
};

// DUMP GAME
//
// This routine dumps the game onto the file "trek.dump".  The
// first two bytes of the file are a version number, which
// reflects whether this image may be used.  Obviously, it must
// change as the size, content, or order of the data structures
// output change.
//
void dumpgame(int v UNUSED)
{
    int version;
    int fd;
    struct dump *d;
    int i;

    if ((fd = creat("trek.dump", 0644)) < 0) {
	warn("cannot open `trek.dump'");
	return;
    }
    version = VERSION;
    write(fd, &version, sizeof version);

    // output the main data areas
    for (d = Dump_template; d->area; d++) {
	write(fd, &d->area, sizeof d->area);
	i = d->count;
	write(fd, d->area, i);
    }

    close(fd);
}

// RESTORE GAME
//
// The game is restored from the file "trek.dump".  In order for
// this to succeed, the file must exist and be readable, must
// have the correct version number, and must have all the appro-
// priate data areas.
//
// Return value is zero for success, one for failure.

int restartgame(void)
{
    int fd;
    int version;

    if ((fd = open("trek.dump", O_RDONLY)) < 0 || read(fd, &version, sizeof version) != sizeof version || version != VERSION || readdump(fd)) {
	printf("cannot restart\n");
	close(fd);
	return 1;
    }

    close(fd);
    return 0;
}

// READ DUMP
//
// This is the business end of restartgame().  It reads in the
// areas.
//
// Returns zero for success, one for failure.
static int readdump(int fd1)
{
    int fd;
    struct dump *d;
    int i;
    long junk;

    fd = fd1;

    for (d = Dump_template; d->area; d++) {
	if (read(fd, &junk, sizeof junk) != (sizeof junk))
	    return 1;
	if ((char *) junk != d->area)
	    return 1;
	i = d->count;
	if (read(fd, d->area, i) != i)
	    return 1;
    }

    // make quite certain we are at EOF
    return read(fd, &junk, 1);
}

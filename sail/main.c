// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"
#include "restart.h"
#include <setjmp.h>
#include <time.h>

int main(int argc, char **argv)
{
    char *p;
    int a, i;
    int fd;

    gid = getgid();
    egid = getegid();
    setegid(gid);

    fd = open("/dev/null", O_RDONLY);
    if (fd < 3)
	exit(1);
    close(fd);

    srand (time(NULL));

    if ((p = strrchr(*argv, '/')) != NULL)
	p++;
    else
	p = *argv;

    if (strcmp(p, "driver") == 0 || strcmp(p, "saildriver") == 0)
	mode = MODE_DRIVER;
    else if (strcmp(p, "sail.log") == 0)
	mode = MODE_LOGGER;
    else
	mode = MODE_PLAYER;

    while ((a = getopt(argc, argv, "dsxlb")) != -1)
	switch (a) {
	    case 'd':
		mode = MODE_DRIVER;
		break;
	    case 's':
		mode = MODE_LOGGER;
		break;
	    case 'x':
		randomize++;
		break;
	    case 'l':
		longfmt++;
		break;
	    case 'b':
		nobells++;
		break;
	    default:
		fprintf(stderr, "SAIL: Unknown flag %s.\n", p);
		exit(1);
	}

    argc -= optind;
    argv += optind;

    if (*argv)
	game = atoi(*argv);
    else
	game = -1;

    if ((i = setjmp(restart)) != 0)
	mode = i;

    switch (mode) {
	case MODE_PLAYER:
	    return pl_main();
	case MODE_DRIVER:
	    return dr_main();
	case MODE_LOGGER:
	    return lo_main();
	default:
	    fprintf(stderr, "SAIL: Unknown mode %d.\n", mode);
	    abort();
    }
}

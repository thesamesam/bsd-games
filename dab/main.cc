// Copyright (c) 2003 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

// main.cc: Main dots program
#include "algor.h"
#include "board.h"
#include "human.h"
#include "ttyscrn.h"
#include <iostream>

// Print the command line usage
static void usage (char* pname)
{
    char* p = strrchr(pname, '/');
    if (p)
	p++;
    else
	p = pname;
    std::cerr << "Usage: " << p << " [-w] [-p <c|h><c|h>] [-n <ngames>] [<ydim> [<xdim>]]" << std::endl;
}

// Play a single game
static void play (BOARD& b, PLAYER* p[2])
{
    // Initialize
    b.init();
    p[0]->init();
    p[1]->init();
    b.paint();

    // Alternate turns between players, scoring each turn
    for (size_t i = 0;; i = (i + 1) & 1) {
	b.score(i, *p[i]);
	if (!p[i]->domove(b)) {
	    // No more moves, game over
	    break;
	}
	b.score(i, *p[i]);
    }

    // Find who won
    p[0]->wl(p[1]->getScore());
    p[1]->wl(p[0]->getScore());

    // Post scores
    b.score(0, *p[0]);
    b.score(1, *p[1]);

    // Post totals
    b.total(0, *p[0]);
    b.total(1, *p[1]);

    // Post games
    b.games(0, *p[0]);
    b.games(1, *p[1]);

    // Post ties
    b.ties(*p[0]);
}

int main (int argc, char** argv)
{
    size_t ny, nx, nn = 1, wt = 0;
    const char* nc = "ch";
    int c;
    int acs = 1;

    while ((c = getopt(argc, argv, "awp:n:")) != -1) {
	switch (c) {
	    case 'a': acs = 0; break;
	    case 'w': wt++; break;
	    case 'p': nc = optarg; break;
	    case 'n': nn = atoi(optarg); break;
	    default:  usage(argv[0]); return EXIT_FAILURE;
	}
    }

    // Get the size of the board if specified
    switch (argc - optind) {
	case 0: ny = nx = 3; break;
	case 1: ny = nx = atoi(argv[optind]); break;
	case 2: nx = atoi(argv[optind]);
		ny = atoi(argv[optind+1]);
		break;
	default: usage(argv[0]); return EXIT_FAILURE;
    }

    PLAYER* p[2];

    // Allocate players
    for (size_t i = 0; i < 2; i++) {
	char n = nc[1] == nc[0] ? i + '0' : nc[i];
	switch (nc[i]) {
	    case 'c':	p[i] = new ALGOR(n); break;
	    case 'h':	p[i] = new HUMAN(n); break;
	    default:	usage(argv[0]); return EXIT_FAILURE;
	}
    }

    GAMESCREEN *sc = TTYSCRN::create(acs, ny, nx);
    if (!sc) {
	printf ("Error: dimensions too large for current screen.\n");
	return EXIT_FAILURE;
    }

    BOARD b(ny, nx, sc);

    // Play games
    while (nn--) {
	play(b, p);
	if (wt)
	    b.getmove();
    }

    if (wt == 0)
	b.getmove();
    // Cleanup
    delete p[0];
    delete p[1];
    return EXIT_SUCCESS;
}

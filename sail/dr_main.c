// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"
#include "driver.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int dr_main(void)
{
    int n;
    struct ship *sp;
    int nat[NNATION];
    int value = 0;

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    if (game < 0 || game >= NSCENE) {
	fprintf(stderr, "DRIVER: Bad game number %d\n", game);
	exit(1);
    }
    cc = &scene[game];
    ls = SHIP(cc->vessels);
    if (sync_open() < 0) {
	perror("driver: syncfile");
	exit(1);
    }
    for (n = 0; n < NNATION; n++)
	nat[n] = 0;
    foreachship(sp) {
	if (sp->file == NULL && (sp->file = (struct File *) calloc(1, sizeof(struct File))) == NULL) {
	    fprintf(stderr, "DRIVER: Out of memory.\n");
	    exit(1);
	}
	sp->file->index = sp - SHIP(0);
	sp->file->loadL = L_ROUND;
	sp->file->loadR = L_ROUND;
	sp->file->readyR = R_LOADED | R_INITIAL;
	sp->file->readyL = R_LOADED | R_INITIAL;
	sp->file->stern = nat[sp->nationality]++;
	sp->file->dir = sp->shipdir;
	sp->file->row = sp->shiprow;
	sp->file->col = sp->shipcol;
    }
    windspeed = cc->windspeed;
    winddir = cc->winddir;
    people = 0;
    for (;;) {
	sleep(7);
	if (Sync() < 0) {
	    value = 1;
	    break;
	}
	if (next() < 0)
	    break;
	unfoul();
	checkup();
	prizecheck();
	moveall();
	thinkofgrapples();
	boardcomp();
	compcombat();
	resolve();
	reload();
	checksails();
	if (Sync() < 0) {
	    value = 1;
	    break;
	}
    }
    sync_close(1);
    return value;
}

// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "pathnames.h"
#include <sys/types.h>
#include <err.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char *recfile = _PATH_RAWSCORES;
enum { MAXPLAYERS = 65534 };

static struct player {
    short uids;
    short scores;
    char *name;
} players[MAXPLAYERS], temp;

int main (void)
{
    short uid, score;
    FILE *fd;
    int noplayers;
    int i, j, notsorted;
    short whoallbest, allbest;
    const char *q;
    struct passwd *p;

    // Revoke setgid privileges
    setregid(getgid(), getgid());

    fd = fopen(recfile, "r");
    if (fd == NULL)
	err(1, "opening `%s'", recfile);
    printf("Snake players scores to date\n");
    if (fread(&whoallbest, sizeof(short), 1, fd) == 0) {
	printf("No scores recorded yet!\n");
	exit(0);
    }
    fread(&allbest, sizeof(short), 1, fd);
    noplayers = 0;
    for (uid = 2;; uid++) {
	if (fread(&score, sizeof(short), 1, fd) == 0)
	    break;
	if (score > 0) {
	    if (noplayers > MAXPLAYERS) {
		printf("too many players: %d\n", noplayers);
		exit(2);
	    }
	    players[noplayers].uids = uid;
	    players[noplayers].scores = score;
	    p = getpwuid(uid);
	    if (p == NULL)
		continue;
	    q = p->pw_name;
	    players[noplayers].name = strdup(q);
	    if (players[noplayers].name == NULL)
		err(1, NULL);
	    noplayers++;
	}
    }

    // bubble sort scores
    for (notsorted = 1; notsorted;) {
	notsorted = 0;
	for (i = 0; i < noplayers - 1; i++) {
	    if (players[i].scores < players[i + 1].scores) {
		temp = players[i];
		players[i] = players[i + 1];
		players[i + 1] = temp;
		notsorted++;
	    }
	}
    }

    j = 1;
    for (i = 0; i < noplayers; i++) {
	printf("%d:\t$%d\t%s\n", j, players[i].scores, players[i].name);
	if (players[i].scores > players[i + 1].scores)
	    j = i + 2;
    }
    return EXIT_SUCCESS;
}

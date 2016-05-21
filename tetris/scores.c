// Copyright (c) 1992 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
//
// Score code for Tetris, by Darren Provine (kilroy@gboro.glassboro.edu)
// modified 22 January 1992, to limit the number of entries any one
// person has.
//
// Major whacks since then.

#include "tetris.h"
#include "scores.h"
#include <sys/file.h>
#include <sys/uio.h>

//----------------------------------------------------------------------

// Within this code, we can hang onto one extra "high score", leaving
// room for our current score (whether or not it is high).
struct highscore _scores [MAXHISCORES] = {{"",0,0,0}};

#define SCOREFILE_MAGIC		"tetris"
#define SCOREFILE_MAGIC_INIT	{'t','e','t','r','i','s'}

struct ScorefileHeader {
    char	magic[6];
    uint16_t	sum;
};

//----------------------------------------------------------------------

static unsigned read_scores (void);
static void write_scores (void);
static int cmpscores (const void *x, const void *y);
static void check_scores (void);

//----------------------------------------------------------------------
// Scorefile reading and writing

static unsigned read_scores (void)
{
    int fd = open (_PATH_SCOREFILE, O_RDONLY);
    if (fd < 0)
	return 0;
    struct ScorefileHeader header;
    struct iovec iov[2] = {{&header,sizeof(header)},{_scores,sizeof(_scores)}};
    while (0 != flock (fd, LOCK_SH))
	sleep (1);
    ssize_t br = readv (fd, iov, ArraySize(iov));
    close (fd);
    // Check that score list appears valid
    if (br != sizeof(header)+sizeof(_scores)
	|| memcmp (header.magic, SCOREFILE_MAGIC, sizeof(header.magic)) != 0
	|| header.sum != bsdsum (_scores, sizeof(_scores), 0))
	return 0;
    return MAXHISCORES;
}

static void write_scores (void)
{
    int fd = open (_PATH_SCOREFILE, O_WRONLY);
    if (fd < 0)
	return;
    while (0 != flock (fd, LOCK_EX))
	sleep (1);
    struct ScorefileHeader header = {
	SCOREFILE_MAGIC_INIT,
	bsdsum (_scores, sizeof(_scores), 0)
    };
    struct iovec iov[2] = {{&header,sizeof(header)},{_scores,sizeof(_scores)}};
    writev (fd, iov, ArraySize(iov));
    ftruncate (fd, sizeof(header)+sizeof(_scores));
    close (fd);
}

// Score comparison function for qsort.
//
// If two scores are equal, the person who had the score first is
// listed first in the highscore file.
static int cmpscores (const void *x, const void *y)
{
    const struct highscore* a = (const struct highscore*) x;
    const struct highscore* b = (const struct highscore*) y;
    int l = b->level * b->score - a->level * a->score;
    if (l < 0)
	return -1;
    if (l > 0)
	return 1;
    if (a->time < b->time)
	return -1;
    if (a->time > b->time)
	return 1;
    return 0;
}

static void check_scores (void)
{
    time_t expiration = time(NULL)-EXPIRATION;
    // Check validity of each score. Zero out any that are bad.
    for (struct highscore *sc = _scores, *scend = &_scores[ArraySize(_scores)]; sc < scend; ++sc)
	if (sc->level < MINLEVEL || sc->level > MAXLEVEL
		|| sc->time < expiration
		|| !sc->name[0] || sc->name[sizeof(sc->name)-1])
	    memset (sc, 0, sizeof(*sc));
    qsort (_scores, ArraySize(_scores), sizeof(_scores[0]), cmpscores);
}

void savescore (unsigned score, unsigned level)
{
    read_scores();
    check_scores();
    struct highscore* nsc = &_scores[MAXHISCORES-1];
    if (nsc->score*nsc->level > score*level)
	return;	// The new score is too low to save
    const char* username = getlogin();
    if (!username)
	return;
    strncpy (nsc->name, username, sizeof(nsc->name)-1);
    nsc->name[sizeof(nsc->name)-1] = 0;
    nsc->score = score;
    nsc->level = level;
    nsc->time = time (NULL);
    check_scores();
    write_scores();
}

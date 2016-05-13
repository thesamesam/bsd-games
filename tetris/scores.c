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
#include <time.h>

//----------------------------------------------------------------------

// Within this code, we can hang onto one extra "high score", leaving
// room for our current score (whether or not it is high).
struct highscore _scores [MAXHISCORES+1];
unsigned _nScores;

#define SCOREFILE_MAGIC		"tetris"
#define SCOREFILE_MAGIC_INIT	{'t','e','t','r','i','s'}

struct ScorefileHeader {
    char	magic[6];
    uint16_t	sum;
    uint32_t	nScores;
    uint32_t	reserved;
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
	goto error;
    while (0 != flock (fd, LOCK_SH))
	sleep (1);
    struct ScorefileHeader header;
    if (read (fd, &header, sizeof(header)) != sizeof(header))
	goto error;
    if (memcmp (header.magic, SCOREFILE_MAGIC, sizeof(header.magic)) != 0)
	goto error;
    if (header.nScores > MAXHISCORES)
	goto error;
    memset (_scores, 0, sizeof(_scores));
    unsigned btr = header.nScores * sizeof(_scores[0]);
    if (read (fd, _scores, btr) != btr)
	goto error;
    if (header.sum != bsdsum (_scores, sizeof(_scores), 0))
	goto error;
    flock (fd, LOCK_UN);
    close (fd);
    return _nScores = header.nScores;
error:
    _nScores = 0;
    memset (_scores, 0, sizeof(_scores));
    flock (fd, LOCK_UN);
    close (fd);
    return 0;
}

static void write_scores (void)
{
    struct ScorefileHeader header = {
	SCOREFILE_MAGIC_INIT,
	bsdsum (_scores, sizeof(_scores), 0),
	_nScores, 0
    };
    int fd = open (_PATH_SCOREFILE, O_WRONLY);
    if (fd < 0)
	goto error;
    while (0 != flock (fd, LOCK_EX))
	sleep (1);
    if (write (fd, &header, sizeof(header)) != sizeof(header))
	goto error;
    unsigned btw = header.nScores * sizeof(_scores[0]);
    if (write (fd, _scores, btw) != btw)
	goto error;
    ftruncate (fd, sizeof(header)+btw);
error:
    flock (fd, LOCK_UN);
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
    assert (_nScores < ArraySize(_scores));
    time_t expiration = time(NULL)-EXPIRATION;
    // Check validity of each score. Zero out any that are bad.
    for (struct highscore *sc = _scores, *scend = &_scores[ArraySize(_scores)]; sc < scend; ++sc)
	if (sc->level < MINLEVEL || sc->level > MAXLEVEL
		|| sc->time < expiration
		|| !sc->name[0] || sc->name[sizeof(sc->name)-1])
	    memset (sc, 0, sizeof(*sc));
    // Sort and count valid scores
    qsort (_scores, ArraySize(_scores), sizeof(_scores[0]), cmpscores);
    while (_nScores && !_scores[_nScores-1].score)
	--_nScores;
    // Truncate at MAXHISCORES
    if (_nScores > MAXHISCORES)
	_nScores = MAXHISCORES;
    memset (&_scores[_nScores], 0, sizeof(_scores[0])*(ArraySize(_scores)-_nScores));
}

void savescore (unsigned score, unsigned level)
{
    read_scores();
    struct highscore* nsc = &_scores[_nScores];
    const char* username = getlogin();
    if (!username)
	return;
    snprintf (nsc->name, sizeof(nsc->name), "%s", username);
    nsc->score = score;
    nsc->level = level;
    nsc->time = time (NULL);
    ++_nScores;
    check_scores();
    write_scores();
}

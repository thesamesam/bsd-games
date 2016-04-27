// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "hangman.h"

// Get a valid word out of the dictionary file
void getword(void)
{
    char *wp, *gp;
    FILE* inf = Dict;
    for (;;) {
	long pos = (double) rand() / (RAND_MAX + 1.0) * (double) Dict_size;
	fseek(inf, pos, SEEK_SET);
	if (fgets(Word, BUFSIZ, inf) == NULL)
	    continue;
	if (fgets(Word, BUFSIZ, inf) == NULL)
	    continue;
	Word[strlen(Word) - 1] = '\0';
	if (strlen(Word) < Minlen)
	    continue;
	for (wp = Word; *wp; wp++)
	    if (!islower((unsigned char) *wp))
		goto cont;
	break;
      cont:;
    }
    gp = Known;
    wp = Word;
    while (*wp) {
	*gp++ = '-';
	wp++;
    }
    *gp = '\0';
}

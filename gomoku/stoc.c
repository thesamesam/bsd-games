// Copyright (c) 1994 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "gomoku.h"

const char *letters = "<ABCDEFGHJKLMNOPQRST>";

struct mvstr {
    int m_code;
    const char *m_text;
};
static const struct mvstr mv[] = {
    {RESIGN, "resign"},
    {RESIGN, "quit"},
    {SAVE, "save"},
    {-1, 0}
};

// Turn the spot number form of a move into the character form.
const char* stoc(int s)
{
    static char buf[32];
    for (int i = 0; mv[i].m_code >= 0; i++)
	if (s == mv[i].m_code)
	    return mv[i].m_text;
    snprintf(buf, sizeof(buf), "%c%d", letters[s % BSZ1], s / BSZ1);
    return buf;
}

// Turn the character form of a move into the spot number form.
int ctos (const char *mp)
{
    int i;
    for (i = 0; mv[i].m_code >= 0; ++i)
	if (strcmp(mp, mv[i].m_text) == 0)
	    return mv[i].m_code;
    if (!isalpha((unsigned char) mp[0]))
	return ILLEGAL;
    i = atoi (&mp[1]);
    if (i < 1 || i > 19)
	return ILLEGAL;
    return PT(lton(mp[0]), i);
}

// Turn a letter into a number.
int lton (int c)
{
    if (islower(c))
	c = toupper(c);
    int i = 1;
    while (i <= BSZ && letters[i] != c)
	++i;
    return i;
}

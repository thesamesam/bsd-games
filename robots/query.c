// Copyright (c) 1980 The Regents of the University of California.
// Copyright (c) 1999 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

#include "robots.h"

// Ask a question and get a yes or no answer.  Default is "no".
int query (const char* prompt)
{
    int y, x;
    getyx(stdscr, y, x);
    move(Y_PROMPT, X_PROMPT);
    addstr(prompt);
    clrtoeol();
    refresh();
    int c = getchar();
    int retval = (c == 'y' || c == 'Y');
    move(Y_PROMPT, X_PROMPT);
    clrtoeol();
    move(y, x);
    return retval;
}

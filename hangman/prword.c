// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "hangman.h"

// Print out the current state of the word
void prword(void)
{
    move(KNOWNY, KNOWNX + sizeof "Word: ");
    addstr(Known);
    clrtoeol();
}

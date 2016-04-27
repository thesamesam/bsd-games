// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "hangman.h"

bool Guessed[26];

char Word[BUFSIZ], Known[BUFSIZ];
const char *const Noose_pict[] = {
    "     ______",
    "     |    |",
    "     |",
    "     |",
    "     |",
    "     |",
    "   __|_____",
    "   |      |___",
    "   |_________|",
    NULL
};

int Errors, Wordnum = 0;
unsigned int Minlen = MINLEN;

double Average = 0.0;

const ERR_POS Err_pos[MAXERRS] = {
    {2, 10, 'O'},
    {3, 10, '|'},
    {4, 10, '|'},
    {5, 9, '/'},
    {3, 9, '/'},
    {3, 11, '\\'},
    {5, 11, '\\'}
};

const char *Dict_name = _PATH_WORDLIST;
FILE *Dict = NULL;
off_t Dict_size = 0;

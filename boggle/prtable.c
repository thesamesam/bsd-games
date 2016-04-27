// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include <curses.h>
#include "extern.h"

#define NCOLS	5

static int get_maxlen(const char *const[], int, int (*)(const char *const *, int));

// Routine to print a table
// Modified from 'ls.c' mods (BJB/83)
// Arguments:
//      base    - address of first entry
//      num     - number of entries
//      d_cols  - number of columns to use if > 0, "best" size if == 0
//      width   - max line width if not zero
//      prentry - address of the routine to call to print the string
//      length  - address of the routine to call to determine the length
//                of string to be printed 
//
// prtable and length are called with the address of the base and
// an index
void prtable(const char *const base[], int num, int d_cols, int width, void (*prentry) (const char *const[], int), int (*length) (const char *const[], int))
{
    int c, j;
    int a, b, cols, loc, maxlen, nrows, z;
    int col UNUSED, row;

    if (num == 0)
	return;
    maxlen = get_maxlen(base, num, length) + 1;
    if (d_cols > 0)
	cols = d_cols;
    else
	cols = width / maxlen;
    if (cols == 0)
	cols = NCOLS;
    nrows = (num - 1) / cols + 1;
    for (a = 1; a <= nrows; a++) {
	b = c = z = loc = 0;
	for (j = 0; j < num; j++) {
	    c++;
	    if (c >= a + b)
		break;
	}
	while (j < num) {
	    (*prentry) (base, j);
	    loc += (*length) (base, j);
	    z++;
	    b += nrows;
	    for (j++; j < num; j++) {
		c++;
		if (c >= a + b)
		    break;
	    }
	    if (j < num) {
		while (loc < z * maxlen) {
		    addch(' ');
		    loc++;
		}
	    }
	}
	getyx(stdscr, row, col);
	move(row + 1, 0);
    }
    refresh();
}

static int get_maxlen(const char *const base[], int num, int (*length) (const char *const *, int))
{
    int max = (*length)(base, 0);
    for (int i = 0, len; i < num; ++i)
	if ((len = (*length) (base, i)) > max)
	    max = len;
    return max;
}

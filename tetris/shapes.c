// Copyright (c) 1992 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
//
// Tetris shapes and related routines.
//
// Note that the first 7 are `well known'.

#include "tetris.h"

#define	TL	-B_COLS-1      // top left
#define	TC	-B_COLS	       // top center
#define	TR	-B_COLS+1      // top right
#define	ML	-1	       // middle left
#define	MR	1	       // middle right
#define	BL	B_COLS-1       // bottom left
#define	BC	B_COLS	       // bottom center
#define	BR	B_COLS+1       // bottom right

const struct shape shapes[] = {
    {7, {TL, TC, MR,}},
    {8, {TC, TR, ML,}},
    {9, {ML, MR, BC,}},
    {3, {TL, TC, ML,}},
    {12, {ML, BL, MR,}},
    {15, {ML, BR, MR,}},
    {18, {ML, MR, 2}}, // sticks out
    {0, {TC, ML, BL,}},
    {1, {TC, MR, BR,}},
    {10, {TC, MR, BC,}},
    {11, {TC, ML, MR,}},
    {2, {TC, ML, BC,}},
    {13, {TC, BC, BR,}},
    {14, {TR, ML, MR,}},
    {4, {TL, TC, BC,}},
    {16, {TR, TC, BC,}},
    {17, {TL, MR, ML,}},
    {5, {TC, BC, BL,}},
    {6, {TC, BC, 2 * B_COLS}} // sticks out
};

// Return true iff the given shape fits in the given position,
// taking the current board into account.
int fits_in(const struct shape *shape, int pos)
{
    const int *o = shape->off;
    if (board[pos] || board[pos + *o++] || board[pos + *o++] || board[pos + *o])
	return 0;
    return 1;
}

// Write the given shape into the current board, turning it on
// if `onoff' is 1, and off if `onoff' is 0.
void place(const struct shape *shape, int pos, int onoff)
{
    const int *o = shape->off;
    board[pos] = onoff;
    board[pos + *o++] = onoff;
    board[pos + *o++] = onoff;
    board[pos + *o] = onoff;
}

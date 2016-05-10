// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#pragma once
#include "../config.h"

// define structure of a deck of cards and other related things

// value of each suit
enum ESuit { SPADES, HEARTS, DIAMONDS, CLUBS, SUITS };

// value of each rank
enum ERank {
    ACE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    RANKS,
    EMPTY = RANKS
};

enum {
    CARDS	= SUITS * RANKS,
    CINHAND	= 4,	// # cards in cribbage hand
    FULLHAND	= 6,	// # cards in dealt hand
    LGAME	= 121,	// number points in a game
    SGAME	= 61	// # points in a short game
};

#define VAL(c)		((c) < 9 ? (c)+1 : 10)	// val of rank

typedef struct {
    int rank;
    int suit;
} CARD;

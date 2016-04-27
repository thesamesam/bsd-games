// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "deck.h"
#include "cribbage.h"

// Initialize a deck of cards to contain one of each type.
void makedeck (CARD d[])
{
    int i, j, k;
    i = time(NULL);
    i = ((i & 0xff) << 8) | ((i >> 8) & 0xff) | 1;
    srand(i);
    k = 0;
    for (i = 0; i < RANKS; i++) {
	for (j = 0; j < SUITS; j++) {
	    d[k].suit = j;
	    d[k++].rank = i;
	}
    }
}

// Given a deck of cards, shuffle it -- i.e. randomize it
// see Knuth, vol. 2, page 125.
void shuffle (CARD d[])
{
    for (int j = CARDS, k; j > 0; --j) {
	k = (rand() >> 4) % j;	// random 0 <= k < j
	CARD c = d[j - 1];	// exchange (j - 1) and k
	d[j - 1] = d[k];
	d[k] = c;
    }
}

// return true if the two cards are equal...
int eq (CARD a, CARD b)
{
    return a.rank == b.rank && a.suit == b.suit;
}

// is_one returns true if a is in the set of cards b
int is_one (CARD a, const CARD b[], int n)
{
    for (int i = 0; i < n; ++i)
	if (eq(a, b[i]))
	    return true;
    return false;
}

// Remove the card a from the deck d of n cards
void cremove (CARD a, CARD d[], int n)
{
    int j = 0;
    for (int i = 0; i < n; ++i)
	if (!eq(a, d[i]))
	    d[j++] = d[i];
    if (j < n)
	d[j].suit = d[j].rank = EMPTY;
}

// Sort a hand of n cards
void sorthand (CARD h[], int n)
{
    CARD *cp, *endp;
    CARD c;
    for (endp = &h[n]; h < endp - 1; h++) {
	for (cp = h + 1; cp < endp; cp++) {
	    if ((cp->rank < h->rank) || (cp->rank == h->rank && cp->suit < h->suit)) {
		c = *h;
		*h = *cp;
		*cp = c;
	    }
	}
    }
}

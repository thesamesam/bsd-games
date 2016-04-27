// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include <sys/types.h>
#include <stdbool.h>

#define	CC_D	deck[0]
#define	CH_D	deck[1]

struct dk_st {			// deck description structure
    int num_cards;		// number of cards in deck
    int last_card;		// number of last card picked
    bool gojf_used;		// set if gojf card out of deck
    u_int64_t *offsets;		// offsets for start of cards
};

typedef struct dk_st DECK;

// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "../config.h"

enum {
    c_CommunityChestDeck,
    c_ChanceCardsDeck,
    c_NumberOfDecks,
    c_CardsPerDeck = 16
};

#define	CC_D	deck[c_CommunityChestDeck]
#define	CH_D	deck[c_ChanceCardsDeck]

struct dk_st {			// deck description structure
    uint8_t	num_cards;	// number of cards in deck
    uint8_t	last_card;	// number of last card picked
    bool	gojf_used;	// set if gojf card out of deck
    uint8_t	pack [c_CardsPerDeck];	// card indexes
};

typedef struct dk_st DECK;

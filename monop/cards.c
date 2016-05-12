// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include "extern.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <endian.h>

enum ECardAction {
    action_Money,
    action_PayAllPlayers,
    action_MoveTo,
    action_MoveBack,
    action_GoToJail,
    action_GoToRailroad,
    action_GoToUtility,
    action_GetOutOfJail,
    action_StreetRepairs,
    action_GeneralRepairs,
};

enum ELocation {
    location_GO,
    location_Reading = 5,
    location_StCharlesPlace = 11,
    location_IllinoisAve = 24,
    location_Boardwalk = 39
};

struct SCard {
    const char*		msg;
    enum ECardAction	action;
    int			n;
};

static const struct SCard c_CommunityChest [c_CardsPerDeck] = {
    { ">> GET OUT OF JAIL FREE <<\n"
	"Keep this card until needed or sold",	action_GetOutOfJail,	0	},
    { "Receive for Services $25.",		action_Money,		25	},
    { "Bank Error in Your Favor.\n"
	"Collect $200.",			action_Money,		200	},
    { "Income Tax Refund.\nCollect $20.",	action_Money,		20	},
    { "Pay Hospital $100",			action_Money,		-100	},
    { "Life Insurance Matures.\nCollect $100.",	action_Money,		100	},
    { "From sale of Stock You get $45.",	action_Money,		45	},
    { "You are Assessed for street repairs.\n"
	"$40 per House, $115 per Hotel",	action_StreetRepairs,	0	},
    { "X-mas Fund Matures.\nCollect $100.",	action_Money,		100	},
    { "You have won Second Prize in a Beauty Contest.\n"
	"Collect $11.",				action_Money,		11,	},
    { "Advance to GO.\n"
	"Collect $200.",			action_MoveTo,		location_GO },
    { "You inherit $100",			action_Money,		100	},
    { "Pay School Tax of $150.",		action_Money,		-150	},
    { ">> GO TO JAIL <<\n"
	"Go Directly to Jail.\n"
	"Do not pass GO.\n"
	"Do not collect $200.",			action_GoToJail,	0	},
    { ">> GRAND OPERA OPENING <<\n"
	"Collect $50 from each player"
	" for opening night seats.",		action_PayAllPlayers,	-50	},
    { "Doctor's fee: pay $50.",			action_Money,		50	}
};
static const struct SCard c_ChanceCards [c_CardsPerDeck] = {
    { ">> GET OUT OF JAIL FREE <<\n"
	"Keep this card until needed or sold",	action_GetOutOfJail,	0	},
    { "Advance to the nearest Railroad, and pay owner\n"
	"Twice the rental to which he is otherwise entitled.\n"
	"If Railroad is unowned"
	" you may buy it from the bank",	action_GoToRailroad,	2	},
    { "Advance to the nearest Utility.\n"
	"If unowned, you may buy it from the bank.\n"
	"If owned, throw dice and pay oner a total of ten times\n"
	"the amount thrown.",			action_GoToUtility,	10	},
    { "Go Back 3 Spaces.",			action_MoveBack,	3	},
    { "Advance to the nearest Railroad, and pay owner\n"
	"Twice the rental to which he is otherwise entitled.\n"
	"If Railroad is unowned"
	" you may buy it from the bank",	action_GoToRailroad,	2	},
    { ">> GO TO JAIL <<\n"
	"Go Directly to Jail.\n"
	"Do not pass GO.\n"
	"Do not collect $200.",			action_GoToJail,	0	},
    { "Take a Ride on the Reading.\n"
	"If you pass GO, collect $200.",	action_MoveTo,		location_Reading },
    { "Take a Walk on the Boardwalk.",		action_MoveTo,		location_Boardwalk },
    { "Advance to Illinois Ave.",		action_MoveTo,		location_IllinoisAve },
    { "Advance to Go.",				action_MoveTo,		location_GO },
    { "Advance to St.Charles Place.\n"
	"If you pass GO, collect $200.",	action_MoveTo,		location_StCharlesPlace },
    { "Make general repairs on all of your Property.\n"
	"For Each House pay $25.\n"
	"For Each Hotel pay $100.",		action_GeneralRepairs,	0	},
    { "You have been elected Chairman of the Board.\n"
	"Pay each player $50.",			action_PayAllPlayers,	50	},
    { "Pay Poor Tax of $15.",			action_Money,		-15	},
    { "Bank pays you Dividend of $50.",		action_Money,		50	},
    { "Your Building and Loan Matures.\n"
	"Collect $150.",			action_Money,		150	}
};

DECK deck[2] = {{0,0,false,{0}},{0,0,false,{0}}};	// Chance and Community Chest

//----------------------------------------------------------------------

static void init_deck (DECK* d);
static void assess_house_hotel_fee (unsigned housecost, unsigned hotelcost);

//----------------------------------------------------------------------

void init_decks (void)
{
    for (unsigned i = 0; i < ArraySize(deck); ++i)
	init_deck (&deck[i]);
}

void get_card (unsigned ideck)
{
    DECK* dp = &deck[ideck];
    // When all cards have been used, reshuffle and restart
    if (dp->last_card >= dp->num_cards)
	init_deck (dp);
    const struct SCard* card = ideck
	? &c_ChanceCards[dp->last_card]
	: &c_CommunityChest[dp->last_card];
    ++dp->last_card;

    // Print the message
    printf (
	"------------------------------\n"
	"%s\n"
	"------------------------------\n"
	, card->msg);

    // Apply the card action
    switch (card->action) {
	default:
	case action_Money:	cur_p->money += card->n; break;
	case action_PayAllPlayers:
	    for (unsigned i = 0; i < num_play; ++i)
		play[i].money += card->n;
	    cur_p->money -= num_play * card->n;
	    break;
	case action_MoveTo:	move (card->n); break;
	case action_MoveBack:	move (-card->n); break;
	case action_GoToJail:	return goto_jail();
	case action_GoToRailroad:
	    spec = true;
	    move ((int) ((cur_p->loc + 5) / 10) * 10 + 5 - cur_p->loc);
	    break;
	case action_GoToUtility:
	    spec = true;
	    if (cur_p->loc >= 12 && cur_p->loc < 28)
		move (28 - cur_p->loc);
	    else
		move ((cur_p->loc > 12 ? 52 : 12) - cur_p->loc);
	    break;
	case action_GetOutOfJail:
	    ++cur_p->num_gojf;
	    dp->gojf_used = true;
	    break;
	case action_StreetRepairs:
	    assess_house_hotel_fee (25, 100);
	    break;
	case action_GeneralRepairs:
	    assess_house_hotel_fee (40, 115);
	    break;
    }
    spec = false;
}

static void init_deck (DECK* d)
{
    d->last_card = d->gojf_used;
    d->num_cards = c_CardsPerDeck - d->last_card;
    for (unsigned i = 0; i < c_CardsPerDeck; ++i)
	d->pack[i] = i;
    // Shuffle cards in pack, omitting gojf if used
    for (unsigned i = d->last_card; i < c_CardsPerDeck; ++i) {
	unsigned j = i + (unsigned)rand() % (c_CardsPerDeck-i);
	uint8_t t = d->pack[i];
	d->pack[i] = d->pack[j];
	d->pack[j] = t;
    }
}

static void assess_house_hotel_fee (unsigned housecost, unsigned hotelcost)
{
    int num_h = 0, num_H = 0;
    for (const OWN* op = cur_p->own_list; op; op = op->next) {
	if (op->sqr->type == PRPTY) {
	    if (op->sqr->desc->houses == 5)
		++num_H;
	    else
		num_h += op->sqr->desc->houses;
	}
    }
    unsigned fee = housecost * num_h + hotelcost * num_H;
    cur_p->money -= fee;
    printf("You had %d Houses and %d Hotels, so that cost you $%d\n", num_h, num_H, fee);
    if (!fee)
	lucky("");
}

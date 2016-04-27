// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "../config.h"
#include "extern.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <endian.h>
#include <stdbool.h>

#define _PATH_CARDS	_PATH_GAME_DATA "monop/cards.pck"

//      These routine deal with the card decks

#define	GOJF	'F'	       // char for get-out-of-jail-free cards

#ifndef DEV
static const char *cardfile = _PATH_CARDS;
#else
static const char *cardfile = "cards.pck";
#endif

static FILE *deckf;

static void set_up(DECK *);
static void printmes(void);

// This routine initializes the decks from the data file, which it opens.
void init_decks(void)
{
    int32_t nc;
    if ((deckf = fopen(cardfile, "r")) == NULL) {
      file_err:
	perror(cardfile);
	exit(1);
    }

    // read number of community chest cards...
    if (fread(&nc, sizeof(nc), 1, deckf) != 1)
	goto file_err;
    CC_D.num_cards = be32toh(nc);
    // ... and number of community chest cards.
    if (fread(&nc, sizeof(nc), 1, deckf) != 1)
	goto file_err;
    CH_D.num_cards = be32toh(nc);
    set_up(&CC_D);
    set_up(&CH_D);
}

// This routine sets up the offset pointers for the given deck.
static void set_up(DECK * dp)
{
    int r1, r2;
    int i;

    dp->offsets = (u_int64_t *) calloc(dp->num_cards, sizeof(u_int64_t));
    if (dp->offsets == NULL)
	err(1, NULL);
    if (fread(dp->offsets, sizeof(u_int64_t), dp->num_cards, deckf) != (size_t) dp->num_cards) {
	perror(cardfile);
	exit(1);
    }
    // convert offsets from big-endian byte order
    for (i = 0; i < dp->num_cards; i++)
	dp->offsets[i] = ntohl(dp->offsets[i]);
    dp->last_card = 0;
    dp->gojf_used = false;
    for (i = 0; i < dp->num_cards; i++) {
	u_int64_t temp;

	r1 = roll(1, dp->num_cards) - 1;
	r2 = roll(1, dp->num_cards) - 1;
	temp = dp->offsets[r2];
	dp->offsets[r2] = dp->offsets[r1];
	dp->offsets[r1] = temp;
    }
}

// This routine draws a card from the given deck
void get_card(DECK * dp)
{
    char type_maj, type_min;
    int num;
    int i, per_h, per_H, num_h, num_H;
    OWN *op;

    do {
	fseek(deckf, dp->offsets[dp->last_card], SEEK_SET);
	dp->last_card = (dp->last_card + 1) % dp->num_cards;
	type_maj = getc(deckf);
    } while (dp->gojf_used && type_maj == GOJF);
    type_min = getc(deckf);
    num = ntohl(getw(deckf));
    printmes();
    switch (type_maj) {
	case '+':	       // get money
	    if (type_min == 'A') {
		for (i = 0; i < num_play; i++)
		    if (i != player)
			play[i].money -= num;
		num = num * (num_play - 1);
	    }
	    cur_p->money += num;
	    break;
	case '-':	       // lose money
	    if (type_min == 'A') {
		for (i = 0; i < num_play; i++)
		    if (i != player)
			play[i].money += num;
		num = num * (num_play - 1);
	    }
	    cur_p->money -= num;
	    break;
	case 'M':	       // move somewhere
	    switch (type_min) {
		case 'F':     // move forward
		    num -= cur_p->loc;
		    if (num < 0)
			num += 40;
		    break;
		case 'J':     // move to jail
		    goto_jail();
		    return;
		case 'R':     // move to railroad
		    spec = true;
		    num = (int) ((cur_p->loc + 5) / 10) * 10 + 5 - cur_p->loc;
		    break;
		case 'U':     // move to utility
		    spec = true;
		    if (cur_p->loc >= 12 && cur_p->loc < 28)
			num = 28 - cur_p->loc;
		    else {
			num = 12 - cur_p->loc;
			if (num < 0)
			    num += 40;
		    }
		    break;
		case 'B':
		    num = -num;
		    break;
	    }
	    move(num);
	    break;
	case 'T':	       // tax
	    if (dp == &CC_D) {
		per_h = 40;
		per_H = 115;
	    } else {
		per_h = 25;
		per_H = 100;
	    }
	    num_h = num_H = 0;
	    for (op = cur_p->own_list; op; op = op->next)
		if (op->sqr->type == PRPTY) {
		    if (op->sqr->desc->houses == 5)
			++num_H;
		    else
			num_h += op->sqr->desc->houses;
		}
	    num = per_h * num_h + per_H * num_H;
	    printf("You had %d Houses and %d Hotels, so that cost you $%d\n", num_h, num_H, num);
	    if (num == 0)
		lucky("");
	    else
		cur_p->money -= num;
	    break;
	case GOJF:	       // get-out-of-jail-free card
	    cur_p->num_gojf++;
	    dp->gojf_used = true;
	    break;
    }
    spec = false;
}

// This routine prints out the message on the card
static void printmes(void)
{
    char c;
    printline();
    fflush(stdout);
    while ((c = getc(deckf)) != '\0')
	putchar(c);
    printline();
    fflush(stdout);
}

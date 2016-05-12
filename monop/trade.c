// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

struct trd_st {			// how much to give to other player
    int trader;			// trader number
    int cash;			// amount of cash
    int gojf;			// # get-out-of-jail-free cards
    OWN *prop_list;		// property list
};

typedef struct trd_st TRADE;

static const char *plist[MAX_PRP + 2];

static int used[MAX_PRP];

static TRADE trades[2];

static void get_list(int, int);
static int set_list(OWN *);
static void summate(void);
static void do_trade(void);
static void move_em(TRADE *, TRADE *);

void trade(void)
{
    trading = true;
    for (unsigned i = 0; i < 2; ++i) {
	trades[i].cash = 0;
	trades[i].gojf = false;
	trades[i].prop_list = NULL;
    }
  over:
    if (num_play == 1) {
	printf("There ain't no-one around to trade WITH!!\n");
	return;
    }
    unsigned tradee = 1 - player;
    if (num_play > 2) {
	tradee = getinp("Which player do you wish to trade with? ", name_list);
	if (tradee == num_play)
	    return;
	if (tradee == player) {
	    printf("You can't trade with yourself!\n");
	    goto over;
	}
    }
    get_list(0, player);
    get_list(1, tradee);
    if (getyn("Do you wish a summary? ") == 0)
	summate();
    if (getyn("Is the trade ok? ") == 0)
	do_trade();
}

// This routine gets the list of things to be trader for the
// player, and puts in the structure given.
static void get_list(int struct_no, int play_no)
{
    for (unsigned numin = 0; numin < MAX_PRP; numin++)
	used[numin] = false;
    unsigned sn = struct_no, pn = play_no;
    PLAY* pp = &play[pn];
    TRADE* tp = &trades[sn];
    tp->trader = pn;
    printf("player %s (%u):\n", pp->name, pn + 1);
    if (pp->own_list) {
	unsigned numin = set_list(pp->own_list);
	for (unsigned num_prp = numin; num_prp;) {
	    unsigned prop = getinp("Which property do you wish to trade? ", plist);
	    if (prop == numin)
		break;
	    else if (used[prop])
		printf("You've already allocated that.\n");
	    else {
		--num_prp;
		used[prop] = true;
		OWN* op = pp->own_list;
		while (prop--)
		    op = op->next;
		add_list(pn, &(tp->prop_list), sqnum(op->sqr));
	    }
	}
    }
    if (pp->money > 0) {
	printf("You have $%d.  ", pp->money);
	tp->cash = get_int("How much are you trading? ");
    }
    if (pp->num_gojf > 0) {
      once_more:
	printf("You have %d get-out-of-jail-free cards. ", pp->num_gojf);
	tp->gojf = get_int("How many are you trading? ");
	if (tp->gojf > pp->num_gojf) {
	    printf("You don't have that many.  Try again.\n");
	    goto once_more;
	}
    }
}

// This routine sets up the list of tradable property.
static int set_list(OWN * the_list)
{
    unsigned i = 0;
    for (const OWN* op = the_list; op; op = op->next)
	if (!used[i])
	    plist[i++] = op->sqr->name;
    plist[i++] = "done";
    plist[i--] = 0;
    return i;
}

// This routine summates the trade.
static void summate(void)
{
    for (unsigned i = 0; i < 2; ++i) {
	TRADE* tp = &trades[i];
	bool some = false;
	printf("Player %s (%d) gives:\n", play[tp->trader].name, tp->trader + 1);
	if (tp->cash > 0)
	    printf("\t$%d\n", tp->cash), some++;
	if (tp->gojf > 0)
	    printf("\t%d get-out-of-jail-free card(s)\n", tp->gojf), some++;
	if (tp->prop_list) {
	    for (const OWN* op = tp->prop_list; op; op = op->next)
		putchar('\t'), printsq(sqnum(op->sqr), true);
	    some = true;
	}
	if (!some)
	    printf("\t-- Nothing --\n");
    }
}

// This routine actually executes the trade.
static void do_trade(void)
{
    move_em(&trades[0], &trades[1]);
    move_em(&trades[1], &trades[0]);
}

// This routine does a switch from one player to another
static void move_em(TRADE * from, TRADE * to)
{
    PLAY* pl_fr = &play[from->trader];
    PLAY* pl_to = &play[to->trader];

    pl_fr->money -= from->cash;
    pl_to->money += from->cash;
    pl_fr->num_gojf -= from->gojf;
    pl_to->num_gojf += from->gojf;
    for (OWN* op = from->prop_list; op; op = op->next) {
	add_list(to->trader, &(pl_to->own_list), sqnum(op->sqr));
	op->sqr->owner = to->trader;
	del_list(from->trader, &(pl_fr->own_list), sqnum(op->sqr));
    }
    set_ownlist(to->trader);
}

// This routine lets a player resign
void resign(void)
{
    unsigned new_own;
    if (cur_p->money <= 0) {
	switch (board[cur_p->loc].type) {
	    case UTIL:
	    case RR:
	    case PRPTY:
		new_own = board[cur_p->loc].owner;
		break;
	    default:	       // Chance, taxes, etc
		new_own = num_play;
		break;
	}
	if (new_own == num_play)
	    printf("You would resign to the bank\n");
	else
	    printf("You would resign to %s\n", name_list[new_own]);
    } else if (num_play == 1) {
	new_own = num_play;
	printf("You would resign to the bank\n");
    } else {
	name_list[num_play] = "bank";
	do {
	    new_own = getinp("Who do you wish to resign to? ", name_list);
	    if (new_own == player)
		printf("You can't resign to yourself!!\n");
	} while (new_own == player);
	name_list[num_play] = "done";
    }
    if (getyn("Do you really want to resign? ") != 0)
	return;
    if (num_play == 1) {
	printf("Then NOBODY wins (not even YOU!)\n");
	exit(0);
    }
    if (new_own < num_play) {  // resign to player
	printf("resigning to player\n");
	trades[0].trader = new_own;
	trades[0].cash = trades[0].gojf = 0;
	trades[0].prop_list = NULL;
	trades[1].trader = player;
	trades[1].cash = cur_p->money > 0 ? cur_p->money : 0;
	trades[1].gojf = cur_p->num_gojf;
	trades[1].prop_list = cur_p->own_list;
	do_trade();
    } else {		       // resign to bank
	printf("resigning to bank\n");
	for (OWN* op = cur_p->own_list; op; op = op->next) {
	    SQUARE* sqp = op->sqr;
	    sqp->owner = -1;
	    sqp->desc->morg = false;
	    if (sqp->type == PRPTY) {
		is_not_monop(sqp->desc->mon_desc);
		sqp->desc->houses = 0;
	    }
	}
	if (cur_p->num_gojf)
	    ret_card(cur_p);
    }
    for (unsigned i = player; i < num_play; i++) {
	name_list[i] = name_list[i + 1];
	if (i + 1 < num_play)
	    play[i] = play[i + 1];
    }
    name_list[num_play--] = 0;
    for (unsigned i = 0; i < N_SQRS; i++)
	if (board[i].owner > (int) player)
	    --board[i].owner;
    player = player ? player-1 : num_play-1;
    next_play();
    if (num_play < 2) {
	printf("\nThen %s WINS!!!!!\n", play[0].name);
	printhold(0);
	printf("That's a grand worth of $%d.\n", play[0].money + prop_worth(&play[0]));
	exit(0);
    }
}

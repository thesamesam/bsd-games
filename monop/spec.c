// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

static const char *const perc[] = {
    "10%", "ten percent", "%", "$200", "200", 0
};

void inc_tax(void)
{			       // collect income tax
    int worth, com_num;

    com_num = getinp("Do you wish to lose 10%% of your total worth or " "$200? ", perc);
    worth = cur_p->money + prop_worth(cur_p);
    printf("You were worth $%d", worth);
    worth /= 10;
    if (com_num > 2) {
	if (worth < 200)
	    printf(".  Good try, but not quite.\n");
	else if (worth > 200)
	    lucky(".\nGood guess.  ");
	cur_p->money -= 200;
    } else {
	printf(", so you pay $%d", worth);
	if (worth > 200)
	    printf("  OUCH!!!!.\n");
	else if (worth < 200)
	    lucky("\nGood guess.  ");
	cur_p->money -= worth;
    }
    if (worth == 200)
	lucky("\nIt makes no difference!  ");
}

void goto_jail(void)
{			       // move player to jail
    cur_p->loc = JAIL;
}

void lux_tax(void)
{			       // landing on luxury tax
    printf("You lose $75\n");
    cur_p->money -= 75;
}

void cc(void)
{			       // draw community chest card
    get_card (c_CommunityChestDeck);
}

void chance(void)
{			       // draw chance card
    get_card (c_ChanceCardsDeck);
}

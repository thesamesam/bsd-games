// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
// Re-coding of advent in C: termination routines

#include "hdr.h"
#include "extern.h"
#include <stdio.h>
#include <stdlib.h>

int score(void)
{			       // sort of like 20000
    int scor = 0;
    mxscor = 0;
    for (int i = 50; i <= maxtrs; ++i) {
	if (ptext[i] && !strlen(ptext[i]))
	    continue;
	k = 12;
	if (i == chest)
	    k = 14;
	if (i > chest)
	    k = 16;
	if (prop[i] >= 0)
	    scor += 2;
	if (place[i] == 3 && prop[i] == 0)
	    scor += k - 2;
	mxscor += k;
    }
    scor += (maxdie - numdie) * 10;
    mxscor += maxdie * 10;
    if (!(scorng || gaveup))
	scor += 4;
    mxscor += 4;
    if (dflag != 0)
	scor += 25;
    mxscor += 25;
    if (closng)
	scor += 25;
    mxscor += 25;
    if (closed) {
	if (bonus == 0)
	    scor += 10;
	if (bonus == 135)
	    scor += 25;
	if (bonus == 134)
	    scor += 30;
	if (bonus == 133)
	    scor += 45;
    }
    mxscor += 45;
    if (place[magzin] == 108)
	scor++;
    mxscor++;
    scor += 2;
    mxscor += 2;
    for (unsigned i = 1; i < ArraySize(hints); ++i)
	if (hinted[i])
	    scor -= hints[i][2];
    return scor;
}

// entry=2 means goto 20000, 3=19000
void done (int entry)
{
    // Player class descriptions
    static const struct {
	unsigned short threshold;
	char title [62];
    } c_AdvClasses[] = {
	{ 0,	"You are obviously a rank amateur.  Better luck next time."	},
	{ 100,	"Your score qualifies you as a Novice class Adventurer."	},
	{ 130,	"You have achieved the rating: \"Experienced Adventurer\"."	},
	{ 200,	"You may now consider yourself a \"Seasoned Adventurer\"."	},
	{ 250,	"You have reached \"Junior Master\" status."		},
	{ 300,	"Your score puts you in Master Adventurer Class C."	},
	{ 330,	"Your score puts you in Master Adventurer Class B."	},
	{ 349,	"Your score puts you in Master Adventurer Class A."	},
	{ 9999,	"You are now an exalted Adventurer Grandmaster!"	}
    };
    if (entry == 1)
	mspeak (1);
    if (entry == 3)
	rspeak (136);
    unsigned sc = score();
    printf("\n\n\nYou scored %u out of a possible %d using %d turns.\n", sc, mxscor, turns);
    unsigned advcls = 0;
    while (advcls+1 < ArraySize(c_AdvClasses) && sc >= c_AdvClasses[advcls+1].threshold)
	++advcls;
    speak (c_AdvClasses[advcls].title);
    if (advcls < ArraySize(c_AdvClasses)-1) {
	unsigned tolevel = c_AdvClasses[advcls+1].threshold - sc;
	printf("To achieve the next higher rating, you need %u more point%s", tolevel, "s.\n"+(tolevel==1));
    }
    exit(EXIT_SUCCESS);
}

void die(int entry)
{
    int i;
    if (entry != 99) {
	rspeak(23);
	oldlc2 = loc;
    }
    if (closng) {	       // 99
	rspeak(131);
	numdie++;
	done(2);
    }
    int yea = yes(81 + numdie * 2, 82 + numdie * 2, 54);
    numdie++;
    if (numdie == maxdie || !yea)
	done(2);
    place[water] = 0;
    place[oil] = 0;
    if (toting(lamp))
	prop[lamp] = 0;
    for (i = 100; i >= 1; i--) {
	if (!toting(i))
	    continue;
	k = oldlc2;
	if (i == lamp)
	    k = 1;
	drop(i, k);
    }
    loc = 3;
    oldloc = loc;
}

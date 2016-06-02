// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

void dig(void)
{
    if (testbit(inven, SHOVEL)) {
	puts("OK");
	ourtime++;
	switch (position) {
	    case 144:	       // copse near beach
		if (!notes[DUG]) {
		    setbit(location[position].objects, DEADWOOD);
		    setbit(location[position].objects, COMPASS);
		    setbit(location[position].objects, KNIFE);
		    setbit(location[position].objects, MACE);
		    notes[DUG] = 1;
		}
		break;

	    default:
		puts("Nothing happens.");
	}
    } else
	puts("You don't have a shovel.");
}

int jump(void)
{
    int n;

    switch (position) {
	default:
	    puts("Nothing happens.");
	    return -1;

	case 242:
	    position = 133;
	    break;
	case 214:
	case 215:
	case 162:
	case 159:
	    position = 145;
	    break;
	case 232:
	    position = FINAL;
	    break;
	case 3:
	    position = 1;
	    break;
	case 172:
	    position = 201;
    }
    puts("Ahhhhhhh...");
    injuries[12] = injuries[8] = injuries[7] = injuries[6] = 1;
    for (n = 0; n < NUMOFOBJECTS; n++)
	if (testbit(inven, n)) {
	    clearbit(inven, n);
	    setbit(location[position].objects, n);
	}
    carrying = 0;
    encumber = 0;
    return 0;
}

void bury(void)
{
    int value;

    if (testbit(inven, SHOVEL)) {
	while (wordtype[++wordnumber] != OBJECT && wordtype[wordnumber] != NOUNS && wordnumber < wordcount)
	    continue;
	value = wordvalue[wordnumber];
	if (wordtype[wordnumber] == NOUNS && (testbit(location[position].objects, value) || value == BODY))
	    switch (value) {
		case BODY:
		    wordtype[wordnumber] = OBJECT;
		    if (testbit(inven, MAID) || testbit(location[position].objects, MAID))
			value = MAID;
		    if (testbit(inven, DEADWOOD) || testbit(location[position].objects, DEADWOOD))
			value = DEADWOOD;
		    if (testbit(inven, DEADGOD) || testbit(location[position].objects, DEADGOD))
			value = DEADGOD;
		    if (testbit(inven, DEADTIME) || testbit(location[position].objects, DEADTIME))
			value = DEADTIME;
		    if (testbit(inven, DEADNATIVE) || testbit(location[position].objects, DEADNATIVE))
			value = DEADNATIVE;
		    break;

		case NATIVE:
		case NORMGOD:
		    puts("She screams as you wrestle her into the hole.");
		case TIMER:
		    power += 7;
		    ego -= 10;
		case AMULET:
		case MEDALION:
		case TALISMAN:
		    wordtype[wordnumber] = OBJECT;
		    break;

		default:
		    puts("Wha..?");
	    }
	if (wordtype[wordnumber] == OBJECT && position > 88 && (testbit(inven, value) || testbit(location[position].objects, value))) {
	    puts("Buried.");
	    if (testbit(inven, value)) {
		clearbit(inven, value);
		carrying -= objwt[value];
		encumber -= objcumber[value];
	    }
	    clearbit(location[position].objects, value);
	    switch (value) {
		case MAID:
		case DEADWOOD:
		case DEADNATIVE:
		case DEADTIME:
		case DEADGOD:
		    ego += 2;
		    printf("The %s should rest easier now.\n", objsht[value]);
	    }
	} else
	    puts("It doesn't seem to work.");
    } else
	puts("You aren't holding a shovel.");
}

void drink(void)
{
    int n;

    if (testbit(inven, POTION)) {
	puts("The cool liquid runs down your throat but turns to fire and you choke.");
	puts("The heat reaches your limbs and tingles your spirit.  You feel like falling");
	puts("asleep.");
	clearbit(inven, POTION);
	WEIGHT = MAXWEIGHT;
	CUMBER = MAXCUMBER;
	for (n = 0; n < NUMOFINJURIES; n++)
	    injuries[n] = 0;
	ourtime++;
	zzz();
    } else
	puts("I'm not thirsty.");
}

int shoot(void)
{
    int firstnumber = wordnumber;
    if (!testbit(inven, LASER))
	puts("You aren't holding a blaster.");
    else {
	wordnumber++;
	while (wordnumber <= wordcount && wordtype[wordnumber] == OBJECT) {
	    int value = wordvalue[wordnumber];
	    printf("%s:\n", objsht[value]);
	    if (testbit(location[position].objects, value)) {
		clearbit(location[position].objects, value);
		++ourtime;
		printf("The %s explode%s\n", objsht[value], (is_plural_object(value) ? "." : "s."));
		if (value == BOMB)
		    die();
	    } else
		printf("I don't see any %s around here.\n", objsht[value]);
	    if (wordnumber < wordcount - 1 && wordvalue[++wordnumber] == AND)
		++wordnumber;
	    else
		return firstnumber;
	}
	// The blaster's only useful purpose is to destroy locked doors.
	if (wordnumber <= wordcount && wordtype[wordnumber] == NOUNS) {
	    ++ourtime;
	    if (wordvalue[wordnumber] == DOOR) {
		switch (position) {
		    case 189:
		    case 231:
			puts("The door is unhinged.");
			location[189].north = 231;
			location[231].south = 189;
			whichway(location[position]);
			break;
		    case 30:
			puts("The wooden door splinters.");
			location[30].west = 25;
			whichway(location[position]);
			break;
		    case 31:
			puts("The laser blast has no effect on the door.");
			break;
		    case 20:
			puts("The blast hits the door and it explodes into flame.  The magnesium burns");
			puts("so rapidly that we have no chance to escape.");
			die();
		    default:
			puts("Nothing happens.");
		}
	    } else
		puts("Shoot what?");
	} else
	    puts("Shoot what?");
    }
    return firstnumber;
}

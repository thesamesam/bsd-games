// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

int wearit(void)
{			       // synonyms = {sheathe, sheath}
    int firstnumber, value;

    firstnumber = wordnumber;
    wordnumber++;
    while (wordnumber <= wordcount && (wordtype[wordnumber] == OBJECT || wordtype[wordnumber] == NOUNS) && wordvalue[wordnumber] != DOOR) {
	value = wordvalue[wordnumber];
	if (value >= 0 && objsht[value] == NULL)
	    break;
	switch (value) {

	    case -1:
		puts("Wear what?");
		return firstnumber;

	    default:
		printf("You can't wear %s%s!\n", A_OR_AN_OR_BLANK(value), objsht[value]);
		return firstnumber;

	    case KNIFE:
		// case SHIRT:
	    case ROBE:
	    case LEVIS:       // wearable things
	    case SWORD:
	    case MAIL:
	    case HELM:
	    case SHOES:
	    case PAJAMAS:
	    case COMPASS:
	    case LASER:
	    case AMULET:
	    case TALISMAN:
	    case MEDALION:
	    case ROPE:
	    case RING:
	    case BRACELET:
	    case GRENADE:

		if (testbit(inven, value)) {
		    clearbit(inven, value);
		    setbit(wear, value);
		    carrying -= objwt[value];
		    encumber -= objcumber[value];
		    ourtime++;
		    printf("You are now wearing %s%s.\n", A_OR_AN_OR_THE(value), objsht[value]);
		} else if (testbit(wear, value))
		    printf("You are already wearing the %s.\n", objsht[value]);
		else
		    printf("You aren't holding the %s.\n", objsht[value]);
		if (wordnumber < wordcount - 1 && wordvalue[++wordnumber] == AND)
		    wordnumber++;
		else
		    return firstnumber;
	}		       // end switch
    }			       // end while
    puts("Don't be ridiculous.");
    return firstnumber;
}

int put(void)
{			       // synonyms = {buckle, strap, tie}
    if (wordvalue[wordnumber + 1] == ON) {
	wordvalue[++wordnumber] = PUTON;
	wordtype[wordnumber] = VERB;
	return cypher();
    }
    if (wordvalue[wordnumber + 1] == DOWN) {
	wordvalue[++wordnumber] = DROP;
	wordtype[wordnumber] = VERB;
	return cypher();
    }
    puts("I don't understand what you want to put.");
    return -1;

}

int draw(void)
{			       // synonyms = {pull, carry}
    return take(wear);
}

int use(void)
{
    wordnumber++;
    if (wordvalue[wordnumber] == AMULET && testbit(inven, AMULET) && position != FINAL) {
	puts("The amulet begins to glow.");
	if (testbit(inven, MEDALION)) {
	    puts("The medallion comes to life too.");
	    if (position == 114) {
		location[position].down = 160;
		whichway(location[position]);
		puts("The waves subside and it is possible to descend to the sea cave now.");
		ourtime++;
		return -1;
	    }
	}
	puts("A light mist falls over your eyes and the sound of purling water trickles in");
	puts("your ears.   When the mist lifts you are standing beside a cool stream.");
	if (position == 229)
	    position = 224;
	else
	    position = 229;
	ourtime++;
	notes[CANTSEE] = 0;
	return 0;
    } else if (position == FINAL)
	puts("The amulet won't work in here.");
    else if (wordvalue[wordnumber] == COMPASS && testbit(inven, COMPASS))
	printf("Your compass points %s.\n", truedirec(NORTH, '-'));
    else if (wordvalue[wordnumber] == COMPASS)
	puts("You aren't holding the compass.");
    else if (wordvalue[wordnumber] == AMULET)
	puts("You aren't holding the amulet.");
    else
	puts("There is no apparent use.");
    return -1;
}

int follow(void)
{
    if (followfight == ourtime) {
	puts("The Dark Lord leaps away and runs down secret tunnels and corridors.");
	puts("You chase him through the darkness and splash in pools of water.");
	puts("You have cornered him.  His laser sword extends as he steps forward.");
	position = FINAL;
	fight(DARK, 75);
	setbit(location[position].objects, TALISMAN);
	setbit(location[position].objects, AMULET);
	return 0;
    } else if (followgod == ourtime) {
	puts("The goddess leads you down a steamy tunnel and into a high, wide chamber.");
	puts("She sits down on a throne.");
	position = 268;
	setbit(location[position].objects, NORMGOD);
	notes[CANTSEE] = 1;
	return 0;
    } else
	puts("There is no one to follow.");
    return -1;
}

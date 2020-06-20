// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

int moveplayer(int thataway, int token)
{
    wordnumber++;
    if ((!game_state (CANTMOVE) && !game_state (LAUNCHED)) || object_is_at (LAND, position) || (fuel > 0 && game_state (LAUNCHED))) {
	if (thataway) {
	    position = thataway;
	    newway(token);
	    ourtime++;
	} else {
	    puts("You can't go this way.");
	    newway(token);
	    update_relative_directions();
	    return 0;
	}
    } else {
	if (game_state (CANTMOVE) && !game_state (LAUNCHED))
	    puts("You aren't able to move; you better drop something.");
	else
	    puts("You are out of fuel; now you will rot in space forever!");
    }
    return 1;
}

void news(void)
{
    int n;
    int hurt;

    if (ourtime > 64 && position < OUTSIDE_BATTLESTAR) {
	puts("An explosion of shuddering magnitude splinters bulkheads and");
	puts("ruptures the battlestar's hull.  You are sucked out into the");
	puts("frozen void of space and killed.");
	die();
    }
    if (ourtime > 48 && position < OUTSIDE_BATTLESTAR)
	puts("Explosions rock the battlestar.");
    if (ourtime > snooze) {
	puts("You drop from exhaustion...");
	zzz();
    }
    if (ourtime > snooze - 5)
	puts("You're getting tired.");
    if (ourtime > (rythmn + CYCLE)) {
	if (game_state (IS_NIGHT)) {
	    convert(TODAY);
	    if (OUTSIDE && ourtime - rythmn - CYCLE < 10) {
		puts("Dew lit sunbeams stretch out from a watery sunrise and herald the dawn.");
		puts("You awake from a misty dream-world into stark reality.");
		puts("It is day.");
	    }
	} else {
	    convert(TONIGHT);
	    remove_object_from (BATHGOD, POOLS);
	    if (OUTSIDE && ourtime - rythmn - CYCLE < 10) {
		puts("The dying sun sinks into the ocean, leaving a blood-stained sunset.");
		puts("The sky slowly fades from orange to violet to black.  A few stars");
		puts("flicker on, and it is night.");
		puts("The world seems completely different at night.");
	    }
	}
	rythmn = ourtime - ourtime % CYCLE;
    }
    if (!game_state (IS_WIZARD)) {
	if ((object_is_at (TALISMAN, INVENTORY) || object_is_at (TALISMAN, WEARING))
		&& (object_is_at (MEDALION, INVENTORY) || object_is_at (MEDALION, WEARING))
		&& (object_is_at (AMULET, INVENTORY) || object_is_at (AMULET, WEARING))) {
	    set_game_state (IS_WIZARD);
	    puts("The three amulets glow and reenforce each other in power.\nYou are now a wizard.");
	}
    }
    if (object_is_at (ELF, position)) {
	printf("%s\n", c_objinfo[ELF].desc);
	fight(ELF, nrand(30));
    }
    if (object_is_at (DARK, position)) {
	printf("%s\n", c_objinfo[DARK].desc);
	fight(DARK, 100);
    }
    if (object_is_at (WOODSMAN, position)) {
	printf("%s\n", c_objinfo[WOODSMAN].desc);
	fight(WOODSMAN, 50);
    }
    switch (position) {
	case 246:
	case 257:	// entering a cave
	case 267:
	case 274:
	    set_game_state (CANTSEE);
	    break;
	case SEA_CAVE_ENTRANCE:
	case 216:	// leaving a cave
	case 230:
	case CAVE_ENTRANCE:
	case HUGE_CHASM:
	    clear_game_state (CANTSEE);
	    break;
    }
    if (object_is_at (GIRL, position))
	set_game_state (MET_GIRL);
    if (game_state (MET_GIRL) && CYCLE * 1.5 - ourtime < 10) {
	create_object_at (GIRLTALK, GARDEN);
	create_object_at (LAMPON, GARDEN);
	create_object_at (ROPE, GARDEN);
    }
    if (position == DOCK && (beenthere[position] || ourtime > CYCLE)) {
	remove_object_from (GIRL, DOCK);
	remove_object_from (MAN, DOCK);
    }
    if (game_state (MET_GIRL) && ourtime - CYCLE * 1.5 > 10) {
	remove_object_from (GIRLTALK, GARDEN);
	remove_object_from (LAMPON, GARDEN);
	remove_object_from (ROPE, GARDEN);
	clear_game_state (MET_GIRL);
    }
    if (object_is_at (DRENIAN, position)) {
	puts("Oh my God, you're being shot at by an alien spacecraft!");
	printf("The targeting computer says we have %d seconds to attack!\n", ourclock);
	fflush(stdout);
	sleep(1);
	if (!visual()) {
	    hurt = nrand(NUMOFINJURIES);
	    injuries[hurt] = 1;
	    puts("Laser blasts sear the cockpit, and the alien veers off in a victory roll.");
	    puts("The starfighter shudders under a terrible explosion.");
	    printf("I'm afraid you have suffered %s.\n", ouch[hurt]);
	} else
	    remove_object_from (DRENIAN, position);
    }
    if (injuries[SKULL] && injuries[INCISE] && injuries[NECK]) {
	puts("I'm afraid you have suffered fatal injuries.");
	die();
    }
    for (n = 0; n < NUMOFINJURIES; n++)
	if (injuries[n] == 1) {
	    injuries[n] = 2;
	    if (WEIGHT > 5)
		WEIGHT -= 5;
	    else
		WEIGHT = 0;
	}
    if (injuries[ARM] == 2) {
	if (CUMBER > 5)
	    CUMBER -= 5;
	else
	    CUMBER = 0;
	injuries[ARM]++;
    }
    if (injuries[RIBS] == 2) {
	if (CUMBER > 2)
	    CUMBER -= 2;
	else
	    CUMBER = 0;
	injuries[RIBS]++;
    }
    if (injuries[SPINE] == 2) {
	WEIGHT = 0;
	injuries[SPINE]++;
    }
    if (carrying > WEIGHT || encumber > CUMBER)
	set_game_state (CANTMOVE);
    else
	clear_game_state (CANTMOVE);
}

void crash(void)
{
    int hurt1, hurt2;

    fuel--;
    if (!location[position].dir.flyhere || (object_is_at (LAND, position) && fuel <= 0)) {
	if (!location[position].dir.flyhere)
	    puts("You're flying too low.  We're going to crash!");
	else {
	    puts("You're out of fuel.  We'll have to crash land!");
	    if (!location[position].dir.down) {
		puts("Your starfighter strikes the ground and explodes into fiery fragments.");
		puts("Thick black smoke billows up from the wreckage.");
		die();
	    }
	    position = location[position].dir.down;
	}
	clear_game_state (LAUNCHED);
	create_object_at (CRASH, position);
	ourtime += nrand(CYCLE / 4);
	puts("The starfighter explodes into the ground and you lose consciousness...");
	zzz();
	hurt1 = nrand(NUMOFINJURIES - 2) + 2;
	hurt2 = nrand(NUMOFINJURIES - 2) + 2;
	injuries[hurt1] = 1;
	injuries[hurt2] = 1;
	injuries[0] = 1;       // abrasions
	injuries[1] = 1;       // lacerations
	printf("I'm afraid you have suffered %s and %s.\n", ouch[hurt1], ouch[hurt2]);
    }
}

int wearit(void)
{			       // synonyms = {sheathe, sheath}
    int firstnumber, value;

    firstnumber = wordnumber;
    wordnumber++;
    while (wordnumber <= wordcount && (wordtype[wordnumber] == OBJECT || wordtype[wordnumber] == NOUNS) && wordvalue[wordnumber] != DOOR) {
	value = wordvalue[wordnumber];
	if (value >= 0 && c_objinfo[value].shdesc == NULL)
	    break;
	switch (value) {

	    case -1:
		puts("Wear what?");
		return firstnumber;

	    default:
		printf("You can't wear %s%s!\n", A_OR_AN_OR_BLANK(value), c_objinfo[value].shdesc);
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

		if (object_is_at (value, INVENTORY)) {
		    remove_object_from (value, INVENTORY);
		    create_object_at (value, WEARING);
		    carrying -= c_objinfo[value].weight;
		    encumber -= c_objinfo[value].cumber;
		    ourtime++;
		    printf("You are now wearing %s%s.\n", A_OR_AN_OR_THE(value), c_objinfo[value].shdesc);
		} else if (object_is_at (value, WEARING))
		    printf("You are already wearing the %s.\n", c_objinfo[value].shdesc);
		else
		    printf("You aren't holding the %s.\n", c_objinfo[value].shdesc);
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
    return take (WEARING);
}

int use(void)
{
    wordnumber++;
    if (wordvalue[wordnumber] == AMULET && object_is_at (AMULET, INVENTORY) && position != FINAL) {
	puts("The amulet begins to glow.");
	if (object_is_at (MEDALION, INVENTORY)) {
	    puts("The medallion comes to life too.");
	    if (position == ABOVE_SEA_CAVE) {
		set_game_state (UNCOVERED_SEA_CAVE);
		update_relative_directions();
		puts("The waves subside and it is possible to descend to the sea cave now.");
		++ourtime;
		return -1;
	    }
	}
	puts("A light mist falls over your eyes and the sound of purling water trickles in");
	puts("your ears. When the mist lifts you are standing beside a cool stream.");
	if (position == CAVE_STREAM_BANK)
	    position = END_OF_THE_ROAD;
	else
	    position = CAVE_STREAM_BANK;
	ourtime++;
	clear_game_state (CANTSEE);
	return 0;
    } else if (position == FINAL)
	puts("The amulet won't work in here.");
    else if (wordvalue[wordnumber] == COMPASS && object_is_at (COMPASS, INVENTORY))
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
	create_object_at (TALISMAN, position);
	create_object_at (AMULET, position);
	return 0;
    } else if (followgod == ourtime) {
	puts("The goddess leads you down a steamy tunnel and into a high, wide chamber.");
	puts("She sits down on a throne.");
	position = GODDESS_THRONE_ROOM;
	create_object_at (NORMGOD, position);
	set_game_state (CANTSEE);
	return 0;
    } else
	puts("There is no one to follow.");
    return -1;
}

void dig (void)
{
    if (object_is_at (SHOVEL, INVENTORY)) {
	puts("OK");
	ourtime++;
	switch (position) {
	    case SECRET_THICKET:	// copse near beach
		if (!game_state (DUG)) {
		    create_object_at (DEADWOOD, position);
		    create_object_at (COMPASS, position);
		    create_object_at (KNIFE, position);
		    create_object_at (MACE, position);
		    set_game_state (DUG);
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

	case CLIFF_WATERFALL:
	    position = 133;
	    break;
	case 159:
	case 162:
	case 214:
	case 215:
	    position = TIDE_POOLS;
	    break;
	case HUGE_CHASM:
	    position = FINAL;
	    break;
	case HANGAR_GALLERY:
	    position = MAIN_HANGAR;
	    break;
	case 172:
	    position = 201;
    }
    puts("Ahhhhhhh...");
    injuries[12] = injuries[8] = injuries[7] = injuries[6] = true;
    for (n = 0; n < NUMOFOBJECTS; n++) {
	if (object_is_at (n, INVENTORY)) {
	    remove_object_from (n, INVENTORY);
	    create_object_at (n, position);
	}
    }
    carrying = 0;
    encumber = 0;
    return 0;
}

void bury (void)
{
    int value;

    if (object_is_at (SHOVEL, INVENTORY)) {
	while (wordtype[++wordnumber] != OBJECT && wordtype[wordnumber] != NOUNS && wordnumber < wordcount)
	    continue;
	value = wordvalue[wordnumber];
	if (wordtype[wordnumber] == NOUNS && (object_is_at (value, position) || value == BODY))
	    switch (value) {
		case BODY:
		    wordtype[wordnumber] = OBJECT;
		    if (object_is_at (MAID, INVENTORY) || object_is_at (MAID, position))
			value = MAID;
		    if (object_is_at (DEADWOOD, INVENTORY) || object_is_at (DEADWOOD, position))
			value = DEADWOOD;
		    if (object_is_at (DEADGOD, INVENTORY) || object_is_at (DEADGOD, position))
			value = DEADGOD;
		    if (object_is_at (DEADTIME, INVENTORY) || object_is_at (DEADTIME, position))
			value = DEADTIME;
		    if (object_is_at (DEADNATIVE, INVENTORY) || object_is_at (DEADNATIVE, position))
			value = DEADNATIVE;
		    break;

		case TIMER:
		    power += 7;
		    ego -= 10;
		    // fallthrough
		case AMULET:
		case MEDALION:
		case TALISMAN:
		    wordtype[wordnumber] = OBJECT;
		    break;

		default:
		    puts("Wha..?");
	    }
	if (wordtype[wordnumber] == OBJECT && position >= FIRST_ISLAND_LAND && (object_is_at (value, INVENTORY) || object_is_at (value, position))) {
	    puts("Buried.");
	    if (object_is_at (value, INVENTORY)) {
		remove_object_from (value, INVENTORY);
		carrying -= c_objinfo[value].weight;
		encumber -= c_objinfo[value].cumber;
	    }
	    remove_object_from (value, position);
	    switch (value) {
		case MAID:
		case DEADWOOD:
		case DEADNATIVE:
		case DEADTIME:
		case DEADGOD:
		    ego += 2;
		    printf("The %s should rest easier now.\n", c_objinfo[value].shdesc);
	    }
	} else
	    puts("It doesn't seem to work.");
    } else
	puts("You aren't holding a shovel.");
}

void drink (void)
{
    if (object_is_at (POTION, INVENTORY)) {
	puts("The cool liquid runs down your throat but turns to fire and you choke. The");
	puts("heat reaches your limbs and tingles your spirit. You feel like falling asleep.");
	remove_object_from (POTION, INVENTORY);
	WEIGHT = MAXWEIGHT;
	CUMBER = MAXCUMBER;
	for (unsigned n = 0; n < NUMOFINJURIES; n++)
	    injuries[n] = false;
	++ourtime;
	zzz();
    } else
	puts("I'm not thirsty.");
}

int shoot (void)
{
    int firstnumber = wordnumber;
    if (!object_is_at (LASER, INVENTORY))
	puts("You aren't holding a blaster.");
    else {
	wordnumber++;
	while (wordnumber <= wordcount && wordtype[wordnumber] == OBJECT) {
	    int value = wordvalue[wordnumber];
	    printf("%s:\n", c_objinfo[value].shdesc);
	    if (object_is_at (value, position)) {
		remove_object_from (value, position);
		++ourtime;
		printf("The %s explode%s\n", c_objinfo[value].shdesc, (is_plural_object(value) ? "." : "s."));
		if (value == BOMB)
		    die();
	    } else
		printf("I don't see any %s around here.\n", c_objinfo[value].shdesc);
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
		    case CAVE_DOOR:
		    case CAVE_ENTRANCE:
			puts("The door is unhinged.");
			set_game_state (OPENED_CAVE_DOOR);
			update_relative_directions();
			break;
		    case KITCHEN_DOOR:
			puts("The wooden door splinters.");
			set_game_state (OPENED_KITCHEN_DOOR);
			update_relative_directions();
			break;
		    case DINING_ROOM_DOOR:
			puts("The laser blast has no effect on the door.");
			break;
		    case LASER_ROOM:
			puts("The blast hits the door and it explodes into flame. The magnesium burns");
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

int take (uint16_t fromloc)
{
    int firstnumber, heavy, bulky, value;

    firstnumber = wordnumber;
    if (wordnumber < wordcount && wordvalue[wordnumber + 1] == OFF) {
	wordnumber++;
	wordvalue[wordnumber] = TAKEOFF;
	wordtype[wordnumber] = VERB;
	return cypher();
    } else {
	wordnumber++;
	while (wordnumber <= wordcount && wordtype[wordnumber] == OBJECT) {
	    value = wordvalue[wordnumber];
	    printf("%s:\n", c_objinfo[value].shdesc);
	    heavy = (carrying + c_objinfo[value].weight) <= WEIGHT;
	    bulky = (encumber + c_objinfo[value].cumber) <= CUMBER;
	    if ((object_is_at (value, fromloc) || game_state (IS_WIZARD)) && heavy && bulky && !object_is_at (value, INVENTORY)) {
		create_object_at (value, INVENTORY);
		carrying += c_objinfo[value].weight;
		encumber += c_objinfo[value].cumber;
		ourtime++;
		if (object_is_at (value, fromloc))
		    printf("Taken.\n");
		else
		    printf("Zap! Taken from thin air.\n");
		remove_object_from (value, fromloc);
		if (value == MEDALION)
		    win--;
	    } else if (object_is_at (value, INVENTORY))
		printf("You're already holding %s%s.\n", A_OR_AN_OR_BLANK(value), c_objinfo[value].shdesc);
	    else if (!object_is_at (value, fromloc))
		printf("I don't see any %s around here.\n", c_objinfo[value].shdesc);
	    else if (!heavy)
		printf("The %s %stoo heavy.\n", c_objinfo[value].shdesc, IS_OR_ARE(value));
	    else
		printf("The %s %stoo cumbersome to hold.\n", c_objinfo[value].shdesc, IS_OR_ARE(value));
	    if (wordnumber < wordcount - 1 && wordvalue[++wordnumber] == AND)
		wordnumber++;
	    else
		return firstnumber;
	}
    }
    // special cases with their own returns

    if (wordnumber <= wordcount && wordtype[wordnumber] == NOUNS)
	switch (wordvalue[wordnumber]) {

	    case SWORD:
		if (object_is_at (SWORD, fromloc)) {
		    wordtype[wordnumber--] = OBJECT;
		    return take(fromloc);
		}
		if (object_is_at (TWO_HANDED, fromloc)) {
		    wordvalue[wordnumber] = TWO_HANDED;
		    wordtype[wordnumber--] = OBJECT;
		    return take(fromloc);
		}
		wordvalue[wordnumber] = BROAD;
		wordtype[wordnumber--] = OBJECT;
		return take(fromloc);

	    case BODY:
		if (object_is_at (MAID, fromloc)) {
		    wordvalue[wordnumber] = MAID;
		    wordtype[wordnumber--] = OBJECT;
		    return take(fromloc);
		} else if (object_is_at (DEADWOOD, fromloc)) {
		    wordvalue[wordnumber] = DEADWOOD;
		    wordtype[wordnumber--] = OBJECT;
		    return take(fromloc);
		} else if (object_is_at (DEADNATIVE, fromloc)) {
		    wordvalue[wordnumber] = DEADNATIVE;
		    wordtype[wordnumber--] = OBJECT;
		    return take(fromloc);
		} else {
		    if (object_is_at (DEADGOD, fromloc)) {
			wordvalue[wordnumber] = DEADGOD;
			wordtype[wordnumber--] = OBJECT;
			return take(fromloc);
		    } else {
			wordvalue[wordnumber] = DEADTIME;
			wordtype[wordnumber--] = OBJECT;
			return take(fromloc);
		    }
		}
		break;

	    case AMULET:
		if (object_is_at (AMULET, position)) {
		    puts("The amulet is warm to the touch, and its beauty catches your breath.");
		    puts("A mist falls over your eyes, but then it is gone. Sounds seem clearer");
		    puts("and sharper but far away as if in a dream. The sound of purling water");
		    puts("reaches you from afar. The mist falls again, and your heart leaps in horror.");
		    puts("The gold freezes your hands and fathomless darkness engulfs your soul.");
		}
		wordtype[wordnumber--] = OBJECT;
		return take(fromloc);

	    case MEDALION:
		if (object_is_at (MEDALION, position)) {
		    puts("The medallion is warm, and it rekindles your spirit with the warmth of life.");
		    puts("Your amulet begins to glow as the medallion is brought near to it, and together\nthey radiate.");
		}
		wordtype[wordnumber--] = OBJECT;
		return take(fromloc);

	    case TALISMAN:
		if (object_is_at (TALISMAN, position))
		    puts("The talisman is cold to the touch, and it sends a chill down your spine.");
		wordtype[wordnumber--] = OBJECT;
		return take(fromloc);

	    case NORMGOD:
		if (!object_is_at (BATHGOD, position))
		    puts("She is not here.");
		else if (object_is_at (AMULET, WEARING) || object_is_at (AMULET, INVENTORY)) {
		    puts("She offers a delicate hand, and you help her out of the sparkling springs.");
		    puts("Water droplets like liquid silver bedew her golden skin, but when they part");
		    puts("from her, they fall as teardrops. She wraps a single cloth around her and");
		    puts("ties it at the waist. Around her neck hangs a golden amulet.");
		    puts("She bids you to follow her, and walks away.");
		    ++pleasure;
		    followgod = ourtime;
		    remove_object_from (BATHGOD, position);
		} else
		    puts("She moves away from you.");
		break;

	    default:
		puts("It doesn't seem to work.");
    } else
	puts("You've got to be kidding.");
    return firstnumber;
}

int throw(const char *name)
{
    unsigned int n;
    int deposit = 0;
    int first, value;

    first = wordnumber;
    if (drop(name) != -1) {
	switch (wordvalue[wordnumber]) {

	    case AHEAD:
		deposit = ahead;
		break;

	    case BACK:
		deposit = back;
		break;

	    case LEFT:
		deposit = left;
		break;

	    case RIGHT:
		deposit = right;
		break;

	    case UP:
		if (position == FINAL && !game_state (ROPE_IN_PIT))
		    deposit = 0;
		else
		    deposit = location[position].dir.up;
		break;

	    case DOWN:
		deposit = location[position].dir.down;
		break;
	}
	wordnumber = first + 1;
	while (wordnumber <= wordcount) {
	    value = wordvalue[wordnumber];
	    if (deposit && object_is_at (value, position)) {
		remove_object_from (value, position);
		if (value != GRENADE)
		    create_object_at (value, deposit);
		else {
		    puts("A thundering explosion nearby sends up a cloud of smoke and shrapnel.");
		    for (n = 0; n < NUMOFWORDS; n++)
			for (unsigned o = 0; o < NUMOFOBJECTS; ++o)
			    remove_object_from (n, deposit);
		    create_object_at (CHAR, deposit);
		}
		if (value == ROPE && position == FINAL)
		    set_game_state (ROPE_IN_PIT);
		switch (deposit) {
		    case CAVE_DOOR:
		    case CAVE_ENTRANCE:
			puts("The stone door is unhinged.");
			set_game_state (OPENED_CAVE_DOOR);
			update_relative_directions();
			break;
		    case KITCHEN_DOOR:
			puts("The wooden door is blown open.");
			set_game_state (OPENED_KITCHEN_DOOR);
			update_relative_directions();
			break;
		    case DINING_ROOM_DOOR:
			puts("The door is not damaged.");
		}
	    } else if (value == GRENADE && object_is_at (value, position)) {
		puts("You are blown into shreds when your grenade explodes.");
		die();
	    }
	    if (wordnumber < wordcount - 1 && wordvalue[++wordnumber] == AND)
		wordnumber++;
	    else
		return first;
	}
	return first;
    }
    return first;
}

int drop (const char *name)
{
    int firstnumber = wordnumber;
    wordnumber++;
    while (wordnumber <= wordcount && (wordtype[wordnumber] == OBJECT || wordtype[wordnumber] == NOUNS)) {
	int value = wordvalue[wordnumber];
	if (value == BODY) {   // special case
	    wordtype[wordnumber] = OBJECT;
	    if (object_is_at (MAID, INVENTORY) || object_is_at (MAID, position))
		value = MAID;
	    else if (object_is_at (DEADWOOD, INVENTORY) || object_is_at (DEADWOOD, position))
		value = DEADWOOD;
	    else if (object_is_at (DEADGOD, INVENTORY) || object_is_at (DEADGOD, position))
		value = DEADGOD;
	    else if (object_is_at (DEADTIME, INVENTORY) || object_is_at (DEADTIME, position))
		value = DEADTIME;
	    else if (object_is_at (DEADNATIVE, INVENTORY) || object_is_at (DEADNATIVE, position))
		value = DEADNATIVE;
	}
	if (wordtype[wordnumber] == NOUNS && value == DOOR) {
	    if (*name == 'K')
		puts("You hurt your foot.");
	    else
		puts("You're not holding a door.");
	} else if (c_objinfo[value].shdesc == NULL) {
	    if (*name == 'K')
		puts("That's not for kicking!");
	    else
		puts("You don't have that.");
	} else {
	    printf("%s:\n", c_objinfo[value].shdesc);
	    if (object_is_at (value, INVENTORY)) {
		remove_object_from (value, INVENTORY);
		carrying -= c_objinfo[value].weight;
		encumber -= c_objinfo[value].cumber;
		if (value == BOMB) {
		    puts("The bomb explodes. A blinding white light and immense concussion obliterate us.");
		    die();
		}
		if (value != AMULET && value != MEDALION && value != TALISMAN)
		    create_object_at (value, position);
		else
		    clear_game_state (IS_WIZARD);
		ourtime++;
		if (*name == 'K')
		    puts("Drop kicked.");
		else
		    printf("%s.\n", name);
	    } else {
		if (*name != 'K') {
		    printf("You aren't holding the %s.\n", c_objinfo[value].shdesc);
		    if (object_is_at (value, position)) {
			if (*name == 'T')
			    puts("Kicked instead.");
			else if (*name == 'G')
			    puts("Given anyway.");
		    }
		} else if (object_is_at (value, position))
		    puts("Kicked.");
		else if (object_is_at (value, WEARING))
		    puts("Not while it's being worn.");
		else
		    puts("Not found.");
	    }
	}
	if (wordnumber < wordcount - 1 && wordvalue[++wordnumber] == AND)
	    wordnumber++;
	else
	    return firstnumber;
    }
    puts("Do what?");
    return -1;
}

int takeoff(void)
{
    wordnumber = take (WEARING);
    return drop("Dropped");
}

int puton(void)
{
    wordnumber = take (position);
    return wearit();
}

int eat(void)
{
    int firstnumber, value;

    firstnumber = wordnumber;
    wordnumber++;
    while (wordnumber <= wordcount) {
	value = wordvalue[wordnumber];
	if (wordtype[wordnumber] != OBJECT || c_objinfo[value].shdesc == NULL)
	    value = -2;
	switch (value) {

	    case -2:
		puts("You can't eat that!");
		return firstnumber;

	    case -1:
		puts("Eat what?");
		return firstnumber;

	    default:
		printf("You can't eat %s%s!\n", A_OR_AN_OR_BLANK(value), c_objinfo[value].shdesc);
		return firstnumber;

	    case PAPAYAS:
	    case PINEAPPLE:
	    case KIWI:
	    case COCONUTS:    // eatable things
	    case MANGO:

		printf("%s:\n", c_objinfo[value].shdesc);
		if (object_is_at (value, INVENTORY) && ourtime > ate - CYCLE && object_is_at (KNIFE, INVENTORY)) {
		    remove_object_from (value, INVENTORY);
		    carrying -= c_objinfo[value].weight;
		    encumber -= c_objinfo[value].cumber;
		    ate = max(ourtime, ate) + CYCLE / 3;
		    snooze += CYCLE / 10;
		    ourtime++;
		    puts("Eaten. You can explore a little longer now.");
		} else if (!object_is_at (value, INVENTORY))
		    printf("You aren't holding the %s.\n", c_objinfo[value].shdesc);
		else if (!object_is_at (KNIFE, INVENTORY))
		    puts("You need a knife.");
		else
		    puts("You're stuffed.");
		if (wordnumber < wordcount - 1 && wordvalue[++wordnumber] == AND)
		    wordnumber++;
		else
		    return firstnumber;
	}		       // end switch
    }			       // end while
    return firstnumber;
}

void kiss (void)
{
    while (wordtype[++wordnumber] != NOUNS && wordnumber <= wordcount)
	continue;
    // The goddess must be "taken" first if bathing. This will remove her from the bath and move her into the throne room.
    if (wordtype[wordnumber] == NOUNS && wordvalue[wordnumber] == NORMGOD && object_is_at (BATHGOD, position)) {
	wordvalue[--wordnumber] = TAKE;
	cypher();
	return;
    }
    if (wordtype[wordnumber] == NOUNS) {
	if (object_is_at (wordvalue[wordnumber], position)) {
	    pleasure++;
	    printf("Kissed.\n");
	    switch (wordvalue[wordnumber]) {
		case NORMGOD:
		    switch (godready++) {
			case 0: puts("She squirms and avoids your advances."); break;
			case 1: puts("She is coming around; she didn't fight it as much."); break;
			case 2: puts("She's beginning to like it."); break;
			default: puts("She's gone limp."); break;
		    }
		    break;
		case NATIVE:
		    puts("Her lips are warm and her body robust. She pulls you down to the ground.");
		    break;
		case TIMER:
		    puts("The old man blushes.");
		    break;
		case MAN:
		    puts("The dwarf punches you in the kneecap.");
		    break;
		default:
		    pleasure--;
	    }
	} else
	    puts("I see nothing like that here.");
    } else
	puts("I'd prefer not to.");
}

void love(void)
{
    int n;

    while (wordtype[++wordnumber] != NOUNS && wordnumber <= wordcount)
	continue;
    if (wordtype[wordnumber] == NOUNS) {
	if ((object_is_at (BATHGOD, position) || object_is_at (NORMGOD, position)) && wordvalue[wordnumber] == NORMGOD) {
	    if (loved) {
		printf("Loved.\n");
		return;
	    }
	    if (godready >= 2) {
		puts("She cuddles up to you, and her mouth starts to work:\n'That was my sister's amulet. The lovely goddess, Purl, was she. The Empire\ncaptured her just after the Darkness came. My other sister, Vert, was killed\nby the Dark Lord himself. He took her amulet and warped its power.\nYour quest was foretold by my father before he died, but to get the Dark Lord's\namulet you must use cunning and skill. I will leave you my amulet,");
		puts("which you may use as you wish. As for me, I am the last goddess of the\nwaters. My father was the Island King, and the rule is rightfully mine.'\n\nShe pulls the throne out into a large bed.");
		power++;
		pleasure += 15;
		ego++;
		if (card(injuries, NUMOFINJURIES)) {
		    puts("Her kisses revive you; your wounds are healed.\n");
		    for (n = 0; n < NUMOFINJURIES; n++)
			injuries[n] = 0;
		    WEIGHT = MAXWEIGHT;
		    CUMBER = MAXCUMBER;
		}
		printf("Goddess:\n");
		if (!loved)
		    create_object_at (MEDALION, position);
		loved = 1;
		ourtime += 10;
		printf("Loved.\n");
		zzz();
		return;
	    } else {
		puts("You wish!");
		return;
	    }
	}
	if (object_is_at (wordvalue[wordnumber], position)) {
	    if (wordvalue[wordnumber] == NATIVE) {
		puts("The girl peels off her sarong and indulges you.");
		power++;
		pleasure += 5;
		printf("Girl:\n");
		ourtime += 10;
		printf("Loved.\n");
		zzz();
	    }
	    if (wordvalue[wordnumber] == MAN || wordvalue[wordnumber] == BODY || wordvalue[wordnumber] == ELF || wordvalue[wordnumber] == TIMER)
		puts("Kinky!");
	    else
		puts("It doesn't seem to work.");
	} else
	    puts("Where's your lover?");
    } else
	puts("It doesn't seem to work.");
}

int zzz(void)
{
    int oldtime;
    int n;

    oldtime = ourtime;
    if ((snooze - ourtime) < (0.75 * CYCLE)) {
	ourtime += 0.75 * CYCLE - (snooze - ourtime);
	printf("<zzz>");
	for (n = 0; n < ourtime - oldtime; n++)
	    printf(".");
	printf("\n");
	snooze += 3 * (ourtime - oldtime);
	if (game_state (LAUNCHED)) {
	    fuel -= (ourtime - oldtime);
	    if (location[position].dir.down) {
		position = location[position].dir.down;
		crash();
	    } else
		clear_game_state (LAUNCHED);
	}
	if (OUTSIDE && nrand(100) < 50) {
	    puts("You are awakened abruptly by the sound of someone nearby.");
	    switch (nrand(4)) {
		case 0:
		    if (count_objects_at (INVENTORY)) {
			n = nrand(NUMOFOBJECTS);
			while (!object_is_at (n, INVENTORY))
			    n = nrand(NUMOFOBJECTS);
			remove_object_from (n, INVENTORY);
			if (n != AMULET && n != MEDALION && n != TALISMAN)
			    create_object_at (n, position);
			carrying -= c_objinfo[n].weight;
			encumber -= c_objinfo[n].cumber;
		    }
		    puts("A fiendish little Elf is stealing your treasures!");
		    fight(ELF, 10);
		    break;
		case 1:
		    create_object_at (DEADWOOD, position);
		    break;
		case 2:
		    create_object_at (HALBERD, position);
		    break;
		default:
		    break;
	    }
	}
    } else
	return 0;
    return 1;
}

void chime(void)
{
    if ((ourtime / CYCLE + 1) % 2 && OUTSIDE) {
	switch ((ourtime % CYCLE) / (CYCLE / 7)) {
	    case 0: puts("It is just after sunrise."); break;
	    case 1: puts("It is early morning."); break;
	    case 2: puts("It is late morning."); break;
	    case 3: puts("It is near noon."); break;
	    case 4: puts("It is early afternoon."); break;
	    case 5: puts("It is late afternoon."); break;
	    case 6: puts("It is near sunset."); break;
	}
    } else if (OUTSIDE) {
	switch ((ourtime % CYCLE) / (CYCLE / 7)) {
	    case 0: puts("It is just after sunset."); break;
	    case 1: puts("It is early evening."); break;
	    case 2: puts("The evening is getting old."); break;
	    case 3: puts("It is near midnight."); break;
	    case 4: puts("These are the wee hours of the morning."); break;
	    case 5: puts("The night is waning."); break;
	    case 6: puts("It is almost morning."); break;
	}
    } else
	puts("I can't tell the time in here.");
}

int give(void)
{
    int obj = -1, result = -1, person = 0, firstnumber, last1, last2;

    last1 = last2 = 0;
    firstnumber = wordnumber;
    while (wordtype[++wordnumber] != OBJECT && wordvalue[wordnumber] != AMULET && wordvalue[wordnumber] != MEDALION && wordvalue[wordnumber] != TALISMAN && wordnumber <= wordcount)
	continue;
    if (wordnumber <= wordcount) {
	obj = wordvalue[wordnumber];
	if (obj == EVERYTHING)
	    wordtype[wordnumber] = -1;
	last1 = wordnumber;
    }
    wordnumber = firstnumber;
    while ((wordtype[++wordnumber] != NOUNS || wordvalue[wordnumber] == obj) && wordnumber <= wordcount);
    if (wordtype[wordnumber] == NOUNS) {
	person = wordvalue[wordnumber];
	last2 = wordnumber;
    }
    // Setting wordnumber to last1 - 1 looks wrong if last1 is 0, e.g.,
    // plain `give'. However, detecting this case is liable to detect
    // `give foo' as well, which would give a confusing error. We
    // need to make sure the -1 value can cause no problems if it arises.
    // If in the below we get to the drop("Given") then drop will look
    // at word 0 for an object to give, and fail, which is OK; then
    // result will be -1 and we get to the end, where wordnumber gets
    // set to something more sensible. If we get to "I don't think
    // that is possible" then again wordnumber is set to something
    // sensible. The wordnumber we leave with still isn't right if
    // you include words the game doesn't know in your command, but
    // that's no worse than what other commands than give do in
    // the same place.
    wordnumber = last1 - 1;
    if (person && object_is_at (person, position)) {
	if (person == NORMGOD && godready < 2 && !(obj == RING || obj == BRACELET))
	    puts("The goddess won't look at you.");
	else
	    result = drop("Given");
    } else {
	puts("I don't think that is possible.");
	wordnumber = max(last1, last2) + 1;
	return 0;
    }
    if (result != -1 && (object_is_at (obj, position) || obj == AMULET || obj == MEDALION || obj == TALISMAN)) {
	remove_object_from (obj, position);
	ourtime++;
	ego++;
	switch (person) {
	    case NATIVE:
		puts("She accepts it shyly.");
		ego += 2;
		break;
	    case NORMGOD:
		if (obj == RING || obj == BRACELET) {
		    puts("She takes the charm and puts it on. A little kiss on the cheek is");
		    puts("your reward.");
		    ego += 5;
		    godready += 3;
		}
		if (obj == AMULET || obj == MEDALION || obj == TALISMAN) {
		    win++;
		    ego += 5;
		    power -= 5;
		    if (win >= 3) {
			puts("The powers of the earth are now legitimate. You have destroyed the Darkness");
			puts("and restored the goddess to her throne. The entire island celebrates with");
			puts("dancing and spring feasts. As a measure of her gratitude, the goddess weds you");
			puts("in the late summer and crowns you Prince Liverwort, Lord of Fungus.");
			remove_object_from (MEDALION, position);
			wintime = ourtime;
			live();
		    }
		}
		break;
	    case TIMER:
		if (obj == COINS) {
		    printf("He fingers the coins for a moment and then looks up agape. `Kind you are and\n"
			    "I mean to repay you as best I can.'  Grabbing a pencil and cocktail napkin...\n\n"
			    "+-----------------------------------------------------------------------------+\n"
			    "|				   xxxxxxxx\\				      |\n"
			    "|				       xxxxx\\	CLIFFS			      |\n"
			    "|		FOREST			  xxx\\				      |\n"
			    "|                          \\\\           x\\               OCEAN         |\n"
			    "|				||	       x\\			      |\n"
			    "|				||  ROAD	x\\			      |\n"
			    "|				||		x\\			      |\n"
			    "|		SECRET		||	  .........			      |\n"
			    "|		 - + -		||	   ........			      |\n"
			    "|		ENTRANCE	||		...      BEACH		      |\n"
			    "|				||		...		  E	      |\n"
			    "|				||		...		  |	      |\n"
			    "|				//		...	    N <-- + --- S     |\n"
			    "|		PALM GROVE     //		...		  |	      |\n"
			    "|			      //		...		  W	      |\n"
			    "+-----------------------------------------------------------------------------+\n"
			    "\n`This map shows a secret entrance to the catacombs.\n"
			    "You will know when you arrive because I left an old pair of shoes there.'\n");
		}
		break;
	}
    }
    wordnumber = max(last1, last2) + 1;
    return firstnumber;
}

int launch(void)
{
    if (object_is_at (STARFIGHTER, position) && !game_state (CANTLAUNCH)) {
	if (fuel > 4) {
	    remove_object_from (STARFIGHTER, position);
	    position = location[position].dir.up;
	    set_game_state (LAUNCHED);
	    ++ourtime;
	    fuel -= 4;
	    puts("You climb into the starfighter and prepare for launch.");
	    puts("With a touch of your thumb the turbo engines ignite, thrusting you back into\nyour seat.");
	    return 1;
	} else
	    puts("Not enough fuel to launch.");
    } else
	puts("Can't launch.");
    return 0;
}

int land(void)
{
    if (game_state (LAUNCHED) && object_is_at (LAND, position) && location[position].dir.down) {
	clear_game_state (LAUNCHED);
	position = location[position].dir.down;
	create_object_at (STARFIGHTER, position);
	fuel -= 2;
	++ourtime;
	puts("You are down.");
	return 1;
    } else
	puts("You can't land here.");
    return 0;
}

_Noreturn void die(void)
{			       // endgame
    printf ("bye.\nYour rating was %s.\n", rate());
    post (' ');
    exit (EXIT_SUCCESS);
}

_Noreturn void live (void)
{
    puts ("\nYou win!");
    post ('!');
    exit (EXIT_SUCCESS);
}

static FILE* score_fp = NULL;

void open_score_file (void)
{
    score_fp = fopen (_PATH_SCORE, "a");
    if (score_fp == NULL && getenv("BATTLESTAR_QUIET") == NULL)
	perror ("open " _PATH_SCORE);
    if (score_fp != NULL && fileno(score_fp) < 3)
	exit(1);
}

void post (char ch)
{
    time_t tv;
    char *date;
    sigset_t sigset, osigset;

    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigprocmask(SIG_BLOCK, &sigset, &osigset);
    tv = time(NULL);
    date = ctime(&tv);
    date[24] = '\0';
    if (score_fp != NULL) {
	fprintf(score_fp, "%s  %8s  %c%20s", date, username, ch, rate());
	if (game_state (IS_WIZARD))
	    fprintf(score_fp, "   WIZARD!\n");
	else
	    fprintf(score_fp, "\n");
    }
    sigprocmask(SIG_SETMASK, &osigset, (sigset_t *) 0);
}

const char* rate (void)
{
    int score = max(max(pleasure, power), ego);
    if (score == pleasure) {
	if (score < 5)		return "novice";
	else if (score < 20)	return "junior voyeur";
	else if (score < 35)	return "Don Juan";
	else			return "Marquis De Sade";
    } else if (score == power) {
	if (score < 5)		return "serf";
	else if (score < 8)	return "Samurai";
	else if (score < 13)	return "Klingon";
	else if (score < 22)	return "Darth Vader";
	else			return "Sauron the Great";
    } else {
	if (score < 5)		return "Polyanna";
	else if (score < 10)	return "philanthropist";
	else if (score < 20)	return "Tattoo";
	else			return "Mr. Roarke";
    }
}

int drive (void)
{
    if (object_is_at (CAR, position)) {
	puts("You hop in the car and turn the key. There is a perceptible grating noise,");
	puts("and an explosion knocks you unconscious...");
	remove_object_from (CAR, position);
	create_object_at (CRASH, position);
	injuries[5] = injuries[6] = injuries[7] = injuries[8] = true;
	ourtime += 15;
	zzz();
	return 0;
    } else
	puts("There is nothing to drive here.");
    return -1;
}

int ride (void)
{
    if (object_is_at (HORSE, position)) {
	puts("You climb onto the stallion and kick it in the guts. The stupid steed launches");
	puts("forward through bush and fern. You are thrown and the horse gallops off.");
	remove_object_from (HORSE, position);
	while (!(position = nrand(NUMOFROOMS+1)) || !OUTSIDE || !beenthere[position] || location[position].dir.flyhere)
	    continue;
	create_object_at (HORSE, position);
	if (location[position].dir.north)
	    position = location[position].dir.north;
	else if (location[position].dir.south)
	    position = location[position].dir.south;
	else if (location[position].dir.east)
	    position = location[position].dir.east;
	else
	    position = location[position].dir.west;
	return 0;
    } else
	puts("There is no horse here.");
    return -1;
}

// synonyms = {strike, smoke} for matches, cigars
void light(void)
{
    if (object_is_at (MATCHES, INVENTORY) && matchcount) {
	puts("Your match splutters to life.");
	ourtime++;
	set_game_state (MATCH_LIGHT);
	matchcount--;
	if (position == BUNGALOW_PORCH) {
	    puts("The whole bungalow explodes with an intense blast.");
	    die();
	}
    } else
	puts("You're out of matches.");
}

// synonyms = {open, unlock}
void dooropen(void)
{
    ++wordnumber;
    if (wordnumber <= wordcount && wordtype[wordnumber] == NOUNS && wordvalue[wordnumber] == DOOR) {
	switch (position) {
	    case CAVE_DOOR:
	    case CAVE_ENTRANCE:
		if (game_state (OPENED_CAVE_DOOR))
		    puts("The door is already open.");
		else
		    puts("The door does not budge.");
		break;
	    case KITCHEN_DOOR:
		if (game_state (OPENED_KITCHEN_DOOR))
		    puts("The door is gone.");
		else
		    puts("The door is locked tight.");
		break;
	    case DINING_ROOM_DOOR:
		puts("That's one immovable door.");
		break;
	    case LASER_ROOM:
		puts("The door is already ajar.");
		break;
	    default:
		puts("What door?");
	}
    } else
	puts("That doesn't open.");
}

int fight(int enemy, int strength)
{
    int lifeline = 0;
    int hurt;
    char auxbuf[LINELENGTH];
    char *next;
    int i;
    int exhaustion;

    exhaustion = 0;
  fighton:
    ourtime++;
    snooze -= 5;
    if (snooze > ourtime)
	exhaustion = CYCLE / (snooze - ourtime);
    else {
	puts("You collapse exhausted, and he pulverizes your skull.");
	die();
    }
    if (snooze - ourtime < 20)
	puts("You look tired! I hope you're able to fight.");
    next = getcom(auxbuf, LINELENGTH, "<fight!>-: ", 0);
    for (i = 0; next && i < 10; i++)
	next = getword(next, words[i], -1);
    parse();
    switch (wordvalue[wordnumber]) {

	case KILL:
	case SMITE:
	    if (object_is_at (TWO_HANDED, INVENTORY))
		hurt = nrand(70) - 2 * card(injuries, NUMOFINJURIES) - count_objects_at(WEARING) - exhaustion;
	    else if (object_is_at (SWORD, INVENTORY) || object_is_at (BROAD, INVENTORY))
		hurt = nrand(50) % (WEIGHT - carrying) - card(injuries, NUMOFINJURIES) - encumber - exhaustion;
	    else if (object_is_at (KNIFE, INVENTORY) || object_is_at (MALLET, INVENTORY) || object_is_at (CHAIN, INVENTORY) || object_is_at (MACE, INVENTORY) || object_is_at (HALBERD, INVENTORY))
		hurt = nrand(15) - card(injuries, NUMOFINJURIES) - exhaustion;
	    else
		hurt = nrand(7) - encumber;
	    if (hurt < 5) {
		switch (nrand(3)) {
		    case 0: puts("You swung wide and missed."); break;
		    case 1: puts("He checked your blow. CLASH! CLANG!"); break;
		    case 2: puts("His filthy tunic hangs by one less thread."); break;
		}
	    } else if (hurt < 10) {
		switch (nrand(3)) {
		    case 0: puts("He's bleeding."); break;
		    case 1: puts("A trickle of blood runs down his face."); break;
		    case 2: puts("A huge purple bruise is forming on the side of his face."); break;
		}
		lifeline++;
	    } else if (hurt < 20) {
		switch (nrand(3)) {
		    case 0: puts("He staggers back quavering."); break;
		    case 1: puts("He jumps back with his hand over the wound."); break;
		    case 2: puts("His shirt falls open with a swath across the chest."); break;
		}
		lifeline += 5;
	    } else if (hurt < 30) {
		switch (nrand(3)) {
		    case 0: printf("A bloody gash opens up on his %s side.\n", nrand(2) ? "left" : "right"); break;
		    case 1: puts("The steel bites home and scrapes along his ribs."); break;
		    case 2: puts("You pierce him, and his breath hisses through clenched teeth."); break;
		}
		lifeline += 10;
	    } else if (hurt < 40) {
		switch (nrand(3)) {
		    case 0:
			puts("You smite him to the ground.");
			if (strength - lifeline > 20)
			    puts("But in a flurry of steel he regains his feet!");
			break;
		    case 1:
			puts("The force of your blow sends him to his knees.");
			puts("His arm swings lifeless at his side.");
			break;
		    case 2:
			puts("Clutching his blood drenched shirt, he collapses stunned.");
			break;
		}
		lifeline += 20;
	    } else {
		switch (nrand(3)) {
		    case 0:
			puts("His ribs crack under your powerful swing, flooding his lungs with blood.");
			break;
		    case 1:
			puts("You shatter his upheld arm in a spray of blood.  The blade continues deep");
			puts("into his back, severing the spinal cord.");
			lifeline += 25;
			break;
		    case 2:
			puts("With a mighty lunge the steel slides in, and gasping, he falls to the ground.");
			lifeline += 25;
			break;
		}
		lifeline += 30;
	    }
	    break;

	case BACK:
	    if (enemy == DARK && lifeline > strength * 0.33) {
		puts("He throws you back against the rock and pummels your face.");
		if (object_is_at (AMULET, INVENTORY) || object_is_at (AMULET, WEARING)) {
		    printf("Lifting the amulet from you, ");
		    if (object_is_at (MEDALION, INVENTORY) || object_is_at (MEDALION, WEARING)) {
			puts("his power grows and the walls of\nthe earth tremble.");
			puts("When he touches the medallion, your chest explodes and the foundations of the\nearth collapse.");
			puts("The planet is consumed by darkness.");
			die();
		    }
		    if (object_is_at (AMULET, INVENTORY)) {
			remove_object_from (AMULET, INVENTORY);
			carrying -= c_objinfo[AMULET].weight;
			encumber -= c_objinfo[AMULET].cumber;
		    } else
			remove_object_from (AMULET, WEARING);
		    puts("he flees down the dark caverns.");
		    remove_object_from (DARK, position);
		    injuries[SKULL] = 1;
		    followfight = ourtime;
		    return 0;
		} else {
		    puts("I'm afraid you have been killed.");
		    die();
		}
	    } else {
		puts("You escape stunned and disoriented from the fight.");
		puts("A victorious bellow echoes from the battlescene.");
		if (back && position != back)
		    moveplayer(back, BACK);
		else if (ahead && position != ahead)
		    moveplayer(ahead, AHEAD);
		else if (left && position != left)
		    moveplayer(left, LEFT);
		else if (right && position != right)
		    moveplayer(right, RIGHT);
		else
		    moveplayer(location[position].dir.down, AHEAD);
		return 0;
	    }

	case SHOOT:
	    if (object_is_at (LASER, INVENTORY)) {
		if (strength - lifeline <= 50) {
		    printf("The %s took a direct hit!\n", c_objinfo[enemy].shdesc);
		    lifeline += 50;
		} else {
		    puts("With his bare hand he deflects the laser blast and whips the pistol from you!");
		    remove_object_from (LASER, INVENTORY);
		    create_object_at (LASER, position);
		    carrying -= c_objinfo[LASER].weight;
		    encumber -= c_objinfo[LASER].cumber;
		}
	    } else
		puts("Unfortunately, you don't have a blaster handy.");
	    break;

	case DROP:
	case DRAW:
	    cypher();
	    ourtime--;
	    break;

	default:
	    puts("You don't have a chance; he is too quick.");
	    break;

    }
    if (lifeline >= strength) {
	printf("You have killed the %s.\n", c_objinfo[enemy].shdesc);
	if (enemy == ELF || enemy == DARK)
	    puts("A watery black smoke consumes his body and then vanishes with a peal of thunder!");
	remove_object_from (enemy, position);
	power += 2;
	set_game_state (JINXED);
	return 0;
    }
    puts("He attacks...");
    // Some embellishments.
    hurt = nrand(NUMOFINJURIES) - (object_is_at (SHIELD, INVENTORY) != 0) - (object_is_at (MAIL, WEARING) != 0) - (object_is_at (HELM, WEARING) != 0);
    hurt += (object_is_at (AMULET, WEARING) != 0) + (object_is_at (MEDALION, WEARING) != 0) + (object_is_at (TALISMAN, WEARING) != 0);
    hurt = hurt < 0 ? 0 : hurt;
    hurt = hurt >= NUMOFINJURIES ? NUMOFINJURIES - 1 : hurt;
    if (!injuries[hurt]) {
	injuries[hurt] = 1;
	printf("I'm afraid you have suffered %s.\n", ouch[hurt]);
    } else
	puts("You emerge unscathed.");
    if (injuries[SKULL] && injuries[INCISE] && injuries[NECK]) {
	puts("I'm afraid you have suffered fatal injuries.");
	die();
    }
    goto fighton;
}

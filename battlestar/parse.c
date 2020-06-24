// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

//{{{ wlist ------------------------------------------------------------

static const struct Word {
    const char	w [13];
    uint8_t	type;
    uint16_t	value;
} wlist[] = {
    {",",		CONJ,		AND		},
    {"a",		VERB,		AHEAD		},
    {"ahead",		VERB,		AHEAD		},
    {"all",		OBJECT,		EVERYTHING	},
    {"amulet",		NOUNS,		AMULET		},
    {"ancient",		ADJS,		0		},
    {"and",		CONJ,		AND		},
    {"attack",		VERB,		SMITE		},
    {"b",		VERB,		BACK		},
    {"back",		VERB,		BACK		},
    {"bar",		OBJECT,		BAR		},
    {"blast",		VERB,		SHOOT		},
    {"blaster",		OBJECT,		LASER		},
    {"block",		OBJECT,		BLOCK		},
    {"blue",		ADJS,		0		},
    {"body",		NOUNS,		MAID		},
    {"bomb",		OBJECT,		BOMB		},
    {"book",		OBJECT,		MATCHES		},
    {"bracelet",	OBJECT,		BRACELET	},
    {"brief",		VERB,		BRIEF		},
    {"broadsword",	OBJECT,		BROAD		},
    {"buckle",		VERB,		PUT		},
    {"bury",		VERB,		BURY		},
    {"car",		OBJECT,		CAR		},
    {"carry",		VERB,		DRAW		},
    {"chain",		OBJECT,		CHAIN		},
    {"chase",		VERB,		FOLLOW		},
    {"cleaver",		OBJECT,		CLEAVER		},
    {"climb",		VERB,		AUXVERB		},
    {"coat",		OBJECT,		MAIL		},
    {"coconut",		OBJECT,		COCONUTS	},
    {"coconuts",	OBJECT,		COCONUTS	},
    {"coins",		OBJECT,		COINS		},
    {"compass",		OBJECT,		COMPASS		},
    {"cut",		VERB,		SMITE		},
    {"d",		VERB,		DOWN		},
    {"dented",		ADJS,		0		},
    {"diamond",		OBJECT,		BLOCK		},
    {"dig",		VERB,		DIG		},
    {"door",		NOUNS,		DOOR		},
    {"down",		VERB,		DOWN		},
    {"draw",		VERB,		DRAW		},
    {"drink",		VERB,		DRINK		},
    {"drive",		VERB,		DRIVE		},
    {"drop",		VERB,		DROP		},
    {"eat",		VERB,		EAT		},
    {"elf",		OBJECT,		ELF		},
    {"everything",	OBJECT,		EVERYTHING	},
    {"fight",		VERB,		KILL		},
    {"fine",		OBJECT,		SWORD		},
    {"follow",		VERB,		FOLLOW		},
    {"fuck",		VERB,		LOVE		},
    {"get",		VERB,		TAKE		},
    {"girl",		NOUNS,		NATIVE_GIRL	},
    {"give",		VERB,		GIVE		},
    {"goddess",		NOUNS,		GODDESS		},
    {"gold",		ADJS,		0		},
    {"golden",		ADJS,		0		},
    {"grenade",		OBJECT,		GRENADE		},
    {"gun",		OBJECT,		LASER		},
    {"hack",		VERB,		SMITE		},
    {"halberd",		OBJECT,		HALBERD		},
    {"helmet",		OBJECT,		HELM		},
    {"horse",		OBJECT,		HORSE		},
    {"i",		VERB,		INVEN		},
    {"inven",		VERB,		INVEN		},
    {"jewels",		OBJECT,		POT		},
    {"jump",		VERB,		JUMP		},
    {"kerosene",	ADJS,		0		},
    {"kick",		VERB,		KICK		},
    {"kill",		VERB,		KILL		},
    {"kingly",		ADJS,		0		},
    {"kiss",		VERB,		KISS		},
    {"kiwi",		OBJECT,		KIWI		},
    {"knife",		OBJECT,		KNIFE		},
    {"l",		VERB,		LEFT		},
    {"lamp",		OBJECT,		LAMPON		},
    {"land",		VERB,		LANDIT		},
    {"lantern",		OBJECT,		LAMPON		},
    {"laser",		OBJECT,		LASER		},
    {"launch",		VERB,		LAUNCH		},
    {"left",		VERB,		LEFT		},
    {"levis",		OBJECT,		LEVIS		},
    {"light",		VERB,		LIGHT		},
    {"lo",		VERB,		LOOK		},
    {"look",		VERB,		LOOK		},
    {"love",		VERB,		LOVE		},
    {"mace",		OBJECT,		MACE		},
    {"maid",		OBJECT,		MAID		},
    {"maid's",		OBJECT,		MAID		},
    {"mail",		OBJECT,		MAIL		},
    {"make",		VERB,		AUXVERB		},
    {"mallet",		OBJECT,		MALLET		},
    {"man",		NOUNS,		MAN		},
    {"mango",		OBJECT,		MANGO		},
    {"match",		OBJECT,		MATCHES		},
    {"matches",		OBJECT,		MATCHES		},
    {"medallion",	NOUNS,		MEDALION	},
    {"mount",		VERB,		RIDE		},
    {"move",		VERB,		AUXVERB		},
    {"native",		NOUNS,		NATIVE_GIRL	},
    {"off",		PREPS,		OFF		},
    {"old",		ADJS,		0		},
    {"on",		PREPS,		ON		},
    {"open",		VERB,		OPEN		},
    {"ostrich",		ADJS,		0		},
    {"pajamas",		OBJECT,		PAJAMAS		},
    {"pants",		OBJECT,		LEVIS		},
    {"papaya",		OBJECT,		PAPAYAS		},
    {"papayas",		OBJECT,		PAPAYAS		},
    {"pineapple",	OBJECT,		PINEAPPLE	},
    {"pistol",		OBJECT,		LASER		},
    {"plumed",		ADJS,		0		},
    {"points",		VERB,		SCORE		},
    {"pot",		OBJECT,		POT		},
    {"potion",		OBJECT,		POTION		},
    {"pull",		VERB,		DRAW		},
    {"purple",		ADJS,		0		},
    {"put",		VERB,		PUT		},
    {"q",		VERB,		QUIT		},
    {"quit",		VERB,		QUIT		},
    {"r",		VERB,		RIGHT		},
    {"ride",		VERB,		RIDE		},
    {"right",		VERB,		RIGHT		},
    {"ring",		OBJECT,		RING		},
    {"robe",		OBJECT,		ROBE		},
    {"rope",		OBJECT,		ROPE		},
    {"rusty",		ADJS,		0		},
    {"save",		VERB,		SAVE		},
    {"scabbard",	OBJECT,		SWORD		},
    {"score",		VERB,		SCORE		},
    {"sheathe",		VERB,		WEARIT		},
    {"shield",		OBJECT,		SHIELD		},
    {"shoes",		OBJECT,		SHOES		},
    {"shoot",		VERB,		SHOOT		},
    {"shovel",		OBJECT,		SHOVEL		},
    {"sleep",		VERB,		SLEEP		},
    {"slice",		VERB,		SMITE		},
    {"smite",		VERB,		SMITE		},
    {"stab",		VERB,		SMITE		},
    {"stallion",	OBJECT,		HORSE		},
    {"starfighter",	OBJECT,		STARFIGHTER	},
    {"start",		VERB,		DRIVE		},
    {"strap",		VERB,		PUT		},
    {"strike",		VERB,		LIGHT		},
    {"su",		VERB,		SU		},
    {"swim",		VERB,		SWIM		},
    {"swing",		VERB,		SMITE		},
    {"sword",		NOUNS,		SWORD		},
    {"take",		VERB,		TAKE		},
    {"talisman",	NOUNS,		TALISMAN	},
    {"the",		ADJS,		0		},
    {"throw",		VERB,		THROW_OBJECT	},
    {"tie",		VERB,		PUT		},
    {"time",		VERB,		TIME		},
    {"timer",		NOUNS,		TIMER		},
    {"to",		ADJS,		0		},
    {"two-handed",	OBJECT,		TWO_HANDED	},
    {"u",		VERB,		UP		},
    {"unlock",		VERB,		OPEN		},
    {"up",		VERB,		UP		},
    {"use",		VERB,		USE		},
    {"verbose",		VERB,		VERBOSE		},
    {"volare",		OBJECT,		CAR		},
    {"warhead",		OBJECT,		BOMB		},
    {"wear",		VERB,		WEARIT		},
    {"woodsman",	OBJECT,		DEADWOOD	},
    {"woodsman's",	OBJECT,		DEADWOOD	}
};

//}}}-------------------------------------------------------------------

char* getcom (char* buf, int size, const char* prompt, const char* error)
{
    for (;;) {
	fputs (prompt, stdout);
	if (fgets (buf, size, stdin) == 0) {
	    if (feof(stdin))
		die();
	    clearerr(stdin);
	    continue;
	}
	while (isspace (*buf))
	    ++buf;
	if (*buf)
	    break;
	if (error)
	    puts(error);
    }
    // If we didn't get to the end of the line, don't read it in next time.
    if (buf[strlen(buf) - 1] != '\n') {
	int i;
	while ((i = getchar()) != '\n' && i != EOF)
	    continue;
    }
    return buf;
}

// shifts to UPPERCASE if flag > 0, lowercase if flag < 0,
// and leaves it unchanged if flag = 0
char* getword (char* buf1, char* buf2)
{
    unsigned cnt = 1;
    while (isspace (*buf1))
	++buf1;
    if (*buf1 != ',') {
	if (!*buf1) {
	    *buf2 = 0;
	    return 0;
	}
	for (; cnt < WORDLEN && *buf1 && !isspace (*buf1) && *buf1 != ','; ++cnt)
	    *buf2++ = tolower (*buf1++);
	if (cnt == WORDLEN)
	    while (*buf1 && !isspace (*buf1))
		++buf1;
    } else
	*buf2++ = *buf1++;
    *buf2 = '\0';
    while (isspace (*buf1))
	++buf1;
    return *buf1 ? buf1 : NULL;
}

static const struct Word* lookup (const char* s)
{
    for (unsigned f = 0, l = ArraySize (wlist); f < l;) {
	unsigned m = (f+l)/2u;
	int c = strcmp (wlist[m].w, s);
	if (c == 0)
	    return &wlist[m];
	else if (c < 0)
	    f = m+1;
	else
	    l = m;
    }
    return NULL;
}

void parse (void)
{
    wordnumber = 0;	       // for cypher
    for (unsigned n = 0; n <= wordcount; ++n) {
	const struct Word* wp = lookup (words[n]);
	if (!wp) {
	    wordvalue[n] = LOOK;
	    wordtype[n] = VERB;
	} else {
	    wordvalue[n] = wp->value;
	    wordtype[n] = wp->type;
	}
    }
    // We never use adjectives for anything, so yank them all.
    for (unsigned n = 1; n < wordcount; ++n) {
	if (wordtype[n] == ADJS) {
	    for (unsigned i = n+1; i < wordcount; ++i) {
		wordtype[i-1] = wordtype[i];
		wordvalue[i-1] = wordvalue[i];
		strcpy (words[i-1], words[i]);
	    }
	    --wordcount;
	}
    }
    // Don't let a comma mean AND if followed by a verb.
    for (unsigned n = 0; n < wordcount; ++n) {
	if (wordvalue[n] == AND && words[n][0] == ',' && wordtype[n+1] == VERB) {
	    wordvalue[n] = -1;
	    wordtype[n] = -1;
	}
    }
    // Trim "AND AND" which can happen naturally at the end of a
    // comma-delimited list.
    for (unsigned n = 1; n < wordcount; ++n) {
	if (wordvalue[n - 1] == AND && wordvalue[n] == AND) {
	    for (unsigned i = n+1; i < wordcount; ++i) {
		wordtype[i - 1] = wordtype[i];
		wordvalue[i - 1] = wordvalue[i];
		strcpy(words[i - 1], words[i]);
	    }
	    --wordcount;
	}
    }

    // If there is a sequence (NOUN | OBJECT) AND EVERYTHING
    // then move all the EVERYTHINGs to the beginning, since that's where
    // they're expected.  We can't get rid of the NOUNs and OBJECTs in
    // case they aren't in EVERYTHING (i.e. not here or nonexistent).
    bool flag = true;
    while (flag) {
	flag = false;
	for (unsigned n = 1; n < wordcount; ++n) {
	    if ((wordtype[n-1] == NOUNS || wordtype[n-1] == OBJECT) && wordvalue[n] == AND && wordvalue[n+1] == EVERYTHING) {
		char tmpword[WORDLEN];
		wordvalue[n+1] = wordvalue[n-1];
		wordvalue[n-1] = EVERYTHING;
		wordtype[n+1] = wordtype[n-1];
		wordtype[n-1] = OBJECT;
		strcpy(tmpword, words[n-1]);
		memcpy(words[n-1], words[n+1], sizeof(words[n-1]));
		strcpy(words[n+1], tmpword);
		flag = true;
	    }
	}
	// And trim EVERYTHING AND EVERYTHING.
	for (unsigned n = 1; n < wordcount; ++n) {
	    if (wordvalue[n-1] == EVERYTHING && wordvalue[n] == AND && wordvalue[n+1] == EVERYTHING) {
		int i;
		for (i = n + 1; i < wordcount; ++i) {
		    wordtype[i-1] = wordtype[i+1];
		    wordvalue[i-1] = wordvalue[i+1];
		    strcpy(words[i-1], words[i+1]);
		}
		wordcount -= 2;
		flag = true;
	    }
	}
    }
}

int cypher (void)
{
    int n;
    int junk;
    int lflag = -1;
    char buffer[10];
    char *filename = NULL, *rfilename;
    size_t filename_len = 0;

    while (wordnumber <= wordcount) {
	if (wordtype[wordnumber] != VERB && !(wordtype[wordnumber] == OBJECT && wordvalue[wordnumber] == KNIFE)) {
	    printf("%s: How's that?\n", (wordnumber == wordcount) ? words[0] : words[wordnumber]);
	    return -1;
	}

	switch (wordvalue[wordnumber]) {
	    case AUXVERB:
		// Take the following word as the verb (e.g. "make love", "climb up").
		++wordnumber;
		continue;

	    case UP:
		if (position == FINAL) {
		    if (game_state (IS_WIZARD))
			puts ("Zap! A gust of wind lifts you up.");
		    else if (!game_state (ROPE_IN_PIT)) {
			puts ("There is no way up.");
			return -1;
		    }
		}
		if (!moveplayer (location[position].dir.up, AHEAD))
		    return -1;
		lflag = 0;
		break;

	    case DOWN:
		if (!moveplayer(location[position].dir.down, AHEAD))
		    return -1;
		lflag = 0;
		break;

	    case LEFT:
		if (!moveplayer(left, LEFT))
		    return -1;
		lflag = 0;
		break;

	    case RIGHT:
		if (!moveplayer(right, RIGHT))
		    return -1;
		lflag = 0;
		break;

	    case AHEAD:
		if (!moveplayer(ahead, AHEAD))
		    return -1;
		lflag = 0;
		break;

	    case BACK:
		if (!moveplayer(back, BACK))
		    return -1;
		lflag = 0;
		break;

	    case SHOOT:
		if (wordnumber < wordcount && wordvalue[wordnumber + 1] == EVERYTHING) {
		    int things;
		    things = 0;
		    for (n = 0; n < NUMOFOBJECTS; ++n)
			if (object_is_at (n, position) && c_objinfo[n].shdesc) {
			    ++things;
			    wordvalue[wordnumber + 1] = n;
			    wordnumber = shoot();
			}
		    if (!things)
			puts("Nothing to shoot at!");
		    ++wordnumber;
		    ++wordnumber;
		} else
		    shoot();
		break;

	    case TAKE:
		if (wordnumber < wordcount && wordvalue[wordnumber + 1] == EVERYTHING) {
		    int things;
		    things = 0;
		    for (n = 0; n < NUMOFOBJECTS; ++n)
			if (object_is_at (n, position) && c_objinfo[n].shdesc) {
			    ++things;
			    wordvalue[wordnumber + 1] = n;

			    // Some objects (type NOUNS) have special
			    // treatment in take(). For these we must
			    // set the type to NOUNS. However for SWORD
			    // and MAID all it does is find which of many
			    // objects is meant, so we need do nothing
			    // here. BATHGOD must become GODDESS as well.
			    // NOUNS with no special case must be included
			    // here to get the right error. DOOR cannot
			    // occur as an object so need not be included.

			    switch (n) {
				case BATHGOD:
				    wordvalue[wordnumber + 1] = GODDESS;
				    // FALLTHROUGH
				case GODDESS:
				case AMULET:
				case MEDALION:
				case TALISMAN:
				case MAN:
				case TIMER:
				case NATIVE_GIRL:
				    wordtype[wordnumber + 1] = NOUNS;
				    break;
				default:
				    wordtype[wordnumber + 1] = OBJECT;
			    }
			    wordnumber = take (position);
			}
		    ++wordnumber;
		    ++wordnumber;
		    if (!things)
			puts("Nothing to take!");
		} else
		    take (position);
		break;

	    case DROP:
		if (wordnumber < wordcount && wordvalue[wordnumber + 1] == EVERYTHING) {
		    int things;
		    things = 0;
		    for (n = 0; n < NUMOFOBJECTS; ++n)
			if (object_is_at (n, INVENTORY)) {
			    ++things;
			    wordvalue[wordnumber + 1] = n;
			    wordnumber = drop("Dropped");
			}
		    ++wordnumber;
		    ++wordnumber;
		    if (!things)
			puts("Nothing to drop!");
		} else
		    drop("Dropped");
		break;

	    case KICK:
	    case THROW_OBJECT:
		if (wordnumber < wordcount && wordvalue[wordnumber + 1] == EVERYTHING) {
		    int things, wv;
		    things = 0;
		    wv = wordvalue[wordnumber];
		    for (n = 0; n < NUMOFOBJECTS; ++n)
			if (object_is_at (n, INVENTORY) || (object_is_at (n, position) && c_objinfo[n].shdesc)) {
			    ++things;
			    wordvalue[wordnumber + 1] = n;
			    wordnumber = throw_object (wordvalue[wordnumber] == KICK ? "Kicked" : "Thrown");
			}
		    wordnumber += 2;
		    if (!things)
			printf("Nothing to %s!\n", wv == KICK ? "kick" : "throw");
		} else
		    throw_object (wordvalue[wordnumber] == KICK ? "Kicked" : "Thrown");
		break;

	    case TAKEOFF:
		if (wordnumber < wordcount && wordvalue[wordnumber + 1] == EVERYTHING) {
		    int things;
		    things = 0;
		    for (n = 0; n < NUMOFOBJECTS; ++n)
			if (object_is_at (n, WEARING)) {
			    ++things;
			    wordvalue[wordnumber + 1] = n;
			    wordnumber = takeoff();
			}
		    wordnumber += 2;
		    if (!things)
			puts("Nothing to take off!");
		} else
		    takeoff();
		break;

	    case DRAW:
		if (wordnumber < wordcount && wordvalue[wordnumber + 1] == EVERYTHING) {
		    int things;
		    things = 0;
		    for (n = 0; n < NUMOFOBJECTS; ++n)
			if (object_is_at (n, WEARING)) {
			    ++things;
			    wordvalue[wordnumber + 1] = n;
			    wordnumber = draw();
			}
		    wordnumber += 2;
		    if (!things)
			puts("Nothing to draw!");
		} else
		    draw();
		break;

	    case PUTON:
		if (wordnumber < wordcount && wordvalue[wordnumber + 1] == EVERYTHING) {
		    int things;
		    things = 0;
		    for (n = 0; n < NUMOFOBJECTS; ++n)
			if (object_is_at (n, position) && c_objinfo[n].shdesc) {
			    ++things;
			    wordvalue[wordnumber + 1] = n;
			    wordnumber = puton();
			}
		    wordnumber += 2;
		    if (!things)
			puts("Nothing to put on!");
		} else
		    puton();
		break;

	    case WEARIT:
		if (wordnumber < wordcount && wordvalue[wordnumber + 1] == EVERYTHING) {
		    int things;
		    things = 0;
		    for (n = 0; n < NUMOFOBJECTS; ++n)
			if (object_is_at (n, INVENTORY)) {
			    ++things;
			    wordvalue[wordnumber + 1] = n;
			    wordnumber = wearit();
			}
		    wordnumber += 2;
		    if (!things)
			puts("Nothing to wear!");
		} else
		    wearit();
		break;

	    case EAT:
		if (wordnumber < wordcount && wordvalue[wordnumber + 1] == EVERYTHING) {
		    int things;
		    things = 0;
		    for (n = 0; n < NUMOFOBJECTS; ++n)
			if (object_is_at (n, INVENTORY)) {
			    ++things;
			    wordvalue[wordnumber + 1] = n;
			    wordnumber = eat();
			}
		    wordnumber += 2;
		    if (!things)
			puts("Nothing to eat!");
		} else
		    eat();
		break;

	    case PUT:
		put();
		break;

	    case INVEN:
		if (count_objects_at (INVENTORY)) {
		    puts("You are holding:\n");
		    for (n = 0; n < NUMOFOBJECTS; ++n)
			if (object_is_at (n, INVENTORY))
			    printf("\t%s\n", c_objinfo[n].shdesc);
		    if (WEIGHT == 0)
			printf("\n= %d kilogram%s (can't lift any weight%s)\n", carrying, (carrying == 1 ? "." : "s."), (carrying ? " or move with what you have" : ""));
		    else
			printf("\n= %d kilogram%s (%d%%)\n", carrying, (carrying == 1 ? "." : "s."), carrying * 100 / WEIGHT);
		    if (CUMBER == 0)
			printf("Your arms can't pick anything up.\n");
		    else
			printf("Your arms are %d%% full.\n", encumber * 100 / CUMBER);
		} else
		    puts("You aren't carrying anything.");

		if (count_objects_at (WEARING)) {
		    puts("\nYou are wearing:\n");
		    for (n = 0; n < NUMOFOBJECTS; ++n)
			if (object_is_at (n, WEARING))
			    printf("\t%s\n", c_objinfo[n].shdesc);
		} else
		    puts("\nYou are stark naked.");
		if (card(injuries, NUMOFINJURIES)) {
		    puts("\nYou have suffered:\n");
		    for (n = 0; n < NUMOFINJURIES; ++n)
			if (injuries[n])
			    printf("\t%s\n", ouch[n]);
		    printf("\nYou can still carry up to %d kilogram%s\n", WEIGHT, (WEIGHT == 1 ? "." : "s."));
		} else
		    puts("\nYou are in perfect health.");
		++wordnumber;
		break;

	    case USE:
		lflag = use();
		break;

	    case OPEN:
		if (wordnumber < wordcount && wordvalue[wordnumber + 1] == EVERYTHING) {
		    int things;
		    things = 0;
		    for (n = 0; n < NUMOFOBJECTS; ++n)
			if (object_is_at (n, INVENTORY)) {
			    ++things;
			    wordvalue[wordnumber + 1] = n;
			    dooropen();
			}
		    wordnumber += 2;
		    if (!things)
			puts("Nothing to open!");
		} else
		    dooropen();
		break;

	    case LOOK:
		if (!game_state (CANTSEE) || object_is_at (LAMPON, INVENTORY) || object_is_at (LAMPON, position)
		    || game_state (MATCH_LIGHT)) {
		    beenthere[position] = 2;
		    writedes();
		    printobjs();
		    if (game_state (MATCH_LIGHT)) {
			puts("\nYour match splutters out.");
			clear_game_state (MATCH_LIGHT);
		    }
		} else
		    puts("I can't see anything.");
		return -1;
		break;

	    case SU:
		if (game_state (IS_WIZARD)) {
		    printf("\nRoom (was %d) = ", position);
		    fgets(buffer, 10, stdin);
		    if (*buffer != '\n')
			sscanf(buffer, "%d", &position);
		    printf("Time (was %d) = ", ourtime);
		    fgets(buffer, 10, stdin);
		    if (*buffer != '\n')
			sscanf(buffer, "%d", &ourtime);
		    printf("Fuel (was %d) = ", fuel);
		    fgets(buffer, 10, stdin);
		    if (*buffer != '\n')
			sscanf(buffer, "%d", &fuel);
		    printf("Torps (was %d) = ", torps);
		    fgets(buffer, 10, stdin);
		    if (*buffer != '\n')
			sscanf(buffer, "%d", &torps);
		    printf("CUMBER (was %d) = ", CUMBER);
		    fgets(buffer, 10, stdin);
		    if (*buffer != '\n')
			sscanf(buffer, "%d", &CUMBER);
		    printf("WEIGHT (was %d) = ", WEIGHT);
		    fgets(buffer, 10, stdin);
		    if (*buffer != '\n')
			sscanf(buffer, "%d", &WEIGHT);
		    printf("Clock (was %d) = ", ourclock);
		    fgets(buffer, 10, stdin);
		    if (*buffer != '\n')
			sscanf(buffer, "%d", &ourclock);
		    printf("Wizard (was %d) = ", game_state (IS_WIZARD));
		    fgets(buffer, 10, stdin);
		    if (*buffer != '\n') {
			sscanf(buffer, "%d", &junk);
			if (!junk)
			    clear_game_state (IS_WIZARD);
		    }
		    printf("\nDONE.\n");
		    return 0;
		} else
		    puts("You aren't a wizard.");
		break;

	    case SCORE:
		printf("\tPLEASURE\tPOWER\t\tEGO\n");
		printf("\t%3d\t\t%3d\t\t%3d\n\n", pleasure, power, ego);
		printf("This gives you the rating of %s in %d turns.\n", rate(), ourtime);
		printf("You have visited %d out of %d rooms this run (%d%%).\n", card(beenthere, NUMOFROOMS), NUMOFROOMS, card(beenthere, NUMOFROOMS) * 100 / NUMOFROOMS);
		break;

	    case SAVE:
		printf("\nSave file name (default %s): ", DEFAULT_SAVE_FILE);
		getline(&filename, &filename_len, stdin);
		if (filename_len == 0 || (filename_len == 1 && filename[0] == '\n'))
		    rfilename = save_file_name(DEFAULT_SAVE_FILE, strlen(DEFAULT_SAVE_FILE));
		else {
		    if (filename[filename_len - 1] == '\n')
			--filename_len;
		    rfilename = save_file_name(filename, filename_len);
		}
		free(filename);
		save(rfilename);
		free(rfilename);
		break;

	    case VERBOSE:
		set_game_state (IS_VERBOSE);
		printf("[Maximum verbosity]\n");
		break;

	    case BRIEF:
		clear_game_state (IS_VERBOSE);
		printf("[Standard verbosity]\n");
		break;

	    case FOLLOW:
		lflag = follow();
		break;

	    case GIVE:
		give();
		break;

	    case KISS:
		kiss();
		break;

	    case LOVE:
		love();
		break;

	    case RIDE:
		lflag = ride();
		break;

	    case DRIVE:
		lflag = drive();
		break;

	    case LIGHT:
		light();
		break;

	    case LAUNCH:
		if (!launch())
		    return -1;
		else
		    lflag = 0;
		break;

	    case LANDIT:
		if (!land())
		    return -1;
		else
		    lflag = 0;
		break;

	    case TIME:
		chime();
		break;

	    case SLEEP:
		zzz();
		break;

	    case DIG:
		dig();
		break;

	    case JUMP:
		lflag = jump();
		break;

	    case BURY:
		bury();
		break;

	    case SWIM:
		puts("Surf's up!");
		break;

	    case DRINK:
		drink();
		break;

	    case QUIT:
		die();

	    default:
		puts("How's that?");
		return -1;
		break;

	}
	if (wordnumber < wordcount && *words[wordnumber++] == ',')
	    continue;
	else
	    return lflag;
    }
    return lflag;
}

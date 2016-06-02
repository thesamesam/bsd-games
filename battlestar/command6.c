// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

int launch(void)
{
    if (testbit(location[position].objects, VIPER) && !notes[CANTLAUNCH]) {
	if (fuel > 4) {
	    clearbit(location[position].objects, VIPER);
	    position = location[position].up;
	    notes[LAUNCHED] = 1;
	    ourtime++;
	    fuel -= 4;
	    puts("You climb into the viper and prepare for launch.");
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
    if (notes[LAUNCHED] && testbit(location[position].objects, LAND) && location[position].down) {
	notes[LAUNCHED] = 0;
	position = location[position].down;
	setbit(location[position].objects, VIPER);
	fuel -= 2;
	ourtime++;
	puts("You are down.");
	return 1;
    } else
	puts("You can't land here.");
    return 0;
}

void die(void)
{			       // endgame
    printf("bye.\nYour rating was %s.\n", rate());
    post(' ');
    exit(0);
}

void diesig (int dummy UNUSED)
{
    die();
}

void live (void)
{
    puts("\nYou win!");
    post('!');
    exit(0);
}

static FILE *score_fp;

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
	if (wiz)
	    fprintf(score_fp, "   wizard\n");
	else if (tempwiz)
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
    if (testbit(location[position].objects, CAR)) {
	puts("You hop in the car and turn the key.  There is a perceptible grating noise,");
	puts("and an explosion knocks you unconscious...");
	clearbit(location[position].objects, CAR);
	setbit(location[position].objects, CRASH);
	injuries[5] = injuries[6] = injuries[7] = injuries[8] = 1;
	ourtime += 15;
	zzz();
	return 0;
    } else
	puts("There is nothing to drive here.");
    return -1;
}

int ride (void)
{
    if (testbit(location[position].objects, HORSE)) {
	puts("You climb onto the stallion and kick it in the guts.  The stupid steed launches");
	puts("forward through bush and fern.  You are thrown and the horse gallops off.");
	clearbit(location[position].objects, HORSE);
	while (!(position = nrand(NUMOFROOMS+1)) || !OUTSIDE || !beenthere[position] || location[position].flyhere)
	    continue;
	setbit(location[position].objects, HORSE);
	if (location[position].north)
	    position = location[position].north;
	else if (location[position].south)
	    position = location[position].south;
	else if (location[position].east)
	    position = location[position].east;
	else
	    position = location[position].west;
	return 0;
    } else
	puts("There is no horse here.");
    return -1;
}

// synonyms = {strike, smoke} for matches, cigars
void light(void)
{
    if (testbit(inven, MATCHES) && matchcount) {
	puts("Your match splutters to life.");
	ourtime++;
	matchlight = 1;
	matchcount--;
	if (position == 217) {
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
	    case 189:
	    case 231:
		if (location[189].north == 231)
		    puts("The door is already open.");
		else
		    puts("The door does not budge.");
		break;
	    case 30:
		if (location[30].west == 25)
		    puts("The door is gone.");
		else
		    puts("The door is locked tight.");
		break;
	    case 31:
		puts("That's one immovable door.");
		break;
	    case 20:
		puts("The door is already ajar.");
		break;
	    default:
		puts("What door?");
	}
    } else
	puts("That doesn't open.");
}

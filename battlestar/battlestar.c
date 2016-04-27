// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

int main(int argc, char **argv)
{
    char mainbuf[LINELENGTH];
    char *next;

    // Open the score file then revoke setgid privileges
    open_score_file();
    setregid(getgid(), getgid());

    if (argc < 2)
	initialize(NULL);
    else if (strcmp(argv[1], "-r") == 0)
	initialize((argc > 2) ? argv[2] : DEFAULT_SAVE_FILE);
    else
	initialize(argv[1]);
  start:
    news();
    if (beenthere[position] <= ROOMDESC)
	beenthere[position]++;
    if (notes[LAUNCHED])
	crash();	       // decrements fuel & crash
    if (matchlight) {
	puts("Your match splutters out.");
	matchlight = 0;
    }
    if (!notes[CANTSEE] || testbit(inven, LAMPON) || testbit(location[position].objects, LAMPON)) {
	writedes();
	printobjs();
    } else
	puts("It's too dark to see anything in here!");
    whichway(location[position]);
  run:
    next = getcom(mainbuf, sizeof mainbuf, ">-: ", "Please type in something.");
    for (wordcount = 0; next && wordcount < NWORD - 1; wordcount++)
	next = getword(next, words[wordcount], -1);
    parse();
    switch (cypher()) {
	case -1:
	    goto run;
	case 0:
	    goto start;
	default:
	    errx(1, "bad return from cypher(): please submit a bug report");
    }
}

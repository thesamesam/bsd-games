// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

//----------------------------------------------------------------------

static void initialize (const char *filename);

//----------------------------------------------------------------------

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
    if (game_state (LAUNCHED))
	crash();	       // decrements fuel & crash
    if (game_state (MATCH_LIGHT)) {
	puts("Your match splutters out.");
	clear_game_state (MATCH_LIGHT);
    }
    if (!game_state (CANTSEE) || object_is_at (LAMPON, INVENTORY) || object_is_at (LAMPON, position)) {
	writedes();
	printobjs();
    } else
	puts("It's too dark to see anything in here!");
    update_relative_directions();
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
	    printf ("bad return from cypher(): please submit a bug report");
	    exit (EXIT_FAILURE);
    }
}

static void initialize (const char *filename)
{
    puts("Version 4.2, fall 1984.");
    puts("First Adventure game written by His Lordship, the honorable");
    puts("Admiral D.W. Riggle\n");

    initialize_curses();
    endwin();
    atexit (cleanup_objects);
    username = player_name();
    wordinit();
    if (filename == NULL) {
	direction = NORTH;
	ourtime = 0;
	snooze = CYCLE * 1.5;
	position = LUXURIOUS_STATEROOM;
	create_object_at (PAJAMAS, WEARING);
	fuel = TANKFULL;
	torps = TORPEDOES;
	place_default_objects();
    } else {
	char* savefile = save_file_name(filename, strlen(filename));
	restore(savefile);
	free(savefile);
    }
}

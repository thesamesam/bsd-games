// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

//{{{ Global variables -------------------------------------------------

int WEIGHT = MAXWEIGHT;
int CUMBER = MAXCUMBER;

int win = 1;
int matchcount = 20;
int followgod = -1;
int followfight = -1;

// current input line
char words [NWORD][WORDLEN] = {};
uint16_t wordvalue [NWORD] = {};
uint8_t wordtype [NWORD] = {};
uint8_t wordcount = 0;
uint8_t wordnumber = 0;

// state of the game
int ourtime = 0;
int position = 0;
int direction = 0;
int left = 0;
int right = 0;
int ahead = 0;
int back = 0;
int fuel = 0;
int torps = 0;
int carrying = 0;
int encumber = 0;
int rythmn = 0;
int ate = 0;
int snooze = 0;
int godready = 0;
int wintime = 0;
int loved = 0;
int pleasure = 0;
int power = 0;
int ego = 0;
uint16_t game_states = 0;
char beenthere [NUMOFROOMS + 1] = {};
char injuries [NUMOFINJURIES] = {};

const char* username = "player";

//}}}-------------------------------------------------------------------

static void initialize (const char *filename);
static void restore (const char *filename);

//----------------------------------------------------------------------

int main (int argc, char **argv)
{
    char mainbuf[LINELENGTH];
    char *next;

    if (argc < 2)
	initialize(NULL);
    else if (strcmp(argv[1], "-r") == 0)
	initialize((argc > 2) ? argv[2] : DEFAULT_SAVE_FILE);
    else
	initialize(argv[1]);
  start:
    news();
    if (beenthere[position] <= ROOMDESC)
	++beenthere[position];
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
    for (wordcount = 0; next && wordcount < NWORD - 1; ++wordcount)
	next = getword (next, words[wordcount]);
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

static void restore (const char *filename)
{
    if (filename == NULL)
	exit (EXIT_FAILURE);	       // Error determining save file name.
    FILE* fp = fopen (filename, "r");
    if (!fp) {
	perror (filename);
	exit (EXIT_FAILURE);
    }
    fread (&WEIGHT, sizeof WEIGHT, 1, fp);
    fread (&CUMBER, sizeof CUMBER, 1, fp);
    fread (&ourclock, sizeof ourclock, 1, fp);
    fread (&game_states, sizeof game_states, 1, fp);
    restore_saved_objects (fp);
    fread (injuries, sizeof injuries, 1, fp);
    fread (&direction, sizeof direction, 1, fp);
    fread (&position, sizeof position, 1, fp);
    fread (&ourtime, sizeof ourtime, 1, fp);
    fread (&fuel, sizeof fuel, 1, fp);
    fread (&torps, sizeof torps, 1, fp);
    fread (&carrying, sizeof carrying, 1, fp);
    fread (&encumber, sizeof encumber, 1, fp);
    fread (&rythmn, sizeof rythmn, 1, fp);
    fread (&followfight, sizeof followfight, 1, fp);
    fread (&ate, sizeof ate, 1, fp);
    fread (&snooze, sizeof snooze, 1, fp);
    fread (&followgod, sizeof followgod, 1, fp);
    fread (&godready, sizeof godready, 1, fp);
    fread (&win, sizeof win, 1, fp);
    fread (&wintime, sizeof wintime, 1, fp);
    fread (&matchcount, sizeof matchcount, 1, fp);
    fread (&loved, sizeof loved, 1, fp);
    fread (&pleasure, sizeof pleasure, 1, fp);
    fread (&power, sizeof power, 1, fp);
    // We must check the last read, to catch truncated save files
    if (fread (&ego, sizeof ego, 1, fp) < 1) {
	printf ("save file %s too short", filename);
	exit (EXIT_FAILURE);
    }
    fclose(fp);
}

void save (const char *filename)
{
    if (filename == NULL)
	return;		       // Error determining save file name.
    FILE* fp = fopen (filename, "w");
    if (!fp) {
	perror (filename);
	return;
    }
    printf ("Saved in %s.\n", filename);
    fwrite (&WEIGHT, sizeof WEIGHT, 1, fp);
    fwrite (&CUMBER, sizeof CUMBER, 1, fp);
    fwrite (&ourclock, sizeof ourclock, 1, fp);
    fwrite (&game_states, sizeof game_states, 1, fp);
    save_objects (fp);
    fwrite (injuries, sizeof injuries, 1, fp);
    fwrite (&direction, sizeof direction, 1, fp);
    fwrite (&position, sizeof position, 1, fp);
    fwrite (&ourtime, sizeof ourtime, 1, fp);
    fwrite (&fuel, sizeof fuel, 1, fp);
    fwrite (&torps, sizeof torps, 1, fp);
    fwrite (&carrying, sizeof carrying, 1, fp);
    fwrite (&encumber, sizeof encumber, 1, fp);
    fwrite (&rythmn, sizeof rythmn, 1, fp);
    fwrite (&followfight, sizeof followfight, 1, fp);
    fwrite (&ate, sizeof ate, 1, fp);
    fwrite (&snooze, sizeof snooze, 1, fp);
    fwrite (&followgod, sizeof followgod, 1, fp);
    fwrite (&godready, sizeof godready, 1, fp);
    fwrite (&win, sizeof win, 1, fp);
    fwrite (&wintime, sizeof wintime, 1, fp);
    fwrite (&matchcount, sizeof matchcount, 1, fp);
    fwrite (&loved, sizeof loved, 1, fp);
    fwrite (&pleasure, sizeof pleasure, 1, fp);
    fwrite (&power, sizeof power, 1, fp);
    fwrite (&ego, sizeof ego, 1, fp);
    fflush(fp);
    if (ferror(fp))
	perror (filename);
    fclose(fp);
}

// Given a save file name (possibly from getline, so without terminating NUL),
// determine the name of the file to be saved to by adding the HOME
// directory if the name does not contain a slash.  Name will be allocated
// with malloc(3).
char* save_file_name (const char* filename, size_t len)
{
    char *home;
    char *newname;
    size_t tmpl;

    if (memchr(filename, '/', len)) {
	newname = malloc(len + 1);
	if (newname == NULL) {
	    printf ("Out of memory");
	    return NULL;
	}
	memcpy(newname, filename, len);
	newname[len] = 0;
    } else {
	home = getenv("HOME");
	if (home != NULL) {
	    tmpl = strlen(home);
	    newname = malloc(tmpl + len + 2);
	    if (newname == NULL) {
		printf ("Out of memory");
		return NULL;
	    }
	    memcpy(newname, home, tmpl);
	    newname[tmpl] = '/';
	    memcpy(newname + tmpl + 1, filename, len);
	    newname[tmpl + len + 1] = 0;
	} else {
	    newname = malloc(len + 1);
	    if (newname == NULL) {
		printf ("Out of memory");
		return NULL;
	    }
	    memcpy(newname, filename, len);
	    newname[len] = 0;
	}
    }
    return newname;
}

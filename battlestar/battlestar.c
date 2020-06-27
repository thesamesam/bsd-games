// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"
#include <sys/uio.h>

//{{{ Global variables -------------------------------------------------

gametime_t ourtime = 0;
gametime_t rythmn = 0;
gametime_t ate = 0;
gametime_t snooze = 3*CYCLE/2;
location_t position = LUXURIOUS_STATEROOM;
uint16_t direction = NORTH;
uint16_t game_states = 0;
uint16_t player_injuries = 0;
struct GameScore game_score = {};
uint8_t fuel = TANKFULL;
uint8_t torps = TORPEDOES;
uint8_t matchcount = 20;
uint8_t godready = 0;
uint8_t win = 1;
uint8_t beenthere [(NUMOFROOMS+1+7)/8] = {};

const char* username = "player";

//}}}-------------------------------------------------------------------

static bool restore (void);

//----------------------------------------------------------------------

int main (void)
{
    initialize_curses();
    endwin();
    atexit (cleanup_objects);
    username = player_name();
    if (!restore()) {
	place_default_objects();
	puts (BOLD_ON "\n"
	    "		===========================	\n"
	    "		    B A T T L E S T A R		\n"
	    "		===========================	\n"
	    BOLD_OFF "\n"
	    "	First Adventure game written by His Lordship,\n"
	    "	     the honorable Admiral D.W. Riggle");
    }
    for (;;) {
	news();
	if (game_state (LAUNCHED))
	    crash();	// decrements fuel & crashes when empty
	if (game_state (MATCH_LIGHT)) {
	    puts("Your match splutters out.");
	    clear_game_state (MATCH_LIGHT);
	}
	if (!game_state (CANTSEE) || object_is_at (LAMPON, INVENTORY) || object_is_at (LAMPON, position)) {
	    if (!visited_location (position) || game_state (IS_VERBOSE)) {
		mark_location_visited (position);
		write_location_long_description();
	    } else
		write_location_short_description();
	    printobjs();
	} else
	    puts ("It's too dark to see anything in here!");
	do {
	    get_player_command (BOLD_ON "> " BOLD_OFF);
	} while (process_command() != 0);
    }
}

//{{{ Save and restore -------------------------------------------------

//{{{2 s_save_array

#define WVAR(v)	{ &v, sizeof(v) }
#define WARR(v)	{ &v[0], sizeof(v) }

static struct iovec s_save_array[] = {
    WVAR (ourtime),
    WVAR (rythmn),
    WVAR (ate),
    WVAR (snooze),
    WVAR (position),
    WVAR (direction),
    WVAR (game_states),
    WVAR (player_injuries),
    WVAR (game_score),
    WVAR (fuel),
    WVAR (torps),
    WVAR (matchcount),
    WVAR (godready),
    WVAR (win),
    WARR (beenthere)
};
//}}}2

static uint16_t savegame_checksum (void)
{
    uint16_t sum = 0;
    for (unsigned i = 0; i < ArraySize(s_save_array); ++i)
	sum = bsdsum ((const uint8_t*) s_save_array[i].iov_base, s_save_array[i].iov_len, sum);
    return saved_objects_checksum (sum);
}

struct save_header { char magictext[6]; uint16_t sum; };

static bool restore (void)
{
    char savename [PATH_MAX];
    snprintf (ArrayBlock(savename), BATTLESTAR_SAVE_NAME, player_homedir());
    int fd = open (savename, O_RDONLY);
    if (fd < 0)
	return false;

    struct save_header header;
    if (sizeof(header) != read (fd, &header, sizeof(header))
	|| 0 >= readv (fd, s_save_array, ArraySize(s_save_array))
	|| 0 >= read_objects_array (fd)) {
	printf ("Error: reading '%s': %s\n", savename, strerror(errno));
	close (fd);
	exit (EXIT_FAILURE);
    }
    if (memcmp (header.magictext, "btlstr", sizeof(header.magictext)) != 0
	|| header.sum != savegame_checksum()) {
	printf ("Error: saved game '%s' is corrupt. Please delete it.\n", savename);
	close (fd);
	exit (EXIT_FAILURE);
    }
    close (fd);
    unlink (savename);
    return true;
}

bool save (void)
{
    const char* homedir = player_homedir();
    char savename [PATH_MAX];
    snprintf (ArrayBlock(savename), _PATH_SAVED_GAMES, homedir);
    if (0 != access (savename, R_OK))
	mkpath (savename, S_IRWXU);
    if (0 != access (savename, W_OK)) {
	printf ("Error: you are not allowed to write to '%s'\n", savename);
	return false;
    }
    snprintf (ArrayBlock(savename), BATTLESTAR_SAVE_NAME, homedir);

    int fd = creat (savename, S_IRUSR| S_IWUSR);
    if (fd < 0) {
	printf ("Error: unable to create save file '%s': %s\n", savename, strerror(errno));
	return false;
    }
    struct save_header header = {{'b','t','l','s','t','r'}, savegame_checksum()};
    if (sizeof(header) != write (fd, &header, sizeof(header))
	|| 0 >= writev (fd, s_save_array, ArraySize(s_save_array))
	|| 0 >= write_objects_array (fd)
	|| 0 > close (fd)) {
	printf ("Error writing save file '%s': %s\n", savename, strerror(errno));
	close (fd);
	return false;
    }
    return true;
}

//}}}-------------------------------------------------------------------

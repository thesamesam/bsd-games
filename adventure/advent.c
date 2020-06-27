// This file is free software, distributed under the BSD license.

#include "advent.h"
#include <signal.h>
#include <sys/uio.h>
#include <sys/stat.h>

//{{{ Global variables -------------------------------------------------

// Database variables
struct trav travel [MAXTRAV] = {};

// Command parser variables
int verb = 0;
int object = 0;
int motion = 0;
char word1 [MAXWORDLEN] = {};
char word2 [MAXWORDLEN] = {};

// Play variables
uint32_t	turns	= 0;
int16_t		clock1	= 30;		// timing variables
int16_t		clock2	= 50;
int16_t		limit	= 1000;		// time limit
uint8_t		tally	= 15;		// item counts
uint8_t		tally2	= 0;
uint8_t		holding	= 0;		// count of held items
uint8_t		numdie	= 0;		// number of deaths
uint8_t		dkill	= 0;		// dwarves killed
uint8_t		dflag	= 0;		// dwarf flag
uint8_t		foobar	= 0;		// fie fie foe foo...
uint8_t		bonus	= 0;		// to pass to end
loc_t		loc	= END_OF_ROAD;	// location variables
loc_t		newloc	= WELLHOUSE;
loc_t		oldloc	= END_OF_ROAD;
loc_t		oldloc2	= END_OF_ROAD;
loc_t		knfloc	= 0;		// knife location
loc_t		chloc	= CHEST_ROOM1;	// chest locations
loc_t		chloc2	= CHEST_ROOM2;
loc_t		daltloc	= 18;		// alternate appearance
obj_t		object1	= 0;		// to help intrans.
bool		lmwarn	= false;	// lamp warning flag
bool		wzdark	= 0;		// game state flags
bool		closing	= 0;
bool		closed	= 0;
bool		panic	= false;
bool		gaveup	= false;	// true if he quit early

bool		saveflg	= false;	// if game being saved

bool	visited [MAXLOC] = {};	// true if has been here

// dwarf locations
loc_t	dloc [MAXDWARVES]  = { 0,KING_HALL,FISSURE_W,Y2_ROOM,44,CO_JUNCTION,CHEST_ROOM1 };
bool	dseen [MAXDWARVES] = {};	// dwarf seen flag
loc_t	odloc [MAXDWARVES] = {};	// dwarf old locations

//}}}-------------------------------------------------------------------
//{{{ Local prototypes

static void initialize (void);
static _Noreturn void on_fatal_signal (int sig);
static uint16_t sum_save_array (void);
static bool save (void);

//}}}-------------------------------------------------------------------

int main (void)
{
    initialize();
    if (restore()) {
	printf ("Game restored.\n");
	describe();
    } else {
	rspeak (65);
	rspeak (1);
    }
    while (!saveflg)
	turn();
    if (saveflg)
	save();
    return EXIT_SUCCESS;
}

//{{{ initialize and signal handling -----------------------------------

static void initialize (void)
{
    // Install signal handlers to ensure cleanup
    static const uint8_t c_sigs[] = {
	SIGINT, SIGQUIT, SIGTERM, SIGILL, SIGBUS,
	SIGABRT, SIGFPE, SIGSYS, SIGSEGV, SIGHUP
    };
    for (unsigned i = 0; i < ArraySize(c_sigs); ++i)
	signal (c_sigs[i], on_fatal_signal);
    srandrand();
}

#define S(s)	(1<<(s))
enum { sigset_Quit = S(SIGINT)|S(SIGQUIT)|S(SIGTERM) };
enum { exitcode_SignalBase = 128 };

static _Noreturn void on_fatal_signal (int sig)
{
    static volatile _Atomic(bool) s_once = false;
    int exitcode = exitcode_SignalBase+sig;
    if (false == atomic_exchange (&s_once, true)) {
	if (S(sig) & sigset_Quit)
	    exitcode = EXIT_SUCCESS;	// terminated by user
	else
	    psignal (sig, "Fatal error");
	save();	// try to save on crash
	exit (exitcode);
    }
    _Exit (exitcode);
}
#undef S

//}}}-------------------------------------------------------------------
//{{{ Game save and restore

//{{{2 s_save_array

#define WVAR(v)	{ &v, sizeof(v) }
#define WARR(v)	{ &v[0], sizeof(v) }

static struct iovec s_save_array[] = {
    WVAR (turns),
    WVAR (clock1),
    WVAR (clock2),
    WVAR (limit),
    WVAR (tally),
    WVAR (tally2),
    WVAR (holding),
    WVAR (numdie),
    WVAR (dkill),
    WVAR (dflag),
    WVAR (foobar),
    WVAR (bonus),
    WVAR (loc),
    WVAR (newloc),
    WVAR (oldloc),
    WVAR (oldloc2),
    WVAR (knfloc),
    WVAR (chloc),
    WVAR (chloc2),
    WVAR (daltloc),
    WVAR (object1),
    WVAR (lmwarn),
    WVAR (wzdark),
    WVAR (closing),
    WVAR (closed),
    WVAR (panic),
    WVAR (gaveup),
    WARR (visited),
    WARR (place),
    WARR (fixed),
    WARR (prop),
    WARR (dloc),
    WARR (dseen),
    WARR (odloc)
};
//}}}2

static uint16_t sum_save_array (void)
{
    uint16_t sum = 0;
    for (unsigned i = 0; i < ArraySize(s_save_array); ++i)
	sum = bsdsum ((const uint8_t*) s_save_array[i].iov_base, s_save_array[i].iov_len, sum);
    return sum;
}

struct save_header { char magictext[6]; uint16_t sum; };

static bool save (void)
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
    snprintf (ArrayBlock(savename), ADVENTURE_SAVE_NAME, homedir);

    int fd = creat (savename, S_IRUSR| S_IWUSR);
    if (fd < 0) {
	printf ("Error: unable to create save file '%s': %s\n", savename, strerror(errno));
	return false;
    }
    struct save_header header = {{'a','d','v','e','n','t'}, sum_save_array()};
    if (sizeof(header) != write (fd, &header, sizeof(header))
	|| 0 >= writev (fd, s_save_array, ArraySize(s_save_array))
	|| 0 > close (fd)) {
	printf ("Error writing save file '%s': %s\n", savename, strerror(errno));
	close (fd);
	return false;
    }
    return true;
}

bool restore (void)
{
    char savename [PATH_MAX];
    snprintf (ArrayBlock(savename), ADVENTURE_SAVE_NAME, player_homedir());
    int fd = open (savename, O_RDONLY);
    if (fd < 0)
	return false;

    struct save_header header;
    if (sizeof(header) != read (fd, &header, sizeof(header))
	|| 0 >= readv (fd, s_save_array, ArraySize(s_save_array))) {
	printf ("Error: reading '%s': %s\n", savename, strerror(errno));
	close (fd);
	exit (EXIT_FAILURE);
    }
    if (memcmp (header.magictext, "advent", sizeof(header.magictext)) != 0
	|| header.sum != sum_save_array()) {
	printf ("Error: saved game '%s' is corrupt. Please delete it.\n", savename);
	close (fd);
	exit (EXIT_FAILURE);
    }
    close (fd);
    unlink (savename);
    return true;
}

//}}}-------------------------------------------------------------------

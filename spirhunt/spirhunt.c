// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
//
//======================================================================
//			SPACE PIRATE HUNT
//======================================================================
//
// C version by Eric P. Allman 5/76 (U.C. Berkeley) with help
//         from Jeff Poskanzer and Pete Rubinstein.
//
// I also want to thank everyone here at Berkeley who
// where crazy enough to play the undebugged game. I want to
// particularly thank Nick Whyte, who made considerable
// suggestions regarding the content of the game. Why, I'll
// never forget the time he suggested the name for the
// "capture" command.
//
// FORTRASH version by Kay R. Fisher (DEC) "and countless others".
// That was adapted from the "original BASIC program" (ha!) by
//         Mike Mayfield (Centerline Engineering).
//
// Additional inspiration taken from FORTRAN version by
//         David Matuszek and Paul Reynolds which runs on the CDC
//         7600 at Lawrence Berkeley Lab, maintained there by
//         Andy Davidson. This version is also available at LLL
//         and at LMSC. In all fairness, this version was the
//         major inspiration for this version of the game (trans-
//         lation:  I ripped off a whole lot of code).
//
// Minor other input from the "Battelle Version 7A" by Joe Miller
//         (Graphics Systems Group, Battelle-Columbus Labs) and
//         Ross Pavlac (Systems Programmer, Battelle Memorial
//         Institute). That version was written in December '74
//         and extensively modified June '75. It was adapted
//         from the FTN version by Ron Williams of CDC Sunnyvale,
//         which was adapted from the Basic version distributed
//         by DEC. It also had "neat stuff swiped" from T. T.
//         Terry and Jim Korp (University of Texas), Hicks (Penn
//         U.), and Rick Maus (Georgia Tech). Unfortunately, it
//         was not as readable as it could have been and so the
//         translation effort was severely hampered. None the
//         less, I got the idea of inhabited starsystems from this
//         version.

#include "spirhunt.h"
#include "getpar.h"
#include <sys/uio.h>
#include <sys/stat.h>
#include <math.h>

//{{{ Local prototypes -------------------------------------------------

static void setup (void);
static void play (void);
static uint16_t sum_save_array (void);
static bool verify_locations (void);
static bool restore (void);

//}}}-------------------------------------------------------------------
//{{{ Global variables

//{{{2 DeviceName
const char* const DeviceName [NDEV] = {
    "warp drive",
    "near scanners",
    "far scanners",
    "plasers",
    "torpedoes",
    "impulse engines",
    "shield control",
    "computer",
    "radio",
    "life support",
    "navigation",
    "cloak",
    "transporter",
    "shuttlecraft",
    "*ERR 14*",
    "*ERR 15*"
};
//}}}2
//{{{2 Systemname
const char *const Systemname[NINHAB] = {
    "ERROR",
    "Talos IV",
    "Rigel III",
    "Deneb VII",
    "Canopus V",
    "Icarus I",
    "Prometheus II",
    "Omega VII",
    "Elysium I",
    "Scalos IV",
    "Procyon IV",
    "Arachnid I",
    "Argo VIII",
    "Triad III",
    "Echo IV",
    "Nimrod III",
    "Nemisis IV",
    "Centarurus I",
    "Kronos III",
    "Spectros V",
    "Beta III",
    "Gamma Tranguli VI",
    "Pyris III",
    "Triachus",
    "Marcus XII",
    "Kaland",
    "Ardana",
    "Stratos",
    "Eden",
    "Arrikis",
    "Epsilon Eridani IV",
    "Exo III"
};
//}}}2

struct event	Event [MAXEVENTS];	// dynamic event list; one entry per pending event
struct quad	Quad [NQUADS][NQUADS];
char		Sect [NSECTS][NSECTS];	// current sector map

struct Ship_struct	Ship;
struct Game_struct	Game;
struct Move_struct	Move;
struct Param_struct	Param;
struct Now_struct	Now;
struct Etc_struct	Etc;

//}}}-------------------------------------------------------------------

int main (void)
{
    srandrand();
    if (!restore()) {
	printf ("\n   * * *   SPACE PIRATE HUNT   * * *\n\n");
	setup();
    }
    play();
    return EXIT_SUCCESS;
}

//{{{ setup ------------------------------------------------------------

static void setup (void)
{
    int i, j;
    float f;
    int d;
    int klump;
    int ix, iy;
    struct quad *q;
    struct event *e;

    Param.bases = Now.bases = nrand(5) + 2;
    Param.time = Now.time = 14;
    Param.pirates = Now.pirates = 14 * (fnrand() + 0.75);
    Param.energy = Ship.energy = 5000;
    Param.torped = Ship.torped = 10;
    Param.shield = Ship.shield = 1500;
    Param.resource = Now.resource = Param.pirates * Param.time;
    Param.reserves = Ship.reserves = 10;
    Param.crew = Ship.crew = 387;
    Param.brigfree = Ship.brigfree = 400;
    Ship.shldup = 1;
    Ship.cond = GREEN;
    Ship.warp = 5.0;
    Ship.warp2 = 25.0;
    Ship.warp3 = 125.0;
    Ship.sinsbad = 0;
    Ship.cloaked = 0;
    Param.date = Now.date = (nrand(20) + 20) * 100;
    f = 2;
    f = log(f + 0.5);
    for (i = 0; i < NDEV; ++i) {
	if (DeviceName[i][0] == '*')
	    Param.damfac[i] = 0;
	else
	    Param.damfac[i] = f;
    }
    Param.dockfac = 0.5;
    Param.regenfac = 0.2;
    if (Param.regenfac < 0.0)
	Param.regenfac = 0.0;
    Param.warptime = 10;
    Param.stopengy = 50;
    Param.shupengy = 40;
    Param.piratepwr = 350;
    Param.plasfac = 0.8;
    Param.hitfac = 0.5;
    Param.pirate_crew = 200;
    Param.srndrprob = 0.0035;
    Param.moveprob[PM_OB] = 45;
    Param.movefac[PM_OB] = .09;
    Param.moveprob[PM_OA] = 40;
    Param.movefac[PM_OA] = -0.05;
    Param.moveprob[PM_EB] = 40;
    Param.movefac[PM_EB] = 0.075;
    Param.moveprob[PM_EA] = 35;
    Param.movefac[PM_EA] = -0.12;
    Param.moveprob[PM_LB] = 0;
    Param.movefac[PM_LB] = 0.0;
    Param.moveprob[PM_LA] = 30;
    Param.movefac[PM_LA] = 0.25;
    Param.eventdly[E_SNOVA] = 0.5;
    Param.eventdly[E_LRTB] = 25.0;
    Param.eventdly[E_PATSB] = 1.0;
    Param.eventdly[E_PDESB] = 3.0;
    Param.eventdly[E_ISSUE] = 1.0;
    Param.eventdly[E_SNAP] = 0.5;
    Param.eventdly[E_ENSLV] = 0.5;
    Param.eventdly[E_REPRO] = 2.0;
    Param.navigcrud[0] = 1.50;
    Param.navigcrud[1] = 0.75;
    Param.cloakenergy = 1000;
    Param.energylow = 1000;
    for (i = 0; i < MAXEVENTS; ++i) {
	e = &Event[i];
	e->date = FLT_MAX;
	e->evcode = 0;
    }
    xsched(E_SNOVA, 1, 0, 0, 0);
    xsched(E_LRTB, Param.pirates, 0, 0, 0);
    xsched(E_PATSB, 1, 0, 0, 0);
    xsched(E_ISSUE, 1, 0, 0, 0);
    xsched(E_SNAP, 1, 0, 0, 0);
    Ship.sectx = nrand(NSECTS);
    Ship.secty = nrand(NSECTS);
    Game.pirates_killed = 0;
    Game.bases_killed = 0;
    Game.stars_killed = 0;
    Game.deaths = 0;
    Game.negenbar = 0;
    Game.captives = 0;
    Game.killinhab = 0;
    Game.helps = 0;
    Game.killed = 0;
    Game.snap = 0;
    Move.endgame = 0;

    // setup stars
    for (i = 0; i < NQUADS; ++i)
	for (j = 0; j < NQUADS; ++j) {
	    q = &Quad[i][j];
	    q->pirates = q->bases = 0;
	    q->scanned = -1;
	    q->stars = nrand(9) + 1;
	    q->holes = nrand(3) - q->stars / 5;
	    q->qsystemname = 0;
	}

    // select inhabited starsystems
    for (d = 1; d < NINHAB; ++d) {
	do {
	    i = nrand(NQUADS);
	    j = nrand(NQUADS);
	    q = &Quad[i][j];
	} while (q->qsystemname);
	q->qsystemname = d;
    }

    // position starbases
    for (i = 0; i < Param.bases; ++i) {
	while (1) {
	    ix = nrand(NQUADS);
	    iy = nrand(NQUADS);
	    q = &Quad[ix][iy];
	    if (q->bases > 0)
		continue;
	    break;
	}
	q->bases = 1;
	Now.base[i].x = ix;
	Now.base[i].y = iy;
	q->scanned = 1001;
	// start near starbase
	if (i == 0) {
	    Ship.quadx = ix;
	    Ship.quady = iy;
	}
    }

    // position pirates
    for (i = Param.pirates; i > 0;) {
	klump = nrand(4) + 1;
	if (klump > i)
	    klump = i;
	while (1) {
	    ix = nrand(NQUADS);
	    iy = nrand(NQUADS);
	    q = &Quad[ix][iy];
	    if (q->pirates + klump > MAX_QUAD_PIRATES)
		continue;
	    q->pirates += klump;
	    i -= klump;
	    break;
	}
    }

    // initialize this quadrant
    printf ("%d pirates\n%d starbase", Param.pirates, Param.bases);
    if (Param.bases > 1)
	printf ("s");
    printf (" at %d,%d", Now.base[0].x, Now.base[0].y);
    for (i = 1; i < Param.bases; ++i)
	printf (", %d,%d", Now.base[i].x, Now.base[i].y);
    printf ("\nIt takes %d units to kill a pirate\n", Param.piratepwr);
    Move.free = 0;
    initquad (false);
    srscan(1);
    attack(0);
}

// Generate quadrant upon entering
//
// A quadrant is initialized from the information held in the Quad
// matrix. Basically, everything is just initialized randomly, except
// for the starship, which goes into a fixed sector.
//
// If there are pirates in the quadrant, the captain is informed that
// the condition is RED, and he is given a chance to put his shields
// up if the computer is working.
//
// The flag 'docked' is set if the ship will be docked on arrival.
//
void initquad (bool docked)
{
    struct quad* q = &Quad[Ship.quadx][Ship.quady];

    // ignored supernova'ed quadrants (this is checked again later anyway
    if (q->stars < 0)
	return;
    Etc.npirates = q->pirates;
    uint8_t nbases = q->bases;
    int8_t nstars = q->stars;
    int8_t nholes = q->holes;

    // have we blundered into a battle zone w/ shields down?
    if (Etc.npirates > 0 && !docked) {
	printf ("Condition RED\n");
	Ship.cond = RED;
	if (!device_damaged (COMPUTER))
	    shield(1);
    }

    // clear out the quadrant
    memset (Sect, EMPTY, sizeof(Sect));

    // initialize the ship
    Sect[Ship.sectx][Ship.secty] = YOURSHIP;

    // initialize pirates
    for (int i = 0; i < Etc.npirates; ++i) {
	int rx, ry;
	sector (&rx, &ry);
	Sect[rx][ry] = PIRATE;
	Etc.pirate[i].x = rx;
	Etc.pirate[i].y = ry;
	Etc.pirate[i].power = Param.piratepwr;
	Etc.pirate[i].srndreq = 0;
    }
    comp_pirate_dist (1);

    // initialize star base
    if (nbases > 0) {
	int rx, ry;
	sector (&rx, &ry);
	Sect[rx][ry] = BASE;
	Etc.starbase.x = rx;
	Etc.starbase.y = ry;
    }

    // initialize inhabited starsystem
    if (q->qsystemname != 0) {
	int rx, ry;
	sector (&rx, &ry);
	Sect[rx][ry] = INHABIT;
	nstars -= 1;
    }

    // initialize black holes
    for (int i = 0; i < nholes; ++i) {
	int rx, ry;
	sector (&rx, &ry);
	Sect[rx][ry] = HOLE;
    }

    // initialize stars
    for (int i = 0; i < nstars; ++i) {
	int rx, ry;
	sector (&rx, &ry);
	Sect[rx][ry] = STAR;
    }
    Move.newquad = 1;
}

void sector (int* x, int* y)
{
    int i, j;
    do {
	i = nrand(NSECTS);
	j = nrand(NSECTS);
    } while (Sect[i][j] != EMPTY);
    *x = i;
    *y = j;
    return;
}

//}}}-------------------------------------------------------------------
//{{{ play

// INSTRUCTION READ AND MAIN PLAY LOOP
//
// Well folks, this is it. Here we have the guts of the game.
// This routine executes moves. It sets up per-move variables,
// gets the command, and executes the command. After the command,
// it calls events() to use up time, attack() to have pirates
// attack if the move was not free, and checkcond() to check up
// on how we are doing after the move.

static void save_game (int f UNUSED)
{
    if (save())
	exit (EXIT_SUCCESS);
    puts ("Error: failed to save the game");
}

static void myreset (int v UNUSED)
    { exit (EXIT_SUCCESS); }

static void play (void)
{
    //{{{2 Command table
    static const struct cvntab Comtab[] = {
	{"ca","pture",	capture,	0},
	{"cl","oak",	shield,		-1},
	{"c","omputer",	computer,	0},
	{"da","mages",	damage_report,	0},
	{"destruct","",	destruct,	0},
	{"do","ck",	dock,		0},
	{"help", "",	help,		0},
	{"i","mpulse",	impulse,	0},
	{"l","rscan",	lrscan,		0},
	{"m","ove",	dowarp,		0},
	{"p","lasers",	plasers,	0},
	{"quit","",	myreset,	0},
	{"ram","",	dowarp,		1},
	{"save","",	save_game,	0},
	{"r","est",	rest,		0},
	{"sh","ield",	shield,		0},
	{"s","rscan",	srscan,		0},
	{"st","atus",	srscan,		-1},
	{"t","orpedo",	torped,		0},
	{"u","ndock",	undock,		0},
	{"v","isual",	visual,		0},
	{"w","arp",	setwarp,	0},
	{}
    };
    //}}}2
    for (;;) {
	Move.free = 1;
	Move.time = 0.0;
	Move.shldchg = 0;
	Move.newquad = 0;
	Move.resting = 0;
	const struct cvntab* r = getcodpar ("\nCommand", Comtab);
	(*r->value)(r->value2);
	events (0);
	attack (0);
	checkcond();
    }
}

// Check for Condition After a Move
//
// Various ship conditions are checked. First we check
// to see if we have already lost the game, due to running
// out of life support reserves, running out of energy,
// or running out of crew members. The check for running
// out of time is in events().
//
// If we are in automatic override mode (Etc.npirates < 0), we
// don't want to do anything else, lest we call autover recursively.
//
// In the normal case, if there is a supernova, we call
// autover() to help us escape. If after calling autover()
// we are still in the grips of a supernova, we get burnt up.
//
// If there are no pirates in this quadrant, we nullify any
// distress calls which might exist.
//
// We then set the condition code, based on the energy level
// and battle conditions.
//
void checkcond (void)
{
    // See if we are still alive and well
    if (Ship.reserves < 0.0)
	lose (L_NOLIFE);
    if (Ship.energy <= 0)
	lose (L_NOENGY);
    if (Ship.crew <= 0)
	lose (L_NOCREW);

    // If in auto override mode, ignore the rest
    if (Etc.npirates < 0)
	return;

    // Call in automatic override if appropriate
    if (Quad[Ship.quadx][Ship.quady].stars < 0)
	autover();
    if (Quad[Ship.quadx][Ship.quady].stars < 0)
	lose (L_SNOVA);

    // Cancel distress call if appropriate
    if (Etc.npirates <= 0)
	kill_distress_call (Ship.quadx, Ship.quady, 1);

    // set condition code
    if (Ship.cond == DOCKED)
	return;
    if (Etc.npirates > 0)
	Ship.cond = RED;
    else if (Ship.energy < Param.energylow)
	Ship.cond = YELLOW;
    else
	Ship.cond = GREEN;
    return;
}

//}}}-------------------------------------------------------------------
//{{{ Game save and restore

//{{{2 s_save_array

#define WVAR(v)	{ &v, sizeof(v) }
#define WARR(v)	{ &v[0], sizeof(v) }

static struct iovec s_save_array[] = {
    WVAR (Ship),
    WVAR (Now),
    WVAR (Param),
    WVAR (Etc),
    WVAR (Game),
    WVAR (Move),
    WARR (Sect),
    WARR (Quad),
    WARR (Event)
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
    snprintf (ArrayBlock(savename), SPIRHUNT_SAVE_NAME, homedir);

    int fd = creat (savename, S_IRUSR| S_IWUSR);
    if (fd < 0) {
	printf ("Error: unable to create save file '%s': %s\n", savename, strerror(errno));
	return false;
    }
    struct save_header header = {{'s','p','i','r','h','t'}, sum_save_array()};
    if (sizeof(header) != write (fd, &header, sizeof(header))
	|| 0 >= writev (fd, s_save_array, ArraySize(s_save_array))
	|| 0 > close (fd)) {
	printf ("Error writing save file '%s': %s\n", savename, strerror(errno));
	close (fd);
	return false;
    }
    return true;
}

static bool verify_locations (void)
{
    return true;
}

static bool restore (void)
{
    char savename [PATH_MAX];
    snprintf (ArrayBlock(savename), SPIRHUNT_SAVE_NAME, player_homedir());
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
    close (fd);
    if (memcmp (header.magictext, "spirht", sizeof(header.magictext)) != 0
	|| header.sum != sum_save_array()
	|| !verify_locations()) {
	printf ("Error: saved game '%s' is corrupt. Please delete it.\n", savename);
	exit (EXIT_FAILURE);
    }
    unlink (savename);
    return true;
}

//}}}-------------------------------------------------------------------
//{{{ Score

// PRINT OUT THE CURRENT SCORE
long score (void)
{
    printf ("\n*** Your score:\n");
    int u = Game.pirates_killed;
    int t = u * Param.piratepwr / 4;
    long s = t;
    if (t)
	printf ("%d pirates killed\t\t\t%6d\n", u, t);
    float r = Now.date - Param.date;
    if (r < 1.0)
	r = 1.0;
    r = Game.pirates_killed / r;
    s += (t = 400 * r);
    if (t != 0)
	printf ("Kill rate %.2f pirates/day\t\t%6d\n", r, t);
    r = Now.pirates;
    r /= Game.pirates_killed + 1;
    s += (t = -400 * r);
    if (t != 0)
	printf ("Penalty for %d pirates remaining\t%6d\n", Now.pirates, t);
    if (Move.endgame > 0) {
	s += (t = 100 * (u = 2));
	printf ("Bonus for winning the game\t\t%6d\n", t);
    }
    if (Game.killed) {
	s -= 500;
	printf ("Penalty for getting killed\t\t  -500\n");
    }
    s += (t = -100 * (u = Game.bases_killed));
    if (t != 0)
	printf ("%d starbases killed\t\t\t%6d\n", u, t);
    s += (t = -100 * (u = Game.helps));
    if (t != 0)
	printf ("%d calls for help\t\t\t%6d\n", u, t);
    s += (t = -5 * (u = Game.stars_killed));
    if (t != 0)
	printf ("%d stars destroyed\t\t\t%6d\n", u, t);
    s += (t = -150 * (u = Game.killinhab));
    if (t != 0)
	printf ("%d inhabited starsystems destroyed\t%6d\n", u, t);
    s += (t = 3 * (u = Game.captives));
    if (t != 0)
	printf ("%d pirates captured\t\t\t%6d\n", u, t);
    s += (t = -(u = Game.deaths));
    if (t != 0)
	printf ("%d casualties\t\t\t\t%6d\n", u, t);
    printf ("\n***  TOTAL\t\t\t%14ld\n", s);
    return s;
}

//}}}-------------------------------------------------------------------
//{{{ Win and lose

// Signal game won
//
// This routine prints out the win message, arranges to print out
// your score, tells you if you have a promotion coming to you,
// cleans up the current input line, and exits.
//
// Pretty straightforward, although the promotion algorithm is
// pretty off the wall.

void win (void)
{
    printf ("\nCongratulations, you have saved the Galaxy\n");
    Move.endgame = 1;

    // print and return the score
    score();

    // clean out input, and request new game
    skiptonl(0);
    exit (EXIT_SUCCESS);
}

// PRINT OUT LOSER MESSAGES
//
// The messages are printed out, the score is computed and
// printed, and the game is restarted. Oh yeh, any special
// actions which need be taken are taken.

_Noreturn void lose (enum LoseReason why)
{
    Game.killed = (why != L_NOTIME);
    Move.endgame = -1;
    static const char Losemsg[] = {
	"You got bored\0"
	"You ran out of time\0"
	"You ran out of energy\0"
	"You have been destroyed\0"
	"You ran into the negative energy barrier\0"
	"You destroyed yourself by nova'ing that star\0"
	"You have been caught in a supernova\0"
	"You just suffocated in outer space\0"
	"You could not be rematerialized\0"
	"\n\032\014 ***\07 Ship's hull has imploded\07 ***\0"
	"You have burned up in a star\0"
	"Well, you destroyed yourself, but it didn't do any good\0"
	"You have been captured by pirates and mercilessly tortured\0"
	"Your last crew member died"
    };
    printf ("\n%s\n", zstrn (Losemsg, why, 14));
    sleep(1);
    score();
    skiptonl(0);
    exit (EXIT_SUCCESS);
}

//}}}-------------------------------------------------------------------
//{{{ Miscellaneous

float fnrand (void)
{
    float t = rand() & 077777;
    return t / 32767.0;
}

//}}}-------------------------------------------------------------------

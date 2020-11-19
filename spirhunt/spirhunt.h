// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#pragma once
#include "../config.h"
#include <float.h>

//{{{ Galactic parameters ----------------------------------------------

enum {
    NSECTS	= 10,	// dimensions of quadrant in sectors
    NQUADS	= 8,	// dimension of galaxy in quadrants
    NINHAB	= 32	// number of quadrants which are inhabited
};

struct quad {			// definition for each quadrant
    uint8_t	bases;		// number of bases in this quadrant
    char	pirates;	// number of pirates in this quadrant
    char	holes;		// number of black holes in this quadrant
    int		scanned;	// star chart entry (see below)
    short	stars;		// number of stars in this quadrant
    char	qsystemname;	// starsystem name (see below)
};

enum {
    Q_DISTRESSED	= 0200,
    Q_SYSTEM		= 077
};

//  systemname conventions:
//      1 -> NINHAB     index into Systemname table for live system.
//      + Q_DISTRESSED  distressed starsystem -- systemname & Q_SYSTEM
//                      is the index into the Event table which will
//                      have the system name
//      0               dead or nonexistent starsystem
//
//  starchart ("scanned") conventions:
//      0 -> 999        taken as is
//      -1              not yet scanned ("...")
//      1000            supernova ("///")
//      1001            starbase + ??? (".1.")

// ascii names of systems
extern const char *const Systemname[NINHAB];

// quadrant definition
extern struct quad Quad[NQUADS][NQUADS];

// defines for sector map  (below)
enum {
    EMPTY	= '.',
    STAR	= '*',
    BASE	= '#',
    YOURSHIP	= 'Y',
    PIRATE	= 'P',
    INHABIT	= '@',
    HOLE	= ' '
};

// current sector map
extern char Sect[NSECTS][NSECTS];

//}}}-------------------------------------------------------------------
//{{{ Ship devices

enum ShipDevice {
    WARP,	// warp engines
    SRSCAN,	// short range scanners
    LRSCAN,	// long range scanners
    PLASER,	// plaser control
    TORPED,	// torpedo control
    IMPULSE,	// impulse engines
    SHIELD,	// shield control
    COMPUTER,	// on board computer
    SSRADIO,	// subspace radio
    LIFESUP,	// life support systems
    SINS,	// Space Inertial Navigation System
    CLOAK,	// cloaking device
    XPORTER,	// transporter
    SHUTTLE,	// shuttlecraft
    NDEV = 16	// max number of devices
};

extern const char* const DeviceName [NDEV];

//}}}-------------------------------------------------------------------
//{{{ Events

enum EventType {
    E_NONE,
    E_LRTB,	// long range tractor beam
    E_PATSB,	// pirate attacks starbase
    E_PDESB,	// pirate destroys starbase
    E_ISSUE,	// distress call is issued
    E_ENSLV,	// pirates enslave a quadrant
    E_REPRO,	// a pirate is reproduced
    E_FIXDV,	// fix a device
    E_ATTACK,	// pirate attack during rest period
    E_SNAP,	// take a snapshot for time warp
    E_SNOVA,	// supernova occurs
    NEVENTS = 12,		// number of different event types
    E_GHOST	= 1<<6,		// ghost of a distress call if ssradio out
    E_HIDDEN	= E_GHOST<<1,	// event that is unreportable because ssradio out
    E_EVENT	= E_GHOST-1	// mask to get event code
};

struct event {
    uint8_t	x;
    uint8_t	y;		// coordinates
    float	date;		// trap date
    uint8_t	evcode;		// enum EventType
    uint8_t	systemname;	// starsystem name
};

// systemname conventions:
//      1 -> NINHAB     index into Systemname table for reported distress calls
//
// evcode conventions:
//      1 -> NEVENTS-1  event type
//      + E_HIDDEN      unreported (SSradio out)
//      + E_GHOST       actually already expired
//      0               unallocated

enum { MAXEVENTS = 24 };	// max number of concurrently pending events

extern struct event Event[MAXEVENTS];	// dynamic event list; one entry per pending event

//}}}-------------------------------------------------------------------
//{{{ Pirates

struct Pirate {
    uint8_t	x;
    uint8_t	y;
    int		power;		// power left
    float	dist;		// distance to your ship
    float	avgdist;	// average over this move
    char	srndreq;	// set if surrender has been requested
};

enum { MAX_QUAD_PIRATES = 9 };	// maximum pirates per quadrant

//}}}-------------------------------------------------------------------
//{{{ Ship information

// condition codes
enum { GREEN, DOCKED, YELLOW, RED };

enum {
    MAXBASES	= 9,	// maximum number of starbases in galaxy
    NBANKS	= 6	// number of plaser banks
};

struct xy {
    uint8_t	x;
    uint8_t	y;
};

// note that much of the stuff in the following structs CAN NOT
// be moved around!!!!

// information regarding the state of the starship
struct Ship_struct {
    float	warp;		// warp factor
    float	warp2;		// warp factor squared
    float	warp3;		// warp factor cubed
    char	shldup;		// shield up flag
    char	cloaked;	// set if cloaking device on
    int		energy;		// starship's energy
    int		shield;		// energy in shields
    float	reserves;	// life support reserves
    int		crew;		// ship's complement
    int		brigfree;	// space left in brig
    char	torped;		// torpedoes
    char	cloakgood;	// set if we have moved
    uint8_t	quadx;		// quadrant x coord
    uint8_t	quady;		// quadrant y coord
    uint8_t	sectx;		// sector x coord
    uint8_t	secty;		// sector y coord
    uint8_t	cond;		// condition code
    char	sinsbad;	// Space Inertial Navigation System condition
    int		distressed;	// number of distress calls
};
extern struct Ship_struct Ship;

// sinsbad is set if SINS is working but not calibrated

//}}}-------------------------------------------------------------------
//{{{ Game-related information

#define SPIRHUNT_SAVE_NAME	"spirhunt"

struct Game_struct {
    int		pirates_killed;	// number of pirates killed
    int		deaths;		// number of deaths onboard your ship
    char	negenbar;	// number of hits on negative energy barrier
    char	bases_killed;	// number of starbases destroyed
    int		stars_killed;	// number of stars killed
    char	killed;		// set if you were killed
    char	killinhab;	// number of inhabited starsystems killed
    char	snap;		// set if snapshot taken
    char	helps;		// number of help calls
    int		captives;	// total number of captives taken
};
extern struct Game_struct Game;

// per move information
struct Move_struct {
    char	free;		// set if a move is free
    char	endgame;	// end of game flag
    char	shldchg;	// set if shields changed this move
    char	newquad;	// set if just entered this quadrant
    char	resting;	// set if this move is a rest
    float	time;		// time used this move
};
extern struct Move_struct Move;

// parametric information
struct Param_struct {
    uint8_t	bases;		// number of starbases
    int8_t	pirates;	// number of pirates
    float	date;
    float	time;		// time left
    float	resource;	// galactic resources
    int		energy;		// starship's energy
    int		shield;		// energy in shields
    float	reserves;	// life support reserves
    int		crew;		// size of ship's complement
    int		brigfree;	// max possible number of captives
    uint8_t	torped;		// torpedoes carried
    float	damfac [NDEV];	// damage factor
    float	dockfac;	// docked repair time factor
    float	regenfac;	// regeneration factor
    int		stopengy;	// energy to do emergency stop
    int		shupengy;	// energy to put up shields
    int		piratepwr;	// pirate initial power
    int		warptime;	// time chewer multiplier
    float	plasfac;	// pirate plaser power eater factor
    char	moveprob[6];	// probability that a pirate moves
    float	movefac[6];	// pirate move distance multiplier
    float	eventdly [NEVENTS]; // event time multipliers
    float	navigcrud[2];	// navigation crudup factor
    int		cloakenergy;	// cloaking device energy per day
    float	hitfac;		// pirate attack factor
    int		pirate_crew;	// number of pirates in a crew
    float	srndrprob;	// surrender probability
    int		energylow;	// low energy mark (cond YELLOW)
};
extern struct Param_struct Param;

// other information kept in a snapshot
struct Now_struct {
    uint8_t	bases;		// number of starbases
    int8_t	pirates;	// number of pirates
    float	date;
    float	time;		// time left
    float	resource;	// galactic resources
    char	distressed;	// number of currently distressed quadrants
    struct event* eventptr [NEVENTS];	// pointer to event structs
    struct xy	base [MAXBASES];// locations of starbases
};
extern struct Now_struct Now;

// Other stuff, not dumped in a snapshot
struct Etc_struct {
    struct Pirate	pirate [MAX_QUAD_PIRATES];	// sorted pirate list
    short		npirates;		// number of pirates in this sector
    struct xy		starbase;		// starbase in current quadrant
    char		snapshot [sizeof Quad + sizeof Event + sizeof Now];	// snapshot for time warp
    char		statreport;		// set to get a status report on a srscan
};
extern struct Etc_struct Etc;

//
//      eventptr is a pointer to the event[] entry of the last
//      scheduled event of each type.  Zero if no such event scheduled.

// pirate move indicies
enum {
    PM_OB,	// Old quadrant, Before attack
    PM_OA,	// Old quadrant, After attack
    PM_EB,	// Enter quadrant, Before attack
    PM_EA,	// Enter quadrant, After attack
    PM_LB,	// Leave quadrant, Before attack
    PM_LA	// Leave quadrant, After attack
};

// "you lose" codes
enum LoseReason {
    L_STILLPLAYING,
    L_NOTIME,	// ran out of time
    L_NOENGY,	// ran out of energy
    L_DSTRYD,	// destroyed by a pirate
    L_NEGENB,	// ran into the negative energy barrier
    L_SUICID,	// destroyed in a nova
    L_SNOVA,	// destroyed in a supernova
    L_NOLIFE,	// life support termination
    L_NOHELP,	// you could not be rematerialized
    L_TOOFAST,	// pretty stupid going at warp 10
    L_STAR,	// ran into a star
    L_DSTRCT,	// self destructed
    L_CAPTURED,	// captured by pirates
    L_NOCREW	// you ran out of crew
};

//}}}-------------------------------------------------------------------
//{{{ Prototypes

// computer.c
void computer(int);

// events.c
int events (bool in_time_warp);
bool output_hidden_distress_calls (void);
struct event* schedule (enum EventType type, float offset, uint8_t x, uint8_t y, uint8_t sysname);
void unschedule (struct event* e);
struct event* xsched (enum EventType ev, int factor, uint8_t x, uint8_t y, uint8_t sysname);
void xresched (struct event* e, enum EventType ev, int factor);
inline static void reschedule (struct event* e, float offset)
    { e->date = Now.date + offset; }

// kill.c
void kill_pirate (int ix, int iy);
void kill_starbase (int qx, int qy);
void kill_starsystem (int x, int y, int f);
void kill_distress_call (unsigned x, unsigned y, bool f);
void nova(int, int);
void snova(int, int);

// move.c
float move_ship (int, int, float, float);
void setwarp (int);
void impulse (int);
void dowarp (int);
void warp (int, int, float);
void autover (void);

// pirates.c
void comp_pirate_dist(int);
void attack(int);
void move_pirates (int);
void capture(int);
struct Pirate* select_pirate (void);

// scan.c
const char *systemname(const struct quad *);
void srscan (int);
void lrscan (int);
void visual (int);

// ship.c
bool device_works (enum ShipDevice d);
void damage_report (int v);
void damage_device (enum ShipDevice dev, float dam);
bool device_damaged (enum ShipDevice d);
void announce_device_damage (enum ShipDevice dev);
void dock (int v);
void undock (int v);
void rest (int v);
void plasers (int v);
void torped (int v);
void shield (int f);
void help (int v);
void destruct (int v);

// spirhunt.c
void initquad (bool docked);
void sector (int* rx, int* ry);
void checkcond (void);
bool save (void);
long score (void);
_Noreturn void win (void);
_Noreturn void lose (enum LoseReason r);
float fnrand (void);

//}}}-------------------------------------------------------------------

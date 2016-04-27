// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
//
//  Global Declarations
//
//     Virtually all non-local variable declarations are made in this
//     file.  Exceptions are those things which are initialized, which
//     are defined in "externs.c", and things which are local to one
//     program file.
//
//     So far as I know, nothing in here must be preinitialized to
//     zero.
//

#pragma once
#include "../config.h"

// external function definitions

//********************  GALAXY  *************************

// galactic parameters
enum {
    NSECTS	= 10,	// dimensions of quadrant in sectors
    NQUADS	= 8,	// dimension of galazy in quadrants
    NINHAB	= 32	// number of quadrants which are inhabited
};

struct quad {			// definition for each quadrant
    unsigned char bases;	// number of bases in this quadrant
    char klings;		// number of Klingons in this quadrant
    char holes;			// number of black holes in this quadrant
    int scanned;		// star chart entry (see below)
    short stars;		// number of stars in this quadrant
    char qsystemname;		// starsystem name (see below)
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
    ENTERPRISE	= 'E',
    QUEENE	= 'Q',
    KLINGON	= 'K',
    INHABIT	= '@',
    HOLE	= ' '
};

// current sector map
extern char Sect[NSECTS][NSECTS];

//*********************** DEVICES *****************************

// device tokens
enum {
    WARP,	// warp engines
    SRSCAN,	// short range scanners
    LRSCAN,	// long range scanners
    PHASER,	// phaser control
    TORPED,	// photon torpedo control
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

// device names
struct device {
    const char *name;		// device name
    const char *person;		// the person who fixes it
};

extern const struct device Device[NDEV];

//**************************  EVENTS  ***************************

enum {
    E_LRTB = 1,	// long range tractor beam
    E_KATSB,	// Klingon attacks starbase
    E_KDESB,	// Klingon destroys starbase
    E_ISSUE,	// distress call is issued
    E_ENSLV,	// Klingons enslave a quadrant
    E_REPRO,	// a Klingon is reproduced
    E_FIXDV,	// fix a device
    E_ATTACK,	// Klingon attack during rest period
    E_SNAP,	// take a snapshot for time warp
    E_SNOVA,	// supernova occurs
    NEVENTS = 12,	// number of different event types
    E_GHOST = 0100,	// ghost of a distress call if ssradio out
    E_HIDDEN = 0200,	// event that is unreportable because ssradio out
    E_EVENT = 077,	// mask to get event code
};

struct event {
    unsigned char x, y;		// coordinates
    double date;		// trap stardate
    char evcode;		// event type
    unsigned char systemname;	// starsystem name
};

// systemname conventions:
//      1 -> NINHAB     index into Systemname table for reported distress calls
//
// evcode conventions:
//      1 -> NEVENTS-1  event type
//      + E_HIDDEN      unreported (SSradio out)
//      + E_GHOST       actually already expired
//      0               unallocated

enum { MAXEVENTS = 25 };	// max number of concurrently pending events

extern struct event Event[MAXEVENTS];	// dynamic event list; one entry per pending event

//****************************  KLINGONS  ******************************

struct kling {
    unsigned char x, y;	// coordinates
    int power;		// power left
    double dist;	// distance to Enterprise
    double avgdist;	// average over this move
    char srndreq;	// set if surrender has been requested
};

enum { MAXKLQUAD = 9 };	// maximum klingons per quadrant

//********************* MISCELLANEOUS **************************

// condition codes
enum { GREEN, DOCKED, YELLOW, RED };

enum {
    MAXBASES	= 9,	// maximum number of starbases in galaxy
    MAXDISTR	= 5,	// maximum concurrent distress calls
    NBANKS	= 6	// number of phaser banks
};

struct xy {
    unsigned char x, y;		// coordinates
};

//      note that much of the stuff in the following structs CAN NOT
//      be moved around!!!!

// information regarding the state of the starship
struct Ship_struct {
    double warp;		// warp factor
    double warp2;		// warp factor squared
    double warp3;		// warp factor cubed
    char shldup;		// shield up flag
    char cloaked;		// set if cloaking device on
    int energy;			// starship's energy
    int shield;			// energy in shields
    double reserves;		// life support reserves
    int crew;			// ship's complement
    int brigfree;		// space left in brig
    char torped;		// torpedoes
    char cloakgood;		// set if we have moved
    int quadx;			// quadrant x coord
    int quady;			// quadrant y coord
    int sectx;			// sector x coord
    int secty;			// sector y coord
    unsigned char cond;		// condition code
    char sinsbad;		// Space Inertial Navigation System condition
    const char *shipname;	// name of current starship
    char ship;			// current starship
    int distressed;		// number of distress calls
};
extern struct Ship_struct Ship;

// sinsbad is set if SINS is working but not calibrated

// game related information, mostly scoring
struct Game_struct {
    int killk;			// number of klingons killed
    int deaths;			// number of deaths onboard Enterprise
    char negenbar;		// number of hits on negative energy barrier
    char killb;			// number of starbases killed
    int kills;			// number of stars killed
    char skill;			// skill rating of player
    char length;		// length of game
    char killed;		// set if you were killed
    char killinhab;		// number of inhabited starsystems killed
    char tourn;			// set if a tournament game
    char passwd[15];		// game password
    char snap;			// set if snapshot taken
    char helps;			// number of help calls
    int captives;		// total number of captives taken
};
extern struct Game_struct Game;

// per move information
struct Move_struct {
    char free;			// set if a move is free
    char endgame;		// end of game flag
    char shldchg;		// set if shields changed this move
    char newquad;		// set if just entered this quadrant
    char resting;		// set if this move is a rest
    double time;		// time used this move
};
extern struct Move_struct Move;

// parametric information
struct Param_struct {
    unsigned char bases;	// number of starbases
    char klings;		// number of klingons
    double date;		// stardate
    double time;		// time left
    double resource;		// Federation resources
    int energy;			// starship's energy
    int shield;			// energy in shields
    double reserves;		// life support reserves
    int crew;			// size of ship's complement
    int brigfree;		// max possible number of captives
    char torped;		// photon torpedos
    double damfac[NDEV];	// damage factor
    double dockfac;		// docked repair time factor
    double regenfac;		// regeneration factor
    int stopengy;		// energy to do emergency stop
    int shupengy;		// energy to put up shields
    int klingpwr;		// Klingon initial power
    int warptime;		// time chewer multiplier
    double phasfac;		// Klingon phaser power eater factor
    char moveprob[6];		// probability that a Klingon moves
    double movefac[6];		// Klingon move distance multiplier
    double eventdly[NEVENTS];	// event time multipliers
    double navigcrud[2];	// navigation crudup factor
    int cloakenergy;		// cloaking device energy per stardate
    double damprob[NDEV];	// damage probability
    double hitfac;		// Klingon attack factor
    int klingcrew;		// number of Klingons in a crew
    double srndrprob;		// surrender probability
    int energylow;		// low energy mark (cond YELLOW)
};
extern struct Param_struct Param;

// Sum of damage probabilities must add to 1000

// other information kept in a snapshot
struct Now_struct {
    unsigned char bases;	// number of starbases
    char klings;		// number of klingons
    double date;		// stardate
    double time;		// time left
    double resource;		// Federation resources
    char distressed;		// number of currently distressed quadrants
    struct event *eventptr[NEVENTS];	// pointer to event structs
    struct xy base[MAXBASES];	// locations of starbases
};
extern struct Now_struct Now;

// Other stuff, not dumped in a snapshot
struct Etc_struct {
    struct kling klingon[MAXKLQUAD];	// sorted Klingon list
    short nkling;		// number of Klingons in this sector
    // < 0 means automatic override mode
    char fast;			// set if speed > 300 baud
    struct xy starbase;		// starbase in current quadrant
    char snapshot[sizeof Quad + sizeof Event + sizeof Now];	// snapshot for time warp
    char statreport;		// set to get a status report on a srscan
};
extern struct Etc_struct Etc;

//
//      eventptr is a pointer to the event[] entry of the last
//      scheduled event of each type.  Zero if no such event scheduled.

// Klingon move indicies
enum {
    KM_OB,	// Old quadrant, Before attack
    KM_OA,	// Old quadrant, After attack
    KM_EB,	// Enter quadrant, Before attack
    KM_EA,	// Enter quadrant, After attack
    KM_LB,	// Leave quadrant, Before attack
    KM_LA	// Leave quadrant, After attack
};

// you lose codes
enum {
    L_STILLPLAYING,
    L_NOTIME,	// ran out of time
    L_NOENGY,	// ran out of energy
    L_DSTRYD,	// destroyed by a Klingon
    L_NEGENB,	// ran into the negative energy barrier
    L_SUICID,	// destroyed in a nova
    L_SNOVA,	// destroyed in a supernova
    L_NOLIFE,	// life support died (so did you)
    L_NOHELP,	// you could not be rematerialized
    L_TOOFAST,	// pretty stupid going at warp 10
    L_STAR,	// ran into a star
    L_DSTRCT,	// self destructed
    L_CAPTURED,	// captured by Klingons
    L_NOCREW	// you ran out of crew
};

//*****************  COMPILE OPTIONS  **********************

// Trace info
#define	xTRACE		1
extern int Trace;

// abandon.c
void abandon(int);

// attack.c
void attack(int);

// autover.c
void autover(void);

// capture.c
void capture(int);
struct kling *selectklingon(void);

// cgetc.c
char cgetc(int);

// check_out.c
int check_out(int);

// checkcond.c
void checkcond(void);

// compkl.c
void compkldist(int);

// computer.c
void computer(int);

// damage.c
void damage(int, double);

// damaged.c
int damaged(int);

// dcrept.c
void dcrept(int);

// destruct.c
void destruct(int);

// dock.c
void dock(int);
void undock(int);

// dumpgame.c
void dumpgame(int);
int restartgame(void);

// dumpme.c
void dumpme(int);

// dumpssradio.c
int dumpssradio(void);

// events.c
int events(int);

// externs.c

// getcodi.c
int getcodi(int *, double *);

// help.c
void help(int);

// impulse.c
void impulse(int);

// initquad.c
void initquad(int);
void sector(int *, int *);

// kill.c
void killk(int, int);
void killb(int, int);
void kills(int, int, int);
void killd(int, int, int);

// klmove.c
void klmove(int);

// lose.c
void lose(int) NORETURN;

// lrscan.c
void lrscan(int);

// move.c
double move(int, int, double, double);

// nova.c
void nova(int, int);

// out.c
void out(int);

// phaser.c
void phaser(int);

// play.c
void myreset(int) NORETURN;
void play(void) NORETURN;

// ram.c
void ram(int, int);

// ranf.c
int ranf(int);
double franf(void);

// rest.c
void rest(int);

// schedule.c
struct event *schedule(int, double, int, int, int);
void reschedule(struct event *, double);
void unschedule(struct event *);
struct event *xsched(int, int, int, int, int);
void xresched(struct event *, int, int);

// score.c
long score(void);

// setup.c
void setup(void);

// setwarp.c
void setwarp(int);

// shield.c
void shield(int);

// snova.c
void snova(int, int);

// srscan.c
void srscan(int);

// systemname.c
const char *systemname(const struct quad *);

// torped.c
void torped(int);

// visual.c
void visual(int);

// warp.c
void dowarp(int);
void warp(int, int, double);

// win.c
void win(void) NORETURN;

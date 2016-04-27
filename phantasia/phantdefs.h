// phantdefs.h - important constants for Phantasia
#pragma once
#include "../config.h"

// ring constants
enum {
    R_NONE,	// no ring
    R_NAZREG,	// regular Nazgul ring (expires)
    R_DLREG,	// regular Dark Lord ring (does not expire)
    R_BAD,	// bad ring
    R_SPOILED	// ring which has gone bad
};

// status constants
// We must #undef these first since setup.c includes headers that reserve S_*
enum {
    S_NOTUSED,	// record not in use
    S_OFF,	// not playing
    S_PLAYING,	// playing - nothing else
    S_CLOAKED,	// playing - cloaked
    S_INBATTLE,	// playing - in battle
    S_MONSTER,	// playing - fighting monster
    S_TRADING,	// playing - at a trading post
    S_HUNGUP	// error occurred with character
};

// tampered constants
enum {
    T_OFF,	// nothing
    T_NRGVOID,	// hit an energy void
    T_GRAIL,	// landed on the holy grail
    T_TRANSPORT,// transported by king
    T_BESTOW,	// gold bestowed by king
    T_CURSED,	// cursed by king
    T_MONSTER,	// monster lobbed by valar
    T_BLESSED,	// blessed by valar
    T_RELOCATE,	// moved by valar
    T_HEAL,	// healed by valar
    T_VAPORIZED,// vaporized by wizard
    T_EXVALAR	// no longer valar
};

// inter-terminal battle status constants
enum {
    I_OFF,	// nothing
    I_RAN,	// ran away
    I_STUCK,	// tried to run unsuccessfully
    I_BLEWIT,	// tried to luckout unsuccessfully
    I_KILLED	// killed foe
};

// constants for altering coordinates
enum {
    A_SPECIFIC,	// coordinates specified
    A_FORCED,	// coordinates specified, ignore Beyond
    A_NEAR,	// coordinates not specified, move near
    A_FAR	// coordinates not specified, move far
};

// constants for character types
enum {
    C_MAGIC,	// magic user
    C_FIGHTER,	// fighter
    C_ELF,	// elf
    C_DWARF,	// dwarf
    C_HALFLING,	// halfling
    C_EXPER,	// experimento
    C_SUPER	// super being
};

// constants for special character types
enum {
    SC_NONE,	// not a special character
    SC_KING,	// king
    SC_COUNCIL,	// council of the wise
    SC_VALAR,	// valar
    SC_EXVALAR	// ex-valar
};

// special monster constants
enum {
    SM_NONE,		// nothing special
    SM_UNICORN,		// unicorn
    SM_MODNAR,		// Modnar
    SM_MIMIC,		// mimic
    SM_DARKLORD,	// Dark Lord
    SM_LEANAN,		// Leanan-Sidhe
    SM_SARUMAN,		// Saruman
    SM_THAUMATURG,	// thaumaturgist
    SM_BALROG,		// balrog
    SM_VORTEX,		// vortex
    SM_NAZGUL,		// nazgul
    SM_TIAMAT,		// Tiamat
    SM_KOBOLD,		// kobold
    SM_SHELOB,		// Shelob
    SM_FAERIES,		// assorted faeries
    SM_LAMPREY,		// lamprey
    SM_SHRIEKER,	// shrieker
    SM_BONNACON,	// bonnacon
    SM_SMEAGOL,		// Smeagol
    SM_SUCCUBUS,	// succubus
    SM_CERBERUS,	// Cerberus
    SM_UNGOLIANT,	// Ungoliant
    SM_JABBERWOCK,	// jabberwock
    SM_MORGOTH,		// Morgoth
    SM_TROLL,		// troll
    SM_WRAITH		// wraith
};

// constants for spells
#define	ML_ALLORNOTHING	0.0    // magic level for 'all or nothing'
#define MM_ALLORNOTHING	1.0    // mana used for 'all or nothing'
#define	ML_MAGICBOLT	5.0    // magic level for 'magic bolt'
#define	ML_FORCEFIELD	15.0   // magic level for 'force field'
#define	MM_FORCEFIELD	30.0   // mana used for 'force field'
#define	ML_XFORM	25.0   // magic level for 'transform'
#define	MM_XFORM	50.0   // mana used for 'transform'
#define	ML_INCRMIGHT	35.0   // magic level for 'increase might'
#define	MM_INCRMIGHT	75.0   // mana used for 'increase might'
#define	ML_INVISIBLE	45.0   // magic level for 'invisibility'
#define	MM_INVISIBLE	90.0   // mana used for 'invisibility'
#define	ML_XPORT	60.0   // magic level for 'transport'
#define	MM_XPORT	125.0  // mana used for 'transport'
#define	ML_PARALYZE	75.0   // magic level for 'paralyze'
#define	MM_PARALYZE	150.0  // mana used for 'paralyze'
#define	MM_SPECIFY	1000.0 // mana used for 'specify'
#define ML_CLOAK	20.0   // magic level for 'cloak'
#define	MEL_CLOAK	7.0    // experience level for 'cloak'
#define	MM_CLOAK	35.0   // mana used for 'cloak'
#define	ML_TELEPORT	40.0   // magic level for 'teleport'
#define	MEL_TELEPORT	12.0   // experience level for 'teleport'
#define	MM_INTERVENE	1000.0 // mana used to 'intervene'

// some miscellaneous constants
enum {
    SZ_DATABUF		= 100,	// size of input buffer
    SZ_NAME		= 21,	// size of player name (incl. trailing nul)
    SZ_PASSWORD		= 9,	// size of password (incl. trailing nul)
    SZ_LOGIN		= 9	// size of login (incl. trailing nul)
};

enum {
    N_DAYSOLD		= 21,	// number of days old for purge
    N_AGE		= 500	// age to degenerate ratio
};
#define	N_GEMVALUE	(1000.0)	// number of gold pieces to gem ratio
#define	N_TAXAMOUNT	(7.0)  // tax percent

#define	D_BEYOND	(1.1e6)	// distance to beyond point of no return
#define D_EXPER		(2000.0)	// distance experimentos are allowed

#define	CH_MARKDELETE	'\001' // used to alter name of deleted players
#define	CH_KILL		'\030' // kill character (ctrl-X)
#define	CH_ERASE	'\010' // erase character (ctrl-H)
#define	CH_NEWLINE	'\n'   // newline
#define	CH_REDRAW	'\014' // redraw screen character (ctrl-L)

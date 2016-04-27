// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include	"trek.h"

// global variable definitions

const struct device Device[NDEV] = {
    {"warp drive", "Scotty"},
    {"S.R. scanners", "Scotty"},
    {"L.R. scanners", "Scotty"},
    {"phasers", "Sulu"},
    {"photon tubes", "Sulu"},
    {"impulse engines", "Scotty"},
    {"shield control", "Sulu"},
    {"computer", "Spock"},
    {"subspace radio", "Uhura"},
    {"life support", "Scotty"},
    {"navigation system", "Chekov"},
    {"cloaking device", "Scotty"},
    {"transporter", "Scotty"},
    {"shuttlecraft", "Scotty"},
    {"*ERR 14*", "Nobody"},
    {"*ERR 15*", "Nobody"}
};

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

struct quad Quad[NQUADS][NQUADS];

// current sector map
char Sect[NSECTS][NSECTS];

struct event Event[MAXEVENTS];	// dynamic event list; one entry per pending event

struct Ship_struct Ship;
struct Game_struct Game;
struct Move_struct Move;
struct Param_struct Param;
struct Now_struct Now;
struct Etc_struct Etc;

int Trace;

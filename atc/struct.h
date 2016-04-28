// Copyright (c) 1987 by Ed James <edjames@berkeley.edu>
// This file is free software, distributed under the BSD license.

#pragma once
#include "def.h"

typedef struct {
    int x, y;
    int dir;			// used only sometimes
} SCREEN_POS;

typedef struct {
    SCREEN_POS p1, p2;
} LINE;

typedef SCREEN_POS EXIT;
typedef SCREEN_POS BEACON;
typedef SCREEN_POS AIRPORT;

typedef struct {
    const char* name;
    int width;
    int height;
    int update_secs;
    int newplane_time;
    int num_exits;
    int num_lines;
    int num_beacons;
    int num_airports;
    const EXIT* exit;
    const LINE* line;
    const BEACON* beacon;
    const AIRPORT* airport;
} C_SCREEN;

typedef struct plane {
    struct plane *next, *prev;
    int status;
    int plane_no;
    int plane_type;
    int orig_no;
    int orig_type;
    int dest_no;
    int dest_type;
    int altitude;
    int new_altitude;
    int dir;
    int new_dir;
    int fuel;
    int xpos;
    int ypos;
    int delayd;
    int delayd_no;
} PLANE;

typedef struct {
    PLANE *head, *tail;
} LIST;

typedef struct {
    char name[10];
    char host[256];
    char game[256];
    int planes;
    int time;
    int real_time;
} SCORE;

#define SCORE_SCANF_FMT	"%9s %255s %255s %d %d %d"

typedef struct displacement {
    int dx;
    int dy;
} DISPLACEMENT;

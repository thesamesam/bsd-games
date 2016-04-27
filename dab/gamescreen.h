// Copyright (c) 2003 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.
// screen.h: Screen base class

#pragma once
#include <stdlib.h>

class PLAYER;

class GAMESCREEN {
public:
    enum EDGE {
	GS_HLINE,
	GS_VLINE,
	GS_ULCORNER,
	GS_URCORNER,
	GS_LLCORNER,
	GS_LRCORNER,
	GS_LTEE,
	GS_RTEE,
	GS_TTEE,
	GS_BTEE,
	GS_PLUS
    };
public:
			GAMESCREEN (void) = default;
    virtual		~GAMESCREEN (void) {}
    virtual void	clean (void) = 0;			// Clear screen
    virtual void	moveto (size_t y, size_t x) = 0;	// Move to x, y
    virtual void	addsym (const int sym) = 0;		// Add character symbol
    virtual void	addedge (const int sym) = 0;		// Add character symbol
    virtual void	redraw (void) = 0;			// Refresh
    virtual int		getinput (void) = 0;			// Get user input
    virtual void	bell (void) = 0;			// Beep
    virtual void	score (size_t, const PLAYER&) = 0;	// Post current score
    virtual void	games (size_t, const PLAYER&) = 0;	// Post games won
    virtual void	total (size_t, const PLAYER&) = 0;	// Post total score
    virtual void	ties (const PLAYER& p) = 0;		// Post tie games
};

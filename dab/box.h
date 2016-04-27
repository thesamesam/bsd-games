// Copyright (c) 2003 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.
// box.cc: Single box utilities; A box is an entity with four edges, four
//	  corners, and a center that maps directly to a board

#pragma once
#include <stdlib.h>

class BOARD;

class POINT {
public:
    int x;
    int y;
};

class BOX {
public:
    enum EDGE {
	first,
	top = first,
	bottom,
	left,
	right,
	last
    };
public:
		BOX (size_t py, size_t px, BOARD& b);
    void	reset (void);		// Clear a box
    void	paint (void);		// Paint a box
    int&	name (void);
    int&	edge (int e);
    void	set (int e);		// Edge maniputations
    void	clr (int e);
    int		isset (int e) const;
    int		count (void) const;	// Count the number of edges in use
    // Useful constants
    // Relative coordinates of the edges from the center of the box.
    static const POINT edges[BOX::last];
    // Relative coordinates of the corners from the center of the box.
    static const POINT corners[BOX::last];
    // Character symbols of the four edges
    static const int syms[BOX::last];
private:
    void	addcorner (size_t y, size_t x);	// add a corner character
private:
    size_t	_centerx;	// Coordinates of the center in board units
    size_t	_centery;
    BOARD&	_b;		// The board we refer to
};

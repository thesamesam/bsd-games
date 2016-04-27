// Copyright (c) 2003 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.
// random.h: Randomizer; returns a random sequence of values from [0..fx) 
//	     returning each value exactly once. After fx calls it returns fx.

#pragma once
#include <stdlib.h>

class RANDOM {
public:
		RANDOM (size_t fx);
		~RANDOM (void);
    size_t	operator() (void);	// Next random
    void	clear (void);		// Reset
private:
    inline int	isset (size_t z)	{ return (_bm[z >> 3] & (1 << (z & 7))) != 0; }
    inline void	set (size_t z)		{ _bm[z >> 3] |= (1 << (z & 7)); }
private:
    char*	_bm;	// Bitmap indicating the numbers used
    size_t	_nv;	// Number of values returned so far
    size_t	_bs;	// Maximum value
};

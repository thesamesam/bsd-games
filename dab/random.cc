// Copyright (c) 2003 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

// random.cc: Randomizer for the dots program

#include "random.h"
#include <time.h>

RANDOM::RANDOM (size_t ns)
:_bs (ns)
{
    _bm = new char[(_bs >> 3) + 1];
    clear();
}

RANDOM::~RANDOM (void)
{
    delete[] _bm;
}

// Reinitialize
void RANDOM::clear (void)
{
    _nv = 0;
    ::srand48 (::time(nullptr));
    ::memset (_bm, 0, (_bs >> 3) + 1);
}

// Return the next random value
size_t RANDOM::operator() (void)
{
    // No more values
    if (_nv == _bs)
	return _bs;
    for (;;) {
	size_t r = ::lrand48();
	size_t z = r % _bs;
        if (!isset(z)) {
	    set(z);
	    _nv++;
	    return z;
	}
    }
}

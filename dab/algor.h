// Copyright (c) 2003 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.
// algor.h: Computer's algorithm

#pragma once
#include "player.h"

class BOARD;
class BOX;

class ALGOR : public PLAYER {
public:
			ALGOR (const char c);
    // Return a proposed move in (y, x, dir)
    virtual void	play (const BOARD& b, size_t& y, size_t& x, int& dir) override;

private:
    // Closure searches
    int		find_closure(size_t& y, size_t& x, int& dir, BOARD& b);
    int		find_max_closure(size_t& y, size_t& x, int& dir, const BOARD& b);
    int		find_min_closure1(size_t& y, size_t& x, int& dir, const BOARD& b, int last);
    int		find_min_closure(size_t& y, size_t& x, int& dir, const BOARD& b);

    // Move searches
    int		find_good_turn(size_t& y, size_t& x, int& dir, const BOARD& b);
    int		find_bad_turn(size_t& y, size_t& x, int& dir, BOARD& b, int last);

    // Move Attempts
    int		try_bad_turn(BOX& box, size_t& y, size_t& x, int& dir, BOARD& b, int last);
    int		try_good_turn(BOX& box, size_t y, size_t x, int& dir, BOARD& b);

    // Utils
    size_t	count_closure(size_t& y, size_t& x, int& dir, BOARD& b);

#ifdef notyet
    size_t	find_single(void);
#endif
private:
    size_t	_edge1;
    size_t	_edge2;
    size_t	_maxedge;
};

// Copyright (c) 2003 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.
// player.h: Player base class

#pragma once
#include "../config.h"

class BOARD;

class PLAYER {
public:
			PLAYER(char who);
    inline virtual	~PLAYER (void) {}
    virtual void	play (const BOARD& b, size_t& y, size_t& x, int& dir) = 0;
    void		init (void);
    int			domove (BOARD& b);
    inline char		getWho (void) const	{ return _who; }
    inline size_t	getScore (void) const	{ return _score; }
    inline size_t	getTotal (void) const	{ return _total; }
    inline size_t	getGames (void) const	{ return _games; }
    inline size_t	getTies (void) const	{ return _ties;   }
    void		wl (size_t sc);
private:
    char		_who;
    size_t		_score;
    size_t		_total;
    size_t		_games;
    size_t		_ties;
};

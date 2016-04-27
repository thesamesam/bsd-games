// Copyright (c) 2003 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.
// ttyscrn.h: Curses based screen for dots

#pragma once
#include "gamescreen.h"

class TTYSCRN : public GAMESCREEN {
public:
    static TTYSCRN*	create(int acs, size_t y, size_t x);
    virtual		~TTYSCRN();
    virtual void	clean(void) override;
    virtual void	moveto(size_t y, size_t x) override;
    virtual void	addsym(const int sym) override;
    virtual void	addedge(const int sym) override;
    virtual void	redraw(void) override;
    virtual void	bell(void) override;
    virtual int		getinput(void) override;
    virtual void	score(size_t s, const PLAYER& p) override;
    virtual void	games(size_t s, const PLAYER& p) override;
    virtual void	total(size_t s, const PLAYER& p) override;
    virtual void	ties(const PLAYER& p) override;
private:
    enum {
	offsx = 2,	// board x offset from top left corner
	offsy = 2,	// board y offset from top left corner
	offsscore = 0,	// score y offset from top of the board
	offstotal = 3,	// total y offset from top of the board
	offsgames = 6,	// games y offset from top of the board
	offsties = 8	// ties y offset from top of the board
    };
    size_t	_sx, _sy;	// board size
    size_t	_tx, _ty;	// tty size
    int		_acs;		// do we want acs?
};

// Copyright (c) 2003 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.
// human.h: Human input interface

#pragma once
#include <stdlib.h>
#include "player.h"

class BOARD;

class HUMAN : public PLAYER {
public:
			HUMAN (const char c);
    // Return move in y, x, and dir
    virtual void	play (const BOARD& b, size_t& y, size_t& x, int& dir) override;
private:
    size_t	_curx, _cury;	// Current cursor position
};

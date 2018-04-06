// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.
//
// This cannot be part of tty.c (as it was earlier) since on some
// systems (e.g. MUNIX) the include files <termio.h> and <sgtty.h> define the
// same constants, and the C preprocessor complains.
//
#include "hack.h"
#include "extern.h"
#include <termios.h>

struct termios termios;

void getioctls(void)
{
    tcgetattr (STDIN_FILENO, &termios);
}

void setioctls(void)
{
    tcsetattr (STDIN_FILENO, TCSADRAIN, &termios);
}

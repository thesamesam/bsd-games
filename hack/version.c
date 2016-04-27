// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#include "hack.h"
#include "extern.h"

#ifdef QUEST
    #define HACK_PROGNAME "Quest"
#else
    #define HACK_PROGNAME "Hack"
#endif

int doversion (void)
{
    pline (HACK_PROGNAME " in " BSDGAMES_VERSTRING);
    return 0;
}

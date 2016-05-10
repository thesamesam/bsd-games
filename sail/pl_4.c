// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"
#include "player.h"

void changesail(void)
{
    int rig, full;

    rig = mc->rig1;
    full = mf->FS;
    if (windspeed == 6 || (windspeed == 5 && mc->class > 4))
	rig = 0;
    if (mc->crew3 && rig) {
	if (!full) {
	    if (sgetch("Increase to Full sails? ", (struct ship *) 0, 1) == 'y') {
		changed = 1;
		Write(W_FS, ms, 1, 0, 0, 0);
	    }
	} else {
	    if (sgetch("Reduce to Battle sails? ", (struct ship *) 0, 1) == 'y') {
		Write(W_FS, ms, 0, 0, 0, 0);
		changed = 1;
	    }
	}
    } else if (!rig)
	Msg("Sails rent to pieces");
}

void acceptsignal(void)
{
    char buf[60];
    char *p = buf;

    *p++ = '"';
    sgetstr("Message? ", p, sizeof buf - 2);
    while (*p++);
    p[-1] = '"';
    *p = 0;
    Writestr(W_SIGNAL, ms, buf);
}

void lookout(void)
{
    struct ship *sp;
    char buf[3];
    char c;

    sgetstr("What ship? ", buf, sizeof buf);
    foreachship(sp) {
	c = *countryname[sp->nationality];
	if ((tolower((unsigned char) c) == *buf || colours(sp) == *buf)
	    && (sp->file->stern == buf[1] || sterncolour(sp) == buf[1]
		|| buf[1] == '?')) {
	    eyeball(sp);
	}
    }
}

const char *saywhat(struct ship *sp, int flag)
{
    if (sp->file->captain[0])
	return sp->file->captain;
    else if (sp->file->struck)
	return "(struck)";
    else if (sp->file->captured != 0)
	return "(captured)";
    else if (flag)
	return "(available)";
    else
	return "(computer)";
}

void eyeball(struct ship *ship)
{
    if (ship->file->dir != 0) {
	Msg("Sail ho! (range %d, %s)", range(ms, ship), saywhat(ship, 0));
	int i = portside(ms, ship, 1) - mf->dir;
	if (i <= 0)
	    i += 8;
	Signal("$$ %s %s %s.", ship, countryname[ship->nationality], classname[ship->specs->class], directionname[i]);
    }
}

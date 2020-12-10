// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "spirhunt.h"
#include "getpar.h"

// get string parameter
void getstrpar (const char* s, char* r, unsigned l)
{
    printf (BOLD_ON "%s:" BOLD_OFF " ", s);
    r[0] = 0;
    fgets (r, l, stdin);
    for (unsigned br = strlen (r); br && r[--br] == '\n';)
	r[br] = 0;
}

// Get integer parameter
int getintpar (const char* s)
{
    char buf [16];
    getstrpar (s, ArrayBlock(buf));
    return atoi (buf);
}

// Get floating parameter
float getfltpar (const char* s)
{
    char buf [16];
    getstrpar (s, ArrayBlock(buf));
    return atof (buf);
}

// get yes/no parameter
bool getynpar (const char* s)
{
    static const struct cvntab Yntab[] = {
	{"y", "es",	(cmdfun) true,	true},
	{"n", "o",	(cmdfun) false,	false},
	{}
    };
    const struct cvntab* a = getcodpar(s, Yntab);
    return a ? a->value2 : false;
}

// Get coded parameter
const struct cvntab* getcodpar (const char* s, const struct cvntab* tab)
{
    for (;;) {
	char buf [16] = {};
	getstrpar (s, ArrayBlock(buf));
	if (!buf[0])
	    return NULL;	// cancel command

	// Print help, if requested
	if (buf[0] == '?') {
	    int c = 4;
	    for (const struct cvntab* r = tab; r->abrev; ++r) {
		char helpbuf [32];
		strcpy (helpbuf, r->abrev);
		strcat (helpbuf, r->full);
		printf ("%14.14s", helpbuf);
		if (--c > 0)
		    continue;
		c = 4;
		printf ("\n");
	    }
	    if (c != 4)
		printf ("\n");
	    continue;
	}

	// Search for command in the table
	const struct cvntab* r = tab;
	for (; r->abrev; ++r) {
	    const char* p = buf;
	    const char* q = r->abrev;
	    for (; *q; ++q)
		if (*p++ != *q)
		    break;
	    if (!*q) {
		for (q = r->full; *p && *q; ++q, ++p)
		    if (*p != *q)
			break;
		if (!*p || !*q)
		    break;
	    }
	}

	// check for not found
	if (r->abrev)
	    return r;
	else
	    printf ("invalid input; ? for valid inputs\n");
    }
}

// Get destination
//
// The user is asked for the destination coordinate SS or QQSS
// Return values with quad > NQUADS indicate error
//
struct xy getdest (void)
{
    struct xy dest = { UINT8_MAX, UINT8_MAX };
    char dbuf[8] = {};
    getstrpar ("Destination", ArrayBlock(dbuf)-1);
    if (!dbuf[0])
	return dest;

    unsigned dlen = 0;
    while (dlen < 5 && ((dbuf[dlen] >= '0' && dbuf[dlen] <= '9') || dbuf[dlen] == '.'))
	++dlen;
    if (dlen < 2)
	return dest;

    struct xy quad = Ship.quad, sect = {};
    const char* d = dbuf;
    if (dlen >= 4) {
	quad.x = *d++ - '0';
	quad.y = *d++ - '0';
	if (quad.x >= NQUADS || quad.y >= NQUADS)
	    return dest;
    }
    if (*d == '.')
	++d;
    sect.x = *d++ - '0';
    sect.y = *d++ - '0';
    if (sect.x >= NSECTS || sect.y >= NSECTS)
	return dest;
    return dest = absolute_sector_coord (quad, sect);
}

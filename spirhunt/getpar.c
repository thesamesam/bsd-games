// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "spirhunt.h"
#include "getpar.h"

//----------------------------------------------------------------------

static bool testterm (void);

//----------------------------------------------------------------------

// Get integer parameter
int getintpar (const char* s)
{
    for (;;) {
	if (testnl() && s)
	    printf ("%s: ", s);
	int n;
	int i = scanf ("%d", &n);
	if (i < 0)
	    exit (EXIT_FAILURE);
	if (i > 0 && testterm())
	    return n;
	printf ("Invalid input; please enter an integer\n");
	skiptonl(0);
    }
}

// Get floating parameter
float getfltpar (const char* s)
{
    for (;;) {
	if (testnl() && s)
	    printf ("%s: ", s);
	float d;
	int i = scanf ("%f", &d);
	if (i < 0)
	    exit (EXIT_FAILURE);
	if (i > 0 && testterm())
	    return d;
	printf ("Invalid input; please enter a float\n");
	skiptonl(0);
    }
}

// get yes/no parameter
bool getynpar (const char* s)
{
    static const struct cvntab Yntab[] = {
	{"y", "es",	(cmdfun) true,	true},
	{"n", "o",	(cmdfun) false,	false},
	{}
    };
    return getcodpar(s, Yntab)->value2;
}

// Get coded parameter
const struct cvntab* getcodpar (const char* s, const struct cvntab* tab)
{
    bool flag = false;
    for (;;) {
	bool f = testnl();
	flag |= f;
	if (flag)
	    printf ("%s: ", s);
	if (f)
	    getchar();	       // throw out the newline
	scanf ("%*[ \t;]");

	char input [32];
	int c = scanf ("%31[^ \t;\n]", input);
	if (c < 0)
	    exit (EXIT_FAILURE);
	if (!c)
	    continue;
	flag = true;

	// if command list, print four per line
	if (input[0] == '?' && input[1] == 0) {
	    c = 4;
	    for (const struct cvntab* r = tab; r->abrev; ++r) {
		strcpy (input, r->abrev);
		strcat (input, r->full);
		printf ("%14.14s", input);
		if (--c > 0)
		    continue;
		c = 4;
		printf ("\n");
	    }
	    if (c != 4)
		printf ("\n");
	    continue;
	}

	// search for in table
	const struct cvntab* r = tab;
	for (; r->abrev; ++r) {
	    const char* p = input;
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
	if (!r->abrev) {
	    printf ("invalid input; ? for valid inputs\n");
	    skiptonl(0);
	} else
	    return r;
    }
}

// get string parameter
void getstrpar (const char* s, char* r, unsigned l, const char* t)
{
    if (!t)
	t = " \t\n;";
    char format [20];
    snprintf (ArrayBlock(format), "%%%u[^%s]", l, t);
    for (;;) {
	bool f = testnl();
	if (f && s)
	    printf ("%s: ", s);
	if (f)
	    getchar();
	scanf ("%*[\t ;]");
	int i = scanf (format, r);
	if (i < 0)
	    exit (EXIT_FAILURE);
	if (i != 0)
	    return;
    }
}

// test if newline is next valid character
bool testnl (void)
{
    char c;
    while ((c = getchar()) != '\n') {
	if ((c >= '0' && c <= '9') || c == '.' || c == '!' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '-') {
	    ungetc (c, stdin);
	    return false;
	}
    }
    ungetc (c, stdin);
    return true;
}

// scan for newline
void skiptonl (int c)
{
    while (c != '\n')
	if (!(c = getchar()))
	    return;
    ungetc ('\n', stdin);
}

// test for valid terminator
static bool testterm (void)
{
    char c = getchar();
    if (!c)
	return true;
    if (c == '.')
	return false;
    if (c == '\n' || c == ';')
	ungetc (c, stdin);
    return true;
}

// TEST FOR SPECIFIED DELIMITER
//
// The standard input is scanned for the parameter. If found,
// it is thrown away and non-zero is returned. If not found,
// zero is returned.
//
bool readdelim (char d)
{
    for (char c; (c = getchar());) {
	if (c == d)
	    return true;
	if (c == ' ')
	    continue;
	ungetc (c, stdin);
	break;
    }
    return false;
}

// Get course and distance
//
// The user is asked for a course and distance. This is used by
// move, impulse, and some of the computer functions.
//
// The return value is zero for success, one for an invalid input
// (meaning to drop the request).
//
int getcodi (int* co, float* di)
{
    *co = getintpar ("Course");
    if (*co < 0 || *co > 360)
	return 1;	// course must be in the interval [0, 360]
    *di = getfltpar ("Distance");
    if (*di <= 0.0 || *di > 15.0)
	return 1;	// distance must be in the interval [0, 15]
    return 0;		// good return
}

// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "getpar.h"
#include "trek.h"

static int testterm(void);

// get integer parameter
int getintpar(const char *s)
{
    int i;
    int n;

    while (1) {
	if (testnl() && s)
	    printf("%s: ", s);
	i = scanf("%d", &n);
	if (i < 0)
	    exit(1);
	if (i > 0 && testterm())
	    return n;
	printf("invalid input; please enter an integer\n");
	skiptonl(0);
    }
}

// get floating parameter
double getfltpar(const char *s)
{
    int i;
    double d;

    while (1) {
	if (testnl() && s)
	    printf("%s: ", s);
	i = scanf("%lf", &d);
	if (i < 0)
	    exit(1);
	if (i > 0 && testterm())
	    return d;
	printf("invalid input; please enter a double\n");
	skiptonl(0);
    }
}

const struct cvntab Yntab[] = {
    {"y", "es", (cmdfun) 1, 1},
    {"n", "o", (cmdfun) 0, 0},
    {NULL, NULL, NULL, 0}
};

// get yes/no parameter
int getynpar(const char *s)
{
    const struct cvntab *r;

    r = getcodpar(s, Yntab);
    return r->value2;
}

// get coded parameter
const struct cvntab *getcodpar(const char *s, const struct cvntab tab[])
{
    char input[100];
    const struct cvntab *r;
    int flag;
    const char *p, *q;
    int c;
    int f;

    flag = 0;
    while (1) {
	flag |= (f = testnl());
	if (flag)
	    printf("%s: ", s);
	if (f)
	    cgetc(0);	       // throw out the newline
	scanf("%*[ \t;]");
	if ((c = scanf("%99[^ \t;\n]", input)) < 0)
	    exit(1);
	if (c == 0)
	    continue;
	flag = 1;

	// if command list, print four per line
	if (input[0] == '?' && input[1] == 0) {
	    c = 4;
	    for (r = tab; r->abrev; r++) {
		strcpy(input, r->abrev);
		strcat(input, r->full);
		printf("%14.14s", input);
		if (--c > 0)
		    continue;
		c = 4;
		printf("\n");
	    }
	    if (c != 4)
		printf("\n");
	    continue;
	}

	// search for in table
	for (r = tab; r->abrev; r++) {
	    p = input;
	    for (q = r->abrev; *q; q++)
		if (*p++ != *q)
		    break;
	    if (!*q) {
		for (q = r->full; *p && *q; q++, p++)
		    if (*p != *q)
			break;
		if (!*p || !*q)
		    break;
	    }
	}

	// check for not found
	if (!r->abrev) {
	    printf("invalid input; ? for valid inputs\n");
	    skiptonl(0);
	} else
	    return r;
    }
}

// get string parameter
void getstrpar(const char *s, char *r, int l, const char *t)
{
    int i;
    char format[20];
    int f;

    if (t == 0)
	t = " \t\n;";
    (void) sprintf(format, "%%%d[^%s]", l, t);
    while (1) {
	if ((f = testnl()) && s)
	    printf("%s: ", s);
	if (f)
	    cgetc(0);
	scanf("%*[\t ;]");
	i = scanf(format, r);
	if (i < 0)
	    exit(1);
	if (i != 0)
	    return;
    }
}

// test if newline is next valid character
int testnl(void)
{
    char c;

    while ((c = cgetc(0)) != '\n')
	if ((c >= '0' && c <= '9') || c == '.' || c == '!' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '-') {
	    ungetc(c, stdin);
	    return 0;
	}
    ungetc(c, stdin);
    return 1;
}

// scan for newline
void skiptonl(int c)
{
    while (c != '\n')
	if (!(c = cgetc(0)))
	    return;
    ungetc('\n', stdin);
    return;
}

// test for valid terminator
static int testterm(void)
{
    char c;
    if (!(c = cgetc(0)))
	return 1;
    if (c == '.')
	return 0;
    if (c == '\n' || c == ';')
	ungetc(c, stdin);
    return 1;
}

// TEST FOR SPECIFIED DELIMITER
//
// The standard input is scanned for the parameter.  If found,
// it is thrown away and non-zero is returned.  If not found,
// zero is returned.
int readdelim(int d)
{
    char c;
    while ((c = cgetc(0)) != '\0') {
	if (c == d)
	    return 1;
	if (c == ' ')
	    continue;
	ungetc(c, stdin);
	break;
    }
    return 0;
}

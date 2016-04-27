// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.
// Re-coding of advent in C: data structure routines

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include "hdr.h"
#include "extern.h"

void dstroy(int object)
{
    move(object, 0);
}

void juggle(int object)
{
    int i, j;

    i = place[object];
    j = fixed[object];
    move(object, i);
    move(object + 100, j);
}

void move(int object, int where)
{
    int from;

    if (object <= 100)
	from = place[object];
    else
	from = fixed[object - 100];
    if (from > 0 && from <= 300)
	carry(object, from);
    drop(object, where);
}

int put(int object, int where, int pval)
{
    move(object, where);
    return -1 - pval;
}

void carry(int object, int where)
{
    int temp;

    if (object <= 100) {
	if (place[object] == -1)
	    return;
	place[object] = -1;
	holdng++;
    }
    if (atloc[where] == object) {
	atloc[where] = links[object];
	return;
    }
    for (temp = atloc[where]; links[temp] != object; temp = links[temp]);
    links[temp] = links[object];
}

void drop(int object, int where)
{
    if (object > 100)
	fixed[object - 100] = where;
    else {
	if (place[object] == -1)
	    holdng--;
	place[object] = where;
    }
    if (where <= 0)
	return;
    links[object] = atloc[where];
    atloc[where] = object;
}

int vocab(const char *word, int type, int value)	// used for storing only
{
    int adr;
    const char *s;
    char *t;
    int hash, i;
    struct hashtab *h;

    for (hash = 0, s = word, i = 0; i < 5 && *s; i++)	// some kind of hash
	hash += *s++;	       // add all chars in the word
    hash = (hash * 3719) & 077777;	// pulled that one out of a hat
    hash %= HTSIZE;	       // put it into range of table

    for (adr = hash;; adr++) { // look for entry in table
	if (adr == HTSIZE)
	    adr = 0;	       // wrap around
	h = &voc[adr];	       // point at the entry
	switch (type) {
	    case -2:	       // fill in entry
		if (h->val)    // already got an entry?
		    goto exitloop2;
		h->val = value;
		h->atab = malloc(length(word));
		if (h->atab == NULL)
		    err(1, NULL);
		for (s = word, t = h->atab; *s;)
		    *t++ = *s++ ^ '=';
		*t = 0 ^ '=';
		// encrypt slightly to thwart core reader
		// printf("Stored \"%s\" (%d ch) as entry %d\n",
		// word, length(word), adr);
		return 0;    // entry unused
	    case -1:	       // looking up user word
		if (h->val == 0)
		    return -1;	// not found
		for (s = word, t = h->atab; *t ^ '=';)
		    if ((*s++ ^ '=') != *t++)
			goto exitloop2;
		if ((*s ^ '=') != *t && s - word < 5)
		    goto exitloop2;
		// the word matched o.k.
		return h->val;
	    default:	       // looking up known word
		if (h->val == 0)
		    errx(1, "Unable to find %s in vocab", word);
		for (s = word, t = h->atab; *t ^ '=';)
		    if ((*s++ ^ '=') != *t++)
			goto exitloop2;
		// the word matched o.k.
		if (h->val / 1000 != type)
		    continue;
		return h->val % 1000;
	}

      exitloop2:	       // hashed entry does not match
	if (adr + 1 == hash || (adr == HTSIZE && hash == 0))
	    errx(1, "Hash table overflow");
    }
}

void prht(void)
{			       // print hash table
    int i, j, l;
    char *c;
    struct hashtab *h;
    for (i = 0; i < HTSIZE / 10 + 1; i++) {
	printf("%4d", i * 10);
	for (j = 0; j < 10; j++) {
	    if (i * 10 + j >= HTSIZE)
		break;
	    h = &voc[i * 10 + j];
	    putchar(' ');
	    if (h->val == 0) {
		printf("-----");
		continue;
	    }
	    for (l = 0, c = h->atab; l < 5; l++)
		if ((*c ^ '='))
		    putchar(*c++ ^ '=');
		else
		    putchar(' ');
	}
	putchar('\n');
    }
}

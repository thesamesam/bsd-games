// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "extern.h"

#define HASHSIZE	256
#define HASHMUL		81
#define HASHMASK	(HASHSIZE - 1)

static int hash(const char *);
static void install(struct wlist *);
static struct wlist *lookup(const char *);

static struct wlist *hashtab[HASHSIZE];

void wordinit(void)
{
    struct wlist *w;

    for (w = wlist; w->string; w++)
	install(w);
}

static int hash(const char *s)
{
    int hashval = 0;
    while (*s) {
	hashval += *s++;
	hashval *= HASHMUL;
	hashval &= HASHMASK;
    }
    return hashval;
}

static struct wlist *lookup(const char *s)
{
    for (struct wlist* wp = hashtab[hash(s)]; wp != NULL; wp = wp->next)
	if (*s == *wp->string && strcmp(s, wp->string) == 0)
	    return wp;
    return NULL;
}

static void install(struct wlist *wp)
{
    int hashval;
    if (lookup(wp->string) == NULL) {
	hashval = hash(wp->string);
	wp->next = hashtab[hashval];
	hashtab[hashval] = wp;
    } else
	printf("Multiply defined %s.\n", wp->string);
}

void parse(void)
{
    struct wlist *wp;
    int n;
    int flag;

    wordnumber = 0;	       // for cypher
    for (n = 0; n <= wordcount; n++) {
	if ((wp = lookup(words[n])) == NULL) {
	    wordvalue[n] = -1;
	    wordtype[n] = -1;
	} else {
	    wordvalue[n] = wp->value;
	    wordtype[n] = wp->article;
	}
    }
    // We never use adjectives for anything, so yank them all.
    for (n = 1; n < wordcount; n++) {
	if (wordtype[n] == ADJS) {
	    int i;
	    for (i = n + 1; i < wordcount; i++) {
		wordtype[i - 1] = wordtype[i];
		wordvalue[i - 1] = wordvalue[i];
		strcpy(words[i - 1], words[i]);
	    }
	    wordcount--;
	}
    }
    // Don't let a comma mean AND if followed by a verb.
    for (n = 0; n < wordcount; n++) {
	if (wordvalue[n] == AND && words[n][0] == ',' && wordtype[n + 1] == VERB) {
	    wordvalue[n] = -1;
	    wordtype[n] = -1;
	}
    }
    // Trim "AND AND" which can happen naturally at the end of a
    // comma-delimited list.
    for (n = 1; n < wordcount; n++) {
	if (wordvalue[n - 1] == AND && wordvalue[n] == AND) {
	    int i;
	    for (i = n + 1; i < wordcount; i++) {
		wordtype[i - 1] = wordtype[i];
		wordvalue[i - 1] = wordvalue[i];
		strcpy(words[i - 1], words[i]);
	    }
	    wordcount--;
	}
    }

    // If there is a sequence (NOUN | OBJECT) AND EVERYTHING
    // then move all the EVERYTHINGs to the beginning, since that's where
    // they're expected.  We can't get rid of the NOUNs and OBJECTs in
    // case they aren't in EVERYTHING (i.e. not here or nonexistent).
    flag = 1;
    while (flag) {
	flag = 0;
	for (n = 1; n < wordcount; n++) {
	    if ((wordtype[n - 1] == NOUNS || wordtype[n - 1] == OBJECT) && wordvalue[n] == AND && wordvalue[n + 1] == EVERYTHING) {
		char tmpword[WORDLEN];
		wordvalue[n + 1] = wordvalue[n - 1];
		wordvalue[n - 1] = EVERYTHING;
		wordtype[n + 1] = wordtype[n - 1];
		wordtype[n - 1] = OBJECT;
		strcpy(tmpword, words[n - 1]);
		strcpy(words[n - 1], words[n + 1]);
		strcpy(words[n + 1], tmpword);
		flag = 1;
	    }
	}
	// And trim EVERYTHING AND EVERYTHING.
	for (n = 1; n < wordcount; n++) {
	    if (wordvalue[n - 1] == EVERYTHING && wordvalue[n] == AND && wordvalue[n + 1] == EVERYTHING) {
		int i;
		for (i = n + 1; i < wordcount; i++) {
		    wordtype[i - 1] = wordtype[i + 1];
		    wordvalue[i - 1] = wordvalue[i + 1];
		    strcpy(words[i - 1], words[i + 1]);
		}
		wordcount--;
		wordcount--;
		flag = 1;
	    }
	}
    }
}

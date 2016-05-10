// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "deck.h"

static uint32_t h2nl(uint32_t h)
{
    unsigned char c[4];
    c[0] = (h >> 24) & 0xff;
    c[1] = (h >> 16) & 0xff;
    c[2] = (h >> 8) & 0xff;
    c[3] = (h >> 0) & 0xff;
    uint32_t rv;
    memcpy(&rv, c, sizeof rv);
    return rv;
}

// This program initializes the card files for monopoly.
// It reads in a data file with Com. Chest cards, followed by
// the Chance card.  The two are separated by a line of "%-".
// All other cards are separated by lines of "%%".  In the front
// of the file is the data for the decks in the same order.
// This includes the seek pointer for the start of each card.
// All cards start with their execution code, followed by the
// string to print, terminated with a null byte.

static const char* infile = "cards.inp";	// input file
static const char* outfile = "cards.pck";	// "packed" file

static DECK deck[2];

static FILE *inf, *outf;

static void getargs(int, char *[]);
static void fwrite_be_offt(off_t, FILE *);
static void count(void);
static void putem(void);

int main(int ac, char *av[])
{
    int i, nc;

    // sanity test
    if (sizeof(int) != 4) {
	fprintf(stderr, "sizeof(int) != 4\n");
	exit(1);
    }

    getargs(ac, av);
    if ((inf = fopen(infile, "r")) == NULL) {
	perror(infile);
	exit(1);
    }
    count();
    // allocate space for pointers.
    CC_D.offsets = calloc(CC_D.num_cards + 1, sizeof(off_t));
    CH_D.offsets = calloc(CH_D.num_cards + 1, sizeof(off_t));
    if (CC_D.offsets == NULL || CH_D.offsets == NULL) {
	fprintf(stderr, "out of memory\n");
	exit(1);
    }
    fseek(inf, 0L, SEEK_SET);
    if ((outf = fopen(outfile, "w")) == NULL) {
	perror(outfile);
	exit(1);
    }

    // these fields will be overwritten after the offsets are calculated,
    // so byte-order doesn't matter yet.
    fwrite(&nc, sizeof(nc), 1, outf);
    fwrite(&nc, sizeof(nc), 1, outf);
    fwrite(CC_D.offsets, sizeof (off_t), CC_D.num_cards, outf);
    fwrite(CH_D.offsets, sizeof (off_t), CH_D.num_cards, outf);

    // write out the cards themselves (calculating the offsets).
    putem();

    fclose(inf);
    fseek(outf, 0, SEEK_SET);

    // number of community chest cards first...
    nc = h2nl(CC_D.num_cards);
    fwrite(&nc, sizeof(nc), 1, outf);
    // ... then number of chance cards.
    nc = h2nl(CH_D.num_cards);
    fwrite(&nc, sizeof(nc), 1, outf);

    // dump offsets in big-endian byte order
    for (i = 0; i < CC_D.num_cards; i++)
	fwrite_be_offt(CC_D.offsets[i], outf);
    for (i = 0; i < CH_D.num_cards; i++)
	fwrite_be_offt(CH_D.offsets[i], outf);

    fflush(outf);
    if (ferror(outf)) {
	perror(outfile);
	exit(1);
    }
    fclose(outf);
    printf("There were %d com. chest and %d chance cards\n", CC_D.num_cards, CH_D.num_cards);
    exit(0);
}

static void getargs(int ac, char *av[])
{
    if (ac > 1)
	infile = av[1];
    if (ac > 2)
	outfile = av[2];
}

// count the cards
static void count(void)
{
    bool newline;
    DECK *in_deck;
    int c;

    newline = true;
    in_deck = &CC_D;
    while ((c = getc(inf)) != EOF) {
	if (newline && c == '%') {
	    newline = false;
	    in_deck->num_cards++;
	    if (getc(inf) == '-')
		in_deck = &CH_D;
	} else
	    newline = (c == '\n');
    }
    in_deck->num_cards++;
}

// put strings in the file
static void putem(void)
{
    bool newline;
    DECK *in_deck;
    int c;
    int num;

    in_deck = &CC_D;
    CC_D.num_cards = 1;
    CH_D.num_cards = 0;
    CC_D.offsets[0] = ftell(outf);
    putc(getc(inf), outf);
    putc(getc(inf), outf);
    for (num = 0; (c = getc(inf)) != '\n';)
	num = num * 10 + (c - '0');
    putw(h2nl(num), outf);
    newline = false;
    while ((c = getc(inf)) != EOF)
	if (newline && c == '%') {
	    putc('\0', outf);
	    newline = false;
	    if (getc(inf) == '-')
		in_deck = &CH_D;
	    while (getc(inf) != '\n')
		continue;
	    in_deck->offsets[in_deck->num_cards++] = ftell(outf);
	    if ((c = getc(inf)) == EOF)
		break;
	    putc(c, outf);
	    putc(c = getc(inf), outf);
	    for (num = 0; (c = getc(inf)) != EOF && c != '\n';)
		num = num * 10 + (c - '0');
	    putw(h2nl(num), outf);
	} else {
	    putc(c, outf);
	    newline = (c == '\n');
	}
    putc('\0', outf);
}

// Write out the off paramater as a 64 bit big endian number
static void fwrite_be_offt (off_t off, FILE* f)
{
    int i;
    unsigned char c[8];
    for (i = 7; i >= 0; i--) {
	c[i] = off & 0xff;
	off >>= 8;
    }
    fwrite(c, sizeof(c), 1, f);
}

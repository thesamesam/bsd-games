// Copyright (c) 1993 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "hdr.h"
#include "extern.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

struct savestruct {
    void *address;
    int width;
};

struct savestruct save_array[] = {
    {&abbnum, sizeof(abbnum)},
    {&attack, sizeof(attack)},
    {&blklin, sizeof(blklin)},
    {&bonus, sizeof(bonus)},
    {&chloc, sizeof(chloc)},
    {&chloc2, sizeof(chloc2)},
    {&clock1, sizeof(clock1)},
    {&clock2, sizeof(clock2)},
    {&closed, sizeof(closed)},
    {&closng, sizeof(closng)},
    {&daltlc, sizeof(daltlc)},
    {&demo, sizeof(demo)},
    {&detail, sizeof(detail)},
    {&dflag, sizeof(dflag)},
    {&dkill, sizeof(dkill)},
    {&dtotal, sizeof(dtotal)},
    {&foobar, sizeof(foobar)},
    {&gaveup, sizeof(gaveup)},
    {&holdng, sizeof(holdng)},
    {&iwest, sizeof(iwest)},
    {&k, sizeof(k)},
    {&k2, sizeof(k2)},
    {&knfloc, sizeof(knfloc)},
    {&kq, sizeof(kq)},
    {&latncy, sizeof(latncy)},
    {&limit, sizeof(limit)},
    {&lmwarn, sizeof(lmwarn)},
    {&loc, sizeof(loc)},
    {&maxdie, sizeof(maxdie)},
    {&mxscor, sizeof(mxscor)},
    {&newloc, sizeof(newloc)},
    {&numdie, sizeof(numdie)},
    {&obj, sizeof(obj)},
    {&oldlc2, sizeof(oldlc2)},
    {&oldloc, sizeof(oldloc)},
    {&panic, sizeof(panic)},
    {&saveday, sizeof(saveday)},
    {&savet, sizeof(savet)},
    {&scorng, sizeof(scorng)},
    {&spk, sizeof(spk)},
    {&stick, sizeof(stick)},
    {&tally, sizeof(tally)},
    {&tally2, sizeof(tally2)},
    {&tkk, sizeof(tkk)},
    {&turns, sizeof(turns)},
    {&verb, sizeof(verb)},
    {&wd1, sizeof(wd1)},
    {&wd2, sizeof(wd2)},
    {&wzdark, sizeof(wzdark)},
    {&yea, sizeof(yea)},
    {atloc, sizeof(atloc)},
    {dloc, sizeof(dloc)},
    {dseen, sizeof(dseen)},
    {fixed, sizeof(fixed)},
    {hinted, sizeof(hinted)},
    {links, sizeof(links)},
    {odloc, sizeof(odloc)},
    {place, sizeof(place)},
    {prop, sizeof(prop)},
    {tk, sizeof(tk)},
    {NULL, 0}
};

// to output the data using checksum to start random #s
int save(const char *outfile)
{
    FILE *out;
    struct savestruct *p;
    char *s;
    long sum;
    int i;

    crc_start();
    for (p = save_array; p->address != NULL; p++)
	sum = crc(p->address, p->width);
    srand (sum);

    if ((out = fopen(outfile, "wb")) == NULL) {
	fprintf(stderr, "Hmm.  The name \"%s\" appears to be magically blocked.\n", outfile);
	return 1;
    }
    fwrite(&sum, sizeof(sum), 1, out);	// Here's the rand() key
    for (p = save_array; p->address != NULL; p++) {
	for (s = p->address, i = 0; i < p->width; i++, s++)
	    *s = (*s ^ rand()) & 0xFF;	// Lightly encrypt
	fwrite(p->address, p->width, 1, out);
    }
    if (fclose(out) != 0) {
	warn("writing %s", outfile);
	return 1;
    }
    return 0;
}

int restore(const char *infile)
{
    FILE *in;
    struct savestruct *p;
    char *s;
    long sum, cksum = 0;
    int i;

    if ((in = fopen(infile, "rb")) == NULL) {
	fprintf(stderr, "Hmm.  The file \"%s\" appears to be magically blocked.\n", infile);
	return 1;
    }
    fread(&sum, sizeof(sum), 1, in);	// Get the seed
    srand (sum);
    for (p = save_array; p->address != NULL; p++) {
	fread(p->address, p->width, 1, in);
	for (s = p->address, i = 0; i < p->width; i++, s++)
	    *s = (*s ^ rand()) & 0xFF;	// Lightly decrypt
    }
    fclose(in);

    crc_start();	       // See if she cheated
    for (p = save_array; p->address != NULL; p++)
	cksum = crc(p->address, p->width);
    if (sum != cksum)	       // Tsk tsk
	return 2;	       // Altered the file
    // We successfully restored, so this really was a save file
    // Get rid of the file, but don't bother checking that we did
    return 0;
}

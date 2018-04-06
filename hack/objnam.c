// Copyright (c) 1985, Stichting Centrum voor Wiskunde en Informatica,
// Copyright (c) 1982 Jay Fenlason <hack@gnu.org>
// This file is free software, distributed under the BSD license.

#include "hack.h"
#include "extern.h"

enum { PREFIX = 15 };

char *strprepend(char *s, char *pref)
{
    int i = strlen(pref);
    if (i > PREFIX) {
	pline("WARNING: prefix too short.");
	return s;
    }
    s -= i;
    strncpy(s, pref, i);	// do not copy trailing 0
    return s;
}

char *sitoa(int a)
{
    static char buf[13];
    sprintf(buf, (a < 0) ? "%d" : "+%d", a);
    return buf;
}

char *typename(int otyp)
{
    static char buf[BUFSZ];
    const struct objclass *ocl = &c_Objects[otyp];
    const char *an = ocl->oc_name;
    const char *dn = object_description(otyp);
    bool nn = is_object_known(otyp);
    switch (ocl->oc_olet) {
	case POTION_SYM:
	    strcpy(buf, "potion");
	    break;
	case SCROLL_SYM:
	    strcpy(buf, "scroll");
	    break;
	case WAND_SYM:
	    strcpy(buf, "wand");
	    break;
	case RING_SYM:
	    strcpy(buf, "ring");
	    break;
	default:
	    if (nn) {
		strcpy(buf, an);
		if (otyp >= TURQUOISE && otyp <= JADE)
		    strcat(buf, " stone");
		if (dn)
		    sprintf(eos(buf), " (%s)", dn);
	    } else {
		strcpy(buf, dn ? dn : an);
		if (ocl->oc_olet == GEM_SYM)
		    strcat(buf, " gem");
	    }
	    return buf;
    }
    // here for ring/scroll/potion/wand
    if (nn)
	sprintf(eos(buf), " of %s", an);
    if (dn)
	sprintf(eos(buf), " (%s)", dn);
    return buf;
}

char *xname(struct obj *obj)
{
    static char bufr[BUFSZ];
    char *buf = &(bufr[PREFIX]);	// leave room for "17 -3 "
    const char *an = c_Objects[obj->otyp].oc_name;
    const char *dn = object_description(obj->otyp);
    bool nn = is_object_known(obj->otyp);
    int pl = (obj->quan != 1);
    if (!obj->dknown && !Blind)
	obj->dknown = 1;       // %% doesnt belong here
    switch (obj->olet) {
	case AMULET_SYM:
	    strcpy(buf, (obj->spe < 0 && obj->known)
		   ? "cheap plastic imitation of the " : "");
	    strcat(buf, "Amulet of Yendor");
	    break;
	case TOOL_SYM:
	    if (!nn) {
		strcpy(buf, dn);
		break;
	    }
	    strcpy(buf, an);
	    break;
	case FOOD_SYM:
	    if (obj->otyp == DEAD_HOMUNCULUS && pl) {
		pl = 0;
		strcpy(buf, "dead homunculi");
		break;
	    }
	    // fungis ?
	    // fallthrough
	case WEAPON_SYM:
	case ARMOR_SYM:
	case ROCK_SYM:
	    strcpy(buf, an);
	    break;
	case POTION_SYM:
	    if (nn || !obj->dknown) {
		strcpy(buf, "potion");
		if (pl) {
		    pl = 0;
		    strcat(buf, "s");
		}
		if (!obj->dknown)
		    break;
		strcat (buf, " of ");
		strcat (buf, an);
	    } else {
		strcpy (buf, dn);
		strcat (buf, " potion");
	    }
	    break;
	case SCROLL_SYM:
	    strcpy(buf, "scroll");
	    if (pl) {
		pl = 0;
		strcat(buf, "s");
	    }
	    if (!obj->dknown)
		break;
	    if (nn) {
		strcat(buf, " of ");
		strcat(buf, an);
	    } else {
		strcat(buf, " labeled ");
		strcat(buf, dn);
	    }
	    break;
	case WAND_SYM:
	    if (!obj->dknown)
		sprintf(buf, "wand");
	    else if (nn)
		sprintf(buf, "wand of %s", an);
	    else
		sprintf(buf, "%s wand", dn);
	    break;
	case RING_SYM:
	    if (!obj->dknown)
		sprintf(buf, "ring");
	    else if (nn)
		sprintf(buf, "ring of %s", an);
	    else
		sprintf(buf, "%s ring", dn);
	    break;
	case GEM_SYM:
	    if (!obj->dknown) {
		strcpy(buf, "gem");
		break;
	    }
	    if (!nn) {
		sprintf(buf, "%s gem", dn);
		break;
	    }
	    strcpy(buf, an);
	    if (obj->otyp >= TURQUOISE && obj->otyp <= JADE)
		strcat(buf, " stone");
	    break;
	default:
	    sprintf(buf, "glorkum %c (0%o) %u %d", obj->olet, obj->olet, obj->otyp, obj->spe);
    }
    if (pl) {
	char *p;

	for (p = buf; *p; ++p) {
	    if (!strncmp(" of ", p, 4)) {
		// pieces of, cloves of, lumps of
		int c1, c2 = 's';

		do {
		    c1 = c2;
		    c2 = *p;
		    *p++ = c1;
		} while (c1);
		goto nopl;
	    }
	}
	p = eos(buf) - 1;
	if (*p == 's' || *p == 'z' || *p == 'x' || (*p == 'h' && p[-1] == 's'))
	    strcat(buf, "es"); // boxes
	else if (*p == 'y' && !strchr(vowels, p[-1]))
	    strcpy(p, "ies");  // rubies, zruties
	else
	    strcat(buf, "s");
    }
  nopl:
    return buf;
}

char *doname(struct obj *obj)
{
    char prefix[PREFIX];
    char *bp = xname(obj);
    if (obj->quan != 1)
	sprintf(prefix, "%u ", obj->quan);
    else
	strcpy(prefix, "a ");
    switch (obj->olet) {
	case AMULET_SYM:
	    if (strncmp(bp, "cheap ", 6))
		strcpy(prefix, "the ");
	    break;
	case ARMOR_SYM:
	    if (obj->owornmask & W_ARMOR)
		strcat(bp, " (being worn)");
	    // fallthrough
	case WEAPON_SYM:
	    if (obj->known) {
		strcat(prefix, sitoa(obj->spe));
		strcat(prefix, " ");
	    }
	    break;
	case WAND_SYM:
	    if (obj->known)
		sprintf(eos(bp), " (%d)", obj->spe);
	    break;
	case RING_SYM:
	    if (obj->owornmask & W_RINGR)
		strcat(bp, " (on right hand)");
	    if (obj->owornmask & W_RINGL)
		strcat(bp, " (on left hand)");
	    if (obj->known && (c_Objects[obj->otyp].bits & SPEC)) {
		strcat(prefix, sitoa(obj->spe));
		strcat(prefix, " ");
	    }
	    break;
    }
    if (obj->owornmask & W_WEP)
	strcat(bp, " (weapon in hand)");
    if (obj->unpaid)
	strcat(bp, " (unpaid)");
    if (!strcmp(prefix, "a ") && strchr(vowels, *bp))
	strcpy(prefix, "an ");
    bp = strprepend(bp, prefix);
    return bp;
}

// used only in hack.fight.c (thitu)
void setan(const char *str, char *buf)
{
    if (strchr(vowels, *str))
	sprintf(buf, "an %s", str);
    else
	sprintf(buf, "a %s", str);
}

char *aobjnam(struct obj *otmp, const char *verb)
{
    char *bp = xname(otmp);
    char prefix[PREFIX];
    if (otmp->quan != 1) {
	sprintf(prefix, "%u ", otmp->quan);
	bp = strprepend(bp, prefix);
    }
    if (verb) {
	// verb is given in plural (i.e., without trailing s)
	strcat(bp, " ");
	if (otmp->quan != 1)
	    strcat(bp, verb);
	else if (!strcmp(verb, "are"))
	    strcat(bp, "is");
	else {
	    strcat(bp, verb);
	    strcat(bp, "s");
	}
    }
    return bp;
}

char *Doname(struct obj *obj)
{
    char *s = doname(obj);

    if ('a' <= *s && *s <= 'z')
	*s -= ('a' - 'A');
    return s;
}

const char *const wrp[] = { "wand", "ring", "potion", "scroll", "gem" };
const char wrpsym[] = { WAND_SYM, RING_SYM, POTION_SYM, SCROLL_SYM, GEM_SYM };

struct obj *readobjnam(char *bp)
{
    char *p;
    int i;
    int cnt, spe, spesgn, typ, heavy;
    char let;
    char *dn, *an;
    // int the = 0; char *oname = 0;
    cnt = spe = spesgn = typ = heavy = 0;
    let = 0;
    an = dn = 0;
    for (p = bp; *p; ++p)
	if ('A' <= *p && *p <= 'Z')
	    *p += 'a' - 'A';
    if (!strncmp(bp, "the ", 4)) {
	// the = 1;
	bp += 4;
    } else if (!strncmp(bp, "an ", 3)) {
	cnt = 1;
	bp += 3;
    } else if (!strncmp(bp, "a ", 2)) {
	cnt = 1;
	bp += 2;
    }
    if (!cnt && digit(*bp)) {
	cnt = atoi(bp);
	while (digit(*bp))
	    ++bp;
	while (*bp == ' ')
	    ++bp;
    }
    if (!cnt)
	cnt = 1;	       // %% what with "gems" etc. ?

    if (*bp == '+' || *bp == '-') {
	spesgn = (*bp++ == '+') ? 1 : -1;
	spe = atoi(bp);
	while (digit(*bp))
	    ++bp;
	while (*bp == ' ')
	    ++bp;
    } else {
	p = strrchr(bp, '(');
	if (p) {
	    if (p > bp && p[-1] == ' ')
		p[-1] = 0;
	    else
		*p = 0;
	    ++p;
	    spe = atoi(p);
	    while (digit(*p))
		++p;
	    if (strcmp(p, ")"))
		spe = 0;
	    else
		spesgn = 1;
	}
    }
    // now we have the actual name, as delivered by xname, say green
    // potions called whisky scrolls labeled "QWERTY" egg dead zruties
    // fortune cookies named hoei wand of wishing elven cloak
    for (p = bp; *p; ++p)
	if (!strncmp(p, " named ", 7)) {
	    *p = 0;
	    // oname = p+7;
	}
    for (p = bp; *p; ++p)
	if (!strncmp(p, " labeled ", 9)) {
	    *p = 0;
	    dn = p + 9;
	}
    // first change to singular if necessary
    if (cnt != 1) {
	// find "cloves of garlic", "blue glass"
	for (p = bp; *p; ++p)
	    if (!strncmp(p, "s of ", 5)) {
		while ((*p = p[1]) != '\0')
		    ++p;
		goto sing;
	    }
	// remove -s or -es (boxes) or -ies (rubies, zruties)
	p = eos(bp);
	if (p[-1] == 's') {
	    if (p[-2] == 'e') {
		if (p[-3] == 'i') {
		    if (!strcmp(p - 7, "cookies"))
			goto mins;
		    strcpy(p - 3, "y");
		    goto sing;
		}
		// note: cloves / knives from clove / knife
		if (!strcmp(p - 6, "knives")) {
		    strcpy(p - 3, "fe");
		    goto sing;
		}
		// note: nurses, axes but boxes
		if (!strcmp(p - 5, "boxes")) {
		    p[-2] = 0;
		    goto sing;
		}
	    }
	  mins:
	    p[-1] = 0;
	} else {
	    if (!strcmp(p - 9, "homunculi")) {
		strcpy(p - 1, "us");	// !! makes string
					// longer
		goto sing;
	    }
	    // here we cannot find the plural suffix
	}
    }
  sing:
    if (!strcmp(bp, "amulet of yendor")) {
	typ = AMULET_OF_YENDOR;
	goto typfnd;
    }
    p = eos(bp);
    if (!strcmp(p - 5, " mail")) {	// Note: ring mail is not a ring !
	let = ARMOR_SYM;
	an = bp;
	goto srch;
    }
    for (i = 0; i < (int) sizeof(wrpsym); ++i) {
	int j = strlen(wrp[i]);
	if (!strncmp(bp, wrp[i], j)) {
	    let = wrpsym[i];
	    bp += j;
	    if (!strncmp(bp, " of ", 4))
		an = bp + 4;
	    // else if(*bp) ??
	    goto srch;
	}
	if (!strcmp(p - j, wrp[i])) {
	    let = wrpsym[i];
	    p -= j;
	    *p = 0;
	    if (p[-1] == ' ')
		p[-1] = 0;
	    dn = bp;
	    goto srch;
	}
    }
    if (!strcmp(p - 6, " stone")) {
	p[-6] = 0;
	let = GEM_SYM;
	an = bp;
	goto srch;
    }
    an = bp;
  srch:
    if (!an && !dn)
	goto any;
    i = 1;
    if (let)
	i = object_sym_base(let);
    while (i <= NROFOBJECTS && (!let || c_Objects[i].oc_olet == let)) {
	const char *zn = c_Objects[i].oc_name;

	if (!zn)
	    goto nxti;
	if (an && strcmp(an, zn))
	    goto nxti;
	if (dn && (!(zn = c_Objects[i].oc_descr) || strcmp(dn, zn)))
	    goto nxti;
	typ = i;
	goto typfnd;
      nxti:
	++i;
    }
  any:
    if (!let)
	let = wrpsym[rn2(sizeof(wrpsym))];
    typ = probtype(let);
  typfnd:
    {
	struct obj *otmp;
	let = c_Objects[typ].oc_olet;
	otmp = mksobj(typ);
	if (heavy)
	    otmp->owt += 15;
	if (cnt > 0 && strchr("%?!*)", let) && (cnt < 4 || (let == WEAPON_SYM && typ <= ROCK && cnt < 20)))
	    otmp->quan = cnt;

	if (spe > 3 && spe > otmp->spe)
	    spe = 0;
	else if (let == WAND_SYM)
	    spe = otmp->spe;
	if (spe == 3 && _u.uluck < 0)
	    spesgn = -1;
	if (let != WAND_SYM && spesgn == -1)
	    spe = -spe;
	if (let == AMULET_SYM)
	    spe = -1;
	else if (typ == WAN_WISHING && rn2(10))
	    spe = (rn2(10) ? -1 : 0);
	otmp->spe = spe;

	if (spesgn == -1)
	    otmp->cursed = 1;

	return otmp;
    }
}

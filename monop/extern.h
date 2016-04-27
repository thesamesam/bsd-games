// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include "deck.h"
#include "monop.h"

extern bool trading, spec, fixing, told_em;
extern const char *const yncoms[], *const comlist[], *name_list[], *const lucky_mes[];
extern int num_play, player, num_doub, num_luck;
extern void (*const func[]) (void);
extern DECK deck[2];
extern MON mon[];
extern PLAY *play, *cur_p;
extern PROP prop[];
extern RR_S rr[];
extern SQUARE board[];
extern UTIL_S util[];

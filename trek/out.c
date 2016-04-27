// Copyright (c) 1980 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#include <stdio.h>
#include <string.h>
#include "trek.h"

// Announce Device Out
void out (int dev)
{
    const struct device* d = &Device[dev];
    printf("%s reports %s ", d->person, d->name);
    if (d->name[strlen(d->name) - 1] == 's')
	printf("are");
    else
	printf("is");
    printf(" damaged\n");
}

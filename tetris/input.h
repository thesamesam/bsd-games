// Copyright (c) 1992 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#pragma once
#include "../config.h"
#include <sys/time.h>

int rwait(struct timeval *);
int tgetchar(void);
void tsleep(void);

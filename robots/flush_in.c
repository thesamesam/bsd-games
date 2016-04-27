// Copyright (c) 1980 The Regents of the University of California.
// Copyright (c) 1999 The NetBSD Foundation, Inc.
// This file is free software, distributed under the BSD license.

#include "robots.h"

// flush_in:
//      Flush all pending input.
void flush_in (void)
{
    tcflush(fileno(stdin), TCIFLUSH);
}

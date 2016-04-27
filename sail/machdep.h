// Copyright (c) 1983 The Regents of the University of California.
// This file is free software, distributed under the BSD license.

#pragma once
#include "../config.h"
#include <signal.h>

enum { TIMEOUT = 300 };	       // Sync() timeout in seconds

// for POSIX systems
#define	blockalarm() \
	do {								\
		sigset_t sigset;					\
		sigemptyset(&sigset);					\
		sigaddset(&sigset, SIGALRM);				\
		sigprocmask(SIG_BLOCK, &sigset, (sigset_t *)0);		\
	} while (0)
#define	unblockalarm() \
	do {								\
		sigset_t sigset;					\
		sigemptyset(&sigset);					\
		sigaddset(&sigset, SIGALRM);				\
		sigprocmask(SIG_UNBLOCK, &sigset, (sigset_t *)0);	\
	} while (0)

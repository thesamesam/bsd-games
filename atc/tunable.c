// Copyright (c) 1987 by Ed James <edjames@berkeley.edu>
// This file is free software, distributed under the BSD license.

// NUM_SCORES - Number of scores that are kept track of.
// Keep this greater than 0, but less than 100.
// 4 lines are printed above the score, one below + your prompt, so
// to prevent scrolling: 6 + NUM_SCORES <= 24 (lines on an average terminal).
int NUM_SCORES = 18;

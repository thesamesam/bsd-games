# BSD-GAMES

This is the bsd-games package for Linux, containing ports of all the
open source games from NetBSD-current. This particular version is a
copy of version 2.17 released in 2005, forked with intent to clean up
the code and build system to modern standards. Also, subprojects that
are maintained elsewhere or are not games, have been removed.

## Installation

To install, you need ncurses, a c11-capable compiler, and a wordlist
(needed for hangman, usually in /usr/share/dict). Then:

```sh
./configure --prefix=/usr
make
make install
```

## Contents

This package contains the following games:

* adventure:	the original adventure by Crowther and Woods
* atc:		air traffic control simulator
* battlestar:	adventure game on a battlestar
* caesar:	performs rotated-alphabet cryptography (like rot13)
* cribbage:	cribbage card game
* dab:		dots and boxes
* drop4:	tetromino packing game
* fish:		go fish card game
* gomoku:	connect-5 version of tic-tac-toe
* hangman:	guess the word before it is too late
* klondike:	curses-based solitaire
* robots:	avoid the evil robots
* sail:		sail your ship into battle
* snake:	grab the cash, avoid the snake, and exit
* worm:		eat the numbers without running into anything
* wump:		hunt the wumpus

## Removed

Many programs that were in the original 2.17 distribution were removed to
make the package more focused on actual gaming rather than preservation
of useless junk of interest only to ancient computer history fanatics.
Perhaps someone would like to maintain a separate bsd-junk package?
Anyway, programs were removed for the following specific reasons:

Some are already maintained as standalone packages:

* backgammon:	[http://gnubg.org/](http://gnubg.org/)
* banner:	[https://packages.debian.org/stable/bsdmainutils](https://packages.debian.org/stable/bsdmainutils)
* factor:	[http://www.gnu.org/software/coreutils/coreutils.html](http://www.gnu.org/software/coreutils/coreutils.html)
* fortune:	[ftp://ftp.ibiblio.org/pub/linux/games/amusements/fortune](ftp://ftp.ibiblio.org/pub/linux/games/amusements/fortune)
* primes:	[http://primesieve.org/](http://primesieve.org/)
* rogue:	[http://coredumpcentral.org/](http://coredumpcentral.org/)
* hack:		[https://www.nethack.org](https://www.nethack.org), [https://github.com/msharov/hack](https://github.com/msharov/hack)

Some were infringing on copyrights:

* boggle:	Exactly like the eponymous game by Hasbro.
* mille:	Mille Bornes is copyrighted by Hasbro.
* monop:	Used Hasbro's Monopoly board and other content.
* trek:		Used Star Trek universe and characters.

Several were multiplayer games for people logged on to the same server,
a situation that no longer exists anywhere.

* hunt:		multiplayer only, and has been broken for some time.
* dm:		a way to stop other users from playing games.
* phantasia:	mostly multiplayer; playing alone consisted of pressing
		"1" until you are randomly killed by one of a never
		ending succession of monsters. There were no rewards,
		score, fun hit descriptions, or anything else.

Finally, a bunch of programs were of no use whatsoever.

* arithmetic:	study aid. We have calculators now.
* quiz:		quizzes on obscure and/or obsolete information.
* wtf:		outdated acronym lookup. We have Google now.
* rain, worms:	text-mode screensavers.
* bcd, ppt:	even museums can't read punchcards any more.
* morse:	morse code translator?
* number:	prints badly formated text numbers.
* pig:		I have no idea what this is for.
* pom:		phase of the moon?
* random:	see fortune above.
* wargames:	a short shell script that prints a one-line message.

# BSD-GAMES

This is the bsd-games package for Linux, containing classic text mode
games from UNIX folklore. Version 3.0 released here is the first update
since 2005, when version 2.17 was released. It contains a comprehensive
overhaul of the entire code base for readability and conformance to
modern C coding standard. The build system is also updated for automatic
installation and ease of packaging.

## Installation

Installation requirements:
* C11 compiler:	gcc 4.6 or clang 3.2
* ncurses:	[https://invisible-island.net/ncurses/](https://invisible-island.net/ncurses/)
* wordlist:	for hangman. Usually already in /usr/share/dict.

```sh
./configure --prefix=/usr && make install
```

Unlike earlier bsd-games versions, this one does not install anything
sgid. Scorefiles in /var/lib/bsdgames are owned and writable by the
users group. For security, each game validates scores when loading.
Saved game files are also validated and will not cause a crash.

## Contents

This package contains the following games:

* adventure:	the original adventure by Crowther and Woods
* atc:		air traffic control simulator
* battlestar:	a tropical adventure
* caesar:	performs rotated-alphabet cryptography (like rot13)
* cribbage:	cribbage card game
* dab:		dots and boxes
* drop4:	tetromino packing game
* gofish:	go fish card game
* gomoku:	connect-5 version of tic-tac-toe
* hangman:	guess the word before it is too late
* klondike:	curses-based solitaire
* robots:	avoid the evil robots
* sail:		sail your ship into battle
* snake:	grab the cash, avoid the snake, and exit
* spirhunt:	hunt space pirates
* worm:		eat the numbers without running into anything
* wump:		hunt the wumpus

## Removed

Many programs that were in the original 2.17 distribution were removed
to focus on providing playable games, rather than a rusty junk pile.

### Maintained elsewhere

* backgammon:	[https://www.gnu.org/software/gnubg/](https://www.gnu.org/software/gnubg/)
* banner:	[https://packages.debian.org/stable/bsdmainutils](https://packages.debian.org/stable/bsdmainutils)
* factor:	[http://www.gnu.org/software/coreutils/coreutils.html](http://www.gnu.org/software/coreutils/coreutils.html)
* fortune:	[https://ibiblio.org/pub/linux/games/amusements/fortune/](https://ibiblio.org/pub/linux/games/amusements/fortune/)
* primes:	[http://primesieve.org/](http://primesieve.org/)
* rogue:	[http://coredumpcentral.org/](http://coredumpcentral.org/)
* hack:		[https://www.nethack.org](https://www.nethack.org), [https://github.com/msharov/hack](https://github.com/msharov/hack)

### Infringing on copyrights

* boggle:	Exactly like the eponymous game by Hasbro.
* mille:	Mille Bornes is copyrighted by Hasbro.
* monop:	Used Hasbro's Monopoly board and other content.

### Local multiplayer for mainframes

* hunt:		multiplayer only, and has been broken for some time.
* dm:		a way to stop other users from playing games.
* phantasia:	mostly multiplayer; playing alone consisted of pressing
		"1" until you are randomly killed by one of a never
		ending succession of monsters. There were no rewards,
		score, fun hit descriptions, or anything else.

### Miscellaneous junk

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

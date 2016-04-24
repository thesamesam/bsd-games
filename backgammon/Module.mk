################ Source files ##########################################

backgammon/NAME	:= backgammon
backgammon/EXE	:= $Obackgammon/${backgammon/NAME}
backgammon/EXE2	:= $Obackgammon/teachgammon
backgammon/SRCS	:= $(wildcard backgammon/*.c)
backgammon/OBJS	:= $(addprefix $O,$(backgammon/SRCS:.c=.o))
backgammon/OBJ1	:= $(addprefix $Obackgammon/,allow.o board.o check.o fancy.o init.o odds.o one.o save.o subs.o table.o extra.o main.o move.o text.o version.o)
backgammon/OBJ2	:= $(addprefix $Obackgammon/,allow.o board.o check.o fancy.o init.o odds.o one.o save.o subs.o table.o data.o teach.o ttext1.o ttext2.o tutor.o)
backgammon/DEPS	:= $(backgammon/OBJS:.o=.d)
backgammon/LIBS	:= ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	backgammon/all backgammon/clean backgammon/run backgammon/install backgammon/uninstall

all:		backgammon/all
backgammon/all:	${backgammon/EXE} ${backgammon/EXE2}
backgammon/run:	${backgammon/EXE}
	@${backgammon/EXE}

${backgammon/EXE}:	${backgammon/OBJ1}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${backgammon/OBJ1} ${backgammon/LIBS}

${backgammon/EXE2}:	${backgammon/OBJ2}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${backgammon/OBJ2} ${backgammon/LIBS}

backgammon/stuff:
	echo ${backgammon/OBJS}

################ Installation ##########################################

ifdef BINDIR
backgammon/EXEI	:= ${BINDIR}/${backgammon/NAME}
backgammon/EXE2I:= ${BINDIR}/teachgammon
backgammon/MANI	:= ${MANDIR}/man6/${backgammon/NAME}.6.gz

install:		backgammon/install
backgammon/install:	${backgammon/EXEI} ${backgammon/EXE2I} ${backgammon/MANI}
${backgammon/EXEI}:	${backgammon/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${backgammon/EXE2I}:	${backgammon/EXE2}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${backgammon/MANI}:	backgammon/${backgammon/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@

uninstall:		backgammon/uninstall
backgammon/uninstall:
	@if [ -f ${backgammon/EXEI} ]; then\
	    echo "Removing ${backgammon/EXEI} ...";\
	    rm -f ${backgammon/EXEI} ${backgammon/EXE2I} ${backgammon/MANI};\
	fi
endif

################ Maintenance ###########################################

clean:	backgammon/clean
backgammon/clean:
	@if [ -d $O/backgammon ]; then\
	    rm -f ${backgammon/EXE} ${backgammon/EXE2} ${backgammon/OBJS} ${backgammon/DEPS} $Obackgammon/.d;\
	    rmdir $O/backgammon;\
	fi

$Obackgammon/.d:	$O.d
	@[ -d $Obackgammon ] || mkdir $Obackgammon && touch $Obackgammon/.d

${backgammon/OBJS}: ${CONFS} backgammon/Module.mk $Obackgammon/.d

-include ${backgammon/DEPS}

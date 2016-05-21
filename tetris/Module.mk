################ Source files ##########################################

tetris/NAME	:= tetris
tetris/EXE	:= $Otetris/${tetris/NAME}
tetris/SRCS	:= $(wildcard tetris/*.c)
tetris/OBJS	:= $(addprefix $O,$(tetris/SRCS:.c=.o))
tetris/DEPS	:= $(tetris/OBJS:.o=.d)
tetris/LIBS	:= ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	tetris/all tetris/clean tetris/run tetris/install tetris/uninstall

all:		tetris/all
tetris/all:	${tetris/EXE}
tetris/run:	${tetris/EXE}
	@${tetris/EXE}

${tetris/EXE}:	${tetris/OBJS}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${tetris/OBJS} ${tetris/LIBS}

################ Installation ##########################################

ifdef BINDIR
tetris/EXEI	:= ${BINDIR}/${tetris/NAME}
tetris/MANI	:= ${MANDIR}/man6/${tetris/NAME}.6.gz
tetris/SCOREI	:= ${STATEDIR}/tetris.scores

install:		tetris/install
tetris/install:	${tetris/EXEI} ${tetris/MANI} ${tetris/SCOREI}
${tetris/EXEI}:	${tetris/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${tetris/MANI}:	tetris/${tetris/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@
${tetris/SCOREI}:
	@echo "Creating initial score file $@ ..."
	@${INSTALLSCORE} /dev/null $@

uninstall:		tetris/uninstall
tetris/uninstall:
	@if [ -f ${tetris/EXEI} ]; then\
	    echo "Removing ${tetris/EXEI} ...";\
	    rm -f ${tetris/EXEI} ${tetris/MANI} ${tetris/SCOREI};\
	fi
endif

################ Maintenance ###########################################

clean:	tetris/clean
tetris/clean:
	@if [ -d $O/tetris ]; then\
	    rm -f ${tetris/EXE} ${tetris/OBJS} ${tetris/DEPS} $Otetris/.d;\
	    rmdir $O/tetris;\
	fi

$Otetris/.d:	$O.d
	@[ -d $Otetris ] || mkdir $Otetris && touch $Otetris/.d

${tetris/OBJS}: ${CONFS} tetris/Module.mk $Otetris/.d

-include ${tetris/DEPS}

################ Source files ##########################################

hangman/NAME	:= hangman
hangman/EXE	:= $Ohangman/${hangman/NAME}
hangman/SRCS	:= $(wildcard hangman/*.c)
hangman/OBJS	:= $(addprefix $O,$(hangman/SRCS:.c=.o))
hangman/DEPS	:= $(hangman/OBJS:.o=.d)
hangman/LIBS	:= ${COMLIB} ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	hangman/all hangman/clean hangman/run hangman/install hangman/uninstall

all:		hangman/all
hangman/all:	${hangman/EXE}
hangman/run:	${hangman/EXE}
	@${hangman/EXE}

${hangman/EXE}:	${hangman/OBJS} ${COMLIB}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${hangman/OBJS} ${hangman/LIBS}

################ Installation ##########################################

ifdef BINDIR
hangman/EXEI	:= ${BINDIR}/${hangman/NAME}
hangman/MANI	:= ${MANDIR}/man6/${hangman/NAME}.6.gz

install:		hangman/install
hangman/install:	${hangman/EXEI} ${hangman/MANI}
${hangman/EXEI}:	${hangman/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${hangman/MANI}:	hangman/${hangman/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@

uninstall:		hangman/uninstall
hangman/uninstall:
	@if [ -f ${hangman/EXEI} ]; then\
	    echo "Removing ${hangman/EXEI} ...";\
	    rm -f ${hangman/EXEI} ${hangman/MANI};\
	fi
endif

################ Maintenance ###########################################

clean:	hangman/clean
hangman/clean:
	@if [ -d $O/hangman ]; then\
	    rm -f ${hangman/EXE} ${hangman/OBJS} ${hangman/DEPS} $Ohangman/.d;\
	    rmdir $O/hangman;\
	fi

$Ohangman/.d:	$O.d
	@[ -d $Ohangman ] || mkdir $Ohangman && touch $Ohangman/.d

${hangman/OBJS}: ${CONFS} hangman/Module.mk $Ohangman/.d

-include ${hangman/DEPS}

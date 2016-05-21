################ Source files ##########################################

canfield/NAME	:= canfield
canfield/EXE	:= $Ocanfield/${canfield/NAME}
canfield/EXE2	:= $Ocanfield/cfscores
canfield/SRCS	:= $(wildcard canfield/*.c)
canfield/OBJS	:= $(addprefix $O,$(canfield/SRCS:.c=.o))
canfield/DEPS	:= $(canfield/OBJS:.o=.d)
canfield/LIBS	:= ${COMLIB} ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	canfield/all canfield/clean canfield/run canfield/install canfield/uninstall

all:		canfield/all
canfield/all:	${canfield/EXE} ${canfield/EXE2}
canfield/run:	${canfield/EXE}
	@${canfield/EXE}

${canfield/EXE}:	$Ocanfield/canfield.o ${COMLIB}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ $Ocanfield/canfield.o ${canfield/LIBS}

${canfield/EXE2}:	$Ocanfield/cfscores.o ${COMLIB}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ $Ocanfield/cfscores.o

################ Installation ##########################################

ifdef BINDIR
canfield/EXEI	:= ${BINDIR}/${canfield/NAME}
canfield/EXE2I	:= ${BINDIR}/cfscores
canfield/MANI	:= ${MANDIR}/man6/${canfield/NAME}.6.gz
canfield/SCOREI	:= ${STATEDIR}/canfield.scores

install:		canfield/install
canfield/install:	${canfield/EXEI} ${canfield/EXE2I} ${canfield/MANI} ${canfield/SCOREI}
${canfield/EXEI}:	${canfield/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${canfield/EXE2I}:	${canfield/EXE2}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${canfield/MANI}:	canfield/${canfield/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@
${canfield/SCOREI}:
	@echo "Creating initial score file $@ ..."
	@${INSTALLSCORE} /dev/null $@

uninstall:		canfield/uninstall
canfield/uninstall:
	@if [ -f ${canfield/EXEI} ]; then\
	    echo "Removing ${canfield/EXEI} ...";\
	    rm -f ${canfield/EXEI} ${canfield/EXE2I} ${canfield/MANI} ${canfield/SCOREI};\
	fi
endif

################ Maintenance ###########################################

clean:	canfield/clean
canfield/clean:
	@if [ -d $O/canfield ]; then\
	    rm -f ${canfield/EXE} ${canfield/EXE2} ${canfield/OBJS} ${canfield/DEPS} $Ocanfield/.d;\
	    rmdir $O/canfield;\
	fi

$Ocanfield/.d:	$O.d
	@[ -d $Ocanfield ] || mkdir $Ocanfield && touch $Ocanfield/.d

${canfield/OBJS}: ${CONFS} canfield/Module.mk $Ocanfield/.d

-include ${canfield/DEPS}

################ Source files ##########################################

phantasia/NAME	:= phantasia
phantasia/EXE	:= $Ophantasia/${phantasia/NAME}
phantasia/SRCS	:= $(filter-out phantasia/setup.c,$(wildcard phantasia/*.c))
phantasia/OBJS	:= $(addprefix $O,$(phantasia/SRCS:.c=.o))
phantasia/DEPS	:= $(phantasia/OBJS:.o=.d)
phantasia/LIBS	:= ${COMLIB} ${CURSES_LIBS} ${MATH_LIBS}

################ Compilation ###########################################

.PHONY:	phantasia/all phantasia/clean phantasia/run phantasia/install phantasia/uninstall

all:		phantasia/all
phantasia/all:	${phantasia/EXE} $Ophantasia/monsters
phantasia/run:	${phantasia/EXE}
	@${phantasia/EXE}

${phantasia/EXE}:	${phantasia/OBJS} ${COMLIB}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${phantasia/OBJS} ${phantasia/LIBS}

$Ophantasia/setup:	$Ophantasia/setup.o ${COMLIB}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ $< ${phantasia/LIBS}
$Ophantasia/monsters:	phantasia/monsters.asc phantasia/setup.c | $Ophantasia/setup
	@echo "    Generating $@ ..."
	@$Ophantasia/setup $< $@

################ Installation ##########################################

ifdef BINDIR
phantasia/EXEI	:= ${BINDIR}/${phantasia/NAME}
phantasia/MANI	:= ${MANDIR}/man6/${phantasia/NAME}.6.gz
phantasia/DATAI	:= ${STATEDIR}/phantasia/monsters
phantasia/SCOREI	:= $(addprefix ${STATEDIR}/phantasia/, gold lastdead mess motd characs scoreboard void)

install:		phantasia/install
phantasia/install:	${phantasia/EXEI} ${phantasia/MANI} ${phantasia/DATAI} ${phantasia/SCOREI}
${phantasia/EXEI}:	${phantasia/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${phantasia/MANI}:	phantasia/${phantasia/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@
${phantasia/DATAI}:	$Ophantasia/monsters
	@echo "Installing $@ ..."
	@${INSTALLSCORE} $< $@
${STATEDIR}/phantasia/%:
	@echo "Creating initial score file $@ ..."
	@${INSTALLSCORE} /dev/null $@

uninstall:		phantasia/uninstall
phantasia/uninstall:
	@if [ -f ${phantasia/EXEI} ]; then\
	    echo "Removing ${phantasia/EXEI} ...";\
	    rm -f ${phantasia/EXEI} ${phantasia/MANI} ${phantasia/DATAI} ${phantasia/SCOREI};\
	    rmdir ${STATEDIR}/phantasia;\
	fi
endif

################ Maintenance ###########################################

clean:	phantasia/clean
phantasia/clean:
	@if [ -d $O/phantasia ]; then\
	    rm -f ${phantasia/EXE} ${phantasia/OBJS} ${phantasia/DEPS} $Ophantasia/setup* $Ophantasia/monsters $Ophantasia/.d;\
	    rmdir $O/phantasia;\
	fi

$Ophantasia/.d:	$O.d
	@[ -d $Ophantasia ] || mkdir $Ophantasia && touch $Ophantasia/.d

${phantasia/OBJS} $Ophantasia/setup.o: ${CONFS} phantasia/Module.mk $Ophantasia/.d

-include ${phantasia/DEPS}

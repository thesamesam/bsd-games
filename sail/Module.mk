################ Source files ##########################################

sail/NAME	:= sail
sail/EXE	:= $Osail/${sail/NAME}
sail/SRCS	:= $(wildcard sail/*.c)
sail/OBJS	:= $(addprefix $O,$(sail/SRCS:.c=.o))
sail/DEPS	:= $(sail/OBJS:.o=.d)
sail/LIBS	:= ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	sail/all sail/clean sail/run sail/install sail/uninstall

all:		sail/all
sail/all:	${sail/EXE}
sail/run:	${sail/EXE}
	@${sail/EXE}

${sail/EXE}:	${sail/OBJS}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${sail/OBJS} ${sail/LIBS}

################ Installation ##########################################

ifdef BINDIR
sail/EXEI	:= ${BINDIR}/${sail/NAME}
sail/MANI	:= ${MANDIR}/man6/${sail/NAME}.6.gz
sail/SCOREI	:= ${STATEDIR}/saillog

install:		sail/install
sail/install:	${sail/EXEI} ${sail/MANI} ${sail/SCOREI}
${sail/EXEI}:	${sail/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${sail/MANI}:	sail/${sail/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@
${sail/SCOREI}:
	@echo "Creating initial score file $@ ..."
	@${INSTALLSCORE} /dev/null $@

uninstall:		sail/uninstall
sail/uninstall:
	@if [ -f ${sail/EXEI} ]; then\
	    echo "Removing ${sail/EXEI} ...";\
	    rm -f ${sail/EXEI} ${sail/MANI} ${sail/SCOREI};\
	fi
endif

################ Maintenance ###########################################

clean:	sail/clean
sail/clean:
	@if [ -d $O/sail ]; then\
	    rm -f ${sail/EXE} ${sail/OBJS} ${sail/DEPS} $Osail/.d;\
	    rmdir $O/sail;\
	fi

$Osail/.d:	$O.d
	@[ -d $Osail ] || mkdir $Osail && touch $Osail/.d

${sail/OBJS}: ${CONFS} sail/Module.mk $Osail/.d

-include ${sail/DEPS}

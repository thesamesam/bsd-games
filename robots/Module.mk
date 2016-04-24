################ Source files ##########################################

robots/NAME	:= robots
robots/EXE	:= $Orobots/${robots/NAME}
robots/SRCS	:= $(wildcard robots/*.c)
robots/OBJS	:= $(addprefix $O,$(robots/SRCS:.c=.o))
robots/DEPS	:= $(robots/OBJS:.o=.d)
robots/LIBS	:= ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	robots/all robots/clean robots/run robots/install robots/uninstall

all:		robots/all
robots/all:	${robots/EXE}
robots/run:	${robots/EXE}
	@${robots/EXE}

${robots/EXE}:	${robots/OBJS}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${robots/OBJS} ${robots/LIBS}

################ Installation ##########################################

ifdef BINDIR
robots/EXEI	:= ${BINDIR}/${robots/NAME}
robots/MANI	:= ${MANDIR}/man6/${robots/NAME}.6.gz
robots/SCOREI	:= ${STATEDIR}/robots_roll

install:		robots/install
robots/install:	${robots/EXEI} ${robots/MANI} ${robots/SCOREI}
${robots/EXEI}:	${robots/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${robots/MANI}:	robots/${robots/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@
${robots/SCOREI}:
	@echo "Creating initial score file $@ ..."
	@${INSTALLSCORE} /dev/null $@

uninstall:		robots/uninstall
robots/uninstall:
	@if [ -f ${robots/EXEI} ]; then\
	    echo "Removing ${robots/EXEI} ...";\
	    rm -f ${robots/EXEI} ${robots/MANI} ${robots/SCOREI};\
	fi
endif

################ Maintenance ###########################################

clean:	robots/clean
robots/clean:
	@if [ -d $O/robots ]; then\
	    rm -f ${robots/EXE} ${robots/OBJS} ${robots/DEPS} $Orobots/.d;\
	    rmdir $O/robots;\
	fi

$Orobots/.d:	$O.d
	@[ -d $Orobots ] || mkdir $Orobots && touch $Orobots/.d

${robots/OBJS}: ${CONFS} robots/Module.mk $Orobots/.d

-include ${robots/DEPS}

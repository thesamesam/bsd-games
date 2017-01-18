################ Source files ##########################################

drop4/NAME	:= drop4
drop4/EXE	:= $Odrop4/${drop4/NAME}
drop4/SRCS	:= $(wildcard drop4/*.c)
drop4/OBJS	:= $(addprefix $O,$(drop4/SRCS:.c=.o))
drop4/DEPS	:= $(drop4/OBJS:.o=.d)
drop4/LIBS	:= ${COMLIB} ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	drop4/all drop4/clean drop4/run drop4/install drop4/uninstall

all:		drop4/all
drop4/all:	${drop4/EXE}
drop4/run:	${drop4/EXE}
	@${drop4/EXE}

${drop4/EXE}:	${drop4/OBJS} ${COMLIB}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${drop4/OBJS} ${drop4/LIBS}

################ Installation ##########################################

ifdef BINDIR
drop4/EXEI	:= ${BINDIR}/${drop4/NAME}
drop4/MANI	:= ${MANDIR}/man6/${drop4/NAME}.6.gz
drop4/SCOREI	:= ${STATEDIR}/drop4.scores

install:		drop4/install
drop4/install:	${drop4/EXEI} ${drop4/MANI} ${drop4/SCOREI}
${drop4/EXEI}:	${drop4/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${drop4/MANI}:	drop4/${drop4/NAME}.6
${drop4/SCOREI}:
	@echo "Creating initial score file $@ ..."
	@${INSTALLSCORE} /dev/null $@

uninstall:		drop4/uninstall
drop4/uninstall:
	@if [ -f ${drop4/EXEI} ]; then\
	    echo "Removing ${drop4/EXEI} ...";\
	    rm -f ${drop4/EXEI} ${drop4/MANI} ${drop4/SCOREI};\
	fi
endif

################ Maintenance ###########################################

clean:	drop4/clean
drop4/clean:
	@if [ -d $O/drop4 ]; then\
	    rm -f ${drop4/EXE} ${drop4/OBJS} ${drop4/DEPS} $Odrop4/.d;\
	    rmdir $O/drop4;\
	fi

$Odrop4/.d:	$O.d
	@[ -d $Odrop4 ] || mkdir $Odrop4 && touch $Odrop4/.d

${drop4/OBJS}: ${CONFS} drop4/Module.mk $Odrop4/.d

-include ${drop4/DEPS}

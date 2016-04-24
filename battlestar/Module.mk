################ Source files ##########################################

battlestar/NAME	:= battlestar
battlestar/EXE	:= $Obattlestar/${battlestar/NAME}
battlestar/SRCS	:= $(wildcard battlestar/*.c)
battlestar/OBJS	:= $(addprefix $O,$(battlestar/SRCS:.c=.o))
battlestar/DEPS	:= $(battlestar/OBJS:.o=.d)
battlestar/LIBS	:= ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	battlestar/all battlestar/clean battlestar/run battlestar/install battlestar/uninstall

all:		battlestar/all
battlestar/all:	${battlestar/EXE}
battlestar/run:	${battlestar/EXE}
	@${battlestar/EXE}

${battlestar/EXE}:	${battlestar/OBJS}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${battlestar/OBJS} ${battlestar/LIBS}

################ Installation ##########################################

ifdef BINDIR
battlestar/EXEI	:= ${BINDIR}/${battlestar/NAME}
battlestar/MANI	:= ${MANDIR}/man6/${battlestar/NAME}.6.gz
battlestar/SCOREI	:= ${STATEDIR}/battlestar.log

install:		battlestar/install
battlestar/install:	${battlestar/EXEI} ${battlestar/MANI} ${battlestar/SCOREI}
${battlestar/EXEI}:	${battlestar/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${battlestar/MANI}:	battlestar/${battlestar/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@
${battlestar/SCOREI}:
	@echo "Creating initial score file $@ ..."
	@${INSTALLSCORE} /dev/null $@

uninstall:		battlestar/uninstall
battlestar/uninstall:
	@if [ -f ${battlestar/EXEI} ]; then\
	    echo "Removing ${battlestar/EXEI} ...";\
	    rm -f ${battlestar/EXEI} ${battlestar/MANI} ${battlestar/SCOREI};\
	fi
endif

################ Maintenance ###########################################

clean:	battlestar/clean
battlestar/clean:
	@if [ -d $O/battlestar ]; then\
	    rm -f ${battlestar/EXE} ${battlestar/OBJS} ${battlestar/DEPS} $Obattlestar/.d;\
	    rmdir $O/battlestar;\
	fi

$Obattlestar/.d:	$O.d
	@[ -d $Obattlestar ] || mkdir $Obattlestar && touch $Obattlestar/.d

${battlestar/OBJS}: ${CONFS} battlestar/Module.mk $Obattlestar/.d

-include ${battlestar/DEPS}

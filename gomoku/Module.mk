################ Source files ##########################################

gomoku/NAME	:= gomoku
gomoku/EXE	:= $Ogomoku/${gomoku/NAME}
gomoku/SRCS	:= $(wildcard gomoku/*.c)
gomoku/OBJS	:= $(addprefix $O,$(gomoku/SRCS:.c=.o))
gomoku/DEPS	:= $(gomoku/OBJS:.o=.d)
gomoku/LIBS	:= ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	gomoku/all gomoku/clean gomoku/run gomoku/install gomoku/uninstall

all:		gomoku/all
gomoku/all:	${gomoku/EXE}
gomoku/run:	${gomoku/EXE}
	@${gomoku/EXE}

${gomoku/EXE}:	${gomoku/OBJS}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${gomoku/OBJS} ${gomoku/LIBS}

################ Installation ##########################################

ifdef BINDIR
gomoku/EXEI	:= ${BINDIR}/${gomoku/NAME}
gomoku/MANI	:= ${MANDIR}/man6/${gomoku/NAME}.6.gz

install:		gomoku/install
gomoku/install:	${gomoku/EXEI} ${gomoku/MANI}
${gomoku/EXEI}:	${gomoku/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${gomoku/MANI}:	gomoku/${gomoku/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@

uninstall:		gomoku/uninstall
gomoku/uninstall:
	@if [ -f ${gomoku/EXEI} ]; then\
	    echo "Removing ${gomoku/EXEI} ...";\
	    rm -f ${gomoku/EXEI} ${gomoku/MANI};\
	fi
endif

################ Maintenance ###########################################

clean:	gomoku/clean
gomoku/clean:
	@if [ -d $O/gomoku ]; then\
	    rm -f ${gomoku/EXE} ${gomoku/OBJS} ${gomoku/DEPS} $Ogomoku/.d;\
	    rmdir $O/gomoku;\
	fi

$Ogomoku/.d:	$O.d
	@[ -d $Ogomoku ] || mkdir $Ogomoku && touch $Ogomoku/.d

${gomoku/OBJS}: ${CONFS} gomoku/Module.mk $Ogomoku/.d

-include ${gomoku/DEPS}

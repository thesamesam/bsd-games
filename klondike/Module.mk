################ Source files ##########################################

klondike/NAME	:= klondike
klondike/EXE	:= $Oklondike/${klondike/NAME}
klondike/SRCS	:= $(wildcard klondike/*.c)
klondike/OBJS	:= $(addprefix $O,$(klondike/SRCS:.c=.o))
klondike/DEPS	:= $(klondike/OBJS:.o=.d)
klondike/LIBS	:= ${COMLIB} ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	klondike/all klondike/clean klondike/run klondike/install klondike/uninstall

all:		klondike/all
klondike/all:	${klondike/EXE}
klondike/run:	${klondike/EXE}
	@${klondike/EXE}

${klondike/EXE}:	${klondike/OBJS} ${COMLIB}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${klondike/OBJS} ${klondike/LIBS}

################ Installation ##########################################

ifdef BINDIR
klondike/EXEI	:= ${BINDIR}/${klondike/NAME}
klondike/MANI	:= ${MANDIR}/man6/${klondike/NAME}.6.gz

install:		klondike/install
klondike/install:	${klondike/EXEI} ${klondike/MANI}
${klondike/EXEI}:	${klondike/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${klondike/MANI}:	klondike/${klondike/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@

uninstall:		klondike/uninstall
klondike/uninstall:
	@if [ -f ${klondike/EXEI} ]; then\
	    echo "Removing ${klondike/EXEI} ...";\
	    rm -f ${klondike/EXEI} ${klondike/MANI};\
	fi
endif

################ Maintenance ###########################################

clean:	klondike/clean
klondike/clean:
	@if [ -d $O/klondike ]; then\
	    rm -f ${klondike/EXE} ${klondike/OBJS} ${klondike/DEPS} $Oklondike/.d;\
	    rmdir $O/klondike;\
	fi

$Oklondike/.d:	$O.d
	@[ -d $Oklondike ] || mkdir $Oklondike && touch $Oklondike/.d

${klondike/OBJS}: ${CONFS} klondike/Module.mk $Oklondike/.d

-include ${klondike/DEPS}

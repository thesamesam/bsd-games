################ Source files ##########################################

cribbage/NAME	:= cribbage
cribbage/EXE	:= $Ocribbage/${cribbage/NAME}
cribbage/SRCS	:= $(wildcard cribbage/*.c)
cribbage/OBJS	:= $(addprefix $O,$(cribbage/SRCS:.c=.o))
cribbage/DEPS	:= $(cribbage/OBJS:.o=.d)
cribbage/LIBS	:= ${COMLIB} ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	cribbage/all cribbage/clean cribbage/run cribbage/install cribbage/uninstall

all:		cribbage/all
cribbage/all:	${cribbage/EXE}
cribbage/run:	${cribbage/EXE}
	@${cribbage/EXE}

${cribbage/EXE}:	${cribbage/OBJS} ${COMLIB}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${cribbage/OBJS} ${cribbage/LIBS}

################ Installation ##########################################

ifdef BINDIR
cribbage/EXEI	:= ${BINDIR}/${cribbage/NAME}
cribbage/MANI	:= ${MANDIR}/man6/${cribbage/NAME}.6.gz

install:		cribbage/install
cribbage/install:	${cribbage/EXEI} ${cribbage/MANI}
${cribbage/EXEI}:	${cribbage/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${cribbage/MANI}:	cribbage/${cribbage/NAME}.6

uninstall:		cribbage/uninstall
cribbage/uninstall:
	@if [ -f ${cribbage/EXEI} ]; then\
	    echo "Removing ${cribbage/EXEI} ...";\
	    rm -f ${cribbage/EXEI} ${cribbage/MANI};\
	fi
endif

################ Maintenance ###########################################

clean:	cribbage/clean
cribbage/clean:
	@if [ -d $O/cribbage ]; then\
	    rm -f ${cribbage/EXE} ${cribbage/OBJS} ${cribbage/DEPS} $Ocribbage/.d;\
	    rmdir $O/cribbage;\
	fi

$Ocribbage/.d:	$O.d
	@[ -d $Ocribbage ] || mkdir $Ocribbage && touch $Ocribbage/.d

${cribbage/OBJS}: ${CONFS} cribbage/Module.mk $Ocribbage/.d

-include ${cribbage/DEPS}

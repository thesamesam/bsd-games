################ Source files ##########################################

dab/NAME	:= dab
dab/EXE		:= $Odab/${dab/NAME}
dab/SRCS	:= $(wildcard dab/*.c)
dab/OBJS	:= $(addprefix $O,$(dab/SRCS:.c=.o))
dab/DEPS	:= $(dab/OBJS:.o=.d)
dab/LIBS	:= ${COMLIB} ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	dab/all dab/clean dab/run dab/install dab/uninstall

all:		dab/all
dab/all:	${dab/EXE}
dab/run:	${dab/EXE}
	@${dab/EXE}

${dab/EXE}:	${dab/OBJS} ${COMLIB}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${dab/OBJS} ${dab/LIBS}

################ Installation ##########################################

ifdef BINDIR
dab/EXEI	:= ${BINDIR}/${dab/NAME}
dab/MANI	:= ${MANDIR}/man6/${dab/NAME}.6.gz

install:		dab/install
dab/install:	${dab/EXEI} ${dab/MANI}
${dab/EXEI}:	${dab/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${dab/MANI}:	dab/${dab/NAME}.6

uninstall:		dab/uninstall
dab/uninstall:
	@if [ -f ${dab/EXEI} ]; then\
	    echo "Removing ${dab/EXEI} ...";\
	    rm -f ${dab/EXEI} ${dab/MANI};\
	fi
endif

################ Maintenance ###########################################

clean:	dab/clean
dab/clean:
	@if [ -d $O/dab ]; then\
	    rm -f ${dab/EXE} ${dab/OBJS} ${dab/DEPS} $Odab/.d;\
	    rmdir $O/dab;\
	fi

$Odab/.d:	$O.d
	@[ -d $Odab ] || mkdir $Odab && touch $Odab/.d

${dab/OBJS}: ${CONFS} dab/Module.mk $Odab/.d

-include ${dab/DEPS}

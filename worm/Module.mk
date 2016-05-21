################ Source files ##########################################

worm/NAME	:= worm
worm/EXE	:= $Oworm/${worm/NAME}
worm/SRCS	:= $(wildcard worm/*.c)
worm/OBJS	:= $(addprefix $O,$(worm/SRCS:.c=.o))
worm/DEPS	:= $(worm/OBJS:.o=.d)
worm/LIBS	:= ${COMLIB} ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	worm/all worm/clean worm/run worm/install worm/uninstall

all:		worm/all
worm/all:	${worm/EXE}
worm/run:	${worm/EXE}
	@${worm/EXE}

${worm/EXE}:	${worm/OBJS} ${COMLIB}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${worm/OBJS} ${worm/LIBS}

################ Installation ##########################################

ifdef BINDIR
worm/EXEI	:= ${BINDIR}/${worm/NAME}
worm/MANI	:= ${MANDIR}/man6/${worm/NAME}.6.gz

install:		worm/install
worm/install:	${worm/EXEI} ${worm/MANI}
${worm/EXEI}:	${worm/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${worm/MANI}:	worm/${worm/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@

uninstall:		worm/uninstall
worm/uninstall:
	@if [ -f ${worm/EXEI} ]; then\
	    echo "Removing ${worm/EXEI} ...";\
	    rm -f ${worm/EXEI} ${worm/MANI};\
	fi
endif

################ Maintenance ###########################################

clean:	worm/clean
worm/clean:
	@if [ -d $O/worm ]; then\
	    rm -f ${worm/EXE} ${worm/OBJS} ${worm/DEPS} $Oworm/.d;\
	    rmdir $O/worm;\
	fi

$Oworm/.d:	$O.d
	@[ -d $Oworm ] || mkdir $Oworm && touch $Oworm/.d

${worm/OBJS}: ${CONFS} worm/Module.mk $Oworm/.d

-include ${worm/DEPS}

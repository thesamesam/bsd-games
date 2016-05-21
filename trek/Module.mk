################ Source files ##########################################

trek/NAME	:= trek
trek/EXE	:= $Otrek/${trek/NAME}
trek/SRCS	:= $(wildcard trek/*.c)
trek/OBJS	:= $(addprefix $O,$(trek/SRCS:.c=.o))
trek/DEPS	:= $(trek/OBJS:.o=.d)
trek/DATA	:= $(wildcard trek/doc/*)
trek/LIBS	:= ${COMLIB} ${CURSES_LIBS} ${MATH_LIBS}

################ Compilation ###########################################

.PHONY:	trek/all trek/clean trek/run trek/install trek/uninstall

all:		trek/all
trek/all:	${trek/EXE}
trek/run:	${trek/EXE}
	@${trek/EXE}

${trek/EXE}:	${trek/OBJS} ${COMLIB}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${trek/OBJS} ${trek/LIBS}

################ Installation ##########################################

ifdef BINDIR
trek/EXEI	:= ${BINDIR}/${trek/NAME}
trek/MANI	:= ${MANDIR}/man6/${trek/NAME}.6.gz
trek/DATADIR	:= ${DATADIR}/trek
trek/DATAI	:= $(patsubst trek/doc/%,${trek/DATADIR}/%,${trek/DATA})

install:		trek/install
trek/install:	${trek/EXEI} ${trek/MANI} ${trek/DATAI}
${trek/EXEI}:	${trek/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${trek/MANI}:	trek/${trek/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@
${trek/DATAI}:	${trek/DATADIR}/%:	trek/doc/%
	@echo "Installing $@ ..."
	@${INSTALLDATA} $< $@

uninstall:		trek/uninstall
trek/uninstall:
	@if [ -f ${trek/EXEI} ]; then\
	    echo "Removing ${trek/EXEI} ...";\
	    rm -f ${trek/EXEI} ${trek/MANI} ${trek/DATAI};\
	    rmdir ${trek/DATADIR};\
	fi
endif

################ Maintenance ###########################################

clean:	trek/clean
trek/clean:
	@if [ -d $O/trek ]; then\
	    rm -f ${trek/EXE} ${trek/OBJS} ${trek/DEPS} $Otrek/.d;\
	    rmdir $O/trek;\
	fi

$Otrek/.d:	$O.d
	@[ -d $Otrek ] || mkdir $Otrek && touch $Otrek/.d

${trek/OBJS}: ${CONFS} trek/Module.mk $Otrek/.d

-include ${trek/DEPS}

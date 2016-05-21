################ Source files ##########################################

hack/NAME	:= hack
hack/EXE	:= $Ohack/${hack/NAME}
hack/SRCS	:= $(filter-out hack/makedefs.c,$(wildcard hack/*.c))
hack/OBJS	:= $(addprefix $O,$(hack/SRCS:.c=.o))
hack/DEPS	:= $(hack/OBJS:.o=.d)
hack/DATA	:= $(wildcard hack/gamehelp/*)
hack/LIBS	:= ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	hack/all hack/clean hack/run hack/install hack/uninstall

all:		hack/all
hack/all:	${hack/EXE}
hack/run:	${hack/EXE}
	@${hack/EXE}

${hack/EXE}:	${hack/OBJS}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${hack/OBJS} ${hack/LIBS}

$Ohack/makedefs:	$Ohack/makedefs.o
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ $<
hack/onames.h:	hack/objects.h hack/makedefs.c | $Ohack/makedefs
	@echo "    Generating $@ ..."
	@$Ohack/makedefs $< > $@

################ Installation ##########################################

ifdef BINDIR
hack/EXEI	:= ${BINDIR}/${hack/NAME}
hack/MANI	:= ${MANDIR}/man6/${hack/NAME}.6.gz
hack/DATAI	:= $(patsubst hack/gamehelp/%,${DATADIR}/hack/%,${hack/DATA})
hack/SCOREI	:= ${STATEDIR}/hack/perm ${STATEDIR}/hack/record

install:		hack/install
hack/install:	${hack/EXEI} ${hack/MANI} ${hack/DATAI} ${hack/SCOREI}
${hack/EXEI}:	${hack/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${hack/MANI}:	hack/${hack/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@
${hack/DATAI}:	${DATADIR}/hack/%:	hack/gamehelp/%
	@echo "Installing $@ ..."
	@${INSTALLDATA} $< $@
${hack/SCOREI}:	${STATEDIR}/hack
${STATEDIR}/hack:
	@echo "Creating hack dir and score files ..."
	@mkdir -m 755 ${STATEDIR}/hack
	@chgrp users ${STATEDIR}/hack
	@${INSTALLSCORE} /dev/null ${STATEDIR}/hack/perm
	@${INSTALLSCORE} /dev/null ${STATEDIR}/hack/record

uninstall:		hack/uninstall
hack/uninstall:
	@if [ -f ${hack/EXEI} ]; then\
	    echo "Removing ${hack/EXEI} ...";\
	    rm -f ${hack/EXEI} ${hack/MANI} ${hack/DATAI} ${hack/SCOREI};\
	    rmdir ${DATADIR}/hack ${STATEDIR}/hack;\
	fi
endif

################ Maintenance ###########################################

clean:	hack/clean
hack/clean:
	@if [ -d $Ohack ]; then\
	    rm -f ${hack/EXE} ${hack/OBJS} ${hack/DEPS} $Ohack/makedefs* $Ohack/.d;\
	    rmdir $Ohack;\
	fi

$Ohack/.d:	$O.d
	@[ -d $Ohack ] || mkdir $Ohack && touch $Ohack/.d

${hack/OBJS} $Ohack/makedefs.o: ${CONFS} hack/Module.mk $Ohack/.d
${hack/OBJS}:		hack/onames.h

-include ${hack/DEPS}

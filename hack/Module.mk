################ Source files ##########################################

hack/NAME	:= hack
hack/EXE	:= $Ohack/${hack/NAME}
hack/SRCS	:= $(filter-out hack/makedefs.c,$(wildcard hack/*.c))
hack/OBJS	:= $(addprefix $O,$(hack/SRCS:.c=.o))
hack/DEPS	:= $(hack/OBJS:.o=.d)
hack/LIBS	:= ${COMLIB} ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	hack/all hack/clean hack/run hack/install hack/uninstall

all:		hack/all
hack/all:	${hack/EXE}
hack/run:	${hack/EXE}
	@${hack/EXE}

${hack/EXE}:	${hack/OBJS} ${COMLIB}
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
hack/SCOREI	:= ${STATEDIR}/hack.scores

install:		hack/install
hack/install:	${hack/EXEI} ${hack/MANI} ${hack/SCOREI}
${hack/EXEI}:	${hack/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${hack/MANI}:	hack/${hack/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@
${hack/SCOREI}:
	@echo "Creating hack score file ..."
	@${INSTALLSCORE} /dev/null ${STATEDIR}/hack.scores

uninstall:		hack/uninstall
hack/uninstall:
	@if [ -f ${hack/EXEI} ]; then\
	    echo "Removing ${hack/EXEI} ...";\
	    rm -f ${hack/EXEI} ${hack/MANI} ${hack/SCOREI};\
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
${hack/OBJS}:	hack/onames.h

-include ${hack/DEPS}

################ Source files ##########################################

mille/NAME	:= mille
mille/EXE	:= $Omille/${mille/NAME}
mille/SRCS	:= $(wildcard mille/*.c)
mille/OBJS	:= $(addprefix $O,$(mille/SRCS:.c=.o))
mille/DEPS	:= $(mille/OBJS:.o=.d)
mille/LIBS	:= ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	mille/all mille/clean mille/run mille/install mille/uninstall

all:		mille/all
mille/all:	${mille/EXE}
mille/run:	${mille/EXE}
	@${mille/EXE}

${mille/EXE}:	${mille/OBJS}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${mille/OBJS} ${mille/LIBS}

################ Installation ##########################################

ifdef BINDIR
mille/EXEI	:= ${BINDIR}/${mille/NAME}
mille/MANI	:= ${MANDIR}/man6/${mille/NAME}.6.gz

install:		mille/install
mille/install:	${mille/EXEI} ${mille/MANI}
${mille/EXEI}:	${mille/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${mille/MANI}:	mille/${mille/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@

uninstall:		mille/uninstall
mille/uninstall:
	@if [ -f ${mille/EXEI} ]; then\
	    echo "Removing ${mille/EXEI} ...";\
	    rm -f ${mille/EXEI} ${mille/MANI};\
	fi
endif

################ Maintenance ###########################################

clean:	mille/clean
mille/clean:
	@if [ -d $O/mille ]; then\
	    rm -f ${mille/EXE} ${mille/OBJS} ${mille/DEPS} $Omille/.d;\
	    rmdir $O/mille;\
	fi

$Omille/.d:	$O.d
	@[ -d $Omille ] || mkdir $Omille && touch $Omille/.d

${mille/OBJS}: ${CONFS} mille/Module.mk $Omille/.d

-include ${mille/DEPS}

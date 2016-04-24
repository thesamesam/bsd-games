################ Source files ##########################################

adventure/NAME	:= adventure
adventure/EXE	:= $Oadventure/${adventure/NAME}
adventure/SRCS	:= $(addprefix adventure/, crc.c done.c init.c io.c main.c save.c subr.c vocab.c wizard.c)
adventure/OBJS	:= $(addprefix $O,$(adventure/SRCS:.c=.o) adventure/data.o)
adventure/DEPS	:= $(adventure/OBJS:.o=.d)
adventure/LIBS	:=

################ Compilation ###########################################

.PHONY:	adventure/all adventure/clean adventure/run adventure/install adventure/uninstall

all:		adventure/all
adventure/all:	${adventure/EXE}
adventure/run:	${adventure/EXE}
	@${adventure/EXE}

${adventure/EXE}:	${adventure/OBJS}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${adventure/OBJS} ${adventure/LIBS}

$Oadventure/setup:	$Oadventure/setup.o
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ $<
$Oadventure/data.c:	adventure/glorkz adventure/setup.c | $Oadventure/setup
	@echo "    Generating $@ ..."
	@$Oadventure/setup $< > $@
$Oadventure/data.o:	$Oadventure/data.c
	@echo "    Compiling $< ..."
	@${CC} ${CFLAGS} -MMD -MT "$@" -o $@ -c $<

################ Installation ##########################################

ifdef BINDIR
adventure/EXEI	:= ${BINDIR}/${adventure/NAME}
adventure/MANI	:= ${MANDIR}/man6/${adventure/NAME}.6.gz

install:		adventure/install
adventure/install:	${adventure/EXEI} ${adventure/MANI}
${adventure/EXEI}:	${adventure/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${adventure/MANI}:	adventure/${adventure/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@

uninstall:		adventure/uninstall
adventure/uninstall:
	@if [ -f ${adventure/EXEI} ]; then\
	    echo "Removing ${adventure/EXEI} ...";\
	    rm -f ${adventure/EXEI} ${adventure/MANI};\
	fi
endif

################ Maintenance ###########################################

clean:	adventure/clean
adventure/clean:
	@if [ -d $O/adventure ]; then\
	    rm -f ${adventure/EXE} ${adventure/OBJS} ${adventure/DEPS} $Oadventure/setup* $Oadventure/data.c $Oadventure/.d;\
	    rmdir $O/adventure;\
	fi

$Oadventure/.d:	$O.d
	@[ -d $Oadventure ] || mkdir $Oadventure && touch $Oadventure/.d

${adventure/OBJS} $Oadventure/setup.o: ${CONFS} adventure/Module.mk $Oadventure/.d

-include ${adventure/DEPS}

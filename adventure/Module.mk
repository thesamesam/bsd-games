################ Source files ##########################################

adventure/NAME	:= adventure
adventure/EXE	:= $Oadventure/${adventure/NAME}
adventure/SRCS	:= $(wildcard adventure/*.c)
adventure/OBJS	:= $(addprefix $O,$(adventure/SRCS:.c=.o))
adventure/DEPS	:= $(adventure/OBJS:.o=.d)
adventure/TXTS	:= $(wildcard adventure/*.txt)

################ Compilation ###########################################

.PHONY:	adventure/all adventure/clean adventure/run adventure/install adventure/uninstall

all:		adventure/all
adventure/all:	${adventure/EXE}
adventure/run:	${adventure/EXE}
	@${adventure/EXE}

${adventure/EXE}:	${adventure/OBJS} ${COMLIB}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ $^

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
	    rm -f ${adventure/EXE} ${adventure/OBJS} ${adventure/DEPS} $Oadventure/.d;\
	    rmdir $O/adventure;\
	fi

$Oadventure/.d:	$O.d
	@[ -d $Oadventure ] || mkdir $Oadventure && touch $Oadventure/.d

${adventure/OBJS}: ${CONFS} adventure/Module.mk $Oadventure/.d

-include ${adventure/DEPS}

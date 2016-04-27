################ Source files ##########################################

monop/NAME	:= monop
monop/EXE	:= $Omonop/${monop/NAME}
monop/SRCS	:= $(wildcard monop/*.c)
monop/OBJS	:= $(addprefix $O,$(monop/SRCS:.c=.o))
monop/MOBJS	:= $(filter-out $Omonop/initdeck.o, ${monop/OBJS})
monop/DEPS	:= $(monop/OBJS:.o=.d)
monop/DATA	:= $Omonop/cards.pck
monop/LIBS	:=

################ Compilation ###########################################

.PHONY:	monop/all monop/clean monop/run monop/install monop/uninstall

all:		monop/all
monop/all:	${monop/EXE} ${monop/DATA}
monop/run:	${monop/EXE}
	@${monop/EXE}

${monop/EXE}:	${monop/MOBJS}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${monop/MOBJS} ${monop/LIBS}

$Omonop/initdeck:	$Omonop/initdeck.o
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ $<
$Omonop/cards.pck:	$Omonop/initdeck monop/cards.inp
	@echo "    Compiling $@ ..."
	@$Omonop/initdeck monop/cards.inp $@

################ Installation ##########################################

ifdef BINDIR
monop/EXEI	:= ${BINDIR}/${monop/NAME}
monop/DATADIR	:= ${DATADIR}/monop
monop/DATAI	:= $(addprefix ${monop/DATADIR}/,$(notdir ${monop/DATA}))
monop/MANI	:= ${MANDIR}/man6/${monop/NAME}.6.gz

install:		monop/install
monop/install:	${monop/EXEI} ${monop/DATAI} ${monop/MANI}
${monop/EXEI}:	${monop/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${monop/DATADIR}/cards.pck:	$Omonop/cards.pck
	@echo "Installing $@ ..."
	@${INSTALLDATA} $< $@
${monop/MANI}:	monop/${monop/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@

uninstall:		monop/uninstall
monop/uninstall:
	@if [ -f ${monop/EXEI} ]; then\
	    echo "Removing ${monop/EXEI} ...";\
	    rm -f ${monop/EXEI} ${monop/DATAI};\
	    rmdir ${monop/DATADIR};\
	fi
endif

################ Maintenance ###########################################

clean:	monop/clean
monop/clean:
	@if [ -d $O/monop ]; then\
	    rm -f ${monop/EXE} ${monop/OBJS} ${monop/DEPS} $Omonop/initdeck $Omonop/cards.pck $Omonop/.d;\
	    rmdir $O/monop;\
	fi

$Omonop/.d:	$O.d
	@[ -d $Omonop ] || mkdir $Omonop && touch $Omonop/.d

${monop/OBJS}: ${CONFS} monop/Module.mk $Omonop/.d

-include ${monop/DEPS}

################ Source files ##########################################

monop/NAME	:= monop
monop/EXE	:= $Omonop/${monop/NAME}
monop/SRCS	:= $(wildcard monop/*.c)
monop/OBJS	:= $(addprefix $O,$(monop/SRCS:.c=.o))
monop/DEPS	:= $(monop/OBJS:.o=.d)
monop/LIBS	:= ${COMLIB}

################ Compilation ###########################################

.PHONY:	monop/all monop/clean monop/run monop/install monop/uninstall

all:		monop/all
monop/all:	${monop/EXE} ${monop/DATA}
monop/run:	${monop/EXE}
	@${monop/EXE}

${monop/EXE}:	${monop/OBJS} ${COMLIB}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${monop/OBJS} ${monop/LIBS}

################ Installation ##########################################

ifdef BINDIR
monop/EXEI	:= ${BINDIR}/${monop/NAME}
monop/MANI	:= ${MANDIR}/man6/${monop/NAME}.6.gz

install:		monop/install
monop/install:	${monop/EXEI} ${monop/MANI}
${monop/EXEI}:	${monop/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${monop/MANI}:	monop/${monop/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@

uninstall:		monop/uninstall
monop/uninstall:
	@if [ -f ${monop/EXEI} ]; then\
	    echo "Removing ${monop/EXEI} ...";\
	    rm -f ${monop/EXEI};\
	fi
endif

################ Maintenance ###########################################

clean:	monop/clean
monop/clean:
	@if [ -d $O/monop ]; then\
	    rm -f ${monop/EXE} ${monop/OBJS} ${monop/DEPS} $Omonop/.d;\
	    rmdir $O/monop;\
	fi

$Omonop/.d:	$O.d
	@[ -d $Omonop ] || mkdir $Omonop && touch $Omonop/.d

${monop/OBJS}: ${CONFS} monop/Module.mk $Omonop/.d

-include ${monop/DEPS}

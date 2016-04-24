################ Source files ##########################################

caesar/NAME	:= caesar
caesar/EXE	:= $Ocaesar/${caesar/NAME}
caesar/SRCS	:= $(wildcard caesar/*.c)
caesar/OBJS	:= $(addprefix $O,$(caesar/SRCS:.c=.o))
caesar/DEPS	:= $(caesar/OBJS:.o=.d)
caesar/LIBS	:= ${MATH_LIBS}

################ Compilation ###########################################

.PHONY:	caesar/all caesar/clean caesar/run caesar/install caesar/uninstall

all:		caesar/all
caesar/all:	${caesar/EXE} ${caesar/EXE2}
caesar/run:	${caesar/EXE}
	@${caesar/EXE}

${caesar/EXE}:	$Ocaesar/caesar.o
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ $Ocaesar/caesar.o ${caesar/LIBS}

################ Installation ##########################################

ifdef BINDIR
caesar/EXEI	:= ${BINDIR}/${caesar/NAME}
caesar/EXE2I	:= ${BINDIR}/rot13
caesar/MANI	:= ${MANDIR}/man6/${caesar/NAME}.6.gz

install:		caesar/install
caesar/install:	${caesar/EXEI} ${caesar/EXE2I} ${caesar/MANI}
${caesar/EXEI}:	${caesar/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${caesar/EXE2I}:	${caesar/EXEI}
	@echo "Installing $@ ..."
	@(cd ${BINDIR}; rm -f rot13; ln ${caesar/NAME} rot13)
${caesar/MANI}:	caesar/${caesar/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@

uninstall:		caesar/uninstall
caesar/uninstall:
	@if [ -f ${caesar/EXEI} ]; then\
	    echo "Removing ${caesar/EXEI} ...";\
	    rm -f ${caesar/EXEI} ${caesar/EXE2I} ${caesar/MANI};\
	fi
endif

################ Maintenance ###########################################

clean:	caesar/clean
caesar/clean:
	@if [ -d $O/caesar ]; then\
	    rm -f ${caesar/EXE} ${caesar/OBJS} ${caesar/DEPS} $Ocaesar/.d;\
	    rmdir $O/caesar;\
	fi

$Ocaesar/.d:	$O.d
	@[ -d $Ocaesar ] || mkdir $Ocaesar && touch $Ocaesar/.d

${caesar/OBJS}: ${CONFS} caesar/Module.mk $Ocaesar/.d

-include ${caesar/DEPS}

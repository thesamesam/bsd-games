################ Source files ##########################################

wump/NAME	:= wump
wump/EXE	:= $Owump/${wump/NAME}
wump/SRCS	:= $(wildcard wump/*.c)
wump/OBJS	:= $(addprefix $O,$(wump/SRCS:.c=.o))
wump/DEPS	:= $(wump/OBJS:.o=.d)
wump/LIBS	:=

################ Compilation ###########################################

.PHONY:	wump/all wump/clean wump/run wump/install wump/uninstall

all:		wump/all
wump/all:	${wump/EXE}
wump/run:	${wump/EXE}
	@${wump/EXE}

${wump/EXE}:	${wump/OBJS}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${wump/OBJS} ${wump/LIBS}

################ Installation ##########################################

ifdef BINDIR
wump/EXEI	:= ${BINDIR}/${wump/NAME}
wump/DATAI	:= ${DATADIR}/wump.info
wump/MANI	:= ${MANDIR}/man6/${wump/NAME}.6.gz

install:		wump/install
wump/install:	${wump/EXEI} ${wump/DATAI} ${wump/MANI}
${wump/EXEI}:	${wump/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${wump/DATAI}:	wump/wump.info
	@echo "Installing $@ ..."
	@${INSTALLDATA} $< $@
${wump/MANI}:	wump/${wump/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@

uninstall:		wump/uninstall
wump/uninstall:
	@if [ -f ${wump/EXEI} ]; then\
	    echo "Removing ${wump/EXEI} ...";\
	    rm -f ${wump/EXEI} ${wump/DATAI} ${wump/MANI};\
	fi
endif

################ Maintenance ###########################################

clean:	wump/clean
wump/clean:
	@if [ -d $O/wump ]; then\
	    rm -f ${wump/EXE} ${wump/OBJS} ${wump/DEPS} $Owump/.d;\
	    rmdir $O/wump;\
	fi

$Owump/.d:	$O.d
	@[ -d $Owump ] || mkdir $Owump && touch $Owump/.d

${wump/OBJS}: ${CONFS} wump/Module.mk $Owump/.d

-include ${wump/DEPS}

################ Source files ##########################################

boggle/NAME	:= boggle
boggle/EXE	:= $Oboggle/${boggle/NAME}
boggle/SRCS	:= $(addprefix boggle/, bog.c help.c mach.c prtable.c timer.c word.c)
boggle/OBJS	:= $(addprefix $O,$(boggle/SRCS:.c=.o))
boggle/DEPS	:= $(boggle/OBJS:.o=.d)
boggle/DATA	:= boggle/helpfile $Oboggle/dictionary $Oboggle/dictindex
boggle/LIBS	:= ${CURSES_LIBS}

################ Compilation ###########################################

.PHONY:	boggle/all boggle/clean boggle/run boggle/install boggle/uninstall

all:		boggle/all
boggle/all:	${boggle/EXE} $Oboggle/dictindex
boggle/run:	${boggle/EXE}
	@${boggle/EXE}

${boggle/EXE}:	${boggle/OBJS}
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ ${boggle/OBJS} ${boggle/LIBS}

$Oboggle/mkdict:	$Oboggle/mkdict.o
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ $<
$Oboggle/mkindex:	$Oboggle/mkindex.o
	@echo "Linking $@ ..."
	@${CC} ${LDFLAGS} -o $@ $<
$Oboggle/dictionary:	${WORDLIST} boggle/mkdict.c | $Oboggle/mkdict
	@echo "    Compiling $@ ..."
	@$Oboggle/mkdict < $< > $@
$Oboggle/dictindex:	$Oboggle/dictionary boggle/mkindex.c | $Oboggle/mkindex
	@echo "    Compiling $@ ..."
	@$Oboggle/mkindex < $< > $@

################ Installation ##########################################

ifdef BINDIR
boggle/EXEI	:= ${BINDIR}/${boggle/NAME}
boggle/MANI	:= ${MANDIR}/man6/${boggle/NAME}.6.gz
boggle/DATADIR	:= ${DATADIR}/boggle
boggle/DATAI	:= $(addprefix ${boggle/DATADIR}/,$(notdir ${boggle/DATA}))

install:		boggle/install
boggle/install:	${boggle/EXEI} ${boggle/MANI} ${boggle/DATAI}
${boggle/EXEI}:	${boggle/EXE}
	@echo "Installing $@ ..."
	@${INSTALLEXE} $< $@
${boggle/MANI}:	boggle/${boggle/NAME}.6
	@echo "Installing $@ ..."
	@gzip -9 -c $< > $@ && chmod 644 $@
${boggle/DATADIR}/helpfile:	boggle/helpfile
	@echo "Installing $@ ..."
	@${INSTALLDATA} $< $@
${boggle/DATADIR}/dictionary:	$Oboggle/dictionary
	@echo "Installing $@ ..."
	@${INSTALLDATA} $< $@
${boggle/DATADIR}/dictindex:	$Oboggle/dictindex
	@echo "Installing $@ ..."
	@${INSTALLDATA} $< $@

uninstall:		boggle/uninstall
boggle/uninstall:
	@if [ -f ${boggle/EXEI} ]; then\
	    echo "Removing ${boggle/EXEI} ...";\
	    rm -f ${boggle/EXEI} ${boggle/MANI} ${boggle/DATAI};\
	    rmdir ${boggle/DATADIR};\
	fi
endif

################ Maintenance ###########################################

clean:	boggle/clean
boggle/clean:
	@if [ -d $O/boggle ]; then\
	    rm -f ${boggle/EXE} ${boggle/OBJS} ${boggle/DEPS} $Oboggle/mkdict* $Oboggle/mkindex* $Oboggle/dictionary $Oboggle/dictindex $Oboggle/.d;\
	    rmdir $O/boggle;\
	fi

$Oboggle/.d:	$O.d
	@[ -d $Oboggle ] || mkdir $Oboggle && touch $Oboggle/.d

${boggle/OBJS} $Oboggle/mkdict.o $Oboggle/mkindex.o: ${CONFS} boggle/Module.mk $Oboggle/.d

-include ${boggle/DEPS}

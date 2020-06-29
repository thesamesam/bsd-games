################ Source files ##########################################

hangman/name	:= hangman
hangman/exe	:= $Ohangman/${hangman/name}
hangman/srcs	:= $(wildcard hangman/*.c)
hangman/objs	:= $(addprefix $O,$(hangman/srcs:.c=.o))
hangman/mans	:= $(wildcard hangman/*.6)
hangman/manz	:= $(addprefix $O,$(hangman/mans:.6=.6.gz))
hangman/deps	:= $(hangman/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	hangman/all hangman/clean hangman/run

all:		hangman/all
hangman/all:	${hangman/exe}

hangman/run:	${hangman/exe}
	@${hangman/exe}

${hangman/exe}:	${hangman/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs}

################ Installation ##########################################

.PHONY: hangman/install hangman/uninstall hangman/uninstall-man

ifdef exed
hangman/exei	:= ${exed}/${hangman/name}

${hangman/exei}:	${hangman/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:		hangman/install
hangman/install:	${hangman/exei}
uninstall:		hangman/uninstall
hangman/uninstall:
	@if [ -f ${hangman/exei} ]; then\
	    echo "Removing ${hangman/exei} ...";\
	    rm -f ${hangman/exei};\
	fi
endif

ifdef mand
hangman/mani	:= $(addprefix ${mand}/,$(notdir ${hangman/manz}))

${hangman/mani}: ${mand}/%:	$Ohangman/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

hangman/install:	${hangman/mani}
hangman/uninstall:	hangman/uninstall-man
hangman/uninstall-man:
	@if [ -f ${hangman/mani} ]; then\
	    echo "Removing ${hangman/mani} ...";\
	    rm -f ${hangman/mani};\
	fi
endif

################ Maintenance ###########################################

clean:	hangman/clean
hangman/clean:
	@if [ -d $O/hangman ]; then\
	    rm -f ${hangman/exe} ${hangman/objs} ${hangman/deps} ${hangman/manz} $Ohangman/.d;\
	    rmdir $O/hangman;\
	fi

${hangman/objs}: Makefile ${confs} hangman/Module.mk $Ohangman/.d

-include ${hangman/deps}

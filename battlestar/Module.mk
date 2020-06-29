################ Source files ##########################################

battlestar/name	:= battlestar
battlestar/exe	:= $Obattlestar/${battlestar/name}
battlestar/srcs	:= $(wildcard battlestar/*.c)
battlestar/objs	:= $(addprefix $O,$(battlestar/srcs:.c=.o))
battlestar/mans	:= $(wildcard battlestar/*.6)
battlestar/manz	:= $(addprefix $O,$(battlestar/mans:.6=.6.gz))
battlestar/deps	:= $(battlestar/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	battlestar/all battlestar/clean battlestar/run

all:		battlestar/all
battlestar/all:	${battlestar/exe}

battlestar/run:	${battlestar/exe}
	@${battlestar/exe}

${battlestar/exe}:	${battlestar/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs}

################ Installation ##########################################

.PHONY: battlestar/install battlestar/uninstall battlestar/uninstall-man

ifdef exed
battlestar/exei	:= ${exed}/${battlestar/name}

${battlestar/exei}:	${battlestar/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:		battlestar/install
battlestar/install:	${battlestar/exei}
uninstall:		battlestar/uninstall
battlestar/uninstall:
	@if [ -f ${battlestar/exei} ]; then\
	    echo "Removing ${battlestar/exei} ...";\
	    rm -f ${battlestar/exei};\
	fi
endif

ifdef mand
battlestar/mani	:= $(addprefix ${mand}/,$(notdir ${battlestar/manz}))

${battlestar/mani}: ${mand}/%:	$Obattlestar/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

battlestar/install:	${battlestar/mani}
battlestar/uninstall:	battlestar/uninstall-man
battlestar/uninstall-man:
	@if [ -f ${battlestar/mani} ]; then\
	    echo "Removing ${battlestar/mani} ...";\
	    rm -f ${battlestar/mani};\
	fi
endif

ifdef scored
battlestar/scorei:= ${scored}/battlestar.scores
${battlestar/scorei}:	| ${scored}
	@echo "Creating initial score file $@ ..."
	@${INSTALL_SCORE} $@

battlestar/install:	${battlestar/scorei}
endif

################ Maintenance ###########################################

clean:	battlestar/clean
battlestar/clean:
	@if [ -d $O/battlestar ]; then\
	    rm -f ${battlestar/exe} ${battlestar/objs} ${battlestar/deps} ${battlestar/manz} $Obattlestar/.d;\
	    rmdir $O/battlestar;\
	fi

${battlestar/objs}: Makefile ${confs} battlestar/Module.mk $Obattlestar/.d

-include ${battlestar/deps}

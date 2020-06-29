################ Source files ##########################################

snake/name	:= snake
snake/exe	:= $Osnake/${snake/name}
snake/srcs	:= $(wildcard snake/*.c)
snake/objs	:= $(addprefix $O,$(snake/srcs:.c=.o))
snake/mans	:= $(wildcard snake/*.6)
snake/manz	:= $(addprefix $O,$(snake/mans:.6=.6.gz))
snake/deps	:= $(snake/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	snake/all snake/clean snake/run

all:		snake/all
snake/all:	${snake/exe}

snake/run:	${snake/exe}
	@${snake/exe}

${snake/exe}:	${snake/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs}

################ Installation ##########################################

.PHONY: snake/install snake/uninstall snake/uninstall-man

ifdef exed
snake/exei	:= ${exed}/${snake/name}

${snake/exei}:	${snake/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:		snake/install
snake/install:	${snake/exei}
uninstall:		snake/uninstall
snake/uninstall:
	@if [ -f ${snake/exei} ]; then\
	    echo "Removing ${snake/exei} ...";\
	    rm -f ${snake/exei};\
	fi
endif

ifdef mand
snake/mani	:= $(addprefix ${mand}/,$(notdir ${snake/manz}))

${snake/mani}: ${mand}/%:	$Osnake/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

snake/install:	${snake/mani}
snake/uninstall:	snake/uninstall-man
snake/uninstall-man:
	@if [ -f ${snake/mani} ]; then\
	    echo "Removing ${snake/mani} ...";\
	    rm -f ${snake/mani};\
	fi
endif

ifdef scored
snake/scorei:= ${scored}/snake.scores
${snake/scorei}:	| ${scored}
	@echo "Creating initial score file $@ ..."
	@${INSTALL_SCORE} $@

snake/install:	${snake/scorei}
endif

################ Maintenance ###########################################

clean:	snake/clean
snake/clean:
	@if [ -d $O/snake ]; then\
	    rm -f ${snake/exe} ${snake/objs} ${snake/deps} ${snake/manz} $Osnake/.d;\
	    rmdir $O/snake;\
	fi

${snake/objs}: Makefile ${confs} snake/Module.mk $Osnake/.d

-include ${snake/deps}

################ Source files ##########################################

robots/name	:= robots
robots/exe	:= $Orobots/${robots/name}
robots/srcs	:= $(wildcard robots/*.c)
robots/objs	:= $(addprefix $O,$(robots/srcs:.c=.o))
robots/mans	:= $(wildcard robots/*.6)
robots/manz	:= $(addprefix $O,$(robots/mans:.6=.6.gz))
robots/deps	:= $(robots/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	robots/all robots/clean robots/run

all:		robots/all
robots/all:	${robots/exe}

robots/run:	${robots/exe}
	@${robots/exe}

${robots/exe}:	${robots/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs}

################ Installation ##########################################

.PHONY: robots/install robots/uninstall robots/uninstall-man

ifdef exed
robots/exei	:= ${exed}/${robots/name}

${robots/exei}:	${robots/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:		robots/install
robots/install:	${robots/exei}
uninstall:		robots/uninstall
robots/uninstall:
	@if [ -f ${robots/exei} ]; then\
	    echo "Removing ${robots/exei} ...";\
	    rm -f ${robots/exei};\
	fi
endif

ifdef mand
robots/mani	:= $(addprefix ${mand}/,$(notdir ${robots/manz}))

${robots/mani}: ${mand}/%:	$Orobots/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

robots/install:	${robots/mani}
robots/uninstall:	robots/uninstall-man
robots/uninstall-man:
	@if [ -f ${robots/mani} ]; then\
	    echo "Removing ${robots/mani} ...";\
	    rm -f ${robots/mani};\
	fi
endif

ifdef scored
robots/scorei:= ${scored}/robots.scores
${robots/scorei}:	| ${scored}
	@echo "Creating initial score file $@ ..."
	@${INSTALL_SCORE} $@

robots/install:	${robots/scorei}
endif

################ Maintenance ###########################################

clean:	robots/clean
robots/clean:
	@if [ -d $O/robots ]; then\
	    rm -f ${robots/exe} ${robots/objs} ${robots/deps} ${robots/manz} $Orobots/.d;\
	    rmdir $O/robots;\
	fi

${robots/objs}: Makefile ${confs} robots/Module.mk $Orobots/.d

-include ${robots/deps}

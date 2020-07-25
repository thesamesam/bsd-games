################ Source files ##########################################

spirhunt/name	:= spirhunt
spirhunt/exe	:= $Ospirhunt/${spirhunt/name}
spirhunt/srcs	:= $(wildcard spirhunt/*.c)
spirhunt/objs	:= $(addprefix $O,$(spirhunt/srcs:.c=.o))
spirhunt/deps	:= $(spirhunt/objs:.o=.d)
spirhunt/mans	:= $(wildcard spirhunt/*.6)
spirhunt/manz	:= $(addprefix $O,$(spirhunt/mans:.6=.6.gz))
spirhunt/data	:= $(wildcard spirhunt/doc/*)

################ Compilation ###########################################

.PHONY:	spirhunt/all spirhunt/clean spirhunt/run

all:		spirhunt/all
spirhunt/all:	${spirhunt/exe}

spirhunt/run:	${spirhunt/exe}
	@${spirhunt/exe}

${spirhunt/exe}:	${spirhunt/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs} -lm

################ Installation ##########################################

.PHONY: spirhunt/install spirhunt/uninstall spirhunt/uninstall-man

ifdef exed
spirhunt/exei	:= ${exed}/${spirhunt/name}

${spirhunt/exei}:	${spirhunt/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:		spirhunt/install
spirhunt/install:	${spirhunt/exei}
uninstall:		spirhunt/uninstall
spirhunt/uninstall:
	@if [ -f ${spirhunt/exei} ]; then\
	    echo "Removing ${spirhunt/exei} ...";\
	    rm -f ${spirhunt/exei};\
	fi
endif

ifdef mand
spirhunt/mani	:= $(addprefix ${mand}/,$(notdir ${spirhunt/manz}))

${spirhunt/mani}: ${mand}/%:	$Ospirhunt/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

spirhunt/install:	${spirhunt/mani}
spirhunt/uninstall:	spirhunt/uninstall-man
spirhunt/uninstall-man:
	@if [ -f ${spirhunt/mani} ]; then\
	    echo "Removing ${spirhunt/mani} ...";\
	    rm -f ${spirhunt/mani};\
	fi
endif

################ Maintenance ###########################################

clean:	spirhunt/clean
spirhunt/clean:
	@if [ -d $O/spirhunt ]; then\
	    rm -f ${spirhunt/exe} ${spirhunt/objs} ${spirhunt/deps} ${spirhunt/manz} $Ospirhunt/.d;\
	    rmdir $O/spirhunt;\
	fi

${spirhunt/objs}: Makefile ${confs} spirhunt/Module.mk $Ospirhunt/.d

-include ${spirhunt/deps}

################ Source files ##########################################

atc/name	:= atc
atc/exe		:= $Oatc/${atc/name}
atc/srcs	:= $(wildcard atc/*.c)
atc/objs	:= $(addprefix $O,$(atc/srcs:.c=.o))
atc/mans	:= $(wildcard atc/*.6)
atc/manz	:= $(addprefix $O,$(atc/mans:.6=.6.gz))
atc/deps	:= $(atc/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	atc/all atc/clean atc/run

all:		atc/all
atc/all:	${atc/exe}

atc/run:	${atc/exe}
	@${atc/exe}

${atc/exe}:	${atc/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs}

################ Installation ##########################################

.PHONY: atc/install atc/uninstall atc/uninstall-man

ifdef exed
atc/exei	:= ${exed}/${atc/name}

${atc/exei}:	${atc/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:		atc/install
atc/install:	${atc/exei}
uninstall:		atc/uninstall
atc/uninstall:
	@if [ -f ${atc/exei} ]; then\
	    echo "Removing ${atc/exei} ...";\
	    rm -f ${atc/exei};\
	fi
endif

ifdef mand
atc/mani	:= $(addprefix ${mand}/,$(notdir ${atc/manz}))

${atc/mani}: ${mand}/%:	$Oatc/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

atc/install:	${atc/mani}
atc/uninstall:	atc/uninstall-man
atc/uninstall-man:
	@if [ -f ${atc/mani} ]; then\
	    echo "Removing ${atc/mani} ...";\
	    rm -f ${atc/mani};\
	fi
endif

ifdef scored
atc/scorei:= ${scored}/atc.scores
${atc/scorei}:	| ${scored}
	@echo "Creating initial score file $@ ..."
	@${INSTALL_SCORE} $@

atc/install:	${atc/scorei}
endif

################ Maintenance ###########################################

clean:	atc/clean
atc/clean:
	@if [ -d $O/atc ]; then\
	    rm -f ${atc/exe} ${atc/objs} ${atc/deps} ${atc/manz} $Oatc/.d;\
	    rmdir $O/atc;\
	fi

${atc/objs}: Makefile ${confs} atc/Module.mk $Oatc/.d

-include ${atc/deps}

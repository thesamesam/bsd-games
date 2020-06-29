################ Source files ##########################################

drop4/name	:= drop4
drop4/exe	:= $Odrop4/${drop4/name}
drop4/srcs	:= $(wildcard drop4/*.c)
drop4/objs	:= $(addprefix $O,$(drop4/srcs:.c=.o))
drop4/mans	:= $(wildcard drop4/*.6)
drop4/manz	:= $(addprefix $O,$(drop4/mans:.6=.6.gz))
drop4/deps	:= $(drop4/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	drop4/all drop4/clean drop4/run

all:		drop4/all
drop4/all:	${drop4/exe}

drop4/run:	${drop4/exe}
	@${drop4/exe}

${drop4/exe}:	${drop4/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs}

################ Installation ##########################################

.PHONY: drop4/install drop4/uninstall drop4/uninstall-man

ifdef exed
drop4/exei	:= ${exed}/${drop4/name}

${drop4/exei}:	${drop4/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:	drop4/install
drop4/install:	${drop4/exei}
uninstall:	drop4/uninstall
drop4/uninstall:
	@if [ -f ${drop4/exei} ]; then\
	    echo "Removing ${drop4/exei} ...";\
	    rm -f ${drop4/exei};\
	fi
endif

ifdef mand
drop4/mani	:= $(addprefix ${mand}/,$(notdir ${drop4/manz}))

${drop4/mani}: ${mand}/%:	$Odrop4/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

drop4/install:	${drop4/mani}
drop4/uninstall:	drop4/uninstall-man
drop4/uninstall-man:
	@if [ -f ${drop4/mani} ]; then\
	    echo "Removing ${drop4/mani} ...";\
	    rm -f ${drop4/mani};\
	fi
endif

ifdef scored
drop4/scorei:= ${scored}/drop4.scores
${drop4/scorei}:	| ${scored}
	@echo "Creating initial score file $@ ..."
	@${INSTALL_SCORE} $@

drop4/install:	${drop4/scorei}
endif

################ Maintenance ###########################################

clean:	drop4/clean
drop4/clean:
	@if [ -d $O/drop4 ]; then\
	    rm -f ${drop4/exe} ${drop4/objs} ${drop4/deps} ${drop4/manz} $Odrop4/.d;\
	    rmdir $O/drop4;\
	fi

${drop4/objs}: Makefile ${confs} drop4/Module.mk $Odrop4/.d

-include ${drop4/deps}

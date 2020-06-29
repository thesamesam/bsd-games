################ Source files ##########################################

gomoku/name	:= gomoku
gomoku/exe	:= $Ogomoku/${gomoku/name}
gomoku/srcs	:= $(wildcard gomoku/*.c)
gomoku/objs	:= $(addprefix $O,$(gomoku/srcs:.c=.o))
gomoku/mans	:= $(wildcard gomoku/*.6)
gomoku/manz	:= $(addprefix $O,$(gomoku/mans:.6=.6.gz))
gomoku/deps	:= $(gomoku/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	gomoku/all gomoku/clean gomoku/run

all:		gomoku/all
gomoku/all:	${gomoku/exe}

gomoku/run:	${gomoku/exe}
	@${gomoku/exe}

${gomoku/exe}:	${gomoku/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs}

################ Installation ##########################################

.PHONY: gomoku/install gomoku/uninstall gomoku/uninstall-man

ifdef exed
gomoku/exei	:= ${exed}/${gomoku/name}

${gomoku/exei}:	${gomoku/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:		gomoku/install
gomoku/install:	${gomoku/exei}
uninstall:		gomoku/uninstall
gomoku/uninstall:
	@if [ -f ${gomoku/exei} ]; then\
	    echo "Removing ${gomoku/exei} ...";\
	    rm -f ${gomoku/exei};\
	fi
endif

ifdef mand
gomoku/mani	:= $(addprefix ${mand}/,$(notdir ${gomoku/manz}))

${gomoku/mani}: ${mand}/%:	$Ogomoku/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

gomoku/install:	${gomoku/mani}
gomoku/uninstall:	gomoku/uninstall-man
gomoku/uninstall-man:
	@if [ -f ${gomoku/mani} ]; then\
	    echo "Removing ${gomoku/mani} ...";\
	    rm -f ${gomoku/mani};\
	fi
endif

################ Maintenance ###########################################

clean:	gomoku/clean
gomoku/clean:
	@if [ -d $O/gomoku ]; then\
	    rm -f ${gomoku/exe} ${gomoku/objs} ${gomoku/deps} ${gomoku/manz} $Ogomoku/.d;\
	    rmdir $O/gomoku;\
	fi

${gomoku/objs}: Makefile ${confs} gomoku/Module.mk $Ogomoku/.d

-include ${gomoku/deps}

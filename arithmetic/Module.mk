################ Source files ##########################################

arithmetic/name	:= arithmetic
arithmetic/exe	:= $Oarithmetic/${arithmetic/name}
arithmetic/srcs	:= $(wildcard arithmetic/*.c)
arithmetic/objs	:= $(addprefix $O,$(arithmetic/srcs:.c=.o))
arithmetic/mans	:= $(wildcard arithmetic/*.6)
arithmetic/manz	:= $(addprefix $O,$(arithmetic/mans:.6=.6.gz))
arithmetic/deps	:= $(arithmetic/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	arithmetic/all arithmetic/clean arithmetic/run

all:		arithmetic/all
arithmetic/all:	${arithmetic/exe}

arithmetic/run:	${arithmetic/exe}
	@${arithmetic/exe}

${arithmetic/exe}:	${arithmetic/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^

################ Installation ##########################################

.PHONY: arithmetic/install arithmetic/uninstall arithmetic/uninstall-man

ifdef exed
arithmetic/exei	:= ${exed}/${arithmetic/name}

${arithmetic/exei}:	${arithmetic/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:		arithmetic/install
arithmetic/install:	${arithmetic/exei}
uninstall:		arithmetic/uninstall
arithmetic/uninstall:
	@if [ -f ${arithmetic/exei} ]; then\
	    echo "Removing ${arithmetic/exei} ...";\
	    rm -f ${arithmetic/exei};\
	fi
endif

ifdef mand
arithmetic/mani	:= $(addprefix ${mand}/,$(notdir ${arithmetic/manz}))

${arithmetic/mani}: ${mand}/%:	$Oarithmetic/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

arithmetic/install:	${arithmetic/mani}
arithmetic/uninstall:	arithmetic/uninstall-man
arithmetic/uninstall-man:
	@if [ -f ${arithmetic/mani} ]; then\
	    echo "Removing ${arithmetic/mani} ...";\
	    rm -f ${arithmetic/mani};\
	fi
endif

################ Maintenance ###########################################

clean:	arithmetic/clean
arithmetic/clean:
	@if [ -d $O/arithmetic ]; then\
	    rm -f ${arithmetic/exe} ${arithmetic/objs} ${arithmetic/deps} ${arithmetic/manz} $Oarithmetic/.d;\
	    rmdir $O/arithmetic;\
	fi

${arithmetic/objs}: Makefile ${confs} arithmetic/Module.mk $Oarithmetic/.d

-include ${arithmetic/deps}

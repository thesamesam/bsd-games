################ Source files ##########################################

fish/name	:= fish
fish/exe	:= $Ofish/${fish/name}
fish/srcs	:= $(wildcard fish/*.c)
fish/objs	:= $(addprefix $O,$(fish/srcs:.c=.o))
fish/mans	:= $(wildcard fish/*.6)
fish/manz	:= $(addprefix $O,$(fish/mans:.6=.6.gz))
fish/deps	:= $(fish/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	fish/all fish/clean fish/run

all:		fish/all
fish/all:	${fish/exe}

fish/run:	${fish/exe}
	@${fish/exe}

${fish/exe}:	${fish/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs}

################ Installation ##########################################

.PHONY: fish/install fish/uninstall fish/uninstall-man

ifdef exed
fish/exei	:= ${exed}/${fish/name}

${fish/exei}:	${fish/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:		fish/install
fish/install:	${fish/exei}
uninstall:		fish/uninstall
fish/uninstall:
	@if [ -f ${fish/exei} ]; then\
	    echo "Removing ${fish/exei} ...";\
	    rm -f ${fish/exei};\
	fi
endif

ifdef mand
fish/mani	:= $(addprefix ${mand}/,$(notdir ${fish/manz}))

${fish/mani}: ${mand}/%:	$Ofish/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

fish/install:	${fish/mani}
fish/uninstall:	fish/uninstall-man
fish/uninstall-man:
	@if [ -f ${fish/mani} ]; then\
	    echo "Removing ${fish/mani} ...";\
	    rm -f ${fish/mani};\
	fi
endif

################ Maintenance ###########################################

clean:	fish/clean
fish/clean:
	@if [ -d $O/fish ]; then\
	    rm -f ${fish/exe} ${fish/objs} ${fish/deps} ${fish/manz} $Ofish/.d;\
	    rmdir $O/fish;\
	fi

${fish/objs}: Makefile ${confs} fish/Module.mk $Ofish/.d

-include ${fish/deps}

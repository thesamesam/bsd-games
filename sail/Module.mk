################ Source files ##########################################

sail/name	:= sail
sail/exe	:= $Osail/${sail/name}
sail/srcs	:= $(wildcard sail/*.c)
sail/objs	:= $(addprefix $O,$(sail/srcs:.c=.o))
sail/mans	:= $(wildcard sail/*.6)
sail/manz	:= $(addprefix $O,$(sail/mans:.6=.6.gz))
sail/deps	:= $(sail/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	sail/all sail/clean sail/run

all:		sail/all
sail/all:	${sail/exe}

sail/run:	${sail/exe}
	@${sail/exe}

${sail/exe}:	${sail/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs}

################ Installation ##########################################

.PHONY: sail/install sail/uninstall sail/uninstall-man

ifdef exed
sail/exei	:= ${exed}/${sail/name}

${sail/exei}:	${sail/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:		sail/install
sail/install:	${sail/exei}
uninstall:		sail/uninstall
sail/uninstall:
	@if [ -f ${sail/exei} ]; then\
	    echo "Removing ${sail/exei} ...";\
	    rm -f ${sail/exei};\
	fi
endif

ifdef mand
sail/mani	:= $(addprefix ${mand}/,$(notdir ${sail/manz}))

${sail/mani}: ${mand}/%:	$Osail/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

sail/install:	${sail/mani}
sail/uninstall:	sail/uninstall-man
sail/uninstall-man:
	@if [ -f ${sail/mani} ]; then\
	    echo "Removing ${sail/mani} ...";\
	    rm -f ${sail/mani};\
	fi
endif

################ Maintenance ###########################################

clean:	sail/clean
sail/clean:
	@if [ -d $O/sail ]; then\
	    rm -f ${sail/exe} ${sail/objs} ${sail/deps} ${sail/manz} $Osail/.d;\
	    rmdir $O/sail;\
	fi

${sail/objs}: Makefile ${confs} sail/Module.mk $Osail/.d

-include ${sail/deps}

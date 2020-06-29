################ Source files ##########################################

worm/name	:= worm
worm/exe	:= $Oworm/${worm/name}
worm/srcs	:= $(wildcard worm/*.c)
worm/objs	:= $(addprefix $O,$(worm/srcs:.c=.o))
worm/mans	:= $(wildcard worm/*.6)
worm/manz	:= $(addprefix $O,$(worm/mans:.6=.6.gz))
worm/deps	:= $(worm/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	worm/all worm/clean worm/run

all:		worm/all
worm/all:	${worm/exe}

worm/run:	${worm/exe}
	@${worm/exe}

${worm/exe}:	${worm/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs}

################ Installation ##########################################

.PHONY: worm/install worm/uninstall worm/uninstall-man

ifdef exed
worm/exei	:= ${exed}/${worm/name}

${worm/exei}:	${worm/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:		worm/install
worm/install:	${worm/exei}
uninstall:		worm/uninstall
worm/uninstall:
	@if [ -f ${worm/exei} ]; then\
	    echo "Removing ${worm/exei} ...";\
	    rm -f ${worm/exei};\
	fi
endif

ifdef mand
worm/mani	:= $(addprefix ${mand}/,$(notdir ${worm/manz}))

${worm/mani}: ${mand}/%:	$Oworm/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

worm/install:	${worm/mani}
worm/uninstall:	worm/uninstall-man
worm/uninstall-man:
	@if [ -f ${worm/mani} ]; then\
	    echo "Removing ${worm/mani} ...";\
	    rm -f ${worm/mani};\
	fi
endif

################ Maintenance ###########################################

clean:	worm/clean
worm/clean:
	@if [ -d $O/worm ]; then\
	    rm -f ${worm/exe} ${worm/objs} ${worm/deps} ${worm/manz} $Oworm/.d;\
	    rmdir $O/worm;\
	fi

${worm/objs}: Makefile ${confs} worm/Module.mk $Oworm/.d

-include ${worm/deps}

################ Source files ##########################################

wump/name	:= wump
wump/exe	:= $Owump/${wump/name}
wump/srcs	:= $(wildcard wump/*.c)
wump/objs	:= $(addprefix $O,$(wump/srcs:.c=.o))
wump/mans	:= $(wildcard wump/*.6)
wump/manz	:= $(addprefix $O,$(wump/mans:.6=.6.gz))
wump/deps	:= $(wump/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	wump/all wump/clean wump/run

all:		wump/all
wump/all:	${wump/exe}

wump/run:	${wump/exe}
	@${wump/exe}

${wump/exe}:	${wump/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^

################ Installation ##########################################

.PHONY: wump/install wump/uninstall wump/uninstall-man

ifdef exed
wump/exei	:= ${exed}/${wump/name}

${wump/exei}:	${wump/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:	wump/install
wump/install:	${wump/exei}
uninstall:	wump/uninstall
wump/uninstall:
	@if [ -f ${wump/exei} ]; then\
	    echo "Removing ${wump/exei} ...";\
	    rm -f ${wump/exei};\
	fi
endif

ifdef mand
wump/mani	:= $(addprefix ${mand}/,$(notdir ${wump/manz}))

${wump/mani}: ${mand}/%:	$Owump/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

wump/install:	${wump/mani}
wump/uninstall:	wump/uninstall-man
wump/uninstall-man:
	@if [ -f ${wump/mani} ]; then\
	    echo "Removing ${wump/mani} ...";\
	    rm -f ${wump/mani};\
	fi
endif

################ Maintenance ###########################################

clean:	wump/clean
wump/clean:
	@if [ -d $O/wump ]; then\
	    rm -f ${wump/exe} ${wump/objs} ${wump/deps} ${wump/manz} $Owump/.d;\
	    rmdir $O/wump;\
	fi

${wump/objs}: Makefile ${confs} wump/Module.mk $Owump/.d

-include ${wump/deps}

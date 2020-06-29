################ Source files ##########################################

klondike/name	:= klondike
klondike/exe	:= $Oklondike/${klondike/name}
klondike/srcs	:= $(wildcard klondike/*.c)
klondike/objs	:= $(addprefix $O,$(klondike/srcs:.c=.o))
klondike/mans	:= $(wildcard klondike/*.6)
klondike/manz	:= $(addprefix $O,$(klondike/mans:.6=.6.gz))
klondike/deps	:= $(klondike/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	klondike/all klondike/clean klondike/run

all:		klondike/all
klondike/all:	${klondike/exe}

klondike/run:	${klondike/exe}
	@${klondike/exe}

${klondike/exe}:	${klondike/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs}

################ Installation ##########################################

.PHONY: klondike/install klondike/uninstall klondike/uninstall-man

ifdef exed
klondike/exei	:= ${exed}/${klondike/name}

${klondike/exei}:	${klondike/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:		klondike/install
klondike/install:	${klondike/exei}
uninstall:		klondike/uninstall
klondike/uninstall:
	@if [ -f ${klondike/exei} ]; then\
	    echo "Removing ${klondike/exei} ...";\
	    rm -f ${klondike/exei};\
	fi
endif

ifdef mand
klondike/mani	:= $(addprefix ${mand}/,$(notdir ${klondike/manz}))

${klondike/mani}: ${mand}/%:	$Oklondike/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

klondike/install:	${klondike/mani}
klondike/uninstall:	klondike/uninstall-man
klondike/uninstall-man:
	@if [ -f ${klondike/mani} ]; then\
	    echo "Removing ${klondike/mani} ...";\
	    rm -f ${klondike/mani};\
	fi
endif

################ Maintenance ###########################################

clean:	klondike/clean
klondike/clean:
	@if [ -d $O/klondike ]; then\
	    rm -f ${klondike/exe} ${klondike/objs} ${klondike/deps} ${klondike/manz} $Oklondike/.d;\
	    rmdir $O/klondike;\
	fi

${klondike/objs}: Makefile ${confs} klondike/Module.mk $Oklondike/.d

-include ${klondike/deps}

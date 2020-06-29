################ Source files ##########################################

cribbage/name	:= cribbage
cribbage/exe	:= $Ocribbage/${cribbage/name}
cribbage/srcs	:= $(wildcard cribbage/*.c)
cribbage/objs	:= $(addprefix $O,$(cribbage/srcs:.c=.o))
cribbage/mans	:= $(wildcard cribbage/*.6)
cribbage/manz	:= $(addprefix $O,$(cribbage/mans:.6=.6.gz))
cribbage/deps	:= $(cribbage/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	cribbage/all cribbage/clean cribbage/run

all:		cribbage/all
cribbage/all:	${cribbage/exe}

cribbage/run:	${cribbage/exe}
	@${cribbage/exe}

${cribbage/exe}:	${cribbage/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs}

################ Installation ##########################################

.PHONY: cribbage/install cribbage/uninstall cribbage/uninstall-man

ifdef exed
cribbage/exei	:= ${exed}/${cribbage/name}

${cribbage/exei}:	${cribbage/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:		cribbage/install
cribbage/install:	${cribbage/exei}
uninstall:		cribbage/uninstall
cribbage/uninstall:
	@if [ -f ${cribbage/exei} ]; then\
	    echo "Removing ${cribbage/exei} ...";\
	    rm -f ${cribbage/exei};\
	fi
endif

ifdef mand
cribbage/mani	:= $(addprefix ${mand}/,$(notdir ${cribbage/manz}))

${cribbage/mani}: ${mand}/%:	$Ocribbage/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

cribbage/install:	${cribbage/mani}
cribbage/uninstall:	cribbage/uninstall-man
cribbage/uninstall-man:
	@if [ -f ${cribbage/mani} ]; then\
	    echo "Removing ${cribbage/mani} ...";\
	    rm -f ${cribbage/mani};\
	fi
endif

################ Maintenance ###########################################

clean:	cribbage/clean
cribbage/clean:
	@if [ -d $O/cribbage ]; then\
	    rm -f ${cribbage/exe} ${cribbage/objs} ${cribbage/deps} ${cribbage/manz} $Ocribbage/.d;\
	    rmdir $O/cribbage;\
	fi

${cribbage/objs}: Makefile ${confs} cribbage/Module.mk $Ocribbage/.d

-include ${cribbage/deps}

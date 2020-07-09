################ Source files ##########################################

gofish/name	:= gofish
gofish/exe	:= $Ogofish/${gofish/name}
gofish/srcs	:= $(wildcard gofish/*.c)
gofish/objs	:= $(addprefix $O,$(gofish/srcs:.c=.o))
gofish/mans	:= $(wildcard gofish/*.6)
gofish/manz	:= $(addprefix $O,$(gofish/mans:.6=.6.gz))
gofish/deps	:= $(gofish/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	gofish/all gofish/clean gofish/run

all:		gofish/all
gofish/all:	${gofish/exe}

gofish/run:	${gofish/exe}
	@${gofish/exe}

${gofish/exe}:	${gofish/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs}

################ Installation ##########################################

.PHONY: gofish/install gofish/uninstall gofish/uninstall-man

ifdef exed
gofish/exei	:= ${exed}/${gofish/name}

${gofish/exei}:	${gofish/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:		gofish/install
gofish/install:	${gofish/exei}
uninstall:		gofish/uninstall
gofish/uninstall:
	@if [ -f ${gofish/exei} ]; then\
	    echo "Removing ${gofish/exei} ...";\
	    rm -f ${gofish/exei};\
	fi
endif

ifdef mand
gofish/mani	:= $(addprefix ${mand}/,$(notdir ${gofish/manz}))

${gofish/mani}: ${mand}/%:	$Ogofish/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

gofish/install:	${gofish/mani}
gofish/uninstall:	gofish/uninstall-man
gofish/uninstall-man:
	@if [ -f ${gofish/mani} ]; then\
	    echo "Removing ${gofish/mani} ...";\
	    rm -f ${gofish/mani};\
	fi
endif

################ Maintenance ###########################################

clean:	gofish/clean
gofish/clean:
	@if [ -d $O/gofish ]; then\
	    rm -f ${gofish/exe} ${gofish/objs} ${gofish/deps} ${gofish/manz} $Ogofish/.d;\
	    rmdir $O/gofish;\
	fi

${gofish/objs}: Makefile ${confs} gofish/Module.mk $Ogofish/.d

-include ${gofish/deps}

################ Source files ##########################################

dab/name	:= dab
dab/exe		:= $Odab/${dab/name}
dab/srcs	:= $(wildcard dab/*.c)
dab/objs	:= $(addprefix $O,$(dab/srcs:.c=.o))
dab/mans	:= $(wildcard dab/*.6)
dab/manz	:= $(addprefix $O,$(dab/mans:.6=.6.gz))
dab/deps	:= $(dab/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	dab/all dab/clean dab/run

all:		dab/all
dab/all:	${dab/exe}

dab/run:	${dab/exe}
	@${dab/exe}

${dab/exe}:	${dab/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs}

################ Installation ##########################################

.PHONY: dab/install dab/uninstall dab/uninstall-man

ifdef exed
dab/exei	:= ${exed}/${dab/name}

${dab/exei}:	${dab/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:	dab/install
dab/install:	${dab/exei}
uninstall:	dab/uninstall
dab/uninstall:
	@if [ -f ${dab/exei} ]; then\
	    echo "Removing ${dab/exei} ...";\
	    rm -f ${dab/exei};\
	fi
endif

ifdef mand
dab/mani	:= $(addprefix ${mand}/,$(notdir ${dab/manz}))

${dab/mani}: ${mand}/%:	$Odab/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

dab/install:	${dab/mani}
dab/uninstall:	dab/uninstall-man
dab/uninstall-man:
	@if [ -f ${dab/mani} ]; then\
	    echo "Removing ${dab/mani} ...";\
	    rm -f ${dab/mani};\
	fi
endif

################ Maintenance ###########################################

clean:	dab/clean
dab/clean:
	@if [ -d $O/dab ]; then\
	    rm -f ${dab/exe} ${dab/objs} ${dab/deps} ${dab/manz} $Odab/.d;\
	    rmdir $O/dab;\
	fi

${dab/objs}: Makefile ${confs} dab/Module.mk $Odab/.d

-include ${dab/deps}

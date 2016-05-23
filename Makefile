-include Config.mk

################ Source files ##########################################

CONFS	:= Config.mk config.h
ONAME   := $(notdir $(abspath $O))

include common/Module.mk
include adventure/Module.mk
include atc/Module.mk
include backgammon/Module.mk
include battlestar/Module.mk
include caesar/Module.mk
include canfield/Module.mk
include cribbage/Module.mk
include dab/Module.mk
include drop4/Module.mk
include fish/Module.mk
include gomoku/Module.mk
include hack/Module.mk
include hangman/Module.mk
include mille/Module.mk
include monop/Module.mk
include phantasia/Module.mk
include robots/Module.mk
include sail/Module.mk
include snake/Module.mk
include trek/Module.mk
include worm/Module.mk
include wump/Module.mk

################ Compilation ###########################################

.PHONY: all clean distclean maintainer-clean install uninstall

all:	${CONFS}

$O%.o:	%.c
	@echo "    Compiling $< ..."
	@${CC} ${CFLAGS} -MMD -MT "$(<:.c=.s) $@" -o $@ -c $<

%.s:	%.c
	@echo "    Compiling $< to assembly ..."
	@${CC} ${CFLAGS} -S -o $@ -c $<

$O%.o:	%.cc
	@echo "    Compiling $< ..."
	@${CXX} ${CXXFLAGS} -MMD -MT "$(<:.cc=.s) $@" -o $@ -c $<

%.s:	%.cc
	@echo "    Compiling $< to assembly ..."
	@${CXX} ${CXXFLAGS} -S -o $@ -c $<

################ Installation ##########################################

ifdef BINDIR
uninstall:
	@[ ! -d ${BINDIR} ] || ${RMPATH} ${BINDIR}
	@[ ! -d ${DATADIR} ] || ${RMPATH} ${DATADIR}
	@[ ! -d ${STATEDIR} ] || ${RMPATH} ${STATEDIR}
endif

################ Maintenance ###########################################

clean:
	@if [ -h ${ONAME} ]; then\
	    rm -f $O.d ${ONAME};\
	    ${RMPATH} ${BUILDDIR};\
	fi

distclean:	clean
	@rm -f ${CONFS} config.status

maintainer-clean: distclean

$O.d:   ${BUILDDIR}/.d
	@[ -h ${ONAME} ] || ln -s ${BUILDDIR} ${ONAME}
${BUILDDIR}/.d:     Makefile
	@mkdir -p ${BUILDDIR} && touch ${BUILDDIR}/.d

Config.mk:	Config.mk.in
config.h:	config.h.in
${CONFS}:	configure
	@if [ -x config.status ]; then echo "Reconfiguring ...";\
	    ./config.status;\
	else echo "Running configure ...";\
	    ./configure;\
	fi

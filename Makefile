-include Config.mk

################ Source files ##########################################

confs	:= Config.mk config.h
oname   := $(notdir $(abspath $O))

################ Compilation ###########################################

.SUFFIXES:
.PHONY: all clean distclean maintainer-clean install uninstall

all:	${CONFS}

$O%.o:	%.c
	@echo "    Compiling $< ..."
	@${CC} ${cflags} -MMD -MT "$(<:.c=.s) $@" -o $@ -c $<

%.s:	%.c
	@echo "    Compiling $< to assembly ..."
	@${CC} ${cflags} -S -o $@ -c $<

$O%.6.gz:	%.6
	@echo "    Compressing $@ ..."
	@gzip -c9 $< > $@

################ Installation ##########################################

ifdef bindir
exed	:= ${DESTDIR}${bindir}

${exed}:
	@echo "Creating $@ ..."
	@${INSTALL} -d $@

installdirs:	${exed}
endif

ifdef man6dir
mand	:= ${DESTDIR}${man6dir}

${mand}:
	@echo "Creating $@ ..."
	@${INSTALL} -d $@

installdirs:	${mand}
endif

ifdef scoredir
scored	:= ${DESTDIR}${scoredir}

${scored}:
	@echo "Creating $@ ..."
	@${INSTALL} -d $@

installdirs:	${scored}
endif

################ Maintenance ###########################################

clean:
	@if [ -d ${builddir} ]; then\
	    rm -f ${exe} ${objs} ${deps} $O.d;\
	    rmdir ${builddir};\
	fi

distclean:	clean
	@rm -f ${oname} ${confs} config.status

maintainer-clean: distclean

$O.d:	${builddir}/.d
	@[ -h ${oname} ] || ln -sf ${builddir} ${oname}
$O%/.d:	$O.d
	@[ -d $(dir $@) ] || mkdir $(dir $@)
	@touch $@
${builddir}/.d:	Makefile
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@touch $@

Config.mk:	Config.mk.in
config.h:	config.h.in
${confs}:	configure
	@if [ -x config.status ]; then echo "Reconfiguring ...";\
	    ./config.status;\
	else echo "Running configure ...";\
	    ./configure;\
	fi

include common/Module.mk
include adventure/Module.mk
include arithmetic/Module.mk
include atc/Module.mk
include battlestar/Module.mk
include caesar/Module.mk
include cribbage/Module.mk
include dab/Module.mk
include drop4/Module.mk
include gofish/Module.mk
include gomoku/Module.mk
include hangman/Module.mk
include klondike/Module.mk
include robots/Module.mk
include sail/Module.mk
include snake/Module.mk
include spirhunt/Module.mk
include worm/Module.mk
include wump/Module.mk

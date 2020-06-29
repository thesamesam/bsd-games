################ Source files ##########################################

caesar/name	:= caesar
caesar/exe	:= $Ocaesar/${caesar/name}
caesar/srcs	:= $(wildcard caesar/*.c)
caesar/objs	:= $(addprefix $O,$(caesar/srcs:.c=.o))
caesar/mans	:= $(wildcard caesar/*.6)
caesar/manz	:= $(addprefix $O,$(caesar/mans:.6=.6.gz))
caesar/deps	:= $(caesar/objs:.o=.d)

################ Compilation ###########################################

.PHONY:	caesar/all caesar/clean caesar/run

all:		caesar/all
caesar/all:	${caesar/exe}

caesar/run:	${caesar/exe}
	@${caesar/exe}

${caesar/exe}:	${caesar/objs} ${comlib}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^

################ Installation ##########################################

.PHONY: caesar/install caesar/uninstall caesar/uninstall-man

ifdef exed
caesar/exei	:= ${exed}/${caesar/name}

${caesar/exei}:	${caesar/exe} | ${exed}
	@echo "Installing $@ ..."
	@${INSTALL_PROGRAM} $< $@

install:		caesar/install
caesar/install:	${caesar/exei}
uninstall:		caesar/uninstall
caesar/uninstall:
	@if [ -f ${caesar/exei} ]; then\
	    echo "Removing ${caesar/exei} ...";\
	    rm -f ${caesar/exei};\
	fi
endif

ifdef mand
caesar/mani	:= $(addprefix ${mand}/,$(notdir ${caesar/manz}))

${caesar/mani}: ${mand}/%:	$Ocaesar/% | ${mand}
	@echo "Installing $@ ..."
	@${INSTALL_DATA} $< $@

caesar/install:	${caesar/mani}
caesar/uninstall:	caesar/uninstall-man
caesar/uninstall-man:
	@if [ -f ${caesar/mani} ]; then\
	    echo "Removing ${caesar/mani} ...";\
	    rm -f ${caesar/mani};\
	fi
endif

################ Maintenance ###########################################

clean:	caesar/clean
caesar/clean:
	@if [ -d $O/caesar ]; then\
	    rm -f ${caesar/exe} ${caesar/objs} ${caesar/deps} ${caesar/manz} $Ocaesar/.d;\
	    rmdir $O/caesar;\
	fi

${caesar/objs}: Makefile ${confs} caesar/Module.mk $Ocaesar/.d

-include ${caesar/deps}

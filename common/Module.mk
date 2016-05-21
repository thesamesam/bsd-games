################ Source files ##########################################

common/NAME	:= common
common/LIB	:= $Ocommon/libcommon.a
common/SRCS	:= $(wildcard common/*.c)
common/OBJS	:= $(addprefix $O,$(common/SRCS:.c=.o))
common/DEPS	:= $(common/OBJS:.o=.d)

COMLIB		:= ${common/LIB}

################ Compilation ###########################################

.PHONY:	common/all common/clean common/run common/install common/uninstall

common/all:	${common/LIB}

${common/LIB}:	${common/OBJS}
	@echo "Linking $@ ..."
	@rm -f $@
	@${AR} -qc $@ ${common/OBJS}
	@${RANLIB} $@

################ Maintenance ###########################################

clean:	common/clean
common/clean:
	@if [ -d $O/common ]; then\
	    rm -f ${common/LIB} ${common/OBJS} ${common/DEPS} $Ocommon/.d;\
	    rmdir $O/common;\
	fi

$Ocommon/.d:	$O.d
	@[ -d $Ocommon ] || mkdir $Ocommon && touch $Ocommon/.d

${common/OBJS}: ${CONFS} common/Module.mk $Ocommon/.d

-include ${common/DEPS}

################ Source files ##########################################

common/name	:= common
common/srcs	:= $(wildcard common/*.c)
common/objs	:= $(addprefix $O,$(common/srcs:.c=.o))
common/deps	:= $(common/objs:.o=.d)
comlib		:= $Ocommon/libcommon.a

################ Compilation ###########################################

.PHONY:	common/all common/clean

common/all:	${comlib}

${comlib}:	${common/objs}
	@echo "Linking $@ ..."
	@rm -f $@
	@${AR} -qc $@ $^
	@${RANLIB} $@

################ Maintenance ###########################################

clean:	common/clean
common/clean:
	@if [ -d $O/common ]; then\
	    rm -f ${comlib} ${common/objs} ${common/deps} $Ocommon/.d;\
	    rmdir $O/common;\
	fi

${common/objs}:	Makefile ${confs} common/Module.mk $Ocommon/.d

-include ${common/deps}

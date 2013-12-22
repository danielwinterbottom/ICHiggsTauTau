# Clear vars used by this make system
define HEADER
SRCS :=
SRCS_EXCLUDES :=
OBJS :=
CLEAN :=
TARGETS :=
SUBDIRS :=
MY_SUBDIRS :=
LIB_DEPS :=
LIB_EXTRA :=

# Clear user vars
$(foreach v,$(VERB_VARS) $(OBJ_VARS) $(DIR_VARS),$(eval $(v) := ))
endef

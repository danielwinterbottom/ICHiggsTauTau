define FOOTER
#SUBDIRS_$(d) := $(patsubst %/,%,$(addprefix $(d)/,$(SUBDIRS)))


COLOR_BL := \033[34m
COLOR_CY := \033[36m
COLOR_PU := \033[35m
COLOR_GR := \033[32m
NOCOLOR := \033[0m
#### MINE
MY_SUBDIRS_$(d) := $(patsubst %/,%,$(addprefix $(d)/,$(SUBDIRS)))
# $(info define MY_SUBDIRS_$(d)=$(MY_SUBDIRS_$(d)))

# Remove path and extension, replacing with a new path leading to /obj and .o as the extension
MY_OBJS_$(d) := $(addsuffix .o, $(addprefix $(d)/obj/,$(basename $(notdir $(wildcard $(d)/src/*.cc)))))

MY_EXES_$(d) := $(addprefix $(d)/bin/,$(basename $(notdir $(wildcard $(d)/test/*.cpp))))
# $(info define MY_EXES_$(d)=$(MY_EXES_$(d)))

MY_EXE_OBJS_$(d) := $(addsuffix .o, $(addprefix $(d)/bin/,$(basename $(notdir $(wildcard $(d)/test/*.cpp)))))
# $(info define MY_EXE_OBJS_$(d)=$(MY_EXE_OBJS_$(d)))

# $(info define MY_OBJS_$(d)=$(MY_OBJS_$(d)))

ifeq ($(d),$(TOP))
 MY_LIBNAME_$(d) := $(notdir $(d))
 MY_LIB_$(d) := 
else
 MY_LIBNAME_$(d) := $(subst /,_,$(patsubst /%,%,$(subst $(TOP),,$(d))))
 MY_LIB_$(d) := $(addsuffix .so,$(addprefix $(d)/lib/libIC,$(MY_LIBNAME_$(d))))
endif

ifeq ($(d),$(RUNDIR))
 MY_SHORT_EXES := $(addprefix bin/,$(basename $(notdir $(wildcard $(d)/test/*.cpp))))
endif

# $(info define MY_LIBNAME_$(d)=$(MY_LIBNAME_$(d)))

# $(info define MY_LIB_$(d)=$(MY_LIB_$(d)))

MY_LIB_DEPS_$(d) := $(foreach x,$(LIB_DEPS),$(TOP)/$(x)/lib/libIC$(x).so)
# $(info define MY_LIB_DEPS_$(d)=$(MY_LIB_DEPS_$(d)))

$(foreach x,$(MY_EXES_$(d)),$(eval MY_EXE_DEP_$(x) := $(LIB_EXTRA)))
$(foreach x,$(MY_LIB_$(d)),$(eval MY_LIB_DEP_$(x) := $(LIB_EXTRA)))


MY_LIB_EXTRA_$(d) := $(LIB_EXTRA)
# $(info define MY_LIB_EXTRA_$(d)=$(MY_LIB_EXTRA_$(d)))

### Targets for a directory are the shared library and the executables
MY_TARGETS_$(d) := $(MY_LIB_$(d)) $(MY_EXES_$(d))

$(MY_LIB_$(d)) : $(MY_OBJS_$(d)) $(MY_LIB_DEPS_$(d))
	@echo -e "$(COLOR_PU)Creating shared library $(subst $(TOP)/,,$@)$(NOCOLOR)"
	$(DOECHO)$(LD) $(LDFLAGS) -o $@ $^ $(LIBS) $(MY_LIB_DEP_$(@))

-include $(MY_OBJS_$(d):.o=.d)
-include $(MY_EXE_OBJS_$(d):.o=.d)

$(d)/obj/%.o: DIR=$(d)

DOECHO=

$(d)/obj/%.o: $(d)/src/%.cc
	@echo -e "$(COLOR_BL)Compiling object file $(subst $(TOP)/,,$@)$(NOCOLOR)"
	$(DOECHO)$(CXX) $(CXXFLAGS) -fPIC -c $<  -o $@
	@echo -e "$(COLOR_CY)Generating dependency file $(subst $(TOP)/,,$(@:.o=.d))$(NOCOLOR)"
	@$(CXX) $(CXXFLAGS) -MM -MP -MT "$@" $< -o $(@:.o=.d)


$(d)/bin/%.o: $(d)/test/%.cpp
	@echo -e "$(COLOR_BL)Compiling object file $(subst $(TOP)/,,$@)$(NOCOLOR)"
	$(DOECHO)$(CXX) $(CXXFLAGS) -fPIC -c $<  -o $@
	@echo -e "$(COLOR_CY)Generating dependency file $(subst $(TOP)/,,$(@:.o=.d))$(NOCOLOR)"
	$(DOECHO)$(CXX) $(CXXFLAGS) -MM -MP -MT "$@" $< -o $(@:.o=.d)

ifeq ($(d),$(RUNDIR))
bin/%: $(d)/bin/%
	@echo -e "==> $(COLOR_GR)Single program '$@' updated$(NOCOLOR)"
endif

$(d)/bin/%: $(d)/bin/%.o $(MY_LIB_$(d)) $(MY_LIB_DEPS_$(d))
	@echo -e "$(COLOR_PU)Linking executable $(subst $(TOP)/,,$@)$(NOCOLOR)"
	$(DOECHO)$(CXX) -o $@  $^ $(LIBS) $(MY_EXE_DEP_$@)

dir_$(d) : $(MY_TARGETS_$(d))

.SECONDARY: $(MY_EXE_OBJS_$(d))

#ifneq ($(strip $(OBJS)),)
#OBJS_$(d) := $(addprefix $(OBJPATH)/,$(OBJS))
#else # Populate OBJS_ from SRCS
#
## Expand wildcards in SRCS if they are given
#ifneq ($(or $(findstring *,$(SRCS)),$(findstring ?,$(SRCS)),$(findstring ],$(SRCS))),)
#  SRCS := $(notdir $(foreach sd,. $(SRCS_VPATH),$(wildcard $(addprefix $(d)/$(sd)/,$(SRCS)))))
#  SRCS := $(filter-out $(SRCS_EXCLUDES), $(SRCS))
#endif
#
#OBJS_$(d) := $(addprefix $(OBJPATH)/,$(addsuffix .o,$(basename $(SRCS))))
#endif
#
#CLEAN_$(d) := $(CLEAN_$(d)) $(filter /%,$(CLEAN) $(TARGETS)) $(addprefix $(d)/,$(filter-out /%,$(CLEAN)))
#
#ifdef TARGETS
#abs_tgts := $(filter /%, $(TARGETS))
#rel_tgts := $(filter-out /%,$(TARGETS))
#TARGETS_$(d) := $(abs_tgts) $(addprefix $(OBJPATH)/,$(rel_tgts))
#$(foreach tgt,$(filter-out $(AUTO_TGTS),$(rel_tgts)),$(eval $(call save_vars,$(OBJPATH)/,$(tgt))))
## Absolute targets are entry points for external (sub)projects which
## have their own build system - what is really interesting is only CMD
## and possibly DEPS however I use this general save_vars (two more vars
## that are not going to be used should not be a problem :P).
#$(foreach tgt,$(abs_tgts),$(eval $(call save_vars,,$(tgt))))
#else
#TARGETS_$(d) := $(OBJS_$(d))
#endif
#
## Save user defined vars
#$(foreach v,$(VERB_VARS),$(eval $(v)_$(d) := $($v)))
#$(foreach v,$(OBJ_VARS),$(eval $(v)_$(d) := $(addprefix $(OBJPATH)/,$($v))))
#$(foreach v,$(DIR_VARS),$(eval $(v)_$(d) := $(filter /%,$($v)) $(addprefix $(d)/,$(filter-out /%,$($v)))))
#
## Update per directory variables that are automatically inherited
#ifeq ($(origin INHERIT_DIR_VARS_$(d)),undefined)
#  INHERIT_DIR_VARS_$(d) := $(or $(INHERIT_DIR_VARS_$(parent_dir)), $(INHERIT_DIR_VARS))
#endif
#$(foreach v,$(INHERIT_DIR_VARS_$(d)),$(if $($(v)_$(d)),,$(eval $(v)_$(d) := $($(v)_$(parent_dir)))))

########################################################################
# Inclusion of subdirectories rules - only after this line one can     #
# refer to subdirectory targets and so on.                             #
########################################################################
$(foreach sd,$(SUBDIRS),$(eval $(call include_subdir_rules,$(sd))))

.PHONY: dir_$(d) clean_$(d) clean_extra_$(d) clean_tree_$(d) dist_clean_$(d)
# .SECONDARY: $(OBJPATH)

# Whole tree targets
all :: $(MY_TARGETS_$(d))

clean_all :: clean_$(d)

# # dist_clean is optimized in skel.mk if we are building in out of project tree
# ifeq ($(strip $(TOP_BUILD_DIR)),)
# dist_clean :: dist_clean_$(d)

# # No point to enforce clean_extra dependency if CLEAN is empty
# ifeq ($(strip $(CLEAN_$(d))),)
# dist_clean_$(d) :
# else
# dist_clean_$(d) : clean_extra_$(d)
# endif
# 	rm -rf $(DIST_CLEAN_DIR)
# endif

########################################################################
#                        Per directory targets                         #
########################################################################

# Again - no point to enforce clean_extra dependency if CLEAN is empty
ifeq ($(strip $(CLEAN_$(d))),)
clean_$(d) :
else
clean_$(d) : clean_extra_$(d)
endif
	rm -f $(subst clean_,,$@)/bin/* $(subst clean_,,$@)/obj/* $(subst clean_,,$@)/lib/* 

# clean_extra is meant for the extra output that is generated in source
# directory (e.g. generated source from lex/yacc) so I'm not using
# TOP_BUILD_DIR below
clean_extra_$(d) :
	rm -rf $(filter %/,$(CLEAN_$(subst clean_extra_,,$@))); rm -f $(filter-out %/,$(CLEAN_$(subst clean_extra_,,$@)))

clean_tree_$(d) : clean_$(d) $(foreach sd,$(MY_SUBDIRS_$(d)),clean_tree_$(sd))

# Skip the target rules generation and inclusion of the dependencies
# when we just want to clean up things :)
ifeq ($(filter clean clean_% dist_clean,$(MAKECMDGOALS)),)

#SUBDIRS_TGTS := $(foreach sd,$(SUBDIRS_$(d)),$(TARGETS_$(sd)))

# Use the skeleton for the "current dir"
#$(eval $(call skeleton,$(d)))
# and for each SRCS_VPATH subdirectory of "current dir"
#$(foreach vd,$(SRCS_VPATH),$(eval $(call skeleton,$(d)/$(vd))))

# Target rules for all "non automatic" targets
#$(foreach tgt,$(filter-out $(AUTO_TGTS),$(TARGETS_$(d))),$(eval $(call tgt_rule,$(tgt))))

# Way to build all targets in given subtree (not just current dir as via
# dir_$(d) - see below)
tree_$(d) : $(MY_TARGETS_$(d)) $(foreach sd,$(MY_SUBDIRS_$(d)),tree_$(sd))

# If the directory is just for grouping its targets will be targets from
# all subdirectories
#ifeq ($(strip $(TARGETS_$(d))),)
#TARGETS_$(d) := $(SUBDIRS_TGTS)
#endif

# This is a default rule - see Makefile
#dir_$(d) : $(TARGETS_$(d))

endif
endef

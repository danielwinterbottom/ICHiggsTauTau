#Necessary to use shell built-in commands
SHELL=bash

SUBDIRS = Core Utilities Modules
all:  $(SUBDIRS)

.PHONY: $(SUBDIRS)
     
Utilities: Core
Modules: Core Utilities 

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	$(MAKE) -C Core/ clean
	$(MAKE) -C Utilities/ clean
	$(MAKE) -C Modules/ clean



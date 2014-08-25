Analysis Build System {#build-system}
=====================================

[TOC]

This page documents the build system for compiling the offline analysis code as well as the folder structure this system expects. Much like the CMSSW framework, code under the **Analysis** folder is arranged into packages, with each package grouping together a related set of classes, programs and scripts. For example, the **Core** folder contains the basic classes for building an analysis and plotting the results, whereas the **HiggsTauTau** folder contains the code specific to the Higgs to taus analysis.

Code in the **Analysis** folder can be thought of as separate from the **ICHiggsTauTau** CMSSW package, and indeed is ignored by the `scram` tool. Instead we use  `make` to resolve dependencies between the different analysis packages and to compile the code. In the past, a separate makefile was provided in each package sub-directory which would have hard-coded calls to the makefiles of other packages which are dependencies. This so-called recursive make system had a number of issues, in particular the need to maintain several makefiles containing mostly repeated content and the recompilation of large amounts of code triggered by any change in other packages. See [this](http://aegis.sourceforge.net/auug97.pdf) page for a detailed discussion of the problems with recursive make. Instead a single makefile is used and which has a global awareness of code in all packages and can efficiently resolve the dependencies whilst still allowing for single packages to to be compiled. This was derived from the example system [here](https://github.com/aostruszka/nonrec-make).

Folder Structure {#build-system-folders}
========================================

Each package must contain, at the minimum, the following files and folders:

		makefile
		Rules.mk
		interface/
		src/
		obj/
		lib/
		test/
		bin/

The `makefile` should just be a symbolic link the file in `Analysis/mk/makefile`. The file `Rules.mk` is a makefile fragment and defines just a few variables to guide the build system in this package.  If you are setting up a new package you can just do:

		mkdir [package]
		cd [package]
		ln -s ../mk/makefile ./
		cp ../Rules.mk ./  # copy the top-level Rules.mk to use as a template
		mkdir interface src obj lib test bin

Class definition and implementation files should be placed in the `interface` and `src` directories respectively. Each file in the `src` should have the extension .cc* and will be compiled to a corresponding object file in the `obj` directory.  All object files are then combined into a shared library which will reside in the `lib` directory, and will have the name of the containing folder prefixed with libIC, e.g. `libICCore.so` is the library name in the **Core** package. The source code for executable programs goes in the `test` directories with the extension .cpp. Each of these files will be compiled to an executable in the `bin` directory and linked against the shared library in the process.

The Rules.mk file  {#build-system-rules-mk}
=================
A file named `Rules.mk` must be placed in each directory to tell the build system that it contains code to be compiled. This file should define three variables, some or all of which may be empty. This is an example from the **HiggsTauTau** package:

		SUBDIRS   :=
		LIB_DEPS  := Core Utilities Modules
		LIB_EXTRA := -lCondFormatsJetMETObjects

SUBDIRS is a space separated list of the any sub-directories that also define a package, it tells make to enter each directory in turn, look for a `Rules.mk` file and apply the compile rules outlined above. In this case it's empty as **HiggsTauTau** does not contain any additional packages. LIB_DEPS lists the packages that this one depends on, i.e. which other shared libraries need to be built first and then linked against the shared library and executables defined here. In this example the **Core**, **Utilities** and **Modules** shared libraries are marked as dependencies. LIB_EXTRA is a string that will added as an argument when the `g++` compiler is invoked and will typically be used to link code against additional external libraries (a number of external libraries are linked by default, see below).

When `make` is invoked it will first look for a `Rules.top` file, which has the same structure as a `Rules.mk` file and is used to identify the top directory where sub-directory scanning should start. It will first look in the current directory then repeatedly change to the enclosing directory until this file is found, which in this case in the **Analysis** folder. The `Rules.top` file here only needs to define the sub-directories to be scanned, and you should add the name of any new packages to this list. Each named directory is then scanned for compile targets by looking for files with the appropriate extensions in the `src` and `test` directories.

Invoking make {#build-system-invoke}
=============

The makefile defines a number of special targets that can be built by running: `make [target]`

 * `make dir` or just `make`: Only compile the code in the current directory, including the minimum set of dependencies in other packages if necessary.
 * `make tree`: Compile the code in the current directory, and recursively into all sub-directories starting from this one (as defined in each `Rules.mk` file)
 * `make all`: Compile every package recursively starting from the top directory
 * `make clean`: Removes every file in the `bin`, `obj` and `lib` directories in the current package
 * `make clean_tree`: Also cleans packages in sub-directories
 * `make clean_all`: Clean every package recursively starting from the top directory
 * `make bin/[program]` Shortcut target for each executable in the current directory. This provides a quick way to update a single program.

 On machines with multiple processor cores add the `-j[n]` option to compile with *n* threads. By default the makefile will just print a summary of each operation to the screen. To print the full command for each step add the option `V=1`. The option `DEBUG=1` can also be added which will cause make to print a list of the targets it finds as it scans directories.


External libraries {#build-system-external}
==================

A number of header file search paths are defined in the makefile, and several external libraries will be linked by default:

 * ROOT: The version of ROOT that ships with the CMSSW version you are using will be searched for header files and the common libraries linked. ROOT header files can be included directly in Analysis code, e.g. `#include "TFile.h"`
 * ROOFIT: As for ROOT, ROOFIT headers can be included directly, e.g. `#include "RooRealVar.h"`
 * boost: The C++ [boost](http://www.boost.org) libraries may also be used. These provide a large range of useful extensions to the core C++ libraries. Most boost libraries are header-only, meaning there is no shared library to link against. A few commonly used exceptions to this are the `filesystem`, `regex` and `program_options` libraries, and these will be linked by default. If your package requires linking to additional boost shared libraries these can be added to the LIB_EXTRA variable in the `Rules.mk` file
 * CMSSW: Files can be included relative to the CMSSW `src` directory, e.g. `#include "PhysicsTools/FWLite/interface/TFileService.h"`. Both the local CMSSW area and the base release will be searched. CMSSW libraries `libFWCoreFWLite` and `libPhysicsToolsFWLite` are linked by default as these are needed to load custom object classes stored in TTrees. The library `libUserCodeICHiggsTauTau` is also linked which contains the class definitions defined in the `Usercode/ICHiggsTauTau` package.

Compile without CMSSW {#build-system-no-cmssw}
==============================================

\todo Write instructions for CMSSW-less make

Generate ROOT dictionaries {#build-system-dict}
===============================================

\todo Write instructions for generating ROOT dictionaries



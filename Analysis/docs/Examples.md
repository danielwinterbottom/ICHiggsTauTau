Tutorial
====================================================

[TOC]

Introduction {#tutorial-introduction}
============
This page explains the basics of the analysis framework and then goes through a few examples.
All the example code can be found in the `Tutorial` folder.  The sub-folders of this directory
can be thought of as individual packages, much like in the CMSSW framework. The basic structure
is:

 * `Rules.mk`: A makefile fragment that contains just a few lines to guide `make` in compiling
   the code in this directory.
 * `interface/`: A directory containing C++ class definitions, files should end with the extension `.h`.
 * `src/`: A directory containing C++ source code with the implementation of the code defined in `interface`. Files 
   here should have the extension `.cc`.
 * `obj/`: This folder will be used by `make` to write an object (.o) file  for each source code file in `src/`.
   An additional .d file is also written for each source file - this contains a list of dependencies and is generated
   automatically during the build process
 * `lib/`: All these object files are linked to form a single shared library, with the name libIC[folder].so
 * `test/`: This contains the C++ source code for programs, therefore all files must define a `main()` function. All files
   should have the extension `.cpp`.
 * `bin/`: This folder contain: an object file, dependency file and executable for each .cpp file in `test`. During compilation
   each executable will be automatically linked against the shared library. ic::Photon 

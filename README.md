Introduction
============

This repository is best considered in two parts. The first is a CMSSW package (UserCode/ICHiggsTauTau), which follows the standard conventions:

 - Physics object classes defined and implemented in the `interface` and `src` directories
 - CMSSW modules defined and implemented in the plugins directory that covert CMSSW objects to these formats
 - Configuration fragments for cmsRun jobs in the `python` directory
 - Complete configurations for cmsRun jobs in the `test` directory
 - Compiled using the `scram` tool, which will produce ROOT dictionaries for the object classes following the specification in the `src/classes.h` and `src/classes_def.xml`

The second part is an offline analysis framework, organised into a series of packages within the `Analysis` directory. This provides:

  - A modular build system implemented with `make`
  - A simple framework for analysing events, in which the work is done by module classes, in a similar fashion to the CMSSW framework
  - The same physics object classes from above are provided in the `Objects` package, with ROOT dictionaries built using the standard `rootcint` method and steered by the `Objects/interface/LinkDef.h` file.


Documentation can be produced by running `doxygen` from within the **ICHiggsTauTau** directory. A recent copy of the html output can be found here (this updates automatically several times a day):
<http://ajgilbert.github.io/ICHiggsTauTau/index.html>

The instructions for setting a CMSSW area with the **ICHiggsTauTau** package are reproduced below.

Setting up CMSSW
================
The following steps set up a new area, but note that only certain CMSSW releases are supported. This replaces the CVS recipe of creating a new area and running the `setup_X_Y_Z.sh` script, and unfortunately this new procedure is more complicated.

All object classes and CMSSW modules compile should successfully with `scram` in each of the supported CMSSW release series:

		CMSSW_4_2_8_patch7
		CMSSW_5_3_X
		CMSSW_7_0_X

The official CMSSW repository is hosted here: <https://github.com/cms-sw/cmssw>.
If you do not already have a GitHub account, please read through the instructions at <http://cms-sw.github.io/cmssw/faq.html>, in particular ensure you have configured git with your personal information:

		git config --global user.name [First Name] [Last Name]
		git config --global user.email [Email Address]
		git config --global user.github [GitHub account name]
		#If you are running at IC you also need to do: git config --global http.sslVerify false
		#This extra line is not needed at CERN. Anywhere else if you see a fatal error message relating to SSL certificate problems it may be required

Before working with CMSSW in git, you will need to create a copy (or fork) of the official CMSSW repository in your own account at <https://github.com/cms-sw/cmssw/fork>. You will be able to store your own CMSSW branches and developments in this forked repository.

Create a new CMSSW area:

		export SCRAM_ARCH=slc5_amd64_gcc462
		# Or newer, e.g. slc5_amd64_gcc472, slc6_amd64_gcc481
		scramv1 project CMSSW CMSSW_X_Y_Z
		cd CMSSW_X_Y_Z/src/
		cmsenv

Initialise this area for git by adding a single package:

		git cms-addpkg FWCore/Version
		# If you are not running on a machine based at CERN, this script will ask if you want to create a new reference repository.
		# You should answer yes to this, and the script will copy the entire cmssw repository to your home folder,
		# which will make setting up subsequent release areas a lot faster.

This command will have created two remote repositories, `official-cmssw` and `my-cmssw`. It will also have created and switched to a new branch, `from-CMSSW_X_Y_Z`. An additional remote should be added which provides the pre-configured branches:

		git remote add ic-cmssw git@github.com:ajgilbert/cmssw.git
		# fetch from the ic-cmssw remote repository and merge the from-CMSSW_X_Y_Z into your own local branch.
		git pull ic-cmssw from-CMSSW_X_Y_Z
		# Check which branch you actually need to merge in here. Descriptive names are useful, e.g. "higgstautau_from-CMSSW_5_3_7"

		# At this point, if you run 'ls' you will not see any new packages in the release area.
		# This is because the repository operates in sparse-checkout mode, hiding folders unless they are
		# explicitly made visible.  This is important, as we don't want to have to compile every single package.
		# To make the packages visible that have been modified from the release tag, run these commands:

		git cms-sparse-checkout CMSSW_X_Y_Z HEAD
		git read-tree -mu HEAD

		# You may optionally make other packages visible that depend on those which have been modified, so that
		# they will be recompiled.
		git-cms-checkdeps -a
		# In principle this should have also been done when using CVS, but is still not essential.

Next, add the IC analysis code package:

		git clone git@github.com:ajgilbert/ICHiggsTauTau.git UserCode/ICHiggsTauTau
		./UserCode/ICHiggsTauTau/init_X_Y_Z.sh  # (if exisits) This script performs a few final tasks in the new cmssw area

At this point everything is ready, and the working area can be compiled in the normal way with `scram`.  New developments that are relevant for everyone can be committed to a branch and pushed to central IC repository (`ic-cmssw`). If you wish to test some new changes, or just share with specific people, it will be safer to work from a new branch, either based on the CMSSW release tag, or some commit on the `from-CMSSW_X_Y_Z` branch, e.g.

		git checkout -b my-analysis-from-CMSSW_X_Y_Z CMSSW_X_Y_Z # from the release tag
		git checkout -b my-analysis-from-CMSSW_X_Y_Z from-CMSSW_X_Y_Z # from the pre-configured branch
		... do some development ...
		git push my-cmssw my-new-branch

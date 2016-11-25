CMSSW & Git {#cmssw-and-git}
============

[TOC]

Setting up CMSSW in the git era {#cmssw-and-git-setup}
===============================

The entire CMSSW area (under the /src directory) is controlled by a single git repository. We create a new branch starting from a given release tag, e.g. `CMSSW_5_3_7`, and through successive commits build up the set of modified packages we need to run our own analysis code. One advantage of this method is that a user doesn't have to build this branch for every new CMSSW area they create, but can simply "fetch" a pre-built branch from a remote fork of the CMSSW git repository.

\note The CMSSW-facing part of this package is designed to compile and run out-of-the-box in a wide range of CMSSW releases. If all you need to do is run some code in this package in an un-modified CMSSW release then there is no need to set up git to the manage the CMSSW working area, just clone this repository into `UserCode/ICHiggsTauTau` following the instructions below and compile normally with `scram`.

Setting up CMSSW using a pre-configured branch {#cmssw-and-git-from-branch}
----------------------------------------------

The official CMSSW repository is hosted here: <https://github.com/cms-sw/cmssw>.
If you do not already have a GitHub account, please read through the instructions at <http://cms-sw.github.io/cmssw/faq.html>, in particular ensure you have configured git with your personal information:

		git config --global user.name [First Name] [Last Name]
		git config --global user.email [Email Address]
		git config --global user.github [GitHub account name]
		#If you are running at IC you also need to do: git config --global http.sslVerify false
		#This extra line is not needed at CERN. Anywhere else if you see a fatal error message relating to SSL certificate problems it may be required

\attention If working from a machine based at IC, the version of git supplied by the operating system may be too old to support some of the commands in these instructions.  A more up-to-date version is provided here `/home/hep/ajg06/git/`, to use this by default add this line to the `.bashrc` file:

		export PATH=/home/hep/ajg06/git/bin:$PATH

Before working with CMSSW in git, you will need to create a copy (or fork) of the official CMSSW repository in your own account at <https://github.com/cms-sw/cmssw/fork>. You will be able to store your own CMSSW branches and developments in this forked repository.

Create a new CMSSW area:

		export SCRAM_ARCH=slc5_amd64_gcc462
		# Or newer, e.g. slc5_amd64_gcc472, slc6_amd64_gcc481,slc6_amd64_gcc491,slc6_amd64_gcc493 (for new 76X samples)
		scramv1 project CMSSW CMSSW_X_Y_Z
		cd CMSSW_X_Y_Z/src/
		cmsenv

Initialise this area for git by adding a single package:

		git cms-addpkg FWCore/Version
		# If you are not running on a machine based at CERN, this script will ask if you want to create a new reference repository.
		# You should answer yes to this, and the script will copy the entire cmssw repository to your home folder,
		# which will make setting up subsequent release areas a lot faster.

This command will have created two remote repositories, `official-cmssw` and `my-cmssw`. It will also have created and switched to a new branch, `from-CMSSW_X_Y_Z`. An additional remote can be added which provides the pre-configured branches shared by the group:

		git remote add ic-cmssw git@github.com:adewit/cmssw.git
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

The CMSSW-facing part of the analysis code uses an external package that we need to add for all of the CMSSW configs to run:

		git clone git@github.com:ajgilbert/ICAnalysis-MVAMETPairProducer.git ICAnalysis/MVAMETPairProducer
		cd ICAnalysis/MVAMETPairProducer
		git checkout 7_6_X_common
		cd ../../

At this point everything is ready, and the working area can be compiled in the normal way with `scram`.  New developments that are relevant for everyone can be committed to a branch and pushed to central IC repository (`ic-cmssw`). If you wish to test some new changes, or just share with specific people, it will be safer to work from a new branch, either based on the CMSSW release tag, or some commit on the `from-CMSSW_X_Y_Z` branch, e.g.

		git checkout -b my-analysis-from-CMSSW_X_Y_Z CMSSW_X_Y_Z # from the release tag
		git checkout -b my-analysis-from-CMSSW_X_Y_Z from-CMSSW_X_Y_Z # from the pre-configured branch
		... do some development ...
		git push my-cmssw my-new-branch

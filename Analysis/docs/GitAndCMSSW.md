CMSSW & Git {#cmssw-and-git}
============

[TOC]

Setting up CMSSW in the git era {#cmssw-and-git-setup}
===============================

The migration from CVS to git forces quite a change to the way in which we prepare a CMSSW area for analysis work. Previously, a simple script was executed that would check-out each required package in turn, specifying a tag for each.  Now, the entire CMSSW area (under the /src directory) is controlled by a single git repository. As git tracks the state of the entire repository it is no longer possible mix together modules with different tags.  Instead, we create a new branch starting from a given release tag, e.g. `CMSSW_5_3_7`, and through successive commits build up the set of modified packages we need to run our own analysis code. One advantage of this method is that a user doesn't have to build this branch for every new CMSSW area they create, but can simply "fetch" a pre-built branch from a remote fork of the CMSSW git repository. 

Setting up CMSSW using a pre-configured branch {#cmssw-and-git-from-branch}
----------------------------------------------
The following steps set up a new area, but note that only certain CMSSW releases are supported. This replaces the CVS recipe of creating a new area and running the `setup_X_Y_Z.sh` script, and unfortunately this new procedure is more complicated.

Supported releases:

    CMSSW_4_2_8_patch7
		CMSSW_5_3_7
		CMSSW_5_3_11

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
		# Newer versions of CMSSW need SCRAM_ARCH=slc5_amd64_gcc472
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
		git pull ic-cmssw from-CMSSW_X_Y_Z # fetch from the ic-cmssw remote repository and merge the from-CMSSW_X_Y_Z into your own local branch.

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

		mkdir UserCode
		cd UserCode
		git clone git@github.com:ajgilbert/ICHiggsTauTau.git
		cd ICHiggsTauTau
		./init_X_Y_Z.sh # This script performs a few final tasks in the new cmssw area

At this point everything is ready, and the working area can be compiled in the normal way with `scram`.  New developments that are relevant for everyone can be committed to the `from-CMSSW_X_Y_Z` branch and pushed to central IC repository (`ic-cmssw`).  If you wish to test some new changes, or just share with specific people, it will be safer to work from a new branch, either based on the CMSSW release tag, or some commit on the `from-CMSSW_X_Y_Z` branch, e.g.

		git checkout -b my-new-branch CMSSW_X_Y_Z # from the release tag
		git checkout -b my-new-branch from-CMSSW_X_Y_Z # from the pre-configured branch
		... do some development ...
		git push my-cmssw my-new-branch

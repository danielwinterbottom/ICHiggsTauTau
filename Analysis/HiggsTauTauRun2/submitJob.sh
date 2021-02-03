#!/bin/bash
source /vols/grid/cms/setup.sh
export LD_LIBRARY_PATH=/vols/cms/ktc17/CMSSW_10_2_19/biglib/slc7_amd64_gcc700:/vols/cms/ktc17/CMSSW_10_2_19/lib/slc7_amd64_gcc700:/vols/cms/ktc17/CMSSW_10_2_19/external/slc7_amd64_gcc700/lib:/cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_2_19/biglib/slc7_amd64_gcc700:/cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_2_19/lib/slc7_amd64_gcc700:/cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_2_19/external/slc7_amd64_gcc700/lib:/cvmfs/cms.cern.ch/slc7_amd64_gcc700/external/llvm/6.0.0-ogkkac/lib64:/cvmfs/cms.cern.ch/slc7_amd64_gcc700/external/gcc/7.0.0-omkpbe2/lib64:/cvmfs/cms.cern.ch/slc7_amd64_gcc700/external/gcc/7.0.0-omkpbe2/lib:/usr/lib64:/cvmfs/grid.cern.ch/centos7-umd4-ui-4.0.3-1_191004/lib64:/cvmfs/grid.cern.ch/centos7-umd4-ui-4.0.3-1_191004/lib:/cvmfs/grid.cern.ch/centos7-umd4-ui-4.0.3-1_191004/usr/lib64:/cvmfs/grid.cern.ch/centos7-umd4-ui-4.0.3-1_191004/usr/lib:$LD_LIBRARY_PATH
./bin/TauSpinnerMasters


Heavy Higgs Kinematic Fit

compile HHKinFit with:   ./compile.sh
  -->creates a shared library (libHHKinFit.so) as well as an excutable (runHHKinFit)

run HHKinFit with:       ./runHHKinFit
  -->performs HHKinFit based on ntuples in the example folder
  -->creates some terminal output as well as a root file (result.root) with the HHKinFit results

Please make sure that the HHKinFit directory is included in the LD_LIBRARY_PATH in order to let
you computer know where the shared library can be found:
export LD_LIBRARY_PATH=<<PATH TO DIRECTORY>>

Open issues:
* non-positive-definit covariance matrix in some cases
* used b-jet energy error underestimated

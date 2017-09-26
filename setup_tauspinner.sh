loc=$PWD
#wget lcgapp.cern.ch/project/simu/HepMC/download/HepMC-2.06.09.tar.gz
#tar xvzf HepMC-2.06.09.tar.gz
#wget tauolapp.web.cern.ch/tauolapp/resources/TAUOLA.1.1.6c/TAUOLA.1.1.6c-LHC.tar.gz
#tar xvzf TAUOLA.1.1.6c-LHC.tar.gz
#mkdir HepMC
#cd HepMC
#cmake -DCMAKE_INSTALL_PREFIX=HepMC \
#      -Dmomentum:STRING=GEV \
#      -Dlength:STRING=CM \
#      ../HepMC-2.06.09
#      make
#      make test
#      make install
#cd ..
cd TAUOLA/
#./configure --with-hepmc=../HepMC/HepMC/; make; make install
./configure --without-hepmc; make; make install
cd ..
##wget http://www.hepforge.org/archive/lhapdf/LHAPDF-6.2.1.tar.gz
##tar xf LHAPDF-6.2.1.tar.gz
cp plugins/tauspinner.xml $CMSSW_BASE/config/toolbox/$SCRAM_ARCH/tools/selected/tauspinner.xml
scram setup tauspinner


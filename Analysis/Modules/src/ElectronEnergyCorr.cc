#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/ElectronEnergyCorr.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"

#include "TRandom3.h"

namespace ic {

  ElectronEnergyCorr::ElectronEnergyCorr(std::string const& name, std::string input_name) : ModuleBase(name) {
    input_name_ = input_name;
    rand_ = new TRandom3();
  }

  ElectronEnergyCorr::~ElectronEnergyCorr() {
    ;
  }

  int ElectronEnergyCorr::PreAnalysis() {
    return 0;
  }

  int ElectronEnergyCorr::Execute(TreeEvent *event) {
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    unsigned run = eventInfo->run();
    std::vector<Electron *> & vec = event->GetPtrVec<Electron>(input_name_);
    for (unsigned i = 0; i < vec.size(); ++i) {
      double corrected_energy = corrVgamma(vec[i]->energy(), fabs(vec[i]->sc_eta()), vec[i]->r9(), rand_, run, 0, true);
      vec[i]->set_pt(corrected_energy / std::cosh(vec[i]->eta()));
      vec[i]->set_energy(corrected_energy);
    }
    return 0;
  }

  int ElectronEnergyCorr::PostAnalysis() {
    return 0;
  }

  void ElectronEnergyCorr::PrintInfo() {
    ;
  }

  double ElectronEnergyCorr::corrVgamma(double en, double absEta, double r9, 
        TRandom3* rr, int run, float par, 
        bool data) {
    double eta = absEta;
    static double mz = 91.19;
    // list of correction coefficiencints for EB
    // |eta| < 1.0 r9 > 0.94, |eta| < 1.0 r9 < 0.94, 4th r9 > 0.94, 4th r9 < 0.94
    static float scaleDataCorrEB[4] = { 0.000, -0.355, -0.210,  1.480}; 
    static float scaleMCCorrEB[4]   = { 0.190, -0.635, -0.758,  0.445};
    static float smearMCCorrEB[4]   = { 0.950,  0.000,  0.322,  1.750}; 

    // list of correction coefficients for EE
    static float scaleDataCorrEE[4] = { 0.00,   2.40,   0.00,   1.05}; 
    static float scaleMCCorrEE[4]   = {-1.70,   0.58,  -1.57,  -1.25};
    static float smearMCCorrEE[4]   = { 2.48,   2.56,   3.32,   2.60}; 

    // Corrections for data, EB
    if ( data && eta < 1.5 ) {
      // run ranges between interventions
      bool run1  = run < 161016;      // very small
      bool run2  = run >= 161016 && run < 162762; // technical stop
      bool run3  = run >= 162762 && run < 163869; // 
      bool run4  = run >= 163869 && run < 165204; // technical stop
      bool run5  = run >= 165204 && run < 166380; // 
      bool run6  = run >= 166380 && run < 166839; // noisy period
      bool run7  = run >= 166839 && run < 167284;
      bool run8  = run >= 167284 && run < 167674; // noisy period
      bool run9  = run >= 167674 && run < 167898;

      bool run10 = run >= 167898 && run < 176100;
      bool run11 = run >= 176100 && run < 176400;
      bool run12 = run >= 176400 && run < 176600;
      bool run13 = run >= 176600 && run < 177000;
      bool run14 = run >= 177000 && run < 177500;
      bool run15 = run >= 177500 && run < 178000;
      bool run16 = run >= 178000 && run < 178300;
      bool run17 = run >= 178300 && run < 178600;
      bool run18 = run >= 178600 && run < 179100;
      bool run19 = run >= 179100 && run < 179600;
      bool run20 = run >= 179600;
      

      int iRun = 0;
      if ( run1  ) iRun = 0;
      if ( run2  ) iRun = 1;
      if ( run3  ) iRun = 2;
      if ( run4  ) iRun = 3;
      if ( run5  ) iRun = 4;
      if ( run6  ) iRun = 5;
      if ( run7  ) iRun = 6;
      if ( run8  ) iRun = 7;
      if ( run9  ) iRun = 8;
      if ( run10 ) iRun = 9;
      if ( run11 ) iRun = 10;
      if ( run12 ) iRun = 11;
      if ( run13 ) iRun = 12;
      if ( run14 ) iRun = 13;
      if ( run15 ) iRun = 14;
      if ( run16 ) iRun = 15;
      if ( run17 ) iRun = 16;
      if ( run18 ) iRun = 17;
      if ( run19 ) iRun = 18;
      if ( run20 ) iRun = 19;

      float corr[20] = {1 - 0.130/mz,  // 1
            1 + 0.040/mz,  // 2
            1 - 0.200/mz,  // 3
            1 - 0.000/mz,  // 4
            1 + 0.112/mz,  // 5
            1 + 0.279/mz,  // 6
            1 + 0.373/mz,  // 7
            1 + 0.120/mz,  // 8
            1 + 0.611/mz,  // 9
            1 + 0.206/mz,  // 10
            1 + 0.580/mz,  // 11
            1 + 0.528/mz,  // 12
            1 + 0.390/mz,  // 13
            1 + 0.593/mz,  // 14
            1 + 0.485/mz,  // 15
            1 + 0.639/mz,  // 16
            1 + 0.617/mz,  // 17
            1 + 0.829/mz,  // 18
            1 + 0.676/mz,  // 19
            1 + 1.171/mz   // 20
      }; 

      // data scaling factors
      en *= corr[iRun];
      if ( eta < 1.0 ) {
        if ( r9 <= 0.94 ) en *= 1 + scaleDataCorrEB[1]/mz; // DONE!
      } else {
        // 4th module
        en *= 1 + scaleDataCorrEB[2]/mz; // r9 > 0.94; DONE!
        if ( r9 <= 0.94 ) 
    // additional correction for r9 < 0.94 DONE!
    en *= 1 + scaleDataCorrEB[3]/mz; 
      }
    }    

    // MC ***************************************************************
    // Corrections for MC, EB
    if ( !data && eta < 1.5 ) {
      // energy scale
      en *= 1 + scaleMCCorrEB[0]/mz;
      en *= rr->Gaus(1, smearMCCorrEB[0]/mz); 

      if ( eta < 1.0 ) {
        if ( r9 <= 0.94 ) {
    en *= 1 + scaleMCCorrEB[1]/mz; // 0.14
    en *= rr->Gaus(1, smearMCCorrEB[1]/mz); // extra 0.35% smearing 
        }
      } else {
        // 4th module
        en *= 1 + scaleMCCorrEB[2]/mz; 
        en *= rr->Gaus(1, smearMCCorrEB[2]/mz); // extra 1.8% ~0.80 GeV
        if ( r9 <= 0.94 ) {
    en *= 1 + scaleMCCorrEB[3]/mz;
    en *= rr->Gaus(1, smearMCCorrEB[3]/mz); // 1.37
        }
      }

    }

    // EE corrections for data
    if ( data && eta > 1.5 ) {

      // Use Paolo's run ranges
      bool run1 = run <= 167913;
      bool run2 = run > 167913 && run <= 172619;
      bool run3 = run > 172619 && run <= 173692;
      bool run4 = run > 173692 && run <= 177139;
      bool run5 = run > 177139 && run <= 178421;
      bool run6 = run > 178421;

      float eeCorr1[6] = {1 - 1.28/mz,
        1 - 2.90/mz,
        1 - 1.65/mz,
        1 - 3.70/mz,
        1 - 4.14/mz,
        1 - 4.14/mz
      };

      float eeCorr2[6] = {1 + 3.00/mz,
        1 - 1.85/mz,
        1 - 1.84/mz,
        1 - 1.33/mz,
        1 - 2.05/mz,
                          1 - 1.62/mz};
      int iRun = 0;
      if ( run1 ) iRun = 0;
      if ( run2 ) iRun = 1;
      if ( run3 ) iRun = 2;
      if ( run4 ) iRun = 3;
      if ( run5 ) iRun = 4;
      if ( run6 ) iRun = 5;

      if ( eta < 2.0 )  {
        en *= eeCorr1[iRun];
        if ( r9 <= 0.94 ) en *= 1 + scaleDataCorrEE[1]/mz;
      } else {
        en *= eeCorr2[iRun];
        if ( r9 <= 0.94 ) en *= 1 + scaleDataCorrEE[3]/mz;
      }
    }
    // MC
    if ( !data && eta > 1.5 ) {
      // scales
      if ( eta <  2.0 && r9 >  0.94 ) en *= 1 + scaleMCCorrEE[0]/mz;
      if ( eta <  2.0 && r9 <= 0.94 ) en *= 1 + scaleMCCorrEE[1]/mz;
      if ( eta >= 2.0 && r9 >  0.94 ) en *= 1 + scaleMCCorrEE[2]/mz;
      if ( eta >= 2.0 && r9 <= 0.94 ) en *= 1 + scaleMCCorrEE[3]/mz;

      // resolutions
      if ( eta <  2.0 && r9 >  0.94 ) en *= rr->Gaus(1, smearMCCorrEE[0]/mz);
      if ( eta <  2.0 && r9 <= 0.94 ) en *= rr->Gaus(1, smearMCCorrEE[1]/mz);
      if ( eta >= 2.0 && r9 >  0.94 ) en *= rr->Gaus(1, smearMCCorrEE[2]/mz);
      if ( eta >= 2.0 && r9 <= 0.94 ) en *= rr->Gaus(1, smearMCCorrEE[3]/mz);
    }

    return en;
  }

}

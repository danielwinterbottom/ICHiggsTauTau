#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/SetDoDY.h"
#include <iostream>
#include <vector>
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvWeights.h"

using namespace ic;

void SetDoDY(ic::mc mc, HinvWeights* xsWeights){

  long double TF_oneMinusAll,TF_htBin1,TF_htBin2,TF_htBin3,TF_htBin4;
  long double IY_Inclusive,IY_htBin1,IY_htBin2,IY_htBin3,IY_htBin4;

  if (mc == mc::spring16_80X){
    //XS_MC_DYJetsToLL and 1 2 3 4
    TF_oneMinusAll = 0.750730267;
    TF_htBin1      = 0.168044214;
    TF_htBin2      = 0.055234681;
    TF_htBin3      = 0.016895704;
    TF_htBin4      = 0.009095134;

    //EVT_MC_DYJetsToLL and 1 2 3 4
    IY_Inclusive   = 49877138;
    IY_htBin1      = 65485168;
    IY_htBin2      = 19695514;
    IY_htBin3      = 5753813;
    IY_htBin4      = 4101383;
  }
  if (mc == mc::fall15_76X){
    //XS_MC_DYJetsToLL and 1 2 3 4
    TF_oneMinusAll = 0.696628989;
    TF_htBin1      = 0.204582155;
    TF_htBin2      = 0.067178037;
    TF_htBin3      = 0.020549051;
    TF_htBin4      = 0.011061768;

    //EVT_MC_DYJetsToLL and 1 2 3 4
    IY_Inclusive   = 9004328.0;
    IY_htBin1      = 65314144.0;
    IY_htBin2      = 19989058.0;
    IY_htBin3      = 5701878.0;
    IY_htBin4      = 4189017.0;
  }
  if (mc == mc::summer12_53X) {
    //XS_MC_DYJetsToLL and 1 2 3 4
    TF_oneMinusAll = 0.723342373;
    TF_htBin1      = 0.190169492;
    TF_htBin2      = 0.061355932;
    TF_htBin3      = 0.017322034;
    TF_htBin4      = 0.007810169;

    //EVT_MC_DYJetsToLL and 1 2 3 4
    IY_Inclusive   = 30459503.0;
    IY_htBin1      = 23970248.0;
    IY_htBin2      = 21852156.0;
    IY_htBin3      = 11015445.0;
    IY_htBin4      = 6402827.0;
    //if(prod=="Apr04"){xsWeights.SetDYInputYields(30459503.0, 23970248.0, 21852156.0, 11015445.0, 6402827.0);}
    //else{xsWeights.SetDYInputYields(30459503.0, 24045248.0, 21852156.0, 11015445.0, 6402827.0);}
  }

  xsWeights->set_do_dy_soup(true);
  xsWeights->set_do_dy_reweighting(false);
  xsWeights->SetDYTargetFractions(TF_oneMinusAll,TF_htBin1,TF_htBin2,TF_htBin3,TF_htBin4);
  xsWeights->SetDYInputYields(IY_Inclusive,IY_htBin1,IY_htBin2,IY_htBin3,IY_htBin4);
}
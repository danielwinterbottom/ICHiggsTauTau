#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/SetDoW.h"
#include <iostream>
#include <vector>
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvWeights.h"

using namespace ic;

void SetDoW(ic::mc mc, HinvWeights* xsWeights){

  long double TF_oneMinusAll,TF_htBin1,TF_htBin2,TF_htBin3,TF_htBin4,TF_htBin5,TF_htBin6,TF_htBin7;
  long double IY_Inclusive,IY_htBin1,IY_htBin2,IY_htBin3,IY_htBin4,IY_htBin5,IY_htBin6,IY_htBin7;

  if (mc == mc::spring16_80X){
    //XS_MC_WJetsToLNu-mg*
    TF_oneMinusAll = 9.65141122e-01;
    TF_htBin1      = 2.64511170e-02;
    TF_htBin2      = 7.07395326e-03;
    TF_htBin3      = 9.61876714e-04;
    TF_htBin4      = 2.36978418e-04;
    TF_htBin5      = 1.08184089e-04;
    TF_htBin6      = 2.61364578e-05;
    TF_htBin7      = 6.32466880e-07;

    //EVT_MC_WJetsToLNu-mg*
    IY_Inclusive   = 28210360;
    IY_htBin1      = 37778906;
    IY_htBin2      = 19851624;
    IY_htBin3      = 7432746;
    IY_htBin4      = 18133257;
    IY_htBin5      = 7854734;
    IY_htBin6      = 7063909;
    IY_htBin7      = 2507809;

    xsWeights->set_do_w_soup(true);
    xsWeights->set_do_w_reweighting(false);
    xsWeights->SetWTargetFractions(TF_oneMinusAll,TF_htBin1,TF_htBin2,TF_htBin3,TF_htBin4,TF_htBin5,TF_htBin6,TF_htBin7);
    xsWeights->SetWInputYields(IY_Inclusive,IY_htBin1,IY_htBin2,IY_htBin3,IY_htBin4,IY_htBin5,IY_htBin6,IY_htBin7);
  }
  else{
    if (mc == mc::fall15_76X){

      //XS_MC_WJetsToLNu-mg*
      TF_oneMinusAll = 9.64822231e-01;
      TF_htBin1      = 2.66852257e-02;
      TF_htBin2      = 7.15251541e-03;
      TF_htBin3      = 9.67786836e-04;
      TF_htBin4      = 3.72241002e-04;
      //xsWeights.SetWTargetFractions(9.539946e-01,2.661281e-02,7.043996e-03,9.168533e-04,3.272767e-04);

      //EVT_MC_WJetsToLNu-mg*
      IY_Inclusive   = 47103549;
      IY_htBin1      = 10205377;
      IY_htBin2      = 4949568;
      IY_htBin3      = 1943664;
      IY_htBin4      = 1041358;
    }
    if (mc == mc::summer12_53X) {

      //XS_MC_WJetsToLNu-mg*
      TF_oneMinusAll = 0.74069073;
      TF_htBin1      = 0.1776316;
      TF_htBin2      = 0.0575658;
      TF_htBin3      = 0.0170724;
      TF_htBin4      = 0.00703947;

      //EVT_MC_WJetsToLNu-mg*
      IY_Inclusive   = 76102995;
      IY_htBin1      = 23141598;
      IY_htBin2      = 34044921;
      IY_htBin3      = 15539503;
      IY_htBin4      = 13382803;
    }

    xsWeights->set_do_w_soup(true);
    xsWeights->set_do_w_reweighting(false);
    xsWeights->SetWTargetFractions(TF_oneMinusAll,TF_htBin1,TF_htBin2,TF_htBin3,TF_htBin4);
    xsWeights->SetWInputYields(IY_Inclusive,IY_htBin1,IY_htBin2,IY_htBin3,IY_htBin4);
  }
}
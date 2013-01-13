#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagWeight.h"
#include <cmath>

namespace ic {



  BTagWeight::BTagWeight() {
    louvain_eff_ = new TF1("sigmoidTimesL","[0]+([3]+[4]*x)/(1+exp([1]-x*[2]))",20,1000);
    rand = new TRandom3(0);

  }
  BTagWeight::~BTagWeight() {
    //delete louvain_eff_;
  }


  double BTagWeight::LouvainBEff(int flavor, std::string const& algo_label, double pt, double eta) const {
    int algo = 1;
    if (algo_label == "SSVHEM") {
      algo = 1;
    } else if (algo_label == "SSVHPT") {
      algo = 2;
    } else {
      std::cerr << "LouvainBEff not available for working point: " << algo_label << std::endl;
      throw;
    }
    flavor = std::abs(flavor);
    switch((flavor)) {
      case 5: {
        if(algo==1) {
          if(fabs(eta)<1.2) {
            louvain_eff_->SetParameters(-1.48976e+01,-2.33468e+00,5.63766e-02,1.54646e+01,-1.76495e-04);
          } else {
            louvain_eff_->SetParameters(-1.49449e+01,-2.48731e+00,5.10619e-02,1.54167e+01,-9.20870e-05);
          }
        } else {
          if(fabs(eta)<1.2) {
            louvain_eff_->SetParameters(-2.22397e+01,-2.73586e+00,5.61474e-02,2.28068e+01,-1.76713e-04);
          } else {
            louvain_eff_->SetParameters(-4.46156e+01,-3.58358e+00,5.06866e-02,4.50875e+01,-9.21825e-05);
          }
        }
        return louvain_eff_->Eval(pt);
      }
      case 4: {
        if(algo==1) {
          if(fabs(eta)<1.2) {
            louvain_eff_->SetParameters(-2.35646e+00,-2.23076e+00,5.30713e-02,2.44600e+00,-4.10810e-05);
          } else {
            louvain_eff_->SetParameters(-2.36127e+00,-2.39206e+00,4.54041e-02,2.44112e+00,-6.91750e-05);
          }
        } else {
          if(fabs(eta)<1.2) {
            louvain_eff_->SetParameters(-1.31439e+01,-3.95421e+00,5.24966e-02,1.32335e+01,-4.11156e-05);
          } else {
            louvain_eff_->SetParameters(-1.31488e+01,-4.11692e+00,4.49178e-02,1.32286e+01,-6.91372e-05);
          }
        }
        return louvain_eff_->Eval(pt);
      }
      default: {
        if(algo==1) {
          if(fabs(eta)<1.2) {
            louvain_eff_->SetParameters(1.69103e-04,5.09983e+00,6.26066e-02,9.38001e-04,2.13956e-06);
          } else {
            louvain_eff_->SetParameters(1.86636e-04,4.41344e+00,4.72039e-02,3.77524e-03,-1.22904e-06);
          }
        } else {
          if(fabs(eta)<1.2) {
            louvain_eff_->SetParameters(-1.31642e+01,-8.03625e+00,3.84933e-03,1.31682e+01,-1.84878e-06);
          } else {
            louvain_eff_->SetParameters(-1.31639e+01,-7.84069e+00,8.51549e-03,1.31682e+01,0.00000e+00);
          }
        }
        return louvain_eff_->Eval(pt);
      }
    }
  }

  double BTagWeight::BEff(int flavor, std::string const& algo_label, double pt, double eta) const {
    int algo = 1;
    double x = pt;
    if (fabs(eta) > 2.4) {
      std::cerr << "Jet eta is too large: " << eta << std::endl;
      throw;
    }
    if (algo_label == "SSVHEM") {
      algo = 1;
    } else if (algo_label == "SSVHPT") {
      algo = 2;
    } else if (algo_label == "CSVM") {
      algo = 3;
    } else {
      std::cerr << "BEff not available for working point: " << algo_label << std::endl;
      throw;
    }
    flavor = std::abs(flavor);
    if (flavor == 5) {
      if (algo == 1) return 0.64939;
      if (algo == 2) return 0.48337;
      if (algo == 3) return 0.72973;  
      // if (algo == 3) {
      //   if (x < 40.) return 0.583456;
      //   if (x < 60.) return 0.683128;
      //   if (x < 80.) return 0.726616;
      //   if (x < 100.) return 0.753785;
      //   if (x < 150.) return 0.760325;
      //   if (x < 200.) return 0.744039;
      //   if (x < 300.) return 0.702678;
      //   return 0.585133;
      // }
      // if (algo == 3) {
      //   if (x < 40.) return 0.523783;
      //   if (x < 60.) return 0.623438;
      //   if (x < 80.) return 0.687386;
      //   if (x < 100.) return 0.708507;
      //   if (x < 150.) return 0.728597;
      //   if (x < 200.) return 0.720094;
      //   if (x < 300.) return 0.67827;
      //   return 0.585133;
      // }


    } else if (flavor == 4) {
      if (algo == 1) return 0.17134;
      if (algo == 2) return 0.06522;
      if (algo == 3) return 0.19249;
    } else {
      if (algo == 1) return (((-0.000420178+(0.00029105*x))+(-8.9398e-07*(x*x)))+(1.35401e-09*(x*(x*x))))+(-7.93826e-13*(x*(x*(x*x))));
      if (algo == 2) return (-2.9605e-05+(2.35624e-05*x))+(-1.77552e-08*(x*x));
      if (algo == 3) return (0.0113428+(5.18983e-05*x))+(-2.59881e-08*(x*x));
    }
    return 1.0;
  }


  double BTagWeight::SF(int flavor, std::string const& algo_label, double pt, double eta, bool is_2012) const {
    int algo = 1;
    double x = pt;
    if (fabs(eta) > 2.4) {
      std::cerr << "Jet eta is too large: " << eta << std::endl;
      throw;
    }
    if (algo_label == "SSVHEM") {
      algo = 1;
    } else if (algo_label == "SSVHPT") {
      algo = 2;
    } else if (algo_label == "CSVM") {
      algo = 3;
    } else {
      std::cerr << "SF not available for working point: " << algo_label << std::endl;
      throw;
    }
    double sf = 1.0;
    flavor = std::abs(flavor);
    if (!is_2012) {
      if (pt < 30.0) x = 30.0;
      if (flavor == 5 || flavor == 4) {
        if (algo == 1) sf = 0.896462*((1.+(0.00957275*x))/(1.+(0.00837582*x)));
        if (algo == 2) sf = 0.422556*((1.+(0.437396*x))/(1.+(0.193806*x)));
        if (algo == 3) sf = 0.6981*((1.+(0.414063*x))/(1.+(0.300155*x)));
      } else {
        if (algo == 1) sf = ((0.890254+(0.000553319*x))+(-1.29993e-06*(x*x)))+(4.19294e-10*(x*(x*x)));
        if (algo == 2) sf = ((0.97409+(0.000646241*x))+(-2.86294e-06*(x*x)))+(2.79484e-09*(x*(x*x)));
        if (algo == 3) sf = ((1.04318+(0.000848162*x))+(-2.5795e-06*(x*x)))+(1.64156e-09*(x*(x*x)));
      }
    } else {
      if (pt < 20.0) x = 20.0;
      if (flavor == 5 || flavor == 4) {
        if (algo == 1) sf = 0.896462*((1.+(0.00957275*x))/(1.+(0.00837582*x))); // This is just the same as 2011
        if (algo == 2) sf = 0.422556*((1.+(0.437396*x))/(1.+(0.193806*x))); // This is just the same as 2011
        if (algo == 3) sf = 0.726981*((1.+(0.253238*x))/(1.+(0.188389*x)));
      } else {
        if (algo == 1) sf = ((0.890254+(0.000553319*x))+(-1.29993e-06*(x*x)))+(4.19294e-10*(x*(x*x))); // This is just the same as 2011
        if (algo == 2) sf = ((0.97409+(0.000646241*x))+(-2.86294e-06*(x*x)))+(2.79484e-09*(x*(x*x))); // This is just the same as 2011
        if (algo == 3) {
          if (fabs(eta) < 0.8)                      sf = ((1.06238+(0.00198635*x))+(-4.89082e-06*(x*x)))+(3.29312e-09*(x*(x*x)));
          if (fabs(eta) >= 0.8 && fabs(eta) < 1.6)  sf = ((1.08048+(0.00110831*x))+(-2.96189e-06*(x*x)))+(2.16266e-09*(x*(x*x)));
          if (fabs(eta) >= 0.8)                     sf = ((1.09145+(0.000687171*x))+(-2.45054e-06*(x*x)))+(1.7844e-09*(x*(x*x)));
        }
      }

    }

    return sf;

  }

  std::pair<float,float> BTagWeight::weight(std::vector<JetInfo> jets, int minTags, int maxTags) const {
    int njets=jets.size();
    if (njets == 0) return (std::pair<float,float>(1.0,1.0));
    int comb= 1 << njets;
    float pMC=0;
    float pData=0;
    for(int i=0;i < comb; i++) {
      float mc=1.;
      float data=1.;
      int ntagged=0;
      for(int j=0;j<njets;j++) {
        bool tagged = ((i >> j) & 0x1) == 1;
        if(tagged) {
          ntagged++;
          mc*=jets[j].eff;
          data*=jets[j].eff*jets[j].sf;
        } else {
          mc*=(1.-jets[j].eff);
          data*=(1.-jets[j].eff*jets[j].sf);
        }
      }     
      if(ntagged >= minTags && ntagged <= maxTags) {
        pMC+=mc;
        pData+=data;
      }
    }
    return std::pair<float,float>(pData,pMC);
  }

  double BTagWeight::GetWeight(std::vector<PFJet *> const& jets, std::string const& tagger, unsigned min, unsigned max, bool is_2012) const {
    std::vector<BTagWeight::JetInfo> infos;
    for (unsigned i = 0; i < jets.size(); ++i) {
      double eff = BEff(jets[i]->parton_flavour(), tagger, jets[i]->pt(), jets[i]->eta());
      double sf = SF(jets[i]->parton_flavour(), tagger, jets[i]->pt(), jets[i]->eta(), is_2012);
      double x = jets[i]->pt();
      unsigned flav = std::abs(jets[i]->parton_flavour());
      if (tagger == "CSVM" && is_2012 && flav !=5 && flav != 4) sf *= 1.10422 + -0.000523856*x + 1.14251e-06*x*x;
      infos.push_back(BTagWeight::JetInfo(eff, sf));
    }
    std::pair<float, float> result = weight(infos, min, max);
    return result.first / result.second;
  }

  void BTagWeight::ReTag(std::vector<PFJet *> & jets, bool is_2012) const {
    for (unsigned i = 0; i < jets.size(); ++i) {
      double eff = BEff(jets[i]->parton_flavour(), "CSVM", jets[i]->pt(), jets[i]->eta());
      double sf = SF(jets[i]->parton_flavour(), "CSVM", jets[i]->pt(), jets[i]->eta(), is_2012);
      // double x = jets[i]->pt();
      // unsigned flav = std::abs(jets[i]->parton_flavour());
      // if (is_2012 && flav !=5 && flav != 4) sf *= 1.10422 + -0.000523856*x + 1.14251e-06*x*x;
      double demoteProb_btag, promoteProb_btag;
      if(sf < 1) {
        demoteProb_btag = fabs(1.0 - sf);
      } else {
        promoteProb_btag = fabs(sf - 1.0)/((sf/eff) - 1.0);
      }
      bool passtag = jets[i]->GetBDiscriminator("combinedSecondaryVertexBJetTags") > 0.679;

        if(passtag) {                       // if tagged
          if(demoteProb_btag > 0 && rand->Uniform() < demoteProb_btag) {
            jets[i]->SetBDiscriminator("combinedSecondaryVertexBJetTags", 0.60);
          }
        } else {
          if(promoteProb_btag > 0 && rand->Uniform() < promoteProb_btag) {
            jets[i]->SetBDiscriminator("combinedSecondaryVertexBJetTags", 0.70);
          }
        }
      }
    }
 




  double BTagWeight::GetLouvainWeight(std::vector<PFJet *> const& jets, std::string const& tagger, unsigned min, unsigned max) const {
    std::vector<BTagWeight::JetInfo> infos;
    for (unsigned i = 0; i < jets.size(); ++i) {
      double eff = LouvainBEff(jets[i]->parton_flavour(), tagger, jets[i]->pt(), jets[i]->eta());
      double sf = SF(jets[i]->parton_flavour(), tagger, jets[i]->pt(), jets[i]->eta(), false);
      infos.push_back(BTagWeight::JetInfo(eff, sf));
    }
    std::pair<float, float> result = weight(infos, min, max);
    return result.first / result.second;
  }


}

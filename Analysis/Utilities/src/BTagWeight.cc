#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagWeight.h"
#include <cmath>

namespace ic {



  BTagWeight::BTagWeight() {
    louvain_eff_ = new TF1("sigmoidTimesL","[0]+([3]+[4]*x)/(1+exp([1]-x*[2]))",20,1000);
    rand = new TRandom3(0);
    static float ptbins_2012[] = {20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600, 800};
    static float ptbins_2011[] = {20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 670};
    static float SFb_error_2012[] = {
      0.0415707,
      0.0204209,
      0.0223227,
      0.0206655,
      0.0199325,
      0.0174121,
      0.0202332,
      0.0182446,
      0.0159777,
      0.0218531,
      0.0204688,
      0.0265191,
      0.0313175,
      0.0415417,
      0.0740446,
      0.0596716 };
    static float SFb_error_2011[] = {
      0.0295675,
      0.0295095,
      0.0210867,
      0.0219349,
      0.0227033,
      0.0204062,
      0.0185857,
      0.0256242,
      0.0383341,
      0.0409675,
      0.0420284,
      0.0541299,
      0.0578761,
      0.0655432 };

    unsigned nbins_2012=sizeof(ptbins_2012)/sizeof(ptbins_2012[0]) - 1;
    unsigned nbins_2011=sizeof(ptbins_2011)/sizeof(ptbins_2011[0]) - 1;
    SFb_error_2012_ = new TH1F("SFb_error_2012", "SFb_error_2012",nbins_2012, ptbins_2012);
    SFb_error_2011_ = new TH1F("SFb_error_2011", "SFb_error_2011",nbins_2011, ptbins_2011);
    for(unsigned i=0; i<nbins_2012; i++)
    { 
       SFb_error_2012_->SetBinContent(i+1, SFb_error_2012[i]) ;
    }
    for(unsigned i=1; i<nbins_2011; i++)
    { 
       SFb_error_2011_->SetBinContent(i+1, SFb_error_2011[i-1]) ;
    }
    //Adding number for 20-30 bin  
    SFb_error_2011_->SetBinContent(1, 0.12);

  }
  BTagWeight::~BTagWeight() {
    //delete louvain_eff_;
  }


  double BTagWeight::LouvainBEff(int flavor, BTagWeight::tagger const& tag, double pt, double eta) const {
    int algo = 1;
    if (tag == tagger::SSVHEM) {
      algo = 1;
    } else if (tag == tagger::SSVHPT) {
      algo = 2;
    } else {
      std::cerr << "LouvainBEff not available for tagger" << std::endl;
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

  double BTagWeight::BEff(BTagWeight::payload const& set, 
              unsigned flavour, 
              BTagWeight::tagger const& algo, 
              double pt, 
              double eta) const {
    eta = fabs(eta);
    if (eta >= 2.4) {
      std::cerr << "Warning in <BTagWeight::BEff>: Jet eta is too large: " << eta << std::endl;
      return 0.0;
    }
    double x = std::min(pt, 670.0);
    if (flavour == 5) {
      if (algo == tagger::SSVHEM) return 0.64939;
      if (algo == tagger::SSVHPT) return 0.48337;
      if (algo == tagger::CSVM)   return 0.72973;  
    } else if (flavour == 4) {
      if (algo == tagger::SSVHEM) return 0.17134;
      if (algo == tagger::SSVHPT) return 0.06522;
      if (algo == tagger::CSVM)   return 0.19249;
    } else {
      if (algo == tagger::SSVHEM) return (((-0.000420178+(0.00029105*x))+(-8.9398e-07*(x*x)))+(1.35401e-09*(x*(x*x))))+(-7.93826e-13*(x*(x*(x*x))));
      if (algo == tagger::SSVHPT) return (-2.9605e-05+(2.35624e-05*x))+(-1.77552e-08*(x*x));
      if (algo == tagger::CSVM)   {
        if (eta < 0.8)                return (0.00967751+(2.54564e-05*x))+(-6.92256e-10*(x*x));
        if (eta >= 0.8 && eta < 1.6)  return (0.00974141+(5.09503e-05*x))+(2.0641e-08*(x*x));
        if (eta >= 1.6 && eta < 2.4)  return (0.013595+(0.000104538*x))+(-1.36087e-08*(x*x));
      }
    }

    return 1.0;
  }


  double BTagWeight::SF(BTagWeight::payload const& set, 
            unsigned flavour, 
            BTagWeight::tagger const& algo, 
            double pt, 
            double eta,
            int Btag_mode,
            int Bfake_mode) const {
    eta = fabs(eta);
    if (eta >= 2.4) {
      std::cerr << "Warning in <BTagWeight::BEff>: Jet eta is too large: " << eta << std::endl;
      return 0.0;
    }
    double sf = 1.0;
    if (flavour == 5 || flavour == 4) {
      if (set == payload::ALL2011) {
        double x = std::max(30., std::min(pt, 670.0));
        if (algo == tagger::SSVHEM) sf = 0.896462*((1.+(0.00957275*x))/(1.+(0.00837582*x)));
        if (algo == tagger::SSVHPT) sf = 0.422556*((1.+(0.437396*x))/(1.+(0.193806*x)));
        if (algo == tagger::CSVM) {
          sf = 0.6981*((1.+(0.414063*x))/(1.+(0.300155*x)));
          //Adding uncertainty shifts for HiggsTauTau systematic study. Currently only implemented for CSVM.
          double err_SFb=SFb_error_2011_->GetBinContent(SFb_error_2011_->FindBin(pt));
          if(Btag_mode==1) sf=sf-err_SFb; 
          if(Btag_mode==2) sf=sf+err_SFb;
        }  
      } else if (set == payload::MORIOND2013) {
        double x = std::max(20., std::min(pt, 800.0));
        if (algo == tagger::CSVM)   sf = 0.726981*((1.+(0.253238*x))/(1.+(0.188389*x)));
      } else if (set == payload::EPS13) {
        double x = std::max(20., std::min(pt, 800.0));
        if (algo == tagger::CSVM) {
            sf = (0.938887+(0.00017124*x))+(-2.76366e-07*(x*x));
            //Adding uncertainty shifts for HiggsTauTau systematic study. Currently only implemented for CSVM.
            double err_SFb=SFb_error_2012_->GetBinContent(SFb_error_2012_->FindBin(pt));
            if(Btag_mode==1) sf=sf-err_SFb; 
            if(Btag_mode==2) sf=sf+err_SFb;
        }
      }
    } else {
      if (set == payload::ALL2011) {
        double x = std::max(20., std::min(pt, 670.0));
        if (algo == tagger::SSVHEM)     sf = ((0.890254+(0.000553319*x))+(-1.29993e-06*(x*x)))+(4.19294e-10*(x*(x*x)));
        if (algo == tagger::SSVHPT)     sf = ((0.97409+(0.000646241*x))+(-2.86294e-06*(x*x)))+(2.79484e-09*(x*(x*x)));
        if (algo == tagger::CSVM) {
          if(Bfake_mode==0) 
          {
            if (eta < 0.8)                sf = ((1.06182+(0.000617034*x))+(-1.5732e-06*(x*x)))+(3.02909e-10*(x*(x*x)));
            if (eta >= 0.8 && eta < 1.6)  sf = ((1.111+(-9.64191e-06*x))+(1.80811e-07*(x*x)))+(-5.44868e-10*(x*(x*x)));
            if (eta >= 1.6 && eta < 2.4)  sf = ((1.08498+(-0.000701422*x))+(3.43612e-06*(x*x)))+(-4.11794e-09*(x*(x*x)));
          }
          //Adding uncertainty shifts for HiggsTauTau systematic study. Currently only implemented for CSVM.
          if(Bfake_mode==1) 
          {
            if (eta < 0.8)                sf = ((0.972455+(7.51396e-06*x))+(4.91857e-07*(x*x)))+(-1.47661e-09*(x*(x*x)));  
            if (eta >= 0.8 && eta < 1.6)  sf = ((1.02055+(-0.000378856*x))+(1.49029e-06*(x*x)))+(-1.74966e-09*(x*(x*x)));
            if (eta >= 1.6 && eta < 2.4)  sf = ((0.983476+(-0.000607242*x))+(3.17997e-06*(x*x)))+(-4.01242e-09*(x*(x*x)));
          }
          if(Bfake_mode==2) 
          {
            if (eta < 0.8)                sf = ((1.15116+(0.00122657*x))+(-3.63826e-06*(x*x)))+(2.08242e-09*(x*(x*x)));
            if (eta >= 0.8 && eta < 1.6)  sf = ((1.20146+(0.000359543*x))+(-1.12866e-06*(x*x)))+(6.59918e-10*(x*(x*x)));
            if (eta >= 1.6 && eta < 2.4)  sf = ((1.18654+(-0.000795808*x))+(3.69226e-06*(x*x)))+(-4.22347e-09*(x*(x*x)));
          }
        }
      } else if (set == payload::MORIOND2013) {
        double x = std::max(20., std::min(pt, 800.0));
        if (algo == tagger::CSVM) {
          if (eta < 0.8)                sf = ((1.06238+(0.00198635*x))+(-4.89082e-06*(x*x)))+(3.29312e-09*(x*(x*x)));
          if (eta >= 0.8 && eta < 1.6)  sf = ((1.08048+(0.00110831*x))+(-2.96189e-06*(x*x)))+(2.16266e-09*(x*(x*x)));
          if (eta >= 1.6 && eta < 2.4)  sf = ((1.09145+(0.000687171*x))+(-2.45054e-06*(x*x)))+(1.7844e-09*(x*(x*x)));
        }
      } else if (set == payload::EPS13) {
        double x = std::max(20., std::min(pt, 1000.0));
        if (algo == tagger::CSVM) {
          if(Bfake_mode==0)
          {
            if (eta < 0.8)                sf = ((1.07541+(0.00231827*x))+(-4.74249e-06*(x*x)))+(2.70862e-09*(x*(x*x)));
            if (eta >= 0.8 && eta < 1.6)  sf = ((1.05613+(0.00114031*x))+(-2.56066e-06*(x*x)))+(1.67792e-09*(x*(x*x)));
            if (eta >= 1.6 && eta < 2.4)  sf = ((1.05625+(0.000487231*x))+(-2.22792e-06*(x*x)))+(1.70262e-09*(x*(x*x)));
          }
          //Adding uncertainty shifts for HiggsTauTau systematic study. Currently only implemented for CSVM.
          if(Bfake_mode==1)
          {
            if (eta < 0.8)                sf = ((0.964527+(0.00149055*x))+(-2.78338e-06*(x*x)))+(1.51771e-09*(x*(x*x))); 
            if (eta >= 0.8 && eta < 1.6)  sf = ((0.946051+(0.000759584*x))+(-1.52491e-06*(x*x)))+(9.65822e-10*(x*(x*x)));
            if (eta >= 1.6 && eta < 2.4)  sf = ((0.956736+(0.000280197*x))+(-1.42739e-06*(x*x)))+(1.0085e-09*(x*(x*x)));
          }
          if(Bfake_mode==2)
          {
            if (eta < 0.8)                sf = ((1.18638+(0.00314148*x))+(-6.68993e-06*(x*x)))+(3.89288e-09*(x*(x*x))); 
            if (eta >= 0.8 && eta < 1.6)  sf = ((1.16624+(0.00151884*x))+(-3.59041e-06*(x*x)))+(2.38681e-09*(x*(x*x)));
            if (eta >= 1.6 && eta < 2.4)  sf = ((1.15575+(0.000693344*x))+(-3.02661e-06*(x*x)))+(2.39752e-09*(x*(x*x)));
          }
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

  double BTagWeight::GetWeight( std::vector<PFJet *> const& jets, 
                    BTagWeight::payload const& set, 
                    BTagWeight::tagger const& algo, 
                    unsigned min, 
                    unsigned max) const {
    std::vector<BTagWeight::JetInfo> infos;
    for (unsigned i = 0; i < jets.size(); ++i) {
      double eff = BEff(set, std::abs(jets[i]->parton_flavour()), algo, jets[i]->pt(), jets[i]->eta());
      double sf = SF(set, std::abs(jets[i]->parton_flavour()), algo, jets[i]->pt(), jets[i]->eta(), 0, 0);
      infos.push_back(BTagWeight::JetInfo(eff, sf));
    }
    std::pair<float, float> result = weight(infos, min, max);
    return result.first / result.second;
  }

  std::map<std::size_t, bool> BTagWeight::ReTag(std::vector<PFJet *> const& jets, 
                                    BTagWeight::payload const& set, 
                                    BTagWeight::tagger const& algo,
                                    int Btag_mode,
                                    int Bfake_mode) const {
    bool verbose = false;
    std::map<std::size_t, bool> pass_result;
    for (unsigned i = 0; i < jets.size(); ++i) {
      rand->SetSeed((int)((jets[i]->eta()+5)*100000));
      double eff = BEff(set, std::abs(jets[i]->parton_flavour()), algo, jets[i]->pt(), jets[i]->eta());
      double sf = SF(set, std::abs(jets[i]->parton_flavour()), algo, jets[i]->pt(), jets[i]->eta(), Btag_mode, Bfake_mode);
      double demoteProb_btag = 0;
      double promoteProb_btag = 0;
      if(sf < 1) {
        demoteProb_btag = fabs(1.0 - sf);
      } else {
        promoteProb_btag = fabs(sf - 1.0)/((sf/eff) - 1.0);
      }
      if (verbose) {
        std::cout << "Jet " << i << " " << jets[i]->vector() << "  csv: " << jets[i]->GetBDiscriminator("combinedSecondaryVertexBJetTags") << "  parton flavour: " << jets[i]->parton_flavour() << std::endl;
        std::cout << "-- random seed: " << ((int)((jets[i]->eta()+5)*100000)) << std::endl;
        std::cout << "-- efficiency: " << eff << std::endl;
        std::cout << "-- scale factor: " << sf << std::endl;
      }
      bool passtag = jets[i]->GetBDiscriminator("combinedSecondaryVertexBJetTags") > 0.679;
      double randVal = rand->Uniform();
      if(passtag) {                       // if tagged
        if(demoteProb_btag > 0. && randVal < demoteProb_btag) {
          // jets[i]->SetBDiscriminator("combinedSecondaryVertexBJetTags", 0.60);
          pass_result[jets[i]->id()] = false;
          if (verbose) std::cout << "-- demote prob is " << demoteProb_btag << ", rand is " << randVal << ". Jet is demoted" << std::endl;
        } else {
          pass_result[jets[i]->id()] = true;            
          if (verbose) std::cout << "-- demote prob is " << demoteProb_btag << ", rand is " << randVal << ". Jet remains tagged" << std::endl;
        }
      } else {
        if(promoteProb_btag > 0. && randVal < promoteProb_btag) {
          // jets[i]->SetBDiscriminator("combinedSecondaryVertexBJetTags", 0.70);
          pass_result[jets[i]->id()] = true;
          if (verbose) std::cout << "-- promote prob is " << promoteProb_btag << ", rand is " << randVal << ". Jet is promoted" << std::endl;
        } else {
          pass_result[jets[i]->id()] = false;  
          if (verbose) std::cout << "-- promote prob is " << promoteProb_btag << ", rand is " << randVal << ". Jet remains untagged" << std::endl;
        }
      }
    }
    return pass_result;
  }
 

  double BTagWeight::GetLouvainWeight(std::vector<PFJet *> const& jets, BTagWeight::tagger const& algo, unsigned min, unsigned max) const {
    std::vector<BTagWeight::JetInfo> infos;
    for (unsigned i = 0; i < jets.size(); ++i) {
      double eff = LouvainBEff(std::abs(jets[i]->parton_flavour()), algo, jets[i]->pt(), jets[i]->eta());
      double sf = SF(payload::ALL2011, std::abs(jets[i]->parton_flavour()), algo, jets[i]->pt(), jets[i]->eta(), 0, 0);
      infos.push_back(BTagWeight::JetInfo(eff, sf));
    }
    std::pair<float, float> result = weight(infos, min, max);
    return result.first / result.second;
  }


}

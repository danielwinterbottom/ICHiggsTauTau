#ifndef ICHiggsTauTau_Module_HTTFakeFactorWeights_h
#define ICHiggsTauTau_Module_HTTFakeFactorWeights_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FakeFactor.h"
#include "RooWorkspace.h"
#include "RooFunctor.h"
#include <string>
#include "TMVA/Reader.h"
namespace ic {

class HTTFakeFactorWeights : public ModuleBase {
 private:
  CLASS_MEMBER(HTTFakeFactorWeights, std::string,  met_label)
  CLASS_MEMBER(HTTFakeFactorWeights, std::string,  jets_label)
  CLASS_MEMBER(HTTFakeFactorWeights, ic::channel,  channel)
  CLASS_MEMBER(HTTFakeFactorWeights, std::string,  ditau_label)
  CLASS_MEMBER(HTTFakeFactorWeights, ic::strategy, strategy)
  CLASS_MEMBER(HTTFakeFactorWeights, ic::era,      era)
  CLASS_MEMBER(HTTFakeFactorWeights, std::string,  categories)
  CLASS_MEMBER(HTTFakeFactorWeights, bool,         do_systematics)
  CLASS_MEMBER(HTTFakeFactorWeights, std::string,  ff_file)
  CLASS_MEMBER(HTTFakeFactorWeights, std::string,  fracs_file)
  CLASS_MEMBER(HTTFakeFactorWeights, bool,         is_embedded)

 public:
  HTTFakeFactorWeights(std::string const& name);
  virtual ~HTTFakeFactorWeights();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();
  
  std::map<std::string, std::shared_ptr<FakeFactor>> fake_factors_;
  std::vector<std::string> category_names_;

  std::shared_ptr<RooWorkspace> w_;
  std::map<std::string, std::shared_ptr<RooFunctor>> fns_;
  
  TH1D *ff_hist_dm0_;
  TH1D *ff_hist_dm1_;
  TH1D *ff_hist_dm10_;

  TH2D *ff_fracs_qcd_;
  TH2D *ff_fracs_wjets_;

  TH2D *ff_fracs_qcd_ss_;
  TH2D *ff_fracs_wjets_ss_;

  TH2D *ff_fracs_qcd_aiso_;
  TH2D *ff_fracs_wjets_aiso_;

  TH2D *ff_fracs_qcd_highmt_;
  TH2D *ff_fracs_wjets_highmt_;

  std::shared_ptr<RooWorkspace> ff_ws_;
  std::shared_ptr<RooWorkspace> ff_ws_us_;
  std::shared_ptr<RooWorkspace> ff_ws_dm_;

  std::vector<std::string> systs_dm_;
  std::vector<std::string> systs_mvadm_;
  std::vector<std::string> systs_us_;

  TMVA::Reader *reader_;
  float met_, pt_1_, pt_2_, mva_dm_2_, mt_1_, m_vis_, pt_tt_, mjj_, n_jets_;
};

}


#endif

#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTSmearScale.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTauRun2/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/Electron.hh"
#include "boost/format.hpp"

namespace ic {

  HTTSmearScale::HTTSmearScale(std::string const& name) : ModuleBase(name), channel_(channel::et), strategy_(strategy::paper2013) {
  }

  HTTSmearScale::~HTTSmearScale() {
    ;
  }

  int HTTSmearScale::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "HTTSmearScale" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
    std::cout << boost::format(param_fmt()) % "input_label"  % input_label_;
    std::cout << boost::format(param_fmt()) % "e_unc_mode"       % e_unc_mode_;
    return 0;
  }

  int HTTSmearScale::Execute(TreeEvent *event) {
    if(channel_ == channel::et || channel_ == channel::em || channel_ == channel::zee) {
      std::vector<Electron *> &electrons = event->GetPtrVec<Electron>(input_label_);
      std::map<std::size_t, double> elec_ss;
      std::map<std::size_t, double> elec_ss_vars;
      for (unsigned i = 0; i < electrons.size() ;++i){
        float preCorr = 1.0;
        float postCorr = 1.0;
        float preCorrPt = 1.0;

        // energy() should be equivalent to ecalTrkEnergyPreCorr()
        preCorr = electrons[i]->energy(); 
        postCorr = electrons[i]->ecalTrkEnergyPostCorr();

        preCorrPt = electrons[i]->pt();

        float shift = postCorr/preCorr;

        // shifting pT and E shifts the full four-vector
        // as required by EGammaPOG
        electrons[i]->set_pt(electrons[i]->pt() * shift);
        electrons[i]->set_energy(electrons[i]->energy() * shift);

        elec_ss[electrons[i]->id()] = shift;

        /* std::cout << "preCorr: " << preCorr << std::endl; */
        /* std::cout << "postCorr: " << postCorr << std::endl; */

        // NEW EDITS
        // now add electron scale and smear uncertainties
        // scale_mode = 1 for DOWN, 2 for UP
        // not using resolution uncertainty for now
        float total_shift = 1.0;
        if (e_unc_mode_ == 2)
          total_shift = electrons[i]->ecalTrkEnergyScaleUp() / preCorr;
        else if (e_unc_mode_ == 1)
          total_shift = electrons[i]->ecalTrkEnergyScaleDown() / preCorr;
        
        /* std::cout << "e_unc_mode: " << e_unc_mode_ << std::endl; */
        
        electrons[i]->set_pt(preCorrPt * total_shift);
        electrons[i]->set_energy(preCorr * total_shift);

        /* std::cout << "energy with up/down shift: " << electrons[i]->energy() << std::endl; */

        elec_ss_vars[electrons[i]->id()] = total_shift;

      }
      event->Add("elec_ss", elec_ss);
      event->Add("elec_ss_vars", elec_ss_vars);
      

      return 0;
    } else return 0;
  }




  int HTTSmearScale::PostAnalysis() {
    return 0;
  }

  void HTTSmearScale::PrintInfo() {
    ;
  }

}

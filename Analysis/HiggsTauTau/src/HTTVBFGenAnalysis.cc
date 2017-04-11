#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTVBFGenAnalysis.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnRootTools.h"

std::vector<ic::GenParticle> FamilyTree (std::vector<ic::GenParticle> &v, ic::GenParticle p, std::vector<ic::GenParticle*> gen_particles, unsigned &outputID){ 
  if(p.daughters().size() == 0){
    unsigned ID = std::fabs(p.pdgid());
    if(ID == 11) outputID = 11;
    else if(ID == 13) outputID = 13;
    if(!(ID==12 || ID==14 || ID==16)){
      v.push_back(p);
    }
  }
  else{
    for(size_t i = 0; i < p.daughters().size(); ++i ){
      ic::GenParticle d = *gen_particles[p.daughters().at(i)];
      FamilyTree(v,d, gen_particles, outputID);
    }
  }
  return v;
}

struct swap_labels{
  bool operator() (std::string a,std::string b) {
    if(a=="t" && b!="t") return false;
    else if(a=="m" &&  (b!="m" && b!="t")) return false;
    else return true;
  }
};

struct PtComparator{
  bool operator() (ic::Candidate a, ic::Candidate b) {
    return (a.vector().Pt() > b.vector().Pt());
  }
};
  
namespace ic {

  HTTVBFGenAnalysis::HTTVBFGenAnalysis(std::string const& name) : ModuleBase(name) {
    fs_ = NULL;
  }

  HTTVBFGenAnalysis::~HTTVBFGenAnalysis() {
    ;
  }

  int HTTVBFGenAnalysis::PreAnalysis() {
     
      
    if(fs_){  
      outtree_ = fs_->make<TTree>("HLT_trigger_ntuple","HLT_trigger_ntuple");
      outtree_->Branch("event"       , &event_       );
      outtree_->Branch("hlt_jpt_1"       , &hlt_jpt_1_       );
      outtree_->Branch("hlt_jpt_2"       , &hlt_jpt_2_       );
      outtree_->Branch("hlt_mjj"       , &hlt_mjj_       );
    }
   // count_ee_ = 0;
   // count_em_ = 0;
  //  count_et_ = 0;
  //  count_mm_ = 0;
  //  count_mt_ = 0;
 //   count_tt_ = 0;
    
    //std::string file = "input/ggh_weights/HqT_weight_pTH_summer16_80X_AllSamples.root";
    //ggh_weights_ = new TFile(file.c_str());
    //ggh_weights_->cd();
    //ggh_hist_ = (TH1F*)gDirectory->Get("Powheg_Nominal");
    //ggh_hist_up_ = (TH1F*)gDirectory->Get("Powheg_ScaleUp");
    //ggh_hist_down_ = (TH1F*)gDirectory->Get("Powheg_ScaleDown");
    //ggh_herwig_hist_      = (TH1F*)gDirectory->Get("Herwig_Nominal"            );
    //ggh_amcnlo_hist_      = (TH1F*)gDirectory->Get("aMC_Nominal"           );
    //ggh_pythiaup_hist_    = (TH1F*)gDirectory->Get("PythiaFragmentUp_Nominal"  );
    //ggh_pythiadown_hist_ = (TH1F*)gDirectory->Get("PythiaFragmentDown_Nominal");
    //ggh_scalehigh_        = (TH1F*)gDirectory->Get("Powheg_Nominal_ScaleHigh"   );
    //ggh_scalelow_         = (TH1F*)gDirectory->Get("Powheg_Nominal_ScaleLow"   );
    return 0;
  }


int ComparePt(TriggerObject *A, TriggerObject *B)
{

return A->vector().Pt()>B->vector().Pt();

}

  int HTTVBFGenAnalysis::Execute(TreeEvent *event) {
    
    //EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    //std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");
    
   std::vector<TriggerObject *> const& VBFobjs = event->GetPtrVec<TriggerObject>("triggerVBF");	
   std::vector<PFJet *> const& jet_objs = event->GetPtrVec<PFJet>("ak4PFJetsCHS");
   std::vector<TriggerObject *>  L1jets;
   std::vector<TriggerObject *>  Calojets;
   std::vector<TriggerObject *>  HLTjets;

   double max_jet_pt = -9999;
   double sec_max_jet_pt = -9999;	
   double max_mjj = -9999;
 /*  for (unsigned i = 0; i < VBFobjs.size(); ++i) 
     {if (VBFobjs.size()>0)     
    	{
	if (VBFobjs.size()>1){
		if (VBFobjs[i]->vector().Pt()>max_jet_pt){
			sec_max_jet_pt = max_jet_pt;
			max_jet_pt = VBFobjs[i]->vector().Pt();}
		else 
			if (VBFobjs[i]->vector().Pt()>sec_max_jet_pt){
				sec_max_jet_pt = VBFobjs[i]->vector().Pt();}
			
	

	}
	else{
		if (VBFobjs[i]->vector().Pt()>max_jet_pt) max_jet_pt = VBFobjs[i]->vector().Pt();
		sec_max_jet_pt = -9999;
      		}
	}
      else break;
       if (VBFobjs.size()>1){
	for (unsigned j = 0; j < VBFobjs.size(); ++j)		
		if (i!=j) 
			if ((VBFobjs[i]->vector()+VBFobjs[j]->vector()).M()>max_mjj)
				max_mjj = (VBFobjs[i]->vector()+VBFobjs[j]->vector()).M();
	}
}*/

    for (unsigned i = 0; i < VBFobjs.size(); ++i)
	{
	int HLT_match = IsFilterMatchedWithIndex(jet_objs[i], VBFobjs, "hltDiPFJetMJJDummy", 0.5).second;
	int L1_match = IsFilterMatchedWithIndex(jet_objs[i], VBFobjs, "hltL1DiJetVBF", 0.5).second;
	int Calo_match = IsFilterMatchedWithIndex(jet_objs[i], VBFobjs, "hltCaloJetsCorrectedMatchedToL1", 0.5).second;
    bool a =IsFilterMatchedWithIndex(jet_objs[i], VBFobjs, "hltL1DiJetVBF", 0.5).first;
    bool b = IsFilterMatchedWithIndex(jet_objs[i], VBFobjs, "hltCaloJetsCorrectedMatchedToL1", 0.5).first;


	if (a == false)
	{

	std::cout<<"Problem with L1_match"<<std::endl;
	}

	if (b==false)
	{

	std::cout<<"Problem with Calo_match"<<std::endl;
	}

	L1jets.push_back(VBFobjs[L1_match]);
	Calojets.push_back(VBFobjs[Calo_match]);
	HLTjets.push_back(VBFobjs[HLT_match]);

	}


for (unsigned i = 0; i < HLTjets.size(); ++i)
     {if (HLTjets.size()>0)
        {
        if (HLTjets.size()>1){
                if (HLTjets[i]->vector().Pt()>max_jet_pt){
                        sec_max_jet_pt = max_jet_pt;
                        max_jet_pt = HLTjets[i]->vector().Pt();}
                else
                        if (HLTjets[i]->vector().Pt()>sec_max_jet_pt){
                                sec_max_jet_pt = HLTjets[i]->vector().Pt();}



        }
        else{
                if (HLTjets[i]->vector().Pt()>max_jet_pt) max_jet_pt =HLTjets[i]->vector().Pt();
                sec_max_jet_pt = -9999;
                }
        }
      else break;
       if (HLTjets.size()>1){
        for (unsigned j = 0; j < HLTjets.size(); ++j)
                if (i!=j)
                        if ((HLTjets[i]->vector()+HLTjets[j]->vector()).M()>max_mjj)
                                max_mjj = (HLTjets[i]->vector()+HLTjets[j]->vector()).M();
        }
    
}
    hlt_jpt_1_ = max_jet_pt;
    hlt_jpt_2_ = sec_max_jet_pt;
    hlt_mjj_ = max_mjj;
  if(fs_) outtree_->Fill();
    
    return 0;



	}
 
 //    event_ = (unsigned long long) eventInfo->event();
   // wt_ = 1;
   // wt_ = eventInfo->weight("wt_mc_sign");
   // wt_dy_=1;
   // if (event->Exists("wt_dy")) wt_dy_ = event->Get<float>("wt_dy");
    //wt_ggh_pt_            = 1;
    //wt_ggh_pt_up_         = 1;
    //wt_ggh_pt_down_       = 1;
    //wt_ggh_pt_herwig_     = 1;
    //wt_ggh_pt_amc_        = 1;
    //wt_ggh_pt_pythiaup_   = 1;
    //wt_ggh_pt_pythiadown_ = 1;
    //wt_ggh_pt_scalehigh_  = 1;
    //wt_ggh_pt_scalelow_   = 1;
    
  
  int HTTVBFGenAnalysis::PostAnalysis() {
    return 0;
  }

  void HTTVBFGenAnalysis::PrintInfo() {
    ;
  }

}

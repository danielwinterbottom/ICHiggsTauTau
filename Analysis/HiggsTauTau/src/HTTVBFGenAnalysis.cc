#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTVBFGenAnalysis.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "Utilities/interface/FnRootTools.h"


double L1pass;
double L1fail;
double L1failSecond;
double L1size0;
double L1size1;
double Calosize0;
double HLTsize0;
double CaloL1tot;
double CaloL1matched;
bool CaloL1;
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
 


struct PtComparatorTriggerObj{
    bool operator() (ic::TriggerObject *a, ic::TriggerObject *b) {
    return (a->vector().Pt() > b->vector().Pt());
  }
};


 bool IsFilterMatchedWithName(ic::TriggerObject *objs, std::string const& filter){
      std::size_t hash = CityHash64(filter);
      
      std::vector<std::size_t> const& labels = objs->filters();
      if (std::find(labels.begin(),labels.end(), hash) == labels.end())  return false;
      return true;
  }



 
namespace ic {

  HTTVBFGenAnalysis::HTTVBFGenAnalysis(std::string const& name) : ModuleBase(name) {
    fs_ = NULL;
  }

  HTTVBFGenAnalysis::~HTTVBFGenAnalysis() {
    ;
  }

  int HTTVBFGenAnalysis::PreAnalysis() {
     
   L1pass = 0;
   L1fail = 0;
   L1failSecond = 0;
   L1size0 = 0;
   L1size1 = 0;
   HLTsize0 = 0;
   Calosize0 = 0; 
   CaloL1matched = 0;
   CaloL1tot = 0; 
   CaloL1 = true; 
   if(fs_){  
      outtree_ = fs_->make<TTree>("HLT_trigger_ntuple","HLT_trigger_ntuple");
      outtree_->Branch("event"       , &event_       );
      outtree_->Branch("hlt_jpt_1"       , &hlt_jpt_1_       );
      outtree_->Branch("hlt_jpt_2"       , &hlt_jpt_2_       );
      outtree_->Branch("hlt_jpt_3"       , &hlt_jpt_3_       );
      outtree_->Branch("hlt_jpt_4"       , &hlt_jpt_4_       );
      outtree_->Branch("calo_jpt_1"       , &calo_jpt_1_       );
      outtree_->Branch("calo_jpt_2"       , &calo_jpt_2_       );

      outtree_->Branch("hlt_jeta_1"       , &hlt_jeta_1_       );
      outtree_->Branch("hlt_jeta_2"       , &hlt_jeta_2_       );
      outtree_->Branch("hlt_jeta_3"       , &hlt_jeta_3_       );
      outtree_->Branch("hlt_jeta_4"       , &hlt_jeta_4_       );


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


  int HTTVBFGenAnalysis::Execute(TreeEvent *event) {
    
    
   std::vector<TriggerObject *> const& VBFobjs = event->GetPtrVec<TriggerObject>("triggerVBF");	
   std::vector<TriggerObject *>  L1jets;
   std::vector<TriggerObject *>  Calojets;
   std::vector<TriggerObject *>  HLTjets;

   hlt_jpt_1_=-9999;
   hlt_jpt_2_=-9999;
   hlt_jpt_3_=-9999;
   hlt_jpt_4_=-9999;
   hlt_jeta_1_=-9999;
   hlt_jeta_2_=-9999;
   hlt_jeta_3_=-9999;
   hlt_jeta_4_=-9999;
   calo_jpt_1_=-9999;
   calo_jpt_2_=-9999;
   
   hlt_mjj_=-9999;

    for (unsigned i = 0; i < VBFobjs.size(); ++i)
	{ 
   bool a = IsFilterMatchedWithName(VBFobjs[i], "hltL1DiJetVBF");
   //bool b = IsFilterMatchedWithName(VBFobjs[i], "hltCaloJetsCorrectedMatchedToL1");
   bool b = IsFilterMatchedWithName(VBFobjs[i], "hltDoubleJetDummy");
   bool c = IsFilterMatchedWithName(VBFobjs[i], "hltDiPFJetMJJDummy");

	if (a) 	L1jets.push_back(VBFobjs[i]);	
	if (b)	Calojets.push_back(VBFobjs[i]);
	if (c)  HLTjets.push_back(VBFobjs[i]);

}



//std::cout<<"A"<<std::endl;
std::sort(HLTjets.begin(), HLTjets.end(), PtComparatorTriggerObj());
std::sort(Calojets.begin(), Calojets.end(), PtComparatorTriggerObj());
std::sort(L1jets.begin(), L1jets.end(), PtComparatorTriggerObj());


//L1jets and Calojets match test
if (L1jets.size()!=0)
	{if (L1jets[0]->vector().Pt()<80) L1fail++;
	if (L1jets.size()>1) 
		if (L1jets[1]->vector().Pt()<30) L1failSecond++;

	}
L1pass++;

if (L1jets.size()==0) L1size0++;
if (Calojets.size()==0) Calosize0++;
if (HLTjets.size()==0) HLTsize0++;

L1size1++;

/*for (unsigned i = 0; i < Calojets.size(); ++i)
{

if ()
CaloL1tot++;

}*/
//if (HLTjets.size()>1)
//std::cout<<HLTjets[0]->vector().Pt()<<" " <<HLTjets[1]->vector().Pt()<<std::endl;
if (HLTjets.size()>0)
	{
    hlt_jpt_1_ = HLTjets[0]->vector().Pt();
    hlt_jeta_1_ = HLTjets[0]->vector().Eta();

	}
if (HLTjets.size()>1)
	{
    hlt_jpt_2_ = HLTjets[1]->vector().Pt();
    hlt_jeta_2_ = HLTjets[1]->vector().Eta();
	}
if (HLTjets.size()>2)
	{
    hlt_jpt_3_ = HLTjets[2]->vector().Pt();
    hlt_jeta_3_ = HLTjets[2]->vector().Eta();	
	}
if (HLTjets.size()>3)
	{
    hlt_jpt_4_ = HLTjets[3]->vector().Pt(); 
    hlt_jeta_4_ = HLTjets[3]->vector().Eta();	
	}
//std::cout<<Calojets.size()<<std::endl;
if (Calojets.size()>0)
        {
    calo_jpt_1_ = Calojets[0]->vector().Pt();
    //calo_jeta_1_ = Calojets[0]->vector().Eta();

        }
if (Calojets.size()>1)
        {
    calo_jpt_2_ = Calojets[1]->vector().Pt();
    //calo_jeta_2_ = Calojets[1]->vector().Eta();
        }


 
 // hlt_mjj_ = max_mjj;
  if(fs_) outtree_->Fill();
    
    return 0;



	}
 
  int HTTVBFGenAnalysis::PostAnalysis() {
 
std::cout<<"L1jets size 0 = "<<1.0*L1size0/L1size1<<std::endl;
std::cout<<"HLT "<<1.0*(HLTsize0)/L1size1 << " CALO size 0 = "<<1.0*(Calosize0)/L1size1<<std::endl;
std::cout<<"L1jets size !=0 but L1jets[0]->vector().Pt()<80: "<<(L1fail/L1pass)<<std::endl;
std::cout<<"L1jets size !=0 but L1jets[1]->vector().Pt()<30: "<<(L1failSecond/L1pass)<<std::endl;
std::cout<<L1fail<<" "<<L1pass<<std::endl;
   return 0;
  }

  void HTTVBFGenAnalysis::PrintInfo() {
    ;
  }

}

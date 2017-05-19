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

struct PtComparatorOfflineObj{
    bool operator() (ic::PFJet *a, ic::PFJet *b) {
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
       
      outtree_->Branch("jpt_1"       , &jpt_1_       );
      outtree_->Branch("jpt_2"       , &jpt_2_       );
      outtree_->Branch("jpt_3"       , &jpt_3_       );
       
      outtree_->Branch("jeta_1"       , &jeta_1_       );
      outtree_->Branch("jeta_2"       , &jeta_2_       );
       
      outtree_->Branch("mjj"       , &mjj_       );
      outtree_->Branch("mjj30"       , &mjj30_       );
       
      outtree_->Branch("HbbPassed"       , &HbbPassed_       );
      outtree_->Branch("HLTPassThroughPassed"       , &HLTPassThroughPassed_       );
      outtree_->Branch("L1Passed"       , &L1Passed_       );
       
      outtree_->Branch("deta"       , &deta_       );
      outtree_->Branch("dphi"       , &dphi_       );
      outtree_->Branch("min_dphi"       , &min_dphi_       );

      outtree_->Branch("hlt_jeta_1"       , &hlt_jeta_1_       );
      outtree_->Branch("hlt_jeta_2"       , &hlt_jeta_2_       );
      outtree_->Branch("hlt_jeta_3"       , &hlt_jeta_3_       );
      outtree_->Branch("hlt_jeta_4"       , &hlt_jeta_4_       );


      outtree_->Branch("hlt_mjj"       , &hlt_mjj_       );
      outtree_->Branch("calo_mjj"       , &calo_mjj_       );
      outtree_->Branch("hlt_mjj30"       , &hlt_mjj30_       );
      outtree_->Branch("calo_mjj30"       , &calo_mjj30_       );
       
      outtree_->Branch("met"       , &met_       );
      outtree_->Branch("calo_met"       , &calo_met_       );
      outtree_->Branch("hlt_met"       , &hlt_met_       );
       

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
   
   std::vector<TriggerObject *> const& VBFobjsHbb = event->GetPtrVec<TriggerObject>("triggerObjectsHbb");
   std::vector<TriggerObject *> const& VBFobjsHLTPassThrough = event->GetPtrVec<TriggerObject>("triggerObjectsVBFPassThrough");
      
      HbbPassed_ = VBFobjsHbb.size()>0;
      HLTPassThroughPassed_ = VBFobjsHLTPassThrough.size()>0;
      

   std::vector<TriggerObject *>  L1jets;
   std::vector<TriggerObject *>  Calojets;
   std::vector<TriggerObject *>  HLTjets;
   std::vector<TriggerObject *>  L1met;
   std::vector<TriggerObject *>  Calomet;
   std::vector<TriggerObject *>  HLTmet;

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
      
   jpt_1_=-9999;
   jpt_2_=-9999;
   jpt_3_=-9999;
      
   jeta_1_=-9999;
   jeta_2_=-9999;
      
   mjj_ = -9999;
   
      deta_= -9999;
      dphi_= -9999;
      min_dphi_ = -9999;
   hlt_mjj_=-9999;
   calo_mjj_=-9999;
   hlt_mjj30_=-9999;
   calo_mjj30_=-9999;
   
      
   calo_met_ = -9999;
   hlt_met_ = -9999;
   met_ = -9999;
      

    for (unsigned i = 0; i < VBFobjs.size(); ++i)
	{
       // std::string Filter;
     //      std::vector<std::size_t> const& labels = VBFobjs[i]->filters();
       // for (unsigned int j = 0;j<labels.size();j++)
        //{
        //    labels[j]>>Filter;
           // std::cout<<Filter<<std::endl;

        //}
   bool a = IsFilterMatchedWithName(VBFobjs[i], "hltL1DiJetVBF");
 //  bool b = IsFilterMatchedWithName(VBFobjs[i], "hltCaloJetsCorrectedMatchedToL1");
   //bool b = IsFilterMatchedWithName(VBFobjs[i], "hltDoubleJetDummy");
   bool b1 = IsFilterMatchedWithName(VBFobjs[i], "TwoJets");
   bool b2 = IsFilterMatchedWithName(VBFobjs[i], "MultipleJets");
        
   bool c = IsFilterMatchedWithName(VBFobjs[i], "hltDiPFJetMJJDummy");

//ALBERT'S
 //  bool a = IsFilterMatchedWithName(VBFobjs[i], "hltL1DiJetVBF");
   //bool b = IsFilterMatchedWithName(VBFobjs[i], "hltCaloJetsCorrectedMatchedToL1");
   //bool b = IsFilterMatchedWithName(VBFobjs[i], "hltDoubleJetOpen");
   //bool c = IsFilterMatchedWithName(VBFobjs[i], "hltDiPFJetOpenMJJOpen");

	if (a) 	L1jets.push_back(VBFobjs[i]);	
	if (b1)	Calojets.push_back(VBFobjs[i]);
    if (b2)	Calojets.push_back(VBFobjs[i]);
	if (c)  HLTjets.push_back(VBFobjs[i]);
        
    bool HLTMET = IsFilterMatchedWithName(VBFobjs[i], "hltPFMETVBF");
    bool CaloMET = IsFilterMatchedWithName(VBFobjs[i], "hltMETVBF");
       
        if (CaloMET){ Calomet.push_back(VBFobjs[i]); calo_met_ = VBFobjs[i]->vector().Pt();}
        if (HLTMET)  {HLTmet.push_back(VBFobjs[i]); hlt_met_ = VBFobjs[i]->vector().Pt();}
        

}
      
      std::vector<Met *> pfMet_vec = event->GetPtrVec<Met>("pfMetFromSlimmed");
      met_ = pfMet_vec.at(0)->vector().Pt();

      if (Calomet.size()>1) std::cout<<"Problem calo met"<<std::endl;
      if (HLTmet.size()>1) std::cout<<"Problem pf met"<<std::endl;

//std::cout<<"A"<<std::endl;
std::sort(HLTjets.begin(), HLTjets.end(), PtComparatorTriggerObj());
std::sort(Calojets.begin(), Calojets.end(), PtComparatorTriggerObj());
std::sort(L1jets.begin(), L1jets.end(), PtComparatorTriggerObj());

// Add offline jet variables
      
      
      
      std::vector<PFJet *> const& jet_objs = event->GetPtrVec<PFJet>("ak4PFJetsCHS");
      std::vector<PFJet *>  Offline_jet_objs;
     
for (unsigned i = 0; i < jet_objs.size(); ++i) {
    
         // int leg1_match = IsFilterMatchedWithIndex(jet_objs[i], VBFobjs, "hltDiPFJetMJJDummy", 0.5).second;
    
          if (IsFilterMatchedWithIndex(jet_objs[i], VBFobjs, "hltDiPFJetMJJDummy", 0.5).first == true)
          {
              
              Offline_jet_objs.push_back(jet_objs[i]);
              
              
          }
          else break;
          
      }

std::sort(Offline_jet_objs.begin(), Offline_jet_objs.end(), PtComparatorOfflineObj());




//L1jets and Calojets match test

unsigned int Match =0;
for (unsigned i = 0; i < Calojets.size(); ++i)
	for (unsigned j = 0; j < L1jets.size(); ++j)
{
std::pair<TriggerObject *, TriggerObject *> L1Calo (Calojets[i],L1jets[j]);
bool a = DRLessThan(L1Calo,0.5);
if (a) {
	Match+=1;
    std::cout<<"Calo: "<<Calojets[i]->vector().Pt()<<" L1: "<<L1jets[j]->vector().Pt()<<std::endl;
	break;
	}

}
if (Match!=Calojets.size())
std::cout<<"Matching problem: Matched :"<<Match<<"/ "<<Calojets.size()<<std::endl;
else std::cout<<Calojets.size()<<" "<<"Matching ok; L1 size: "<<L1jets.size() <<std::endl;

if (L1jets.size()!=0)
	{if (L1jets[0]->vector().Pt()<80) L1fail++;
	if (L1jets.size()>1) 
		if (L1jets[1]->vector().Pt()<30) L1failSecond++;

	}
L1pass++;

if (L1jets.size()==0) L1size0++;
if (Calojets.size()==0) Calosize0++;
if (HLTjets.size()==0) HLTsize0++;

      L1Passed_ =L1jets.size()>0;
      
L1size1++;


//Insert mjj for HLT and Calo jets 

double hlt_mjj = -9999;
double calo_mjj = -9999;
double hlt_mjj30 = -9999;
double calo_mjj30 = -9999;

     
      
      

for (unsigned i = 0; i < HLTjets.size(); ++i)
for (unsigned j = 0; j < HLTjets.size(); ++j)
{

if ((HLTjets.size()>1)&&(i!=j)){
		
    if ((HLTjets[i]->vector()+HLTjets[j]->vector()).M()>hlt_mjj)
					hlt_mjj = (HLTjets[i]->vector()+HLTjets[j]->vector()).M();
        

   if ((HLTjets[i]->vector().Pt()>30)&&(HLTjets[j]->vector().Pt()>30)&&((HLTjets[i]->vector()+HLTjets[j]->vector()).M()>hlt_mjj30))
                    hlt_mjj30 = (HLTjets[i]->vector()+HLTjets[j]->vector()).M();


}
}
    for (unsigned i = 0; i < Calojets.size(); ++i)
        for (unsigned j = 0; j < Calojets.size(); ++j)
        {
          
            
            if ((Calojets.size()>1)&&(i!=j)){
                
                if ((Calojets[i]->vector()+Calojets[j]->vector()).M()>calo_mjj)
                    calo_mjj = (Calojets[i]->vector()+Calojets[j]->vector()).M();
                
                
                if ((Calojets[i]->vector().Pt()>30)&&(Calojets[j]->vector().Pt()>30)&&((Calojets[i]->vector()+Calojets[j]->vector()).M()>calo_mjj30))
                    calo_mjj30 = (Calojets[i]->vector()+Calojets[j]->vector()).M();
                
                
        }

        }
      
      double offline_mjj = -9999;
      double offline_mjj30 = -9999;
      double minPhi = 1000000000000;
      for (unsigned i = 0; i < Offline_jet_objs.size(); ++i)
      {
          if (Offline_jet_objs[i]->vector().Pt()>30)
          if (minPhi>TMath::Abs(Offline_jet_objs[i]->vector().phi()-pfMet_vec.at(0)->vector().phi()))
          minPhi =TMath::Abs(Offline_jet_objs[i]->vector().phi()-pfMet_vec.at(0)->vector().phi());
          
          
      for (unsigned j = 0; j < Offline_jet_objs.size(); ++j)
          {
              
              if ((Offline_jet_objs.size()>1)&&(i!=j)){
                  
                  if ((Offline_jet_objs[i]->vector()+Offline_jet_objs[j]->vector()).M()>offline_mjj)
                      offline_mjj = (Offline_jet_objs[i]->vector()+Offline_jet_objs[j]->vector()).M();
              
                if ((Offline_jet_objs[i]->vector().Pt()>30)&&(Offline_jet_objs[j]->vector().Pt()>30)&&((Offline_jet_objs[i]->vector()+Offline_jet_objs[j]->vector()).M()>offline_mjj30))
                      offline_mjj30 = (Offline_jet_objs[i]->vector()+Offline_jet_objs[j]->vector()).M();
                  
                  
              
                  
              }
              
          }
              
              }
            
            
if (minPhi!=1000000000000) min_dphi_ = minPhi;
              
if (Offline_jet_objs.size()>1) deta_ =TMath::Abs(Offline_jet_objs[0]->vector().eta()-Offline_jet_objs[1]->vector().eta());
              
if (Offline_jet_objs.size()>1) dphi_ =TMath::Abs(Offline_jet_objs[0]->vector().phi()-Offline_jet_objs[1]->vector().phi());
      
/*for (unsigned i = 0; i < Calojets.size(); ++i)
{

if ()
CaloL1tot++;

}*/
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

      
      if (Offline_jet_objs.size()>0)
      {
          jpt_1_ = Offline_jet_objs[0]->vector().Pt();
          jeta_1_ = Offline_jet_objs[0]->vector().Eta();
          
      }
      if (Offline_jet_objs.size()>1)
      {
          jpt_2_ = Offline_jet_objs[1]->vector().Pt();
          jeta_2_ = Offline_jet_objs[1]->vector().Eta();
      }
      if (Offline_jet_objs.size()>2)
      {
          jpt_3_ = Offline_jet_objs[2]->vector().Pt();
          
      }
      
      

 
  hlt_mjj_ = hlt_mjj;
  hlt_mjj30_ = hlt_mjj30;
  calo_mjj_ = calo_mjj;
  calo_mjj30_ = calo_mjj30;
  mjj_ = offline_mjj;
  mjj30_ = offline_mjj30;
              
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

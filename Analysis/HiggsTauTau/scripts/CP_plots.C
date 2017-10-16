
void OfflineCPPlots(std::string outfile_name, std::string channel, int cp_chan){
  std::string cp_channel = "cp_channel=="+std::to_string(cp_chan);
  std::string dir_name = "/vols/cms/dw515/CPStudies/Offline/";
      
  TFile *f1 = new TFile((dir_name+"GluGluToHToTauTau_M-125_tt_2016.root").c_str());    
  TFile *f2 = new TFile((dir_name+"SUSYGluGluToHToTauTau_M-120_tt_2016.root").c_str());    
  TFile *f3 = new TFile((dir_name+"DYJetsToLL_tt_2016.root").c_str());
  
  TTree *t1 = (TTree*)f1->Get("ntuple");
  TTree *t2 = (TTree*)f2->Get("ntuple");
  TTree *t3 = (TTree*)f3->Get("ntuple");    
  TH1D *h = new TH1D();
  std::string plot_string = "aco_angle_1>>h";
  std::string binning = "(10,0,6.3)";
  std::string baseline = "1";
  if (channel == "tt") baseline = "mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto";
  std::string cut_string_1 = "wt*("+baseline+")*("+cp_channel+")";
  
  if(cp_chan>1){
    cut_string_1+="*(cp_sign_1<0)";    
  }
  
  TFile *fout = new TFile(outfile_name.c_str(),"RECREATE");
  fout->cd();
  
  t1->Draw((plot_string+binning).c_str(),cut_string_1.c_str(),"goff");
  h = (TH1D*)gDirectory->Get("h");
  h->SetName("even_1");
  h->Write();
  t2->Draw((plot_string+binning).c_str(),cut_string_1.c_str(),"goff");
  h = (TH1D*)gDirectory->Get("h");
  h->SetName("odd_1");
  h->Write();
  t3->Draw((plot_string+binning).c_str(),cut_string_1.c_str(),"goff");
  h = (TH1D*)gDirectory->Get("h");
  h->SetName("none_1");
  h->Clone()->Write();
  
  
  if(cp_chan>1){
    std::string cut_string_2 = "wt*("+baseline+")*("+cp_channel+")";
    cut_string_2+="*(cp_sign_1>0)";      
    t1->Draw((plot_string+binning).c_str(),cut_string_2.c_str(),"goff");
    h = (TH1D*)gDirectory->Get("h");
    h->SetName("even_2");
    h->Write();
    t2->Draw((plot_string+binning).c_str(),cut_string_2.c_str(),"goff");
    h = (TH1D*)gDirectory->Get("h");
    h->SetName("odd_2");
    h->Write();
    t3->Draw((plot_string+binning).c_str(),cut_string_2.c_str(),"goff");
    h = (TH1D*)gDirectory->Get("h");
    h->SetName("none_2");
    h->Write();   
  
  }
}  

void GenCPPlots(std::string outfile_name, std::string channel, int cp_chan){
  std::string cp_channel = "cp_channel=="+std::to_string(cp_chan);
  std::string dir_name = "/vols/cms/dw515/CPStudies/Gen2/";
      
  TFile *f1 = new TFile((dir_name+"GluGluToHToTauTau_M-125_tt_2016.root").c_str());    
  TFile *f2 = new TFile((dir_name+"SUSYGluGluToHToTauTau_M-120_tt_2016.root").c_str());    
  TFile *f3 = new TFile((dir_name+"DYJetsToLL_tt_2016.root").c_str());
  
  TTree *t1 = (TTree*)f1->Get("gen_ntuple");
  TTree *t2 = (TTree*)f2->Get("gen_ntuple");
  TTree *t3 = (TTree*)f3->Get("gen_ntuple");    
  TH1D *h = new TH1D();
  std::string plot_string = "aco_angle_1>>h";
  std::string binning = "(20,0,6.3)";
  std::string cut_string_1 = "wt*(pt_1>20&&pt_2>20)*("+cp_channel+")*(channel==\""+channel+"\")";
  
  if(cp_chan>1){
    cut_string_1+="*(cp_sign_1<0)";    
  }
  
  TFile *fout = new TFile(outfile_name.c_str(),"RECREATE");
  fout->cd();

  t1->Draw((plot_string+binning).c_str(),cut_string_1.c_str(),"goff");
  h = (TH1D*)gDirectory->Get("h");
  h->SetName("even_1");
  h->Write();
  t2->Draw((plot_string+binning).c_str(),cut_string_1.c_str(),"goff");
  h = (TH1D*)gDirectory->Get("h");
  h->SetName("odd_1");
  h->Write();
  t3->Draw((plot_string+binning).c_str(),cut_string_1.c_str(),"goff");
  h = (TH1D*)gDirectory->Get("h");
  h->SetName("none_1");
  h->Clone()->Write();

  
  if(cp_chan>1){
    std::string cut_string_2 = "wt*(pt_1>20&&pt_2>20)*("+cp_channel+")*(channel==\""+channel+"\")";
    cut_string_2+="*(cp_sign_1>0)";      
    t1->Draw((plot_string+binning).c_str(),cut_string_2.c_str(),"goff");
    h = (TH1D*)gDirectory->Get("h");
    h->SetName("even_2");
    h->Write();
    t2->Draw((plot_string+binning).c_str(),cut_string_2.c_str(),"goff");
    h = (TH1D*)gDirectory->Get("h");
    h->SetName("odd_2");
    h->Write();
    t3->Draw((plot_string+binning).c_str(),cut_string_2.c_str(),"goff");
    h = (TH1D*)gDirectory->Get("h");
    h->SetName("none_2");
    h->Write();   

  }
}    

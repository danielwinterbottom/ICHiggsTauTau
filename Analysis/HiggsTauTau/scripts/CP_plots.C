//void OfflineCPPlots(){
//std::string cp_channel = "cp_channel=3";
//std::string channel = "tt";
//bool offline = false;
//std::string dir_name;
//std::string outfile_name;
//std::string tree_name;
//if (offline) {dir_name = "/vols/cms/dw515/CPStudies/Offline/"; outfile_name = "offline_cp_comparrisons_2.root"; tree_name = "ntuple";}
//else {dir_name = "/vols/cms/dw515/CPStudies/Gen/"; outfile_name = "gen_cp_comparrisons_2.root"; tree_name = "gen_ntuple";}
//    
//TFile *f1 = new TFile((dir_name+"GluGluToHToTauTau_M-125_tt_2016.root").c_str());    
//TFile *f2 = new TFile((dir_name+"SUSYGluGluToHToTauTau_M-120_tt_2016.root").c_str());    
//TFile *f3 = new TFile((dir_name+"DYJetsToLL_tt_2016.root").c_str());
//
//TTree *t1 = (TTree*)f1->Get(tree_name.c_str());
//TTree *t2 = (TTree*)f2->Get(tree_name.c_str());
//TTree *t3 = (TTree*)f3->Get(tree_name.c_str());    
//
//std::string plot_string = "aco_angle_1>>h";
//std::string binning = "(20,0,6.3)";
//std::string cut_string_1 = "wt*(cp_sign_1<0)"+"*("+cp_channel+")";
//std::string cut_string_2 = "wt*(cp_sign_1>0)"+"*("+cp_channel+")";    
//TFile *fout = new TFile(outfile_name.c_str(),"RECREATE");
//fout->cd();
//t1->Draw((plot_string+"1"+binning).c_str(),cut_string_1.c_str(),"goff");
//h1->SetName("even_1");
//h1->Write();
//t1->Draw((plot_string+"2"+binning).c_str(),cut_string_2.c_str(),"goff");
//h2->SetName("even_2");
//h2->Write();
//
//t2->Draw((plot_string+"3"+binning).c_str(),cut_string_1.c_str(),"goff");
//h3->SetName("odd_1");
//h3->Write();
//t2->Draw((plot_string+"4"+binning).c_str(),cut_string_2.c_str(),"goff");
//h4->SetName("odd_2");
//h4->Write();  
//
//t3->Draw((plot_string+"5"+binning).c_str(),cut_string_1.c_str(),"goff");
//h5->SetName("none_1");
//h5->Clone()->Write();
//t3->Draw((plot_string+"6"+binning).c_str(),cut_string_2.c_str(),"goff");
//h6->SetName("none_2");
//h6->Write();   
//}   

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
  std::string cut_string_1 = "wt*("+cp_channel+")*(channel==\""+channel+"\")";
  
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
    std::string cut_string_2 = "wt*("+cp_channel+")*(channel==\""+channel+"\")";
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

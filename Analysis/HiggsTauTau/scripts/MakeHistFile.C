{
TFile *fout = new TFile("test.root","RECREATE");
TFile *f1 = new TFile("DYJetsToLL_zmm_2016.root");
TTree *t1 = (TTree*)f1->Get("gen_ntuple");
TH1D *nlo = new TH1D("nlo","",20,0,200);
TH1D *nlo_b = new TH1D("nlo_b","",20,0,200);
t1->Draw("genpT>>nlo","wt", "goff");
t1->Draw("genpT>>nlo_b","wt*(n_bquarks>0)", "goff");

fout->cd();
nlo->Write();
nlo_b->Write();
f1->Close();

TH1D *lo = new TH1D("lo","",20,0,200);
TH1D *lo_b = new TH1D("lo_b","",20,0,200);
TH1D *lo_wtzpt = new TH1D("lo_wtzpt","",20,0,200);
TH1D *lo_b_wtzpt = new TH1D("lo_b_wtzpt","",20,0,200);
TH1D *temp = new TH1D("temp","",20,0,200);

TH1D *lo_btagcat = new TH1D("lo_btagcat","",20,0,200);
TH1D *lo_inccat = new TH1D("lo_inccat","",20,0,200);



std::vector<std::string> filenames = {"DYJetsToLL-LO-ext2_zmm_2016.root", "DY1JetsToLL-LO_zmm_2016.root", "DY2JetsToLL-LO_zmm_2016.root", "DY3JetsToLL-LO_zmm_2016.root", "DY4JetsToLL-LO_zmm_2016.root"};
for (unsigned i=0;i<1&& i< filenames.size();++i){
  f1 = new TFile(filenames[i].c_str());
  t1 = (TTree*)f1->Get("gen_ntuple");
  temp = new TH1D("temp","",20,0,200);
  t1->Draw("genpT>>temp","wt", "goff");
  lo->Add(temp);
  t1->Draw("genpT>>temp","wt*(n_bquarks>0)", "goff");
  lo_b->Add(temp);
  t1->Draw("genpT>>temp","wt*wt_zpt", "goff");
  lo_wtzpt->Add(temp);
  t1->Draw("genpT>>temp","wt*wt_zpt*(n_bquarks>0)", "goff");
  lo_b_wtzpt->Add(temp);
  f1->Close();
}

for (unsigned i=0;i<1&& i< filenames.size();++i){
  f1 = new TFile(filenames[i].c_str());
  t1 = (TTree*)f1->Get("ntuple");
  temp = new TH1D("temp","",20,0,200);
  t1->Draw("pt_tt>>temp","wt*(iso_1<0.15 && iso_2<0.15 && m_vis>80 && m_vis<120 && n_bjets>=1 && n_jets<=1)", "goff");
  lo_inccat->Add(temp);

  t1->Draw("pt_tt>>temp","wt*(iso_1<0.15 && iso_2<0.15 && m_vis>80 && m_vis<120 && n_bjets>=1 && n_jets<=1 && n_bquarks>0)", "goff");
  lo_btagcat->Add(temp);

  f1->Close();
}
double total = lo_inccat->Integral(0, lo_inccat->GetNbinsX()+1);
double total_withbs = lo_btagcat->Integral(0, lo_btagcat->GetNbinsX()+1);

std::cout << "Total = " << total << ", with bquarks = " << total_withbs << ", % = " << total_withbs/total*100 << std::endl;


fout->cd();
lo->Write();
lo_b->Write();
lo_wtzpt->Write();
lo_b_wtzpt->Write();




}

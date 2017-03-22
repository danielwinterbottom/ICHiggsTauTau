{
    
std::vector<std::string> file_names = {"datacard_pt_tt_inclusive_zmm_2016_mvis50to80.root", "datacard_pt_tt_inclusive_zmm_2016_mvis80to120.root", "datacard_pt_tt_inclusive_zmm_2016_mvis120to160.root", "datacard_pt_tt_inclusive_zmm_2016_mvis160to200.root", "datacard_pt_tt_inclusive_zmm_2016_mvis200to400.root", "datacard_pt_tt_inclusive_zmm_2016_mvis400to800.root"};

std::vector<std::string> gen_file_names = {"datacard_genpT_inclusive_zmm_2016_mvis50to80.root", "datacard_genpT_inclusive_zmm_2016_mvis80to120.root", "datacard_genpT_inclusive_zmm_2016_mvis120to160.root", "datacard_genpT_inclusive_zmm_2016_mvis160to200.root", "datacard_genpT_inclusive_zmm_2016_mvis200to400.root", "datacard_genpT_inclusive_zmm_2016_mvis400to800.root"};

double x_bins[9] = {0,50,80,120,160,200,400,800,10000};
double y_bins[13] = {0,40,80,120,160,200,240,280,320,400,500,600,10000};
    
int n_xbins = 8;
int n_ybins = 12;

std::vector<TH1D*> data_hist_vector;
std::vector<TH1D*> mc_hist_vector;
std::vector<TH1D*> syst_hist_vector;
// add first histogram with content = 1!
TH1D *h_first = new TH1D("h_first","h_first",1,0,10000);
h_first->SetBinContent(1,1);
data_hist_vector.push_back(h_first);
mc_hist_vector.push_back(h_first);
syst_hist_vector.push_back(h_first);
                
for(unsigned i=0; i<file_names.size(); ++i){
  std::string file_name = file_names[i];
  TFile f(file_name.c_str());
  TH1D *h1 = (TH1D*)f.Get("zmm_inclusive/data_obs")->Clone();
  TH1D *h2 = (TH1D*)f.Get("zmm_inclusive/total_bkg")->Clone();
  TH1D *h3 = (TH1D*)f.Get("zmm_inclusive/ZLL")->Clone();
  h1->SetDirectory(0);
  h2->SetDirectory(0);
  h3->SetDirectory(0);
  //subtract ZLL from background
  h2->Add(h3,-1);
  //subtract background from data
  h1->Add(h2,-1);
  data_hist_vector.push_back(h1);
  mc_hist_vector.push_back(h3);
  f.Close();  
}

for(unsigned i=0; i<gen_file_names.size(); ++i){
  std::string file_name = gen_file_names[i];
  TFile f(file_name.c_str());
  TH1D *h1 = (TH1D*)f.Get("zmm_inclusive/ZLL")->Clone();
  h1->SetDirectory(0);
  syst_hist_vector.push_back(h1);
  f.Close();  
}

double data_total=0;
double mc_total=0;
double syst_total=0;
for(unsigned i=1; i<data_hist_vector.size();++i){
  data_total+=data_hist_vector[i]->Integral();
  mc_total+=mc_hist_vector[i]->Integral();
  syst_total+=syst_hist_vector[i]->Integral();   
}
for(unsigned i=1; i<data_hist_vector.size();++i){
  data_hist_vector[i]->Scale(1/data_total);
  mc_hist_vector[i]->Scale(1/mc_total);
  syst_hist_vector[i]->Scale(1/syst_total);
}

//add end histogram with content =1
TH1D *h_last = new TH1D("h_last","h_last",1,0,10000);
h_last->SetBinContent(1,1);
data_hist_vector.push_back(h_last);
mc_hist_vector.push_back(h_last);
syst_hist_vector.push_back(h_last);

TH2D *h_data = new TH2D("zptmass_histo","zptmass_histo",n_xbins,x_bins,n_ybins,y_bins);
TH2D *h_mc = new TH2D("zptmass_histo","zptmass_histo",n_xbins,x_bins,n_ybins,y_bins);

for(unsigned i=0; i<h_data->GetNbinsX(); ++i){
  unsigned x_bin = i+1;
  TH1D *h = data_hist_vector[i];
  TH1D *h_syst = syst_hist_vector[i];
  TH1D *h_den = mc_hist_vector[i];
  h->Divide(h_den);
  h_syst->Divide(h_den);
  
  for(unsigned j=0; j<h_data->GetNbinsY(); ++j){
      unsigned y_bin = j+1;
      double z_pt = h_data->GetYaxis()->GetBinCenter(y_bin);
      unsigned bin = h->GetXaxis()->FindBin(z_pt);
      double content = h->GetBinContent(bin);
      if(y_bin == h_data->GetNbinsY()) content = 1;
      h_data->SetBinContent(x_bin,y_bin,content);
  }
  
}

TFile *fout = new TFile("z_pt_mass_weights_inclusive_v1.root","RECREATE");
fout->cd();


h_data->Write();      
    
    
}

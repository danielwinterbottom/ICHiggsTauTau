void makeZWeightsFile(std::string outfile){

double x_bins[10] = {0,50,80,90,100,120,160,200,400,800};
double y_bins[15] = {0,10,20,30,40,60,80,100,120,160,200,280,320,400,600};
//std::vector<std::string> MC_add_strings = {""/*, "_ESUp", "_ESDown","_TTUp", "_TTDown"/*, "_IDUp", "_IDDown", "_IsoUp", "_IsoDown", "_TrgUp", "_TrgDown"*/};
std::vector<std::string> MC_add_strings = {"","_bkgUp", "_bkgDown"};
int n_xbins = 9;
int n_ybins = 14;
TFile *fout = new TFile(outfile.c_str(),"RECREATE");
std::vector<std::string> file_names = {"datacard_pt_tt_inclusive_zmm_2016_mvis50to80.root", "datacard_pt_tt_inclusive_zmm_2016_mvis80to90.root", "datacard_pt_tt_inclusive_zmm_2016_mvis90to100.root", "datacard_pt_tt_inclusive_zmm_2016_mvis100to120.root", "datacard_pt_tt_inclusive_zmm_2016_mvis120to160.root", "datacard_pt_tt_inclusive_zmm_2016_mvis160to200.root","datacard_pt_tt_inclusive_zmm_2016_mvis200to400.root","datacard_pt_tt_inclusive_zmm_2016_mvis400toinf.root"};
for(unsigned i=0; i<MC_add_strings.size(); ++i){
    std::string MC_add_string = MC_add_strings[i];
    double tt_scale=1.0;
    if (MC_add_strings[i]=="_TTDown" || MC_add_strings[i]=="_TTUp") MC_add_string = "";
    if (MC_add_strings[i]=="_TTUp") tt_scale = 1.06;
    if (MC_add_strings[i]=="_TTDown") tt_scale = 0.94;
    double bkg_scale=1.0;
    if (MC_add_strings[i]=="_bkgDown" || MC_add_strings[i]=="_bkgUp") MC_add_string = "";
    if (MC_add_strings[i]=="_bkgUp") bkg_scale = 1.2;
    if (MC_add_strings[i]=="_bkgDown") bkg_scale = 0.8;
    std::string hist_name = "zptmass_histo"+MC_add_strings[i];    
    TH2D *h_2dweights = new TH2D(hist_name.c_str(),hist_name.c_str(),n_xbins,x_bins,n_ybins,y_bins);
    h_2dweights       ->Sumw2();
    
    std::vector<TH1D*> data_hist_vector;
    std::vector<TH1D*> mc_hist_vector;
    // add first histogram with content = 1!
    TH1D *h_first = new TH1D("h_first","h_first",1,0,10000);
    h_first->SetBinContent(1,1);
    data_hist_vector.push_back(h_first);
    mc_hist_vector.push_back(h_first);
                    
    for(unsigned i=0; i<file_names.size(); ++i){
      std::string file_name = file_names[i];
      TFile f(file_name.c_str());
      TH1D *h1 = (TH1D*)f.Get(("zmm_inclusive/data_obs"))->Clone();
      TH1D *h2 = (TH1D*)f.Get(("zmm_inclusive/total_bkg"+MC_add_string).c_str())->Clone();
      TH1D *h3 = (TH1D*)f.Get(("zmm_inclusive/ZL"+MC_add_string).c_str())->Clone();
      h3->Add((TH1D*)f.Get(("zmm_inclusive/ZJ"+MC_add_string).c_str())->Clone());
      h3->Add((TH1D*)f.Get(("zmm_inclusive/ZTT"+MC_add_string).c_str())->Clone());
      TH1D *h4 = (TH1D*)f.Get(("zmm_inclusive/TT"+MC_add_string).c_str())->Clone();
      
      TH1D *h5 = (TH1D*)f.Get(("zmm_inclusive/total_bkg"+MC_add_string).c_str())->Clone();

      h1->SetDirectory(0);
      h2->SetDirectory(0);
      h3->SetDirectory(0);
      h4->SetDirectory(0);
      //subtract ZLL from background
      h2->Add(h3,-1);
      if (tt_scale!=1.0){
        //subtract TT from background
        h2->Add(h4,-1);
        h4->Scale(tt_scale);
        //Add shifted ttbar to backgrounds
        h2->Add(h4);
      }
      if (bkg_scale!=1.0){
        //subtract TT from background
        h5->Add(h3,-1);
        h5->Scale(bkg_scale);
        //Add shifted ttbar to backgrounds
        h2 = h5;
      }
      //subtract background from data
      h1->Add(h2,-1);
      data_hist_vector.push_back(h1);
      mc_hist_vector.push_back(h3);
      f.Close();  
    }
    
    TH2D *h_data = new TH2D(hist_name.c_str(),"",n_xbins,x_bins,n_ybins,y_bins);
    TH2D *h_mc = new TH2D("zptmass_histo_mc","zptmass_histo_mc",n_xbins,x_bins,n_ybins,y_bins);
    h_data->Sumw2();
    h_mc  ->Sumw2();
    
    for(unsigned i=0; i<h_data->GetNbinsX(); ++i){
      unsigned x_bin = i+1;
      TH1D *h = data_hist_vector[i];
      
      for(unsigned j=0; j<h_data->GetNbinsY(); ++j){
          unsigned y_bin = j+1;
          double z_pt = h_data->GetYaxis()->GetBinCenter(y_bin);
          unsigned bin = h->GetXaxis()->FindBin(z_pt);
          double content = h->GetBinContent(bin);
          double error = h->GetBinError(bin);
          if(x_bin==1){ content = 1; error = 0;}
          h_data->SetBinContent(x_bin,y_bin,content);
          h_data->SetBinError(x_bin,y_bin,error);          
          
      }
      
    }
    for(unsigned i=0; i<h_mc->GetNbinsX(); ++i){
      unsigned x_bin = i+1;
      TH1D *h = mc_hist_vector[i];
      
      for(unsigned j=0; j<h_mc->GetNbinsY(); ++j){
          unsigned y_bin = j+1;
          double z_pt = h_mc->GetYaxis()->GetBinCenter(y_bin);
          unsigned bin = h->GetXaxis()->FindBin(z_pt);
          double content = h->GetBinContent(bin);
          double error = h->GetBinError(bin);
          if(x_bin==1){ content = 1; error = 0;}
          h_mc->SetBinContent(x_bin,y_bin,content);
          h_mc->SetBinError(x_bin,y_bin,error);
      }
      
    }
    
    double data_total=0;
    double mc_total=0;
    for(unsigned i=1; i<=(unsigned)h_data->GetNbinsX();++i){
      for(unsigned j=1; j<=(unsigned)h_data->GetNbinsY();++j){
        if(!(i == 1 )){
            double data_content=h_data->GetBinContent(i,j);
            data_total+=data_content;
            double mc_content=h_mc->GetBinContent(i,j);
            mc_total+=mc_content;        
        }
      }
    }
    for(unsigned i=1; i<=(unsigned)h_data->GetNbinsX();++i){
      for(unsigned j=1; j<=(unsigned)h_data->GetNbinsY();++j){
        if(!(i == 1 )){
            double data_content=h_data->GetBinContent(i,j);
            h_data->SetBinContent(i,j,data_content/data_total);
            double data_error=h_data->GetBinError(i,j);
            h_data->SetBinError(i,j,data_error/mc_total);
            double mc_content=h_mc->GetBinContent(i,j);
            h_mc->SetBinContent(i,j,mc_content/mc_total);
            double mc_error=h_mc->GetBinError(i,j);
            h_mc->SetBinError(i,j,mc_error/mc_total);
        }
      }
    }
    h_data->Divide(h_mc);  
    h_2dweights = (TH2D*)h_data->Clone();
    //set the overflow bins to the maximum bin
    for (unsigned i=1; i<=(unsigned)h_2dweights->GetNbinsX()+1;++i){
      int y_bin = h_2dweights->GetNbinsY()+1;
      int x_bin = i;
      if (i == h_2dweights->GetNbinsX()+1) x_bin = i-1;
      double content = h_2dweights->GetBinContent(x_bin,y_bin-1);
      double error = h_2dweights->GetBinError(x_bin,y_bin-1);
      h_2dweights->SetBinContent(x_bin,y_bin,content);
      h_2dweights->SetBinError(x_bin,y_bin,error);
    }
    for (unsigned i=1; i<=(unsigned)h_2dweights->GetNbinsY()+1;++i){
      int x_bin = h_2dweights->GetNbinsX()+1;
      int y_bin = i;
      if (i == h_2dweights->GetNbinsY()+1) y_bin = i-1;
      double content = h_2dweights->GetBinContent(x_bin-1,y_bin);
      double error = h_2dweights->GetBinError(x_bin-1,y_bin);
      h_2dweights->SetBinContent(x_bin,y_bin,content);
      h_2dweights->SetBinError(x_bin,y_bin,error);
    }
    h_2dweights->SetBinContent(h_2dweights->GetNbinsX()+1,h_2dweights->GetNbinsY()+1,h_2dweights->GetBinContent(h_2dweights->GetNbinsX(),h_2dweights->GetNbinsY()));
    h_2dweights->SetBinError(h_2dweights->GetNbinsX()+1,h_2dweights->GetNbinsY()+1,h_2dweights->GetBinError(h_2dweights->GetNbinsX(),h_2dweights->GetNbinsY()));
    
    fout->cd();
    h_2dweights->Write();
    
    
    //TH1D *h_stat_up;
    //TH1D *h_stat_down;
    //double error; double content;
    ////M400pT0 stat
    //h_stat_up = (TH1D*)h_2dweights->Clone();
    //h_stat_down = (TH1D*)h_2dweights->Clone();
    //h_stat_up->SetName("zptmass_histo_StatM400pT80Up");
    //h_stat_up->SetTitle("zptmass_histo_StatM400pT80Up");
    //h_stat_down->SetName("zptmass_histo_StatM400pT80Down");
    //h_stat_down->SetTitle("zptmass_histo_StatM400pT80Down");
    //error = h_2dweights->GetBinError(h_2dweights->FindBin(400,80));
    //content = h_2dweights->GetBinContent(h_2dweights->FindBin(400,80));
    //int bin = h_2dweights->GetXaxis()->FindBin(400);
    //for(unsigned i=7; i<=15; ++i){
    //  h_stat_up->SetBinContent(bin,i,content+error);
    //  h_stat_up->SetBinContent(bin+1,i,content+error);
    //  h_stat_down->SetBinContent(bin,i,content-error);
    //  h_stat_down->SetBinContent(bin+1,i,content-error);
    //}
    //
    //h_stat_up->Write();
    //h_stat_down->Write();
    
    std::cout << "-------------------------" << std::endl;
    
    for (unsigned i=0; i<(unsigned)h_2dweights->GetNbinsX()+1; ++i){
      for (unsigned j=0; j<(unsigned)h_2dweights->GetNbinsY(); ++j){
          double mass   = h_2dweights->GetYaxis()->GetBinLowEdge(i+1);
          double pt     = h_2dweights->GetZaxis()->GetBinLowEdge(j+1);
          double weight = h_2dweights->GetBinContent(i+1,j+1);
          double error =  h_2dweights->GetBinError(i+1,j+1);      
          std::cout << "mass = " << mass << ", pt = " << pt << ", weight = " << weight << ", error = " << error*100/weight << "%" << std::endl;
      }
    }
    std::cout << "-------------------------" << std::endl;
    
}

}


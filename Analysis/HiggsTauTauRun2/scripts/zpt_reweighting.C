void zpt_reweighting(std::string outfile="zpt_reweighting_LO.root"){
  // initialising binning
  double x_bins[17] = {0,50,60,70,80,90,100,120,140,160,180,200,300,400,600,800,1000};
  double y_bins[15] = {0,10,20,30,40,60,80,100,120,160,200,280,320,400,600};
  int n_xbins = 16;
  int n_ybins = 14;
  TFile *fout = new TFile(outfile.c_str(),"RECREATE");
  // datacards (var pt_tt with mvis cuts)
  std::vector<std::string> file_names = {
    "input/zpt/LO/2018/zmm/datacard_pt_tt_inclusive_zmm_2018_mvis50to60.root",
    "input/zpt/LO/2018/zmm/datacard_pt_tt_inclusive_zmm_2018_mvis60to70.root",
    "input/zpt/LO/2018/zmm/datacard_pt_tt_inclusive_zmm_2018_mvis70to80.root",
    "input/zpt/LO/2018/zmm/datacard_pt_tt_inclusive_zmm_2018_mvis80to90.root",
    "input/zpt/LO/2018/zmm/datacard_pt_tt_inclusive_zmm_2018_mvis90to100.root",
    "input/zpt/LO/2018/zmm/datacard_pt_tt_inclusive_zmm_2018_mvis100to120.root",
    "input/zpt/LO/2018/zmm/datacard_pt_tt_inclusive_zmm_2018_mvis120to140.root",
    "input/zpt/LO/2018/zmm/datacard_pt_tt_inclusive_zmm_2018_mvis140to160.root",	
    "input/zpt/LO/2018/zmm/datacard_pt_tt_inclusive_zmm_2018_mvis160to180.root",
    "input/zpt/LO/2018/zmm/datacard_pt_tt_inclusive_zmm_2018_mvis180to200.root",
    "input/zpt/LO/2018/zmm/datacard_pt_tt_inclusive_zmm_2018_mvis200to300.root",	
    "input/zpt/LO/2018/zmm/datacard_pt_tt_inclusive_zmm_2018_mvis300to400.root",
    "input/zpt/LO/2018/zmm/datacard_pt_tt_inclusive_zmm_2018_mvis400to600.root",
    "input/zpt/LO/2018/zmm/datacard_pt_tt_inclusive_zmm_2018_mvis600to800.root",		 
    "input/zpt/LO/2018/zmm/datacard_pt_tt_inclusive_zmm_2018_mvis800toinf.root",	
    "input/zpt/LO/2017/zmm/datacard_pt_tt_inclusive_zmm_2017_mvis50to60.root",
    "input/zpt/LO/2017/zmm/datacard_pt_tt_inclusive_zmm_2017_mvis60to70.root",
    "input/zpt/LO/2017/zmm/datacard_pt_tt_inclusive_zmm_2017_mvis70to80.root",
    "input/zpt/LO/2017/zmm/datacard_pt_tt_inclusive_zmm_2017_mvis80to90.root",
    "input/zpt/LO/2017/zmm/datacard_pt_tt_inclusive_zmm_2017_mvis90to100.root",
    "input/zpt/LO/2017/zmm/datacard_pt_tt_inclusive_zmm_2017_mvis100to120.root",
    "input/zpt/LO/2017/zmm/datacard_pt_tt_inclusive_zmm_2017_mvis120to140.root",
    "input/zpt/LO/2017/zmm/datacard_pt_tt_inclusive_zmm_2017_mvis140to160.root",	
    "input/zpt/LO/2017/zmm/datacard_pt_tt_inclusive_zmm_2017_mvis160to180.root",
    "input/zpt/LO/2017/zmm/datacard_pt_tt_inclusive_zmm_2017_mvis180to200.root",
    "input/zpt/LO/2017/zmm/datacard_pt_tt_inclusive_zmm_2017_mvis200to300.root",	
    "input/zpt/LO/2017/zmm/datacard_pt_tt_inclusive_zmm_2017_mvis300to400.root",
    "input/zpt/LO/2017/zmm/datacard_pt_tt_inclusive_zmm_2017_mvis400to600.root",
    "input/zpt/LO/2017/zmm/datacard_pt_tt_inclusive_zmm_2017_mvis600to800.root",		 
    "input/zpt/LO/2017/zmm/datacard_pt_tt_inclusive_zmm_2017_mvis800toinf.root",		 
    "input/zpt/LO/2016_postVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis50to60.root",
    "input/zpt/LO/2016_postVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis60to70.root",
    "input/zpt/LO/2016_postVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis70to80.root",
    "input/zpt/LO/2016_postVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis80to90.root",
    "input/zpt/LO/2016_postVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis90to100.root",
    "input/zpt/LO/2016_postVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis100to120.root",
    "input/zpt/LO/2016_postVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis120to140.root",
    "input/zpt/LO/2016_postVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis140to160.root",	
    "input/zpt/LO/2016_postVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis160to180.root",
    "input/zpt/LO/2016_postVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis180to200.root",
    "input/zpt/LO/2016_postVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis200to300.root",	
    "input/zpt/LO/2016_postVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis300to400.root",
    "input/zpt/LO/2016_postVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis400to600.root",
    "input/zpt/LO/2016_postVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis600to800.root",		 
    "input/zpt/LO/2016_postVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis800toinf.root",		 
    "input/zpt/LO/2016_preVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis50to60.root",
    "input/zpt/LO/2016_preVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis60to70.root",
    "input/zpt/LO/2016_preVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis70to80.root",
    "input/zpt/LO/2016_preVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis80to90.root",
    "input/zpt/LO/2016_preVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis90to100.root",
    "input/zpt/LO/2016_preVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis100to120.root",
    "input/zpt/LO/2016_preVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis120to140.root",
    "input/zpt/LO/2016_preVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis140to160.root",	
    "input/zpt/LO/2016_preVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis160to180.root",
    "input/zpt/LO/2016_preVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis180to200.root",
    "input/zpt/LO/2016_preVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis200to300.root",	
    "input/zpt/LO/2016_preVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis300to400.root",
    "input/zpt/LO/2016_preVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis400to600.root",
    "input/zpt/LO/2016_preVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis600to800.root",		 
    "input/zpt/LO/2016_preVFP/zmm/datacard_pt_tt_inclusive_zmm_2016_mvis800toinf.root",			 
  };
  
  std::vector<TH1D*> data_hist_vector;
  std::vector<TH1D*> mc_hist_vector;

  // add first histogram with content = 1!
  TH1D *h_first = new TH1D("h_first","h_first",1,0,10000);
  h_first->SetBinContent(1,1);
  data_hist_vector.push_back(h_first);
  mc_hist_vector.push_back(h_first);
 
  double data_integral = 0; 
  double mc_integral = 0;
  for(unsigned i=0; i<file_names.size(); ++i){
    std::string file_name = file_names[i];
    TFile f(file_name.c_str());
    // cloning data_obs from datacard
    TH1D *h1 = (TH1D*)f.Get("zmm_inclusive/data_obs")->Clone();
    // cloning bkgs from datacard
    TH1D *h_ZTT = (TH1D*)f.Get("zmm_inclusive/ZTT")->Clone();
    TH1D *h_ZL = (TH1D*)f.Get("zmm_inclusive/ZL")->Clone();
    TH1D *h_ZJ = (TH1D*)f.Get("zmm_inclusive/ZJ")->Clone();
    TH1D *h_TTJ = (TH1D*)f.Get("zmm_inclusive/TTJ")->Clone();
    TH1D *h_VVJ = (TH1D*)f.Get("zmm_inclusive/VVJ")->Clone();
    TH1D *h_W = (TH1D*)f.Get("zmm_inclusive/W")->Clone();
    TH1D *h_QCD = (TH1D*)f.Get("zmm_inclusive/QCD")->Clone();
    TH1D *h_TTL = (TH1D*)f.Get("zmm_inclusive/TTL")->Clone();
    TH1D *h_VVL = (TH1D*)f.Get("zmm_inclusive/VVL")->Clone();

    h1->SetDirectory(0);
    h_ZTT->SetDirectory(0);
    h_ZL->SetDirectory(0);
    h_ZJ->SetDirectory(0);
    h_TTJ->SetDirectory(0);
    h_VVJ->SetDirectory(0);
    h_W->SetDirectory(0);
    h_QCD->SetDirectory(0);
    h_TTL->SetDirectory(0);
    h_VVL->SetDirectory(0);

    TH1* h_bkg = new TH1D(*h_ZTT);
    h_bkg->SetDirectory(0);
    //add all backgrounds except ZL
    h_bkg->Add(h_ZJ,1);
    h_bkg->Add(h_TTJ,1);
    h_bkg->Add(h_VVJ,1);
    h_bkg->Add(h_W,1);
    h_bkg->Add(h_QCD,1);
    h_bkg->Add(h_TTL,1);
    h_bkg->Add(h_VVL,1);
    // subtract backgrounds from data
    h1->Add(h_bkg,-1);

    data_integral += h1->Integral(-1,-1);
    mc_integral += h_ZL->Integral(-1,-1);    
    // store (data-bkg) and ZL histograms in vectors
    data_hist_vector.push_back(h1);
    mc_hist_vector.push_back(h_ZL);
    f.Close();

    // old code
    // --------
    //TH1D *h2 = (TH1D*)f.Get("zmm_inclusive/total_bkg")->Clone();
    //TH1D *h3 = (TH1D*)f.Get("zmm_inclusive/ZL")->Clone();
    //h1->SetDirectory(0);
    //h2->SetDirectory(0);
    //h3->SetDirectory(0);
    //subtract ZLL from background
    //h2->Add(h3,-1);
    //h2->GetBinContent(1);
    //subtract background from data
    //h1->Add(h2,-1);
    //data_hist_vector.push_back(h1);
    //mc_hist_vector.push_back(h3);
    //f.Close(); 
    // ------- 
 }
  std::string hist_name = "zptmass_histo"; 
  TH2D *h_data = new TH2D(hist_name.c_str(),"",n_xbins,x_bins,n_ybins,y_bins);
  TH2D *h_mc = new TH2D("zptmass_histo_mc","zptmass_histo_mc",n_xbins,x_bins,n_ybins,y_bins);
  h_data->Sumw2();
  h_mc  ->Sumw2();
  // filling data TH2D
  for(unsigned i=0; i<h_data->GetNbinsX(); ++i){
    unsigned x_bin = i+1;
    TH1D *h = data_hist_vector[i];
    if (i != 0) {
      TH1D *h_2 = data_hist_vector[15+i];
      TH1D *h_3 = data_hist_vector[30+i];
      TH1D *h_4 = data_hist_vector[45+i];
      h->SetDirectory(0);
      h_2->SetDirectory(0);
      h_3->SetDirectory(0);
      h_4->SetDirectory(0);
      h->Add(h_2,1);
      h->Add(h_3,1);
      h->Add(h_4,1);
    }

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
  // filling mc TH2D
  for(unsigned i=0; i<h_mc->GetNbinsX(); ++i){
    unsigned x_bin = i+1;
    TH1D *h = mc_hist_vector[i];
    if (i != 0) {
      TH1D *h_2 = mc_hist_vector[15+i];
      TH1D *h_3 = mc_hist_vector[30+i];
      TH1D *h_4 = mc_hist_vector[45+i];
      h->SetDirectory(0);
      h_2->SetDirectory(0);
      h_3->SetDirectory(0);
      h_4->SetDirectory(0);
      h->Add(h_2,1);
      h->Add(h_3,1);
      h->Add(h_4,1);
    }
 
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


  // now both TH2D have been filled but, we need to normalise them before dividing them through
  for(unsigned i=1; i<=(unsigned)h_data->GetNbinsX();++i){
    for(unsigned j=1; j<=(unsigned)h_data->GetNbinsY();++j){
      if(!(i == 1 )){
          double data_content=h_data->GetBinContent(i,j);
          h_data->SetBinContent(i,j,data_content/data_integral);
          //std::cout << i << " " << j << " "<< data_content/data_integral << std::endl;
          double data_error=h_data->GetBinError(i,j);
          h_data->SetBinError(i,j,data_error/data_integral);
          double mc_content=h_mc->GetBinContent(i,j);
          h_mc->SetBinContent(i,j,mc_content/mc_integral);
          double mc_error=h_mc->GetBinError(i,j);
          h_mc->SetBinError(i,j,mc_error/mc_integral);
      }
    }
  }
  
    
  double check = h_data->GetBinContent(2,1)/h_mc->GetBinContent(2,1);
  std::cout << "(2,1) data/MC: " << check << std::endl;
  std::cout << "(Data-Total_Bkg) + ZL = " << data_integral << " " << h_data->Integral(2,-1,1,-1) << std::endl;
  std::cout << "ZL = " <<  mc_integral << " " << h_mc->Integral(2,-1,1,-1) << std::endl;
  // Divide the data/mc TH2Ds
  h_data->Divide(h_mc); 
  std::string hist_name_2D = "zptmass_histo_2D"; 
  TH2D *h_2dweights = new TH2D(hist_name_2D.c_str(),hist_name_2D.c_str(),n_xbins,x_bins,n_ybins,y_bins);
  h_2dweights->Sumw2(); 
  // clone the divided TH2D
  h_2dweights = (TH2D*)h_data->Clone();
  std::cout << "weight: " << h_data->GetBinContent(2,1) << std::endl;

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
  
  
  std::cout << "-------------------------" << std::endl;
  
  for (unsigned i=0; i<(unsigned)h_2dweights->GetNbinsX()+1; ++i){
    for (unsigned j=0; j<(unsigned)h_2dweights->GetNbinsY(); ++j){
        double mass   = h_2dweights->GetXaxis()->GetBinLowEdge(i+1);
        double pt     = h_2dweights->GetYaxis()->GetBinLowEdge(j+1);
        double weight = h_2dweights->GetBinContent(i+1,j+1);
        double error =  h_2dweights->GetBinError(i+1,j+1);      
        std::cout << "mass = " << mass << ", pt = " << pt << ", weight = " << weight << ", error = " << error*100/weight << "%" << std::endl;
    }
  }
  std::cout << "-------------------------" << std::endl;
}

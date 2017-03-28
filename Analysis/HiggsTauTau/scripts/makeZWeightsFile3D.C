void makeZWeightsFile3D(std::string outfile, std::string MC_add_string){
std::vector<std::string> cats = {"0jet", "1jet", "ge2jet", "inclusive"};

double x_bins[4] = {0,1,2,10000};
double y_bins[9] = {0,50,80,120,160,200,400,800,10000};
double z_bins[15] = {0,10,20,30,40,60,80,100,120,160,200,280,320,600,10000};
int n_xbins=3;
int n_ybins = 8;
int n_zbins = 14;
    
TH3D *h3_data = new TH3D("znjetmasspt_histo","znjetmasspt_histo", n_xbins, x_bins, n_ybins, y_bins, n_zbins, z_bins);
TH3D *h3_mc = new TH3D("znjetmasspt_histo_mc","znjetmasspt_histo_mc", n_xbins, x_bins, n_ybins, y_bins, n_zbins, z_bins);
TH3D *h3_data_normxbins = new TH3D("znjetmasspt_histo_normxbins","znjetmasspt_histo_normxbins", n_xbins, x_bins, n_ybins, y_bins, n_zbins, z_bins);
TH3D *h3_mc_normxbins = new TH3D("znjetmasspt_histo_normxbins_mc","znjetmasspt_histo_normxbins_mc", n_xbins, x_bins, n_ybins, y_bins, n_zbins, z_bins);
TH2D *h_2dweights = new TH2D("zptmass_histo","zptmass_histo",n_ybins,y_bins,n_zbins,z_bins);
h3_data           ->Sumw2();
h3_mc             ->Sumw2();
h3_data_normxbins ->Sumw2();
h3_mc_normxbins   ->Sumw2();
h_2dweights       ->Sumw2();
for(unsigned cati=0; cati<cats.size(); ++cati){
    std::vector<std::string> file_names = {"datacard_pt_tt_"+cats[cati]+"_zmm_2016_mvis50to80.root", "datacard_pt_tt_"+cats[cati]+"_zmm_2016_mvis80to120.root", "datacard_pt_tt_"+cats[cati]+"_zmm_2016_mvis120to160.root", "datacard_pt_tt_"+cats[cati]+"_zmm_2016_mvis160to200.root","datacard_pt_tt_"+cats[cati]+"_zmm_2016_mvis200to400.root","datacard_pt_tt_"+cats[cati]+"_zmm_2016_mvis400to800.root"};
    
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
      TH1D *h1 = (TH1D*)f.Get(("zmm_"+cats[cati]+"/data_obs").c_str())->Clone();
      TH1D *h2 = (TH1D*)f.Get(("zmm_"+cats[cati]+"/total_bkg"+MC_add_string).c_str())->Clone();
      TH1D *h3 = (TH1D*)f.Get(("zmm_"+cats[cati]+"/ZLL"+MC_add_string).c_str())->Clone();
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
    
    //add end histogram with content =1
    TH1D *h_last = new TH1D("h_last","h_last",1,0,10000);
    h_last->SetBinContent(1,1);
    data_hist_vector.push_back(h_last);
    mc_hist_vector.push_back(h_last);
    
    TH2D *h_data = new TH2D("zptmass_histo","zptmass_histo",n_ybins,y_bins,n_zbins,z_bins);
    TH2D *h_mc = new TH2D("zptmass_histo_mc","zptmass_histo_mc",n_ybins,y_bins,n_zbins,z_bins);
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
          if(y_bin == h_data->GetNbinsY() || x_bin==1 || x_bin==h_data->GetNbinsX()){ content = 1; error = 0;}
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
          if(y_bin == h_mc->GetNbinsY() || x_bin==1 || x_bin==h_mc->GetNbinsX()){ content = 1; error = 0;}
          h_mc->SetBinContent(x_bin,y_bin,content);
          h_mc->SetBinError(x_bin,y_bin,error);
      }
      
    }
    
    if(cats[cati] == "inclusive"){
      double data_total=0;
      double mc_total=0;
      for(unsigned i=1; i<=(unsigned)h_data->GetNbinsX();++i){
        for(unsigned j=1; j<=(unsigned)h_data->GetNbinsY();++j){
          if(!(i == 1 || i== h_data->GetNbinsX() || j==h_data->GetNbinsY())){
              double data_content=h_data->GetBinContent(i,j);
              data_total+=data_content;
              double mc_content=h_mc->GetBinContent(i,j);
              mc_total+=mc_content;        
          }
        }
      }
      for(unsigned i=1; i<=(unsigned)h_data->GetNbinsX();++i){
        for(unsigned j=1; j<=(unsigned)h_data->GetNbinsY();++j){
          if(!(i == 1 || i== h_data->GetNbinsX() || j==h_data->GetNbinsY())){
              double data_content=h_data->GetBinContent(i,j);
              data_total+=data_content;
              h_data->SetBinContent(i,j,data_content/data_total);
              double mc_content=h_mc->GetBinContent(i,j);
              h_mc->SetBinContent(i,j,mc_content/mc_total);
          }
        }
      }
      h_data->Divide(h_mc);  
      h_2dweights = (TH2D*)h_data->Clone();
      continue;    
    }
    // Set 3D hist bins content and errors
    for(unsigned i=0; i<(unsigned)h_data->GetNbinsX(); ++i){
        for(unsigned j=0; j<(unsigned)h_data->GetNbinsY(); ++j){
          double content = h_data->GetBinContent(i+1,j+1);
          double error = h_data->GetBinError(i+1,j+1);
          h3_data->SetBinContent(cati+1,i+1,j+1,content);
          h3_data->SetBinError(cati+1,i+1,j+1,error);
        }
    }
    for(unsigned i=0; i<(unsigned)h_mc->GetNbinsX(); ++i){
        for(unsigned j=0; j<(unsigned)h_mc->GetNbinsY(); ++j){
          double content = h_mc->GetBinContent(i+1,j+1);
          double error = h_mc->GetBinError(i+1,j+1);
          h3_mc->SetBinContent(cati+1,i+1,j+1,content);
          h3_mc->SetBinError(cati+1,i+1,j+1,error);
        }
    }
    
    for(unsigned i=0; i<(unsigned)h_data->GetNbinsX(); ++i){
        for(unsigned j=0; j<(unsigned)h_data->GetNbinsY(); ++j){
          double content = h_data->GetBinContent(i+1,j+1);
          double error = h_data->GetBinError(i+1,j+1);
          h3_data_normxbins->SetBinContent(cati+1,i+1,j+1,content);
          h3_data_normxbins->SetBinError(cati+1,i+1,j+1,error);
        }
    }
    for(unsigned i=0; i<(unsigned)h_mc->GetNbinsX(); ++i){
        for(unsigned j=0; j<(unsigned)h_mc->GetNbinsY(); ++j){
          double content = h_mc->GetBinContent(i+1,j+1);
          double error = h_mc->GetBinError(i+1,j+1);
          h3_mc_normxbins->SetBinContent(cati+1,i+1,j+1,content);
          h3_mc_normxbins->SetBinError(cati+1,i+1,j+1,error);
        }
    }
}

//Normalize histograms - *_normxbins histogram is normalized seperatly in jet bins so that re-weighting leaved n_jets distribution un-changed

double h3_data_total_0jet = 0;
double h3_data_total_1jet = 0;
double h3_data_total_2jet = 0;
for (unsigned i=0; i<(unsigned)h3_data->GetNbinsX(); ++i){
  for (unsigned j=0; j<(unsigned)h3_data->GetNbinsX(); ++j){
    for (unsigned k=0; k<(unsigned)h3_data->GetNbinsZ(); ++k){
       if(!(j==0 || j==h3_data->GetNbinsY()-1 ||k==h3_data->GetNbinsZ()-1)){ 
         double content = h3_data->GetBinContent(i+1,j+1,k+1);
         if(i==0) h3_data_total_0jet+=content;   
         if(i==1) h3_data_total_1jet+=content;
         if(i==2) h3_data_total_2jet+=content;
       }
    }
  }
}
double h3_data_total = h3_data_total_0jet + h3_data_total_1jet + h3_data_total_2jet;

for (unsigned i=0; i<(unsigned)h3_data->GetNbinsX(); ++i){
  for (unsigned j=0; j<(unsigned)h3_data->GetNbinsX(); ++j){
    for (unsigned k=0; k<(unsigned)h3_data->GetNbinsZ(); ++k){
       double content = h3_data->GetBinContent(i+1,j+1,k+1);
       double error = h3_data->GetBinError(i+1,j+1,k+1);
       if(!(j==0 || j==h3_data->GetNbinsY()-1 ||k==h3_data->GetNbinsZ()-1)){
         h3_data->SetBinContent(i+1,j+1,k+1,content/h3_data_total);
         h3_data->SetBinError(i+1,j+1,k+1,error/h3_data_total);
         if(i==0){ h3_data_normxbins->SetBinContent(i+1,j+1,k+1,content/h3_data_total_0jet); h3_data_normxbins->SetBinError(i+1,j+1,k+1,error/h3_data_total_0jet); }
         if(i==1){ h3_data_normxbins->SetBinContent(i+1,j+1,k+1,content/h3_data_total_1jet); h3_data_normxbins->SetBinError(i+1,j+1,k+1,error/h3_data_total_1jet); }
         if(i==2){ h3_data_normxbins->SetBinContent(i+1,j+1,k+1,content/h3_data_total_2jet); h3_data_normxbins->SetBinError(i+1,j+1,k+1,error/h3_data_total_2jet); }
       }
    }
  }
}

double h3_mc_total_0jet = 0;
double h3_mc_total_1jet = 0;
double h3_mc_total_2jet = 0;
for (unsigned i=0; i<(unsigned)h3_mc->GetNbinsX(); ++i){
  for (unsigned j=0; j<(unsigned)h3_mc->GetNbinsX(); ++j){
    for (unsigned k=0; k<(unsigned)h3_mc->GetNbinsZ(); ++k){
       if(!(j==0 || j==h3_mc->GetNbinsY()-1 ||k==h3_mc->GetNbinsZ()-1)){
         double content = h3_mc->GetBinContent(i+1,j+1,k+1);
         if(i==0) h3_mc_total_0jet+=content;   
         if(i==1) h3_mc_total_1jet+=content;
         if(i==2) h3_mc_total_2jet+=content;
       }
    }
  }
}
double h3_mc_total = h3_mc_total_0jet + h3_mc_total_1jet + h3_mc_total_2jet;

for (unsigned i=0; i<(unsigned)h3_mc->GetNbinsX(); ++i){
  for (unsigned j=0; j<(unsigned)h3_mc->GetNbinsX(); ++j){
    for (unsigned k=0; k<(unsigned)h3_mc->GetNbinsZ(); ++k){
       double content = h3_mc->GetBinContent(i+1,j+1,k+1);
       double error = h3_mc->GetBinError(i+1,j+1,k+1);
       if(!(j==0 || j==h3_mc->GetNbinsY()-1 ||k==h3_mc->GetNbinsZ()-1)){
         h3_mc->SetBinContent(i+1,j+1,k+1,content/h3_mc_total);
         h3_mc->SetBinError(i+1,j+1,k+1,error/h3_mc_total);
         if(i==0){ h3_mc_normxbins->SetBinContent(i+1,j+1,k+1,content/h3_mc_total_0jet); h3_mc_normxbins->SetBinError(i+1,j+1,k+1,error/h3_mc_total_0jet); }
         if(i==1){ h3_mc_normxbins->SetBinContent(i+1,j+1,k+1,content/h3_mc_total_1jet); h3_mc_normxbins->SetBinError(i+1,j+1,k+1,error/h3_mc_total_1jet); }
         if(i==2){ h3_mc_normxbins->SetBinContent(i+1,j+1,k+1,content/h3_mc_total_2jet); h3_mc_normxbins->SetBinError(i+1,j+1,k+1,error/h3_mc_total_2jet); }
       }
    }
  }
}

// Divide to get weights and save to file
h3_data->Divide(h3_mc);
h3_data_normxbins->Divide(h3_mc_normxbins);

TFile *fout = new TFile(outfile.c_str(),"RECREATE");
fout->cd();

h_2dweights->Write();
h3_data->Write();      
h3_data_normxbins->Write();

for (unsigned i=0; i<(unsigned)h3_data->GetNbinsX(); ++i){
  for (unsigned j=0; j<(unsigned)h3_data->GetNbinsY(); ++j){
    for (unsigned k=0; k<(unsigned)h3_data->GetNbinsZ(); ++k){
      double n_jets = h3_data->GetXaxis()->GetBinLowEdge(i+1);    
      double mass   = h3_data->GetYaxis()->GetBinLowEdge(j+1);
      double pt     = h3_data->GetZaxis()->GetBinLowEdge(k+1);
      double weight = h3_data->GetBinContent(i+1,j+1,k+1);
      double error = h3_data->GetBinError(i+1,j+1,k+1);      
      std::cout << "n_jets = " << n_jets << ", mass = " << mass << ", pt = " << pt << ", weight = " << weight << ", error = " << error*100/weight << "%" << std::endl;
    }
  }
}
std::cout << "-------------------------" << std::endl;

//for (unsigned i=0; i<(unsigned)h3_data_normxbins->GetNbinsX(); ++i){
//  for (unsigned j=0; j<(unsigned)h3_data_normxbins->GetNbinsY(); ++j){
//    for (unsigned k=0; k<(unsigned)h3_data_normxbins->GetNbinsZ(); ++k){
//      double n_jets = h3_data_normxbins->GetXaxis()->GetBinLowEdge(i+1);    
//      double mass   = h3_data_normxbins->GetYaxis()->GetBinLowEdge(j+1);
//      double pt     = h3_data_normxbins->GetZaxis()->GetBinLowEdge(k+1);
//      double weight = h3_data_normxbins->GetBinContent(i+1,j+1,k+1);
//      std::cout << "n_jets = " << n_jets << ", mass = " << mass << ", pt = " << pt << ", weight = " << weight << std::endl;
//    }
//  }
//}

}


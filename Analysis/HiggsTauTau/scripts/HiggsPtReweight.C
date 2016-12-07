double StandDev(std::vector<double> a) {
  unsigned N = a.size();
  double mean=0;
  for(unsigned i=0; i<N; ++i){
    mean+=a[i];
  }
  mean=mean/N;
  double sum=0; 
  for(unsigned i=0; i<N; ++i){
    sum+=pow(a[i]-mean,2);  
  }
  sum = sum/N;
  return sqrt(sum);
}

void SetStyle(){
  TStyle *myStyle  = new TStyle("MyStyle","");
  myStyle->SetCanvasBorderMode(0);
  myStyle->SetPadBorderMode(0);
  myStyle->SetCanvasColor(0);
  myStyle->SetStatColor(0);
  myStyle->SetLegendBorderSize(0);
  myStyle->SetCanvasDefX(900);
  myStyle->SetCanvasDefY(900);
  myStyle->SetCanvasDefH(850);
  myStyle->SetCanvasDefW(850);
  myStyle->SetPadBottomMargin(0.1);
  myStyle->SetPadTopMargin(0.1);
  myStyle->SetPadLeftMargin(0.125);
  myStyle->SetPadRightMargin(0.125);
  myStyle->SetPadTickX(1);
  myStyle->SetPadTickY(1);
  myStyle->SetFrameBorderMode(0);
  myStyle->SetLineWidth(1);
  myStyle->SetOptStat(1110);
  myStyle->SetStatX(0.9);
  myStyle->SetStatY(0.93);
  myStyle->SetStatW(0.22);
  myStyle->SetStatH(0.15);
  myStyle->SetTitleYOffset(1.5);
  myStyle->SetTitleFont(62);
  myStyle->SetTitleFont(62,"Y");
  myStyle->SetLabelFont(62);
  myStyle->SetLabelFont(62,"Y");
  myStyle->SetOptTitle(0);
  myStyle->SetHistLineStyle(0);
  myStyle->SetHistLineWidth(3);
  myStyle->SetMarkerSize(0.9);
  myStyle->SetMarkerStyle(20);
  gROOT->SetStyle("MyStyle");
  gROOT->UseCurrentStyle();
  gROOT->ForceStyle();    
}

void DrawHist(std::vector<TH1D*> hists, std::vector<std::string> legend_entries, std::string out_name, bool ratio, bool Normalize, std::string title, bool intLabels, bool log, double ratio_range, bool bottomLeg){
  bool doLegend = legend_entries.size() > 0;
  SetStyle();
  TGaxis::SetMaxDigits(3);
  THStack *hs = new THStack("hs","");
  TCanvas *c1 = new TCanvas();
  if(log) c1->SetLogy();
  double ymin = 0.8 - 0.05*hists.size();
  TLegend *leg = new TLegend(0.65,ymin,0.8,0.8);
  if(bottomLeg){
    double ymax = 0.1 + 0.05*hists.size();
    leg = new TLegend(0.60,0.1,0.8,ymax);    
  }
  leg->SetTextSize(0.03);
  leg->SetFillStyle(0);
  int col_num=0;
  for(unsigned i=0; i<hists.size(); ++i){
    col_num ++;
    if(col_num==7) col_num++;
    if(col_num==10) col_num++;
    
    hists[i]->SetLineColor(col_num);
    hists[i]->SetMarkerColor(col_num);
    for(unsigned j=0; j<(unsigned)hists[i]->GetNbinsX()+1; ++j){
      hists[i]->SetBinError(j,hists[i]->GetBinContent(j)*0.00001);    
    } //get rid of vartical xerror bars by setting them to small value - purley for aesthetic reasons
    TH1D *htemp = (TH1D*)hists[i]->Clone();
    if(Normalize){
      double norm = 1./hists[0]->Integral();
      htemp->Scale(norm);
      hists[i]->GetYaxis()->SetTitle("Normalized Entries");
    }
    hs->Add(htemp);
    std::string leg_string = "";
    if(i<legend_entries.size()) leg_string = legend_entries[i];
    leg->AddEntry(hists[i],leg_string.c_str(),"lp");
  }
  
  if(!ratio){
    hs->Draw("nostack p");
    hs->GetXaxis()->SetTitle(hists[0]->GetXaxis()->GetTitle());
    hs->GetYaxis()->SetTitle(hists[0]->GetYaxis()->GetTitle());
    if(intLabels){
     TGaxis *axis = (TGaxis*)hs->GetXaxis();
     axis->SetOption("I");
    }
    hs->Draw("nostack p");
    c1->Update();
    if(doLegend) leg->Draw();
  } else{
   TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
   pad1->SetBottomMargin(0);
   pad1->SetFillStyle(4000);
   pad1->SetFrameFillStyle(4000);       
   pad1->Draw();             
   pad1->cd();
   hs->Draw("nostack p");
   float scale1 = pad1->GetAbsHNDC();
   hs->GetYaxis()->SetLabelSize(hs->GetYaxis()->GetLabelSize()/scale1);
   hs->GetYaxis()->SetTitleSize(hs->GetYaxis()->GetTitleSize()/scale1);
   hs->GetYaxis()->SetTitle(hists[0]->GetYaxis()->GetTitle());
   hs->GetYaxis()->SetTitleOffset(1.1);
   hs->SetMinimum(hs->GetMaximum()/100000);
   hs->Draw("nostack p");
   c1->Update();
   if(doLegend) leg->Draw();
    
   THStack *hs_ratio = new THStack("hs_ratio","");
   TH1D *h_denum = (TH1D*)hists[0]->Clone();
   for(unsigned i=0; i<hists.size(); ++i){
     TH1D *htemp = (TH1D*)hists[i]->Clone();
     htemp->Divide(h_denum);
     hs_ratio->Add((TH1D*)htemp->Clone());
   }
   c1->cd(); 
   TPad *pad2 = new TPad("pad2", "pad2", 0, 0.02, 1, 0.3);
   pad2->SetFillStyle(4000);
   pad2->SetFrameFillStyle(4000); 
   pad2->SetTopMargin(0);
   pad2->SetBottomMargin(0.2);
   pad2->Draw();
   pad2->cd();
   hs_ratio->Draw("nostack p"); 
   float scale = pad2->GetAbsHNDC();
   pad2->SetBottomMargin(0.3); 
   hs_ratio->GetXaxis()->SetTitle(hists[0]->GetXaxis()->GetTitle());
   if(intLabels){
     hs_ratio->GetXaxis()->SetNdivisions(hists[0]->GetNbinsX()+1,0,0);
   }
   hs_ratio->GetYaxis()->SetTitle("Ratio");
   hs_ratio->GetYaxis()->SetTitleOffset(0.4);
   hs_ratio->GetYaxis()->CenterTitle();
   hs_ratio->GetXaxis()->SetLabelSize(hs_ratio->GetXaxis()->GetLabelSize()/scale);
   hs_ratio->GetYaxis()->SetLabelSize(hs_ratio->GetYaxis()->GetLabelSize()/scale);
   hs_ratio->GetXaxis()->SetTitleSize(hs_ratio->GetXaxis()->GetTitleSize()/scale);
   hs_ratio->GetYaxis()->SetTitleSize(hs_ratio->GetYaxis()->GetTitleSize()/scale);
   double ratio_max = 1+ratio_range;
   double ratio_min = 1-ratio_range;
   hs_ratio->SetMinimum(ratio_min);
   hs_ratio->SetMaximum(ratio_max);
   hs_ratio->GetYaxis()->SetNdivisions(3,5,0);
   hs_ratio->Draw("nostack p"); 
   pad2->Update();
   c1->Update();
   c1->cd();  
  }
  
  TLatex *tex = new TLatex(0.7,0.95,(title).c_str());
  tex->SetNDC();
  tex->SetTextFont(44);
  tex->SetTextSize(23);
  tex->SetLineWidth(2);
  tex->Draw();
  
  c1->Print((out_name+".pdf").c_str());
  
}

int HiggsPtReweight(){

std::vector<double> bins;

for(unsigned i=0;i<=100;i+=2){ bins.push_back(i);   }
for(unsigned i=104;i<=200;i+=4){ bins.push_back(i); }
for(unsigned i=210;i<=300;i+=10){ bins.push_back(i);}
for(unsigned i=320;i<=500;i+=20){ bins.push_back(i);}
unsigned bin_num = bins.size()-1;
double* bins_array = &bins[0];
    
TFile *fout1 = new TFile("HqT_weight_pTH_summer16_80X_AllSamples.root","RECREATE");    
TFile *fout2 = new TFile("HqT_weight_pTH_summer16_80X.root","RECREATE"); 

TH1D *htemp;
TH1D *hout = new TH1D("hout","",bin_num,bins_array);
hout->GetXaxis()->SetTitle("Higgs p_{T} [GeV]");
hout->GetYaxis()->SetTitle("Weight");

std::vector<TH1D*> scale_hists;
std::vector<std::string> legend_entries;

TH1D *h1 = new TH1D("h1","",250,0,500);
std::string file_name = "HqToutput/HqtSpectrum.out";    
ifstream infile (file_name);
if(infile.is_open()){
  double Pt; double bin; double dummy;
  for(unsigned i=0; i<10; ++i) infile.ignore(10000,'\n');
  while ( infile >> Pt >> dummy >> dummy >> dummy >> dummy >> bin ){
    //if(bin < 0) bin=0;
    h1->SetBinContent(h1->FindBin(Pt),bin);
  }
  infile.close();
}     
h1->Scale(1/h1->Integral());


fout1->cd();

TH1D *h2 = (TH1D*)h1->Rebin(bin_num,"h2",bins_array); 
h2->Write("HqT2p0_PtSpectrum");

//TH1D *hscale_temp = new TH1D("hscale_temp","",250,0,500);
//for(unsigned i=0; i<14; ++i){
//  //if(!(i==0 || i==3 || i==6)) continue;
//  std::string file_name = Form("HqToutputOLD/Scale/HqtSpectrum_Scale_%u.out",i);    
//  ifstream infile (file_name);
//  if(infile.is_open()){
//    double Pt; double bin; double dummy;
//    for(unsigned j=0; j<10; ++j) infile.ignore(10000,'\n');
//    while ( infile >> Pt >> dummy >> dummy >> dummy >> dummy >> bin ){
//      //if(bin < 0) bin=0;  
//      hscale_temp->SetBinContent(hscale_temp->FindBin(Pt),bin);
//    }
//    infile.close();
//  }     
//  hscale_temp->Scale(1/hscale_temp->Integral());
//  for(unsigned i =1; i<= hscale_temp->GetNbinsX();++i){
//    hscale_temp->SetBinError(i,0);
//  }
//  scale_hists.push_back((TH1D*)hscale_temp->Clone());
//  if (i == 8 ){
//    legend_entries.push_back("nominal");  
//    std::reverse( scale_hists.begin(), scale_hists.end() );
//    std::reverse( legend_entries.begin(), legend_entries.end() );  
//  }
//  else {
//    legend_entries.push_back(Form("%u",i));
//  }
//}
//DrawHist(scale_hists, legend_entries, "scale_reweight_error_all", true, false, "", false, false, 0.3, false);

std::vector<double> bins2;
for(double i=0; i<=300; i+=10) bins2.push_back(i);
double* bins_array2 = &bins2[0];
int  binnum2 = bins2.size()-1;

TH1D *hscale_temp = new TH1D("hscale_temp","",150,0,300);
//TH1D *hblah = new TH1D("hblah","",binnum2,bins_array2);
for(unsigned i=0; i<7; ++i){
  if(!(i==0 || i==3 || i==6)) continue;
  std::string file_name = Form("HqToutput/ScaleNew/HqtSpectrum_ScaleNew_%u.out",i);    
  ifstream infile (file_name);
  if(infile.is_open()){
    double Pt; double bin; double dummy;
    for(unsigned j=0; j<10; ++j) infile.ignore(10000,'\n');
    while ( infile >> Pt >> dummy >> dummy >> dummy >> dummy >> bin ){
      //if(bin < 0) bin=0;  
      hscale_temp->SetBinContent(hscale_temp->FindBin(Pt),bin);
    }
    infile.close();
  }     
  //hscale_temp->Scale(1/hscale_temp->Integral());
  for(unsigned i =1; i<= hscale_temp->GetNbinsX();++i){
    hscale_temp->SetBinError(i,0);
  }
  //scale_hists.push_back((TH1D*)hscale_temp->Clone());
  //if(i==0) legend_entries.push_back("scale down");
  //if(i==6) legend_entries.push_back("scale up");
  if(i==3) {
    TH1D *hblah = (TH1D*)hscale_temp->Rebin(binnum2,"hblah",bins_array2);
    std::cout << hblah->Integral() << std::endl;
    scale_hists.push_back((TH1D*)hblah->Clone());  
    legend_entries.push_back("HqT2.0");  
    std::reverse( scale_hists.begin(), scale_hists.end() );
    std::reverse( legend_entries.begin(), legend_entries.end() );    
  }
}
for(unsigned i=0; i< scale_hists.size(); ++i){
  //TH1D *hblah = new TH1D();
  //scale_hists[i]->Rebin(binnum2,"hblah",bins_array2);
  //scale_hists[i] = (TH1D*)hblah->Clone();
    scale_hists[i]->Scale(1/scale_hists[i]->Integral());
}

TH1D *h_hres = new TH1D("h_hres","",30,0,300);

std::string file_name_hres = "HResoutput/HResSpectrum.out.top";    
ifstream infile2 (file_name_hres);
if(infile2.is_open()){
  double Pt; double bin; double error;
  for(unsigned i=0; i<707; ++i) infile2.ignore(10000,'\n');
  while ( infile2 >> Pt >> bin >> error ){
    //if(bin < 0) bin=0;
    std::cout << "Pt = " << Pt << " val = " << bin << std::endl;
    if(Pt<=300){
      h_hres->SetBinContent(h_hres->FindBin(Pt),bin);
      h_hres->SetBinError(h_hres->FindBin(Pt),error);
    }
  }
  infile2.close();
}     
h_hres->Scale(1/(h_hres->Integral()));

TFile *fin = new TFile("/vols/cms/dw515/Offline/output/GenComps/GluGluHToTauTau_M-125_mt_2016.root");
TTree *gen_tree = (TTree*)fin->Get("gen_ntuple");
  
htemp = new TH1D("htemp","",30,0,300);
gen_tree->Draw("HiggsPt>>htemp","wt","goff");
//htemp->Rebin(5);
htemp->Scale(1/(htemp->Integral()));

//std::string file_prefix = "/vols/cms/dw515/Offline/output/GenComps/GluGluHToTauTau_M-125_mt_2016.root
scale_hists.push_back((TH1D*)htemp->Clone());
legend_entries.push_back("PowHeg");
std::reverse( scale_hists.begin(), scale_hists.end() );
std::reverse( legend_entries.begin(), legend_entries.end() ); 
scale_hists.push_back((TH1D*)h_hres);
legend_entries.push_back("HRes2.3");
DrawHist(scale_hists, legend_entries, "scale_reweight_error", true, false, "", false, false, 0.3, false);

//std::vector<TH1D*> pdf_hists;
//std::vector<std::string> pdflegend_entries;
//TH1D *hpdf_temp = new TH1D("hpdf_temp","",150,0,300);
//for(unsigned i=0; i<=100; ++i){
//  std::string file_name = Form("HqToutput/PDF/HqtSpectrum_PDF_%u.out",i);    
//  ifstream infile (file_name);
//  if(infile.is_open()){
//  std::string s;
//  for (int i=0; i<7;i++) std::getline(infile, s);
//  if(s.find("62.5")==std::string::npos) continue;
//  //std::cout << s << std::endl;
//    double Pt; double bin; double dummy;
//    for(unsigned j=0; j<3; ++j) infile.ignore(10000,'\n');
//    while ( infile >> Pt >> dummy >> dummy >> dummy >> dummy >> bin ){
//      //if(bin < 0) bin=0;
//      hpdf_temp->SetBinContent(hpdf_temp->FindBin(Pt),bin);
//      //std::cout << bin << std::endl;
//    }
//    infile.close();
//  }     
//  hpdf_temp->Scale(1/hpdf_temp->Integral(3,hpdf_temp->GetNbinsX()));
//  pdf_hists.push_back((TH1D*)hpdf_temp->Clone());
//  pdflegend_entries.push_back(Form("%u",i+1));
//  //hpdf_temp->Draw();
//  //htemp->Write("HqT2p0_PtSpectrum");
//}
//
//TH1D *h_pdfup = new TH1D("h_pdfup","", 150,0,300);
//TH1D *h_pdfdown = new TH1D("h_pdfdown","", 150,0,300);
//std::vector<TH1D*> pdf_var_hists;
//for(unsigned i=3; i<=pdf_hists[0]->GetNbinsX(); ++i){
//  std::vector<double> a;
//  for(unsigned j=0; j<pdf_hists.size(); ++j){
//    a.push_back(pdf_hists[j]->GetBinContent(i));    
//  }
//  double up = pdf_hists[0]->GetBinContent(i) + StandDev(a);
//  double down = pdf_hists[0]->GetBinContent(i) - StandDev(a);
//  h_pdfup->SetBinContent(i,up);
//  h_pdfdown->SetBinContent(i,down);
//}
//pdf_hists[0]->GetXaxis()->SetTitle("Higgs P_{T} [GeV]");
//pdf_hists[0]->GetYaxis()->SetTitle("Normalized Entries");
//pdf_var_hists.push_back(pdf_hists[0]);
//pdf_var_hists.push_back(h_pdfup);
//pdf_var_hists.push_back(h_pdfdown);
//pdflegend_entries={"nominal","pdf up","pdf down"};
//DrawHist(pdf_var_hists, pdflegend_entries, "pdf_reweight_error", true, false, "", false, false, 0.015, false);

//std::string file_prefix = "/vols/cms/dw515/Offline/output/GenComps/";    
//std::map<std::string, std::string>  filenames_;
//filenames_["aMC"]             = file_prefix+"GluGluHToTauTau_amcatnlo_M-125_mt_2016.root"; 
//filenames_["Powheg"]              = file_prefix+"GluGluHToTauTau_M-125_mt_2016.root";
//filenames_["PythiaFragmentDown"]  = file_prefix+"GluGluHToTauTau_PythiaFragment_Down_M-125_mt_2016.root";
//filenames_["PythiaFragmentUp"]    = file_prefix+"GluGluHToTauTau_PythiaFragment_Up_M-125_mt_2016.root";
//filenames_["Herwig"]            = file_prefix+"GluGluHToTauTau_herwigpp_M-125_mt_2016.root";    
//
//typedef std::map<std::string, std::string>::iterator it_type;
//for(it_type iterator = filenames_.begin(); iterator != filenames_.end(); iterator++){
//  TFile *fin = new TFile(iterator->second.c_str());
//  TTree *gen_tree = (TTree*)fin->Get("gen_ntuple");
//  
//  htemp = new TH1D("htemp","",bin_num,bins_array);
//  htemp->GetXaxis()->SetTitle("Higgs p_{T} [GeV]");
//  std::string wt_string = "wt";
//  gen_tree->Draw("HiggsPt>>htemp",wt_string.c_str(),"goff");
//  htemp->Scale(1/htemp->Integral());
//  hout->Divide(h2,htemp);
//  for(unsigned i =1; i<= hout->GetNbinsX();++i){
//    hout->SetBinError(i,0);
//  }
//  fout1->cd();
//  hout->Write((iterator->first+"_Nominal").c_str()); 
//  
//  if(iterator->first == "Powheg"){
//    fout2->cd();
//    hout->Write("Nominal");
//    fout1->cd();
//    //TH1D *h2 = (TH1D*)h1->Rebin(bin_num,"h2",bins_array);
//    hout->Divide((TH1D*)scale_hists[1]->Rebin(bin_num,"",bins_array),htemp);
//    for(unsigned i =1; i<= hout->GetNbinsX();++i){
//      hout->SetBinError(i,0);
//      if(hout->GetBinContent(i)<0){
//        hout->SetBinContent(i,hout->GetBinContent(i+1));
//      }
//    }
//    hout->Write("Powheg_ScaleDown");
//    fout2->cd();
//    hout->Write("Down");
//    fout1->cd();
//    hout->Divide((TH1D*)scale_hists[2]->Rebin(bin_num,"",bins_array),htemp);
//    for(unsigned i =1; i<= hout->GetNbinsX();++i){
//      hout->SetBinError(i,0);
//      if(hout->GetBinContent(i)<0){
//        hout->SetBinContent(i,hout->GetBinContent(i+1));
//      }
//    }
//    hout->Write("Powheg_ScaleUp");
//    fout2->cd();
//    hout->Write("Up");
//    fout1->cd();
//    
//    for(unsigned i=0; i<9; ++i){
//      if(!(i==4 || i==8)) continue;
//          //0=low, 6= scale high
//      std::string wt_name = Form("wt*scale_variation_wts[%u]",i);
//      delete htemp;
//      htemp = new TH1D("htemp","",bin_num,bins_array);
//      gen_tree->Draw("HiggsPt>>htemp",wt_name.c_str(),"goff");
//      std::cout << htemp->Integral() << std::endl;
//      htemp->Scale(1/htemp->Integral());
//      hout->Divide(h2,htemp);
//      std::string hist_name;
//      if(i==4) hist_name = iterator->first+"_Nominal_ScaleHigh";
//      if(i==8) hist_name = iterator->first+"_Nominal_ScaleLow";
//      fout1->cd();
//      for(unsigned i =1; i<= hout->GetNbinsX();++i){
//        hout->SetBinError(i,0);
//      }
//      hout->Write(hist_name.c_str());
//    }
//   // 
//   // for(unsigned i=0; i<100; ++i){
//   //   std::string wt_name = Form("wt*NNPDF_wts[%u]",i);
//   //   gen_tree->Draw("HiggsPt>>htemp",wt_name.c_str(),"goff");
//   //   htemp->Scale(1/htemp->Integral(0,htemp->GetNbinsX()));
//   //   hout->Divide(h2,htemp);
//   //   std::string hist_name = iterator->first+Form("_NNPDF_%u",i);
//   //   fout1->cd();
//   //   hout->Write(hist_name.c_str());
//   //   htemp->Write((hist_name+"_PtSpectrum").c_str());
//   // }
//  }
//
//}

fout1->Close();    
fout2->Close(); 
return 0;
}

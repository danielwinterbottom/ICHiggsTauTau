{
std::string sample = "VBF";
std::string channel = "mt";
std::string filename = "/vols/cms/dw515/Offline/CMSSW_8_0_9/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/output/Aug11_PreSel_sv_v6/"+sample+"HToTauTau_M-125_"+channel+"_2016.root";
TFile *f0 = new TFile(filename.c_str());
TTree *t0 = (TTree*)f0->Get("gen_ntuple");
THStack *hs = new THStack("hs","");
THStack *hs2 = new THStack("hs2","");

//std::string cut_string = "*(mt_1<50 && n_jets>=2 && mjj>200 && jdeta>2 && pt_1>20 && pt_2>20)";
std::string cut_string = "";
double NSig =0;
double NSigMin=1000000000000000;
double NSigMax=0;
unsigned maxindex;
unsigned minindex;

for(unsigned i=0; i<9; ++i){
  if(i==5 || i==7) continue;
  std::string wt_name = Form("scale_variation_wts[%u]",i);
  std::string cut_string="";
  //cut_string = "*(n_jets>=2 && jdeta>3.5 && mjj>500 && mt_1<50 && n_bjets==0 && mva_olddm_medium_2>0.5)";
  //cut_string = "*(n_jets==0 && mva_olddm_medium_2>0.5 && mt_1<40)";
  wt_name+=cut_string;
  t0->Draw("m_vis>>h",wt_name.c_str(),"goff");
  t0->Draw(Form("m_vis>>h%i",i),wt_name.c_str(),"goff");
  if(i!=0){
    double temp=h->Integral(0,h->GetNbinsX()+1);
    if(temp > NSigMax){ NSigMax = temp; maxindex=i;} 
    if(temp < NSigMin){ NSigMin = temp; minindex=i;}
  } else NSig = h->Integral(0,h->GetNbinsX()+1);
}

std::cout << NSig << std::endl;
std::cout << NSigMin << "  " << minindex << std::endl;
std::cout << NSigMax << "  " << maxindex << std::endl;
double uncert = (NSigMax-NSigMin)/(2*NSig);
std::cout << uncert << std::endl;

hs->Add(h0);
h0->SetLineColor(1);
hs->Add(h1);
h1->SetLineColor(2);
hs->Add(h2);
h2->SetLineColor(4);
hs->Add(h3);
h3->SetLineColor(5);
hs->Add(h4);
h4->SetLineColor(6);
hs->Add(h6);
h6->SetLineColor(7);
hs->Add(h8);
h8->SetLineColor(8);

TCanvas c1;

hs->Draw("nostack");
hs->GetXaxis()->SetTitle("m_{vis} [GeV]");
hs->GetYaxis()->SetTitle("Entries");
hs->GetXaxis()->SetRangeUser(0,300);

TLegend *leg = new TLegend(0.6,0.62,0.875,0.9);
leg->SetTextSize(0.03);
leg->AddEntry("h0","#mu_{R}=1, #mu_{F}=1","le");
leg->AddEntry("h1","#mu_{R}=1, #mu_{F}=2","le");
leg->AddEntry("h2","#mu_{R}=1, #mu_{F}=0.5","le");
leg->AddEntry("h3","#mu_{R}=2, #mu_{F}=1","le");
leg->AddEntry("h4","#mu_{R}=2, #mu_{F}=2","le");
leg->AddEntry("h6","#mu_{R}=0.5, #mu_{F}=1","le");
leg->AddEntry("h8","#mu_{R}=0.5, #mu_{F}=0.5","le");
leg->Draw();

std::string uncert_string = Form("%0.1f %%",uncert*100);
TLatex *   tex = new TLatex(0.45,0.3,(channel+" inclusive:"+" uncertainty = "+uncert_string).c_str());
tex->SetNDC();
tex->SetTextFont(44);
tex->SetTextSize(23);
tex->SetLineWidth(2);
tex->Draw();

std::string out_name = "scale_variation_"+sample+"_"+channel+".pdf";
c1.Print(out_name.c_str());

double NSigMin2=1000000000000000;
double NSigMax2=0;
double NSig2=0;

for(unsigned i=0; i<2; ++i){
  std::string wt_name = Form("alpha_s_wts[%u]",i);
  wt_name+=cut_string;
  t0->Draw("m_vis>>h1",wt_name.c_str(),"goff");
  t0->Draw(Form("m_vis>>h1_%i",i+1),wt_name.c_str(),"goff");
  double temp=h1->Integral(0,h1->GetNbinsX()+1);
  if(temp > NSigMax2){ NSigMax2 = temp;} 
  if(temp < NSigMin2){ NSigMin2 = temp;}
}

t0->Draw("m_vis>>h1_0",("1"+cut_string).c_str(),"goff");
NSig2 = h1_0->Integral(0,h1_0->GetNbinsX()+1);

std::cout << NSig2 << std::endl;
if(NSig2 > NSigMax2) NSigMax2 = NSig2;
if(NSig2 < NSigMin2) NSigMin2 = NSig2;
std::cout << NSigMin2 << std::endl;
std::cout << NSigMax2 << std::endl;
double uncert2 = (NSigMax2-NSigMin2)/(2*NSig2);
std::cout << uncert2 << std::endl;

hs2->Add(h1_0);
h1_0->SetLineColor(1);
hs2->Add(h1_1);
h1_1->SetLineColor(2);
hs2->Add(h1_2);
h1_2->SetLineColor(4);

hs2->Draw("nostack");

hs2->GetXaxis()->SetTitle("m_{vis} [GeV]");
hs2->GetYaxis()->SetTitle("Entries");
hs2->GetXaxis()->SetRangeUser(0,300);

TLegend *leg2 = new TLegend(0.62,0.75,0.875,0.9);
leg2->SetTextSize(0.03, le);
leg2->AddEntry("h1_0","#alpha_{s} = 1.8","le");
leg2->AddEntry("h1_1","#alpha_{s} = 1.7","le");
leg2->AddEntry("h1_2","#alpha_{s} = 1.9","le");

leg2->Draw();

std::string uncert_string2 = Form("%0.1f %%",uncert2*100);
TLatex *   tex2 = new TLatex(0.45,0.3,(channel+" inclusive:"+" uncertainty = "+uncert_string2).c_str());
tex2->SetNDC();
tex2->SetTextFont(44);
tex2->SetTextSize(23);
tex2->SetLineWidth(2);
tex2->Draw();

std::string out_name2 = "alpha_s_"+sample+"_"+channel+".pdf";
c1.Print(out_name2.c_str());


}

{
std::string filename = "";
TFile *f0 = new TFile(filename.c_str());
TTree *t0 = (TTree*)f0->Get("ntuple");
THStack *hs = new THStack("hs","");

double NSig =0;
double NSigMin=1000000000000000;
double NSigMax=0;
unsigned maxindex;
unsigned minindex;

for(unsigned i=0; i<9; ++i){
  if(i==5 || i==7) continue;
  std::string wt_name = Form("wt*scale_variation_wts[%u]",i);
  std::cut_string="";
  //cut_string = "*(n_jets>=2 && jdeta>2 && mjj>200 && mt_1<40 && n_bjets>=0 && mva_olddm_medium_2>0.5)";
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
h2->SetLineColor(3);
hs->Add(h3);
h3->SetLineColor(4);
hs->Add(h4);
h4->SetLineColor(5);
hs->Add(h6);
h6->SetLineColor(6);
hs->Add(h8);
h8->SetLineColor(7);

hs->Draw("nostack");
}

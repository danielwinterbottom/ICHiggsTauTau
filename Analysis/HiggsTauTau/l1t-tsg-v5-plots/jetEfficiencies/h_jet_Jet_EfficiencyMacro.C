{
TString foldername = "l1t-tsg-v5-plots/electronEfficiencies/";
TString histname = "h_e_IsoEG_Efficiency";
TString histnameTotal = "h_e_Total";

TFile *f1 = new TFile("output/Trigger_2016/EfficienciesV5/mtEfficienciesv5_3.root");


//output/Trigger_2016/EfficienciesV5/mtEfficienciesv5_2.root
TH1D* htemp;

TH1D *htotal = (TH1D*)f1->Get("EfficienciesGenMatch/TriggerEfficiencies1/"+histnameTotal);


TH1D *h1 = (TH1D*)f1->Get("EfficienciesGenMatch/TriggerEfficiencies1/"+histname);
TH1D *h2 = (TH1D*)f1->Get("EfficienciesGenMatch/TriggerEfficiencies2/"+histname);
TH1D *h3 = (TH1D*)f1->Get("EfficienciesGenMatch/TriggerEfficiencies3/"+histname);
TH1D *h4 = (TH1D*)f1->Get("EfficienciesGenMatch/TriggerEfficiencies4/"+histname);
int rebinN =2;
     //h1->Scale(2);
     //h2->Scale(2);
     //h3->Scale(2);
h1->Multiply(htotal);
h2->Multiply(htotal);
h3->Multiply(htotal);
htotal->Rebin(rebinN,"htotalnew");
h1->Rebin(rebinN);
h1->Divide(htotalnew);
h2->Rebin(rebinN);
h2->Divide(htotalnew);
h3->Rebin(rebinN);
h3->Divide(htotalnew);


TGaxis::SetMaxDigits(3);
gStyle->SetOptStat(0);
//h1->GetXaxis()->SetLimits(0,1);
//h2->GetXaxis()->SetLimits(0,1);
//h3->GetXaxis()->SetLimits(0,1);
h1->GetXaxis()->SetTitle("Offline electron p_{T} GeV");
//h1->GetYaxis()->SetTitle("Probability");
//h1->GetYaxis()->SetRangeUser(0, 1);
//h1->GetXaxis()->SetRangeUser(0, 100);

h1->SetFillStyle(0);
h1->SetMarkerStyle(33);
h1->SetMarkerSize(1.7);
ci = TColor::GetColor("#0000cc");
h1->SetLineColor(ci);
h1->SetLineWidth(2);
h1->SetMarkerColor(ci);
h1->GetYaxis()->SetTitleOffset(1.4);
//TF1 *fit1 = new TF1("fit1","[0]*TMath::Erf((x-[1])/[2]) + [3]*TMath::Erf((x-[4])/[5])",15,100);
//fit1->SetLineColor(ci);
//fit1->SetParameters(1,10,2,0.1,20,4);
//h1->Fit("fit1","","",15,100);

h2->SetFillStyle(0);
ci = TColor::GetColor("#cc0000");
h2->SetMarkerColor(ci);
h2->SetLineColor(ci);
h2->SetLineWidth(2);
h2->SetMarkerStyle(33);
h2->SetMarkerSize(1.7);
//TF1 *fit2 = new TF1("fit2","[0]*TMath::Erf((x-[1])/[2]) + [3]*TMath::Erf((x-[4])/[5])",15,100);
//fit2->SetLineColor(ci);:
//fit2->SetParameters(1,10,2,0.1,20,4);
//h2->Fit("fit2","","",15,100);
//fit2->Draw();

ci = TColor::GetColor("#00cc00");
h3->SetFillStyle(0);
h3->SetMarkerColor(ci);
h3->SetLineColor(ci);
h3->SetLineWidth(2);
h3->SetMarkerStyle(33);
h3->SetMarkerSize(1.7);
//TF1 *fit3 = new TF1("fit3","[0]*TMath::Erf((x-[1])/[2]) + [3]*TMath::Erf((x-[4])/[5]) + [6] +[7]*x",15,100);
//fit3->SetLineColor(ci);
//fit3->SetParameters(1,10,2,0.1,20,4,0,0);
//h3->Fit("fit3","","",15,100);

h4->SetFillStyle(0);
h4->SetMarkerColor(kViolet-1);
h4->SetLineColor(kViolet-1);
h4->SetLineWidth(2);
h4->SetMarkerStyle(33);
h4->SetMarkerSize(1.7);
TF1 *fit4 = new TF1("fit4","[0]*TMath::Erf((x-[1])/[2])",15,100);
fit4->SetLineColor(kViolet-1);
fit4->SetParameters(5,40,14);
//h4->Fit("fit4","","",40,80);

h1->Draw("P");
h2->Draw("SAMEP");
h3->Draw("SAMEP");
//h4->Draw("SAMEP");


int Ncount1 =0;
for(int i=1; i < h1->GetSize() -1; i++){
  if(h1->GetBinContent(i)>0) Ncount1++;
}
TGraph *gr1 = new TGraph(Ncount1);
gr1->SetLineColor(ci);
int j=0;
for(int i=0; i < h1->GetSize() -1; i++){
  if(h1->GetBinContent(i)>0){
    gr1->SetPoint(j,h1->GetBinCenter(i),h1->GetBinContent(i));
    j++;
  }
}

int Ncount2 =0;
for(int i=1; i < h2->GetSize() -1; i++){
  if(h2->GetBinContent(i)>0) Ncount2++;
}
TGraph *gr2 = new TGraph(Ncount2);
gr2->SetLineColor(ci);
j=0;
for(int i=0; i < h2->GetSize() -1; i++){
  if(h2->GetBinContent(i)>0){
    gr2->SetPoint(j,h2->GetBinCenter(i),h2->GetBinContent(i));
    j++;
  }
}

int Ncount3 =0;
for(int i=1; i < h3->GetSize() -1; i++){
  if(h3->GetBinContent(i)>0) Ncount3++;
}
TGraph *gr3 = new TGraph(Ncount3);
gr3->SetLineColor(ci);
j=0;
for(int i=0; i < h3->GetSize() -1; i++){
  if(h3->GetBinContent(i)>0){
    gr3->SetPoint(j,h3->GetBinCenter(i),h3->GetBinContent(i));
    j++;
  }
}

int Ncount4 =0;
for(int i=1; i < h4->GetSize() -1; i++){
  if(h4->GetBinContent(i)>0) Ncount4++;
}
TGraph *gr4 = new TGraph(Ncount4);
gr4->SetLineColor(kViolet-1);
j=0;
for(int i=0; i < h4->GetSize() -1; i++){
  if(h4->GetBinContent(i)>0){
    gr4->SetPoint(j,h4->GetBinCenter(i),h4->GetBinContent(i));
    j++;
  }
}

//gr1->Draw("same c");
//gr2->Draw("same c");
//gr3->Draw("same c");
//gr4->Draw("same c");
  c1->SetGridx(1);
  c1->SetGridy(1);
//leg = new TLegend(0.16,0.64,0.48,0.84);
leg = new TLegend(0.44,0.2,0.85,0.4);
leg->SetTextSize(0.03);
leg->AddEntry(h1,"L1 IsoEG p_{T} #geq 10 GeV ","pl");
leg->AddEntry(h2,"L1 IsoEG p_{T} #geq 15 GeV ","pl");
leg->AddEntry(h3,"L1 IsoEG p_{T} #geq 20 GeV ","pl");
//leg->AddEntry(h4,"L1 Jet #geq 40 GeV","pl");
leg->Draw();

     TLatex *   tex2 = new TLatex(0.75,0.92,"l1t-tsg-v5");
tex2->SetNDC();
   tex2->SetTextFont(44);
   tex2->SetTextSize(23);
   tex2->SetLineWidth(2);
   tex2->Draw();

      //////////TLatex *   tex3 = new TLatex(0.25,0.92,"Offline jet |#eta| < 4.7");
 TLatex *   tex3 = new TLatex(0.15,0.92,"Offline electron |#eta| < 2.4 ");
tex3->SetNDC();
   tex3->SetTextFont(44);
   tex3->SetTextSize(23);
   tex3->SetLineWidth(2);
   tex3->Draw();

/*TPaveText *pt = new TPaveText(0.25,0.07,2.1,0.12,"NBNF");
pt->SetBorderSize(0);
pt->SetFillStyle(0);
pt->AddText("Offline Jets p_{T} > 30 GeV");
pt->Draw();
*/

c1->Print(foldername+histname+".C");
c1->Print(foldername+histname+".png");
c1->Print(foldername+histname+".pdf");
}

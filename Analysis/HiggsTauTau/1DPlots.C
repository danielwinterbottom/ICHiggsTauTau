{
TString foldername = "V58pt1VBFPlots/";
TString histname = "h_L1METHFHiggsPtRes";
int nRebin = 2;
   
TFile *f1 = new TFile("output/Trigger_2016/V58pt1Plots.root");
TH1D * h1 = (TH1D*)f1->Get("VBFPlots/"+histname);
h1->Rebin(nRebin);
gStyle->SetOptStat(0);
h1->GetYaxis()->SetLabelOffset(100);
h1->GetYaxis()->SetTitleOffset(1);
h1->GetYaxis()->SetTitle("Normalized Entries");
/*h1->SetFillStyle(0);
h1->SetLineColor(kRed);*/
TGaxis::SetMaxDigits(3);

h1->GetXaxis()->SetTitle("(L1 MET - gen HiggsPt)/(gen HiggsPt) [inc HF]");
h1->DrawNormalized("");

//c1->SetLogy();
/*
TFile *f2 = new TFile("output/Trigger_2016/VBFNonSpecific/ttOutput_1.root");
TH1D * h2 = (TH1D*)f2->Get("L1TFilter/"+histname);
h2->Rebin(nRebin);
h2->SetFillStyle(0);
h2->SetLineColor(kBlue);
h2->DrawNormalized("same");

 TLatex *   tex3 = new TLatex(0.15,0.92,"Gen VBF jets m_{jj} > 400 GeV, |#Delta#eta|_{jj} > 3.0  ");
tex3->SetNDC();
   tex3->SetTextFont(44);
   tex3->SetTextSize(23);
   tex3->SetLineWidth(2);
   //tex3->Draw();

      TLatex *   tex2 = new TLatex(0.65,0.92,"l1t-integration-v58.1");
tex2->SetNDC();
   tex2->SetTextFont(44);
   tex2->SetTextSize(23);
   tex2->SetLineWidth(2);
   tex2->Draw();
*/
/*   
leg = new TLegend(0.55,0.7,0.82,0.85);
leg->AddEntry(h1,"ZeroBias ","l");
leg->AddEntry(h2,"VBF H#rightarrow#tau_{h}#tau_{h} ","l");


leg->Draw();
*/
   
c1->Print(foldername+histname+".C");
c1->Print(foldername+histname+".png");
c1->Print(foldername+histname+".pdf");
   
/*
 
  TH1D *h_genj1_Pt;
       TH1D *h_q1_Pt;
      TH1D *h_q2_Pt;
      TH1D *h_qq_Mqq;
      TH1D *h_qq_DeltaEta;
      h_qq_VecPt
      
      TH1D *h_genj1_Pt;
      TH1D *h_genj2_Pt;
      TH1D *h_genjj_Mjj;
      TH1D *h_genjj_DeltaEta;
      h_genjj_VecPt
      TH1D *h_j1_Pt;
      TH1D *h_j2_Pt;
      TH1D *h_jj_Mjj;
      TH1D *h_jj_DeltaEta;
      h_jj_VecPt
      TH1D *h_l1j1_Pt;
      TH1D *h_l1j2_Pt;
      TH1D *h_l1jj_l1Mjj;
      TH1D *h_l1jj_DeltaEta;
      h_l1jj_VecPt
      
      h_VecJetPtResGenH
      h_VecJetPtResGenJet
      h_HPtRes
      h_VBFPtRes
      */
// h_JetsMjjResGenJet
//h_JetsDeltaEtaResGenJet
//h_AveJetPtResGenJet
}

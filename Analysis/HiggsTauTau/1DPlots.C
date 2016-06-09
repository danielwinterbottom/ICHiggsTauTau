{
TString foldername = "V58pt1VBFPlots/";
TString histname = "h_l1jj_VecPt";

   
TFile *f1 = new TFile("output/Trigger_2016/V58pt1Efficiencies_MVAMet.root");
TH1D * h1 = (TH1D*)f1->Get("VBFPlots/"+histname);
h1->Rebin(2);
gStyle->SetOptStat(0);
h1->GetYaxis()->SetLabelOffset(100);
h1->GetYaxis()->SetTitleOffset(1);
h1->GetYaxis()->SetTitle("Normalized Entries");

TGaxis::SetMaxDigits(3);

h1->GetXaxis()->SetTitle("L1 jets VecPt [GeV]");
h1->Draw("");

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

   
c1->Print(foldername+histname+".C");
c1->Print(foldername+histname+".png");
c1->Print(foldername+histname+".pdf");
   
/*
 
  TH1D *h_genj1_Pt;
       TH1D *h_q1_Pt;
      TH1D *h_q2_Pt;
      TH1D *h_qq_Mqq;
      TH1D *h_qq_DeltaEta;
      
      TH1D *h_genj1_Pt;
      TH1D *h_genj2_Pt;
      TH1D *h_genjj_Mqq;
      TH1D *h_genjj_DeltaEta;
      TH1D *h_j1_Pt;
      TH1D *h_j2_Pt;
      TH1D *h_jj_Mjj;
      TH1D *h_jj_DeltaEta;
      TH1D *h_l1j1_Pt;
      TH1D *h_l1j2_Pt;
      TH1D *h_l1jj_l1Mjj;
      TH1D *h_l1jj_DeltaEta;
      h_l1jj_VecPt*/
// h_JetsMjjResGenJet
//h_JetsDeltaEtaResGenJet
//h_AveJetPtResGenJet
}

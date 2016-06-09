{
TString foldername = "V58Pt1Resolutions/";
TString histname = "h_JetsDeltaEtaResGenJet";
TFile *f1 = new TFile("output/Trigger_2016/V58pt1Efficiencies_MVAMet.root");

TH1D * h1 = (TH1D*)f1->Get("VBFPlots/"+histname);

h1->GetYaxis()->SetTitleOffset(1);
h1->GetYaxis()->SetTitle("Normalized Entries");
//h1->GetXaxis()->SetRangeUser(0, 0.2);
h1->GetYaxis()->SetLabelOffset(100);
//h1->GetXaxis()->SetTitle("Jet (L1 VecPt - Gen VecPt)/(Gen VecPt)");
h1->Draw();

      TLatex *   tex2 = new TLatex(0.65,0.92,"l1t-integration-v58.1");
tex2->SetNDC();
   tex2->SetTextFont(44);
   tex2->SetTextSize(23);
   tex2->SetLineWidth(2);
   tex2->Draw();

gStyle->SetOptStat(0);
c1->Print(foldername+histname+"_v58pt1.C");
c1->Print(foldername+histname+"_v58pt1.png");
c1->Print(foldername+histname+"_v58pt1.pdf");


}

{
TFile *f = new TFile("zpt_weights_summer2016.root","READ");

TH2D *h = (TH2D*)f->Get("zptmass_histo");
h->GetXaxis()->SetRangeUser(0,800);
h->GetYaxis()->SetRangeUser(0,600);
h->GetYaxis()->SetTitle("p_{T} [GeV]");
h->GetXaxis()->SetTitle("M [GeV]");
h->GetZaxis()->SetRangeUser(0.85,1.55);

gStyle->SetPalette(1);
//gStyle->SetContour(10);
h->SetContour(7);
h->Draw("colz");
c1->Print("zpt_weights.pdf");
}

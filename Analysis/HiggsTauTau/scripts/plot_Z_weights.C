void Plot_weights(std::string input_file = "input.root"){
//zpt_weights_summer2016_v3.root
//ExtrapUncert_v3.root
TH1D *h_nom;
TH1D *h_extrap_nom;
TFile *f = new TFile((input_file).c_str());
TH2D *h = (TH2D*)f->Get("zptmass_histo");
//h->GetXaxis()->SetRange(2,h->GetNbinsX());
//h->GetYaxis()->SetRange(1,h->GetNbinsY());
h->GetXaxis()->SetTitle("Mass [GeV]");
h->GetYaxis()->SetTitle("pT [GeV]");
h->GetZaxis()->SetTitle("weight");
gStyle->SetPalette(1, 0);
TCanvas c1;
h_nom = (TH1D*)h->Clone();
h->Draw("COLZ");
c1.Print("Z_pt_weights.pdf");

for(unsigned i=1; i<=(unsigned)h->GetNbinsX()+1; ++i){
  for(unsigned j=1; j<=(unsigned)h->GetNbinsY()+1;++j){
    double content = h->GetBinContent(i,j);
    double error = h->GetBinError(i,j);
    double mass = h->GetXaxis()->GetBinLowEdge(i);
    double pT = h->GetYaxis()->GetBinLowEdge(j);
    h->SetBinContent(i,j,error/content*100);
    std::cout << mass << "    " << pT << "    " << content << "    " << error/content*100 <<" %" << std::endl;
  }

}
h->GetZaxis()->SetTitle("weight stat. uncert. (%)");
h->Draw("COLZ");
c1.Print("Z_pt_weights_uncerts.pdf");




}

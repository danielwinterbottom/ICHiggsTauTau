void CompareSFs(std::string file1, std::string file2,std::string file3, std::string file4, std::string hist1, std::string hist2, std::string hist3, std::string hist4){

f1 = new TFile(file1.c_str());
f2 = new TFile(file2.c_str());
f3 = new TFile(file3.c_str());
f4 = new TFile(file4.c_str());

h1 = (TH1D*)f1->Get(hist1.c_str());
h2 = (TH1D*)f2->Get(hist2.c_str());
h3 = (TH1D*)f3->Get(hist3.c_str());
h4 = (TH1D*)f4->Get(hist4.c_str());

h1->Divide(h3);
h2->Divide(h4);

for(unsigned i=1; i<=h1->GetNbinsX(); ++i){
  for(unsigned j=1; j<=h1->GetNbinsY(); ++j){  
    std::cout << "pT = " << h1->GetXaxis()->GetBinLowEdge(i) << ", eta = " << h1->GetYaxis()->GetBinLowEdge(j)<< ", SF1 = " << h1->GetBinContent(i,j) << ", SF2 = " << h2->GetBinContent(i,j) << ", ratio = " << h1->GetBinContent(i,j)/h2->GetBinContent(i,j) << std::endl;
  }
}

h1->SetStats(0);
h1->GetXaxis()->SetTitle("pT (GeV)");
h1->GetYaxis()->SetTitle("abs(eta)");
h1->Divide(h2);
h1->GetXaxis()->SetRangeUser(25, 100);
h1->GetYaxis()->SetRangeUser(0, 2.1);
h1->Draw("colz");


}

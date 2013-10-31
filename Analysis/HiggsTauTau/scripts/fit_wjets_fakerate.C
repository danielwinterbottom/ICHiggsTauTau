{

  TFile *f = new TFile("talk_plots/mt50/datacard_pt_2_inclusive_mt50_et_2011.root");
  gDirectory->cd("eleTau_inclusive_mt50");
  TH1F *data = (TH1F*)data_obs->Clone();
  data->Add(QCD,-1);
  data->Add(TT,-1);
  data->Add(VV,-1);
  data->Add(ZL,-1);
  data->Add(ZJ,-1);
  data->Add(ZTT,-1);
  data->Divide(W);
  data->SetStats(0);
  data->SetTitle("");
  data->GetYaxis()->SetTitle("(data-#SigmaBkg)/W");
  data->GetXaxis()->SetTitle("Tau p_{T} [GeV]");
  data->GetXaxis()->SetRangeUser(20,220);
  data->Draw();
  TF1 *f1 = new TF1("f1", "pol3", 0, 300);
  data->Fit("f1","R");
  data->GetXaxis()->SetRangeUser(20,220);
  c1->Update();
  c1->SaveAs("etau_2011_fit.pdf");



}

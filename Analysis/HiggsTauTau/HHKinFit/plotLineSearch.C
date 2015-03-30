plotLineSearch(){
  TFile* f = new TFile("out97.root","READ");
  
  for (int i=0; i<f->GetListOfKeys()->GetSize(); i++){
  
  TDirectory* dir  = (TDirectory*) f->Get(Form("%u",i));
//   TGraph2D* grF = (TGraph2D*) dir->Get("F");
//   TGraph* grLine = (TGraph*) dir->Get("Line");
//   TGraph* grStartPoint = (TGraph*) dir->Get("StartPoint");
//   TGraph* grEndPoint = (TGraph*) dir->Get("EndPoint");
  TGraph* grFOnLine = (TGraph*) dir->Get("FOnLine");
  TGraph* grStartPointOnLine = (TGraph*) dir->Get("StartPointOnLine");
  TGraph* grEndPointOnLine = (TGraph*) dir->Get("EndPointOnLine");
  
  
  TCanvas* c = new TCanvas ("c","c",600,600);
/*
  TCanvas* c = new TCanvas ("c","c",1200,600);
  c->Divide(2);
  c->cd(1);
  grF->Draw("CONT3");
  c->Update();
  
  grLine->SetLineWidth(2);
  grLine->Draw("L");
  
  grStartPoint->SetMarkerStyle(20);  
  grStartPoint->SetMarkerSize(2);
  grStartPoint->Draw("P");
  
  grEndPoint->SetMarkerStyle(20);  
  grEndPoint->SetMarkerColor(kGreen);  
  grEndPoint->SetMarkerSize(2);
  grEndPoint->Draw("P");
  
  
  c->cd(2);*/
  grFOnLine->Draw("AL");
 
  grEndPointOnLine->SetMarkerStyle(20);  
  grEndPointOnLine->SetMarkerColor(kGreen);  
  grEndPointOnLine->SetMarkerSize(2);
  grEndPointOnLine->Draw("P");
  
  grStartPointOnLine->SetMarkerStyle(20);  
  grStartPointOnLine->SetMarkerColor(kBlack);  
  grStartPointOnLine->SetMarkerSize(2);
  grStartPointOnLine->Draw("P");
  }
}
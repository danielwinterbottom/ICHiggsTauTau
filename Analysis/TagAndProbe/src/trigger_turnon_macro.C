
double efficiency(double m, double m0, double sigma, double alpha, double n, double norm) const 
 { 
   const double sqrtPiOver2 = 1.2533141373;
   const double sqrt2 = 1.4142135624;

   double sig = fabs((double) sigma);
   
   double t = (m - m0)/sig ;
   
   if (alpha < 0)
     t = -t;

   double absAlpha = fabs(alpha / sig);
   double a = TMath::Power(n/absAlpha,n)*exp(-0.5*absAlpha*absAlpha);
   double b = absAlpha - n/absAlpha;

   //   if (a>=std::numeric_limits<double>::max()) return -1. ;

   double ApproxErf ;
   double arg = absAlpha / sqrt2 ;
   if (arg > 5.) ApproxErf = 1 ;
   else if (arg < -5.) ApproxErf = -1 ;
   else ApproxErf = TMath::Erf(arg) ;

   double leftArea = (1 + ApproxErf) * sqrtPiOver2 ;
   double rightArea = ( a * 1/TMath::Power(absAlpha - b,n-1)) / (n - 1);
   double area = leftArea + rightArea;

   if ( t <= absAlpha ){
     arg = t / sqrt2 ;
     if (arg > 5.) ApproxErf = 1 ;
     else if (arg < -5.) ApproxErf = -1 ;
     else ApproxErf = TMath::Erf(arg) ;
     return norm * (1 + ApproxErf) * sqrtPiOver2 / area ;
   }
   else{
     return norm * (leftArea +  a * (1/TMath::Power(t-b,n-1) - 1/TMath::Power(absAlpha - b,n-1)) / (1 - n)) / area ;
   }
  
 }

Double_t eff(Double_t *x, Double_t *par) {
  Double_t xx = x[0];
  Double_t m0 = par[0];
  Double_t sigma = par[1];
  Double_t alpha = par[2];
  Double_t n = par[3];
  Double_t norm = par[4];
 
  return efficiency(xx,m0,sigma,alpha,n,norm);
}


Double_t ratio(Double_t *x, Double_t *par) {
  Double_t xx = x[0];
  Double_t m0 = par[0];
  Double_t sigma = par[1];
  Double_t alpha = par[2];
  Double_t n = par[3];
  Double_t norm = par[4];

  Double_t m0_ = par[5];
  Double_t sigma_ = par[6];
  Double_t alpha_ = par[7];
  Double_t n_ = par[8];
  Double_t norm_ = par[9];

  double denom = efficiency(xx,m0_,sigma_,alpha_,n_,norm_);

  if(denom!=0)
    return efficiency(xx,m0,sigma,alpha,n,norm)/denom;
  else 
    return 0.0;
      
}


Double_t ratioFlat(Double_t *x, Double_t *par) {
  Double_t xx = x[0];
  Double_t m0 = par[0];
  Double_t m0_ = par[1];
  return m0/m0_;
}


void fit(TString file1 /*,TString file2*/, TString data ,/*TString mc,*/bool flat = false,float min=15.) {
  TFile *f1 = new TFile(file1); 
  //TFile *f2 = new TFile(file2); 
  gStyle->SetOptFit(0);
  TF1* fit1;
  //TF1* fit2;

  if(!flat) {
  fit1 = new TF1("fit"+data,eff,min,100.,5);
  //fit2 = new TF1("fit"+mc,eff,min,100.,5);
  //fit1->SetParameters(21.999,1.46,0.9742,2.4796,0.9372);
  //fit1->SetParameters(15.9199,0.83,0.57,2.167,0.9326);
  //fit1->SetParameters(15.9561,0.0076,0.000247,2.092,0.9554);
  //good for muon endcaps:
 fit1->SetParameters(15.998,7.7658e-05,6.3339e-08,1.6565,0.8614);
  //fit1->SetParameters(17.07,0.000100,0.00060,1.56,0.965);
  //fit1->SetParameters(16.6377,-1.23117,2.14735,1.17365,0.957028);
  //good for muon middle eta region:
  //fit1->SetParameters(17.4203,0.6201,1.13239,1.22927,0.93235);
  //fit1->SetParameters(16.7859,0.443337,0.571078,1.62214,0.919211);
  //fit1->SetParameters(16.5,0.004877,0.000561,1.5,0.861);
  //fit1->SetParameters(20.5,0.2,0.04,1.001,54);
  //good for muon low eta region:
  //fit1->SetParameters(15.9694,0.070174,0.0302466,1.8763,0.96322);
  //fit1->SetParameters(15.93,0.026,0.00487,1.9138,0.9777);
  //fit1->SetParameters(17.46,0.0494554,0.0628053,1.32332,0.90886);
  //fit1->SetParameters(15.03,0.55278,1.34236,1.003,3.36767); 
  //Ele15 barrel
  //fit1->SetParameters(14.8772,0.311255,0.221021,1.87734,0.986665);
  //Ele15 endcap
 // fit1->SetParameters(19.6586,0.682633,0.279486,2.66423,0.973455);
 //Ele20 Endcap
 //fit1->SetParameters(23.63,1.6077,1.72093,1.4131,1.1396);
  //fit2->SetParameters(19.1,1.2,1.7,1.4,0.8);
  }
  else
    {
      fit1 = new TF1("fit"+data,"pol0",min,50.);
    //  fit2 = new TF1("fit"+mc,"pol0",min,50.);
      fit1->SetParameter(0.,1.);
     // fit2->SetParameter(0.,1.)
    }


  fit1->SetLineWidth(2);
  //fit2->SetLineWidth(2);

 
  TGraphAsymmErrors* g1 =(TGraphAsymmErrors*) f1->Get(data);
  TCanvas *c1 = new TCanvas("c","DATA");
  c1->cd();
  
  g1->SetTitle(0);  
  g1->GetYaxis()->SetTitle("Efficiency");  
  g1->GetXaxis()->SetTitleColor(1);  
  g1->GetXaxis()->SetTitle("p_{T} (GeV)");  
  g1->Draw("AP");
    for(unsigned int i=0;i<30;++i) {
      g1->Fit(fit1,"","",min,100);
    }

/*
  TCanvas *c2 = new TCanvas("c2","DATA");
  c2->cd();
  g1->SetTitle(0);  
  g1->GetYaxis()->SetTitle("Efficiency");  
  g1->GetXaxis()->SetTitleColor(1);  
  g1->GetXaxis()->SetTitle("p_{T} (GeV)");  
  g1->Draw("AP");

  fit2 = new TF1("fit2"+data,eff,min,100.,5);
  fit2->SetParameters(fit1->GetParameter(0),
			fit1->GetParameter(1),
			fit1->GetParameter(2),
			fit1->GetParameter(3),
			fit1->GetParameter(4)
            );

  fit2->SetLineWidth(2);
    for(unsigned int i=0;i<20;++i) {
      g1->Fit(fit2,"","",min,100);
    }
*/
/*
  TGraphAsymmErrors* g2 =(TGraphAsymmErrors*) f2->Get(mc);
  TCanvas *c2 = new TCanvas("cc","MC");
  c2->cd();
  g2->Draw("AP");
  for(unsigned int i=0;i<20;++i) {
    g2->Fit(fit2,"","",min,100.);
   }
*/
  c1->Update();
  //c2->Update();

  c1->SaveAs(data+".png");
  c1->SaveAs(data+".pdf");
  //c2->SaveAs(mc+".png");
  //c2->SaveAs(mc+".pdf");


/*
  TF1 *corr ;
  if(!flat) {
    corr= new TF1("corr"+data,ratio,min,100.,10);
    corr->SetParameters(fit1->GetParameter(0),
			fit1->GetParameter(1),
			fit1->GetParameter(2),
			fit1->GetParameter(3),
			fit1->GetParameter(4),
			fit2->GetParameter(0),
			fit2->GetParameter(1),
			fit2->GetParameter(2),
			fit2->GetParameter(3),
			fit2->GetParameter(4));
  }
  else
    {
      corr= new TF1("corr"+data,ratioFlat,min,50.,2);
      corr->SetParameters(fit1->GetParameter(0),
			  fit2->GetParameter(0));
    }


  corr->SetLineWidth(2);
  TCanvas *c3 = new TCanvas("c3");
  c3->cd();
  c3->DrawFrame(0.,0.,100.,2.);
  corr->Draw("L");
  c3->SaveAs("corr"+data+".png");
*/
  TFile *fc = new TFile("corrections.root","RECREATE");
  fc->cd();
  fit1->Write("",TObject::kOverwrite);
  //fit1->Draw("same");
  c1->Write();
  g1->Write();
  //fit2->Write("",TObject::kOverwrite);
  //corr->Write("",TObject::kOverwrite);
  fc->Close();


  cout << "Fits completed in "<<data /*<< " and " <<mc */<< endl;
  printf("Results in DATA-------------\n");
  fit1->Print();
  //fit2->Print();

  //printf("Results in MC-------------\n");
  //fit2->Print();


} 

void fitAll() {
 //  fit("trigger/MuTauEff.root","trigger/MuTauEff.root","EffLooseTau10","EffLooseTau10MC",false,15.);
//   fit("trigger/MuTauEff.root","trigger/MuTauEff.root","EffLooseTau15","EffLooseTau10MC",false,15.);
//   fit("trigger/MuTauEff.root","trigger/MuTauEff.root","EffMu12","Mu12EffMC",true);
//   fit("trigger/MuTauEff.root","trigger/MuTauEff.root","EffMu15","Mu12EffMC",true);
//   fit("CorrectionFactors/TauEffCurves.root","CorrectionFactors/TauEffCurves.root","EffLooseTau20MTB","EffLooseTau15BMC",false,19.);
//   fit("CorrectionFactors/TauEffCurves.root","CorrectionFactors/TauEffCurves.root","EffLooseTau15B","EffLooseTau15BMC",false,14.);
//   fit("CorrectionFactors/TauEffCurves.root","CorrectionFactors/TauEffCurves.root","EffLooseTau10B","EffLooseTau15BMC",false,14.);
//    fit("CorrectionFactors/TauEffCurves.root","CorrectionFactors/TauEffCurves.root","EffLooseTau20B","EffMediumTau20BMC",false,19.);
//    fit("CorrectionFactors/TauEffCurves.root","CorrectionFactors/TauEffCurves.root","EffMediumTau20B","EffMediumTau20BMC",false,19.);
//    fit("CorrectionFactors/TauEffCurves.root","CorrectionFactors/TauEffCurves.root","EffTightTau20B","EffMediumTau20BMC",false,19.);
  
//   fit("CorrectionFactors/TauEffCurves.root","CorrectionFactors/TauEffCurves.root","EffLooseTau20MTE","EffLooseTau15EMC",false,19.);
//   fit("CorrectionFactors/TauEffCurves.root","CorrectionFactors/TauEffCurves.root","EffLooseTau15E","EffLooseTau15EMC",false,14.);
//   fit("CorrectionFactors/TauEffCurves.root","CorrectionFactors/TauEffCurves.root","EffLooseTau10E","EffLooseTau15EMC",false,14.);
//   fit("CorrectionFactors/TauEffCurves.root","CorrectionFactors/TauEffCurves.root","EffLooseTau20E","EffMediumTau20EMC",false,19.);
//   fit("CorrectionFactors/TauEffCurves.root","CorrectionFactors/TauEffCurves.root","EffMediumTau20E","EffMediumTau20EMC",false,19.);
//   fit("CorrectionFactors/TauEffCurves.root","CorrectionFactors/TauEffCurves.root","EffTightTau20E","EffMediumTau20EMC",false,19.);
  
//  fit("trigger/EleTauEff.root","trigger/EleTauEff.root","EffMediumTau20","EffLooseTau15MC3",false,19.);
//   fit("trigger/EleTauEff.root","trigger/EleTauEff.root","EffTightTau20","EffLooseTau15MC3",false,19.);
//   
//   fit("trigger/EleTauEff.root","trigger/EleTauEff.root","EffEle15EB","Ele15EffMCEB",false,15.);
//   fit("trigger/EleTauEff.root","trigger/EleTauEff.root","EffEle15EE","Ele15EffMCEE",false,15.);
// 
   fit("CorrectionFactors/EffCurvesOld.root","CorrectionFactors/EffCurvesOld.root","EffEle18EB","EffEle18EBMC2",false,15);
//   fit("trigger/EleTauEff.root","trigger/EleTauEff.root","EffEle18EE","Ele15EffMCEE",false,15.);
}

void fitAll2() {
  fit("trigger/MuTauEff.root","trigger/MuTauEff.root","EffMu12Tau10","EffMu12Tau10MC",false);
  fit("trigger/MuTauEff.root","trigger/MuTauEff.root","EffMu15Tau15","EffMu12Tau10MC",false);
  fit("trigger/LepTrigEffData.root","trigger/LepTrigEff.root","EffMu12","IsoMu12EffMC",true);
  fit("trigger/LepTrigEffData.root","trigger/LepTrigEff.root","EffMu15","IsoMu12EffMC",true);
  fit("trigger/EleTauEff.root","trigger/EleTauEff.root","EffEle15LooseTau20","EffEle15LooseTau20MCThin",false);
  fit("trigger/EleTauEff.root","trigger/EleTauEff.root","EffEle15MediumTau20","EffEle15LooseTau20MCThin",false);
  fit("trigger/EleTauEff.root","trigger/EleTauEff.root","EffEle15TightTau20","EffEle15LooseTau20MCThin",false);
  
  fit("trigger/EleTrigEff.root","trigger/EleTrigEff.root","EffEle15EB","Ele15EffMCEB",false);
  fit("trigger/EleTrigEff.root","trigger/EleTrigEff.root","EffEle15EE","Ele15EffMCEE",false);

  fit("trigger/EleTrigEff.root","trigger/EleTrigEff.root","EffEle18EB","Ele15EffMCEB",false);
  fit("trigger/EleTrigEff.root","trigger/EleTrigEff.root","EffEle18EE","Ele15EffMCEE",false);
}





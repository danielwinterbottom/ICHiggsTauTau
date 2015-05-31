void TrigWeightFit1D(){
  TFile *f = new TFile("../data/scale_factors/HLTEffParkedABCD_1D.root");
  TFile *out = new TFile("HLTEffParkedABCD_1DFit.root","RECREATE");
  
  unsigned nVars=4;
  std::vector<std::string> histstofit[4];
  std::vector<std::string> mjjhists;
  mjjhists.push_back("hData_MJJ_1D_A");
  mjjhists.push_back("hData_MJJ_1D_BC");
  mjjhists.push_back("hData_MJJ_1D_D");
  std::vector<std::string> metl1hists;
  metl1hists.push_back("hData_MET_L1_A");
  metl1hists.push_back("hData_MET_L1_BC");
  metl1hists.push_back("hData_MET_L1_D");
  std::vector<std::string> methists;
  methists.push_back("hData_MET_1D_A");
  methists.push_back("hData_MET_1D_BC");
  methists.push_back("hData_MET_1D_D");
  std::vector<std::string> jet2hists;
  jet2hists.push_back("hData_JET2_1D_A");
  jet2hists.push_back("hData_JET2_1D_BC");
  jet2hists.push_back("hData_JET2_1D_D");

  histstofit[0]=mjjhists;
  histstofit[1]=metl1hists;
  histstofit[2]=methists;
  histstofit[3]=jet2hists;

  //create a function with 3 parameters in the range [-3,3]
  for(unsigned iVar=0;iVar<nVars;iVar++){
    for(unsigned iTurnon=0;iTurnon<histstofit[iVar].size();iTurnon++){
      TH1F *hpx = (TH1F*)f->Get(histstofit[iVar][iTurnon].c_str()); //LOOP OVER THE TRIG TURN ONS
      double rangelow,rangehigh;
      if(iVar==0){
	rangelow=600;
	rangehigh=2200;
      }
      else if(iVar==1){
	rangelow=60;
	rangehigh=400;
      }
      else if(iVar==2){
	rangelow=0;
	rangehigh=400;
      }
      else if(iVar==3){
	rangelow=0;
	rangehigh=100;
      }
      TF1 *func = new TF1("erf","0.5*(1+TMath::Erf((x-[0])/(sqrt([1]))))",rangelow,rangehigh);
      func->SetParameters(100,10000);
      func->SetParNames("Turn On","Sigma");
      func->SetParLimits(0,-10000,10000);
      func->SetParLimits(1,0,100000);
      hpx->Fit("erf","R");
      TCanvas *c1=new TCanvas();
      hpx->Draw();
      c1->SaveAs((histstofit[iVar][iTurnon]+".pdf").c_str());
      TF1 *fitresult=hpx->GetFunction("erf");
      //Get name to save as
      std::string outname=histstofit[iVar][iTurnon].substr(1);
      out->WriteObject(fitresult,("f"+outname).c_str());
    }
  }
}

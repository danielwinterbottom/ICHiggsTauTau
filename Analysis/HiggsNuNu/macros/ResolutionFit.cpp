{
  //OPEN SIGNAL MC FILE
  TFile *f = new TFile("../MCforresmeasurment5etabins.root");//../output/nunu/MET130/MC_VBF_HToZZTo4Nu_M-120.root");
  
  
  //SET UP LOOP OVER PT AND ETA BINS
  int nptbins=13;
  int netabins=5;
  string etabins[netabins]={"0p0-0p5","0p5-1p1","1p1-1p7","1p7-2p3","2p3-5p0"};
  //   int netabins=10;
  //   std::string etabins[netabins]={"0p0-0p5","0p5-1p0","1p0-1p5","1p5-2p0","2p0-2p5","2p5-3p0","3p0-3p5","3p5-4p0","4p0-4p5","4p5-9p9"};
  string ptbins[nptbins]={"0-20","20-40","40-60","60-80","80-100","100-120","120-140","140-160","160-180","180-200","200-250","250-300","300-inf"};  
  
  TGraphErrors *res[netabins]; //MAKE OUTPUT TGRAPH

  for(int i=0;i<netabins;i++){//loop over eta bins
    res[i] = new TGraphErrors();
    for(int j=0;j<nptbins;j++){//loop over pt bins
      TH1F *recogenptratio;
      f->GetObject(("ResMeasurement/recogenjetptratio_"+etabins[i]+"_"+ptbins[j]).c_str(),recogenptratio);
      //std::cout<<"opened histogram "<<("ResMeasurement/recogenjetptratio_"+etabins[i]+"_"+ptbins[j])<<" successfully"<<std::endl;
      
      //FIT TO A GAUSSIAN
      recogenptratio->Sumw2();
      if(recogenptratio->GetEntries()==0){
	continue;
      }
      recogenptratio->Fit("gaus");

      //       TCanvas *c2 = new TCanvas();
      //       recogenptratio->Draw();
      //       c2->SaveAs(("recogenptratio"+etabins[i]+"_"+ptbins[j]+".pdf").c_str());

      //GET FIT RESULT
      TF1 *fitfunc=recogenptratio->GetFunction("gaus");
      double norm=fitfunc->GetParameter(0);
      double normerr=fitfunc->GetParError(0);
      double mean=fitfunc->GetParameter(1);
      double meanerr=fitfunc->GetParError(1);
      double sigma=fitfunc->GetParameter(2);
      double sigmaerr=fitfunc->GetParError(2);
      std::cout<<"For eta "<<etabins[i]<<" and pt "<<ptbins[j]<<" Sigma_MC is: "<<sigma<<"+/-"<<sigmaerr<<". Mean and norm are "<<mean<<"+/-"<<meanerr<<" "<<norm<<"+/-"<<normerr<<std::endl;

      
      //GET PT BIN MIDPOINT AND BIN WIDTH
      std::string s=ptbins[j];
      std::string delimiter = "-";
      size_t pos = 0;
      std::string token;
      int k=0;
      double binedges[2];
      pos = s.find(delimiter);
      token = s.substr(0, pos);
      if(token=="inf"){
	token="400";//SET INF BIN TO LOOK LIKE IT'S UP TO 400
      }
      if(token=="0"){//SKIP FIRST BIN AS RESOLUTION POORlY DEFINED
	continue;
      }
      binedges[0]=atof(token.c_str());
      //std::cout << token << std::endl;
      s.erase(0, pos + delimiter.length());
      if(s=="inf"){
	s="400";//SET INF BIN TO LOOK LIKE IT'S UP TO 400
      }
      binedges[1]=atof(s.c_str());
      x=(binedges[0]+binedges[1])/2;
      xerr=x-binedges[0];

      int pointindex=res[i]->GetN();
      res[i]->SetPoint(pointindex,x,sigma);
      res[i]->SetPointError(pointindex,xerr,sigmaerr);
    }
  }

  //GET SPRING10 RESOLUTION
  //   TFile file("spring10ptresolutions10etabins.root","read");
  //   TF1 *tf[10];
  //   file->GetObject("fsigma_Spring10_PtResolution_AK5PF_JetEta0.25",tf[0]);
  //   file->GetObject("fsigma_Spring10_PtResolution_AK5PF_JetEta0.75",tf[1]);
  //   file->GetObject("fsigma_Spring10_PtResolution_AK5PF_JetEta1.25",tf[2]);
  //   file->GetObject("fsigma_Spring10_PtResolution_AK5PF_JetEta1.75",tf[3]);
  //   file->GetObject("fsigma_Spring10_PtResolution_AK5PF_JetEta2.25",tf[4]);
  //   file->GetObject("fsigma_Spring10_PtResolution_AK5PF_JetEta2.75",tf[5]);
  //   file->GetObject("fsigma_Spring10_PtResolution_AK5PF_JetEta3.25",tf[6]);
  //   file->GetObject("fsigma_Spring10_PtResolution_AK5PF_JetEta3.75",tf[7]);
  //   file->GetObject("fsigma_Spring10_PtResolution_AK5PF_JetEta4.25",tf[8]);
  //   file->GetObject("fsigma_Spring10_PtResolution_AK5PF_JetEta4.75",tf[9]);

  TFile file("spring10ptresolutions5etabins.root","read");
  TF1 *tf[5];
  file->GetObject("fsigma_Spring10_PtResolution_AK5PF_JetEta0.25",tf[0]);
  file->GetObject("fsigma_Spring10_PtResolution_AK5PF_JetEta0.8",tf[1]);
  file->GetObject("fsigma_Spring10_PtResolution_AK5PF_JetEta1.4",tf[2]);
  file->GetObject("fsigma_Spring10_PtResolution_AK5PF_JetEta2",tf[3]);
  file->GetObject("fsigma_Spring10_PtResolution_AK5PF_JetEta3.65",tf[4]);
  
  //FIT RESOLUTION
  TF1 *resfunc[5];
  
  
  //OUTPUT
  TFile outfile("MCresolutions.root","recreate");
  std::string names[5]={"0<#eta<0.5","0.5<#eta<1.1","1.1<#eta<1.7","1.7<#eta<2.3","2.3<#eta<5"};
  for(int i = 0;i<netabins;i++){
    TCanvas *c1 = new TCanvas();
    TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
    leg->AddEntry(res[i],"Resolution from our W MC","lp");
    leg->AddEntry(tf[i],"Resolution from Spring10","l");
    
    resfunc[i] = new TF1("resfunc","sqrt((([0]<0)?-1:1)*[0]*[0]/(x*x)+[1]*[1]*pow(x,[2]-1)+[3]*[3])",0.,400.);
    res[i]->Fit(resfunc[i]);
    res[i]->GetFunction("resfunc")->SetLineColor(4);
    
    tf[i]->SetRange(20,400);
    res[i]->GetXaxis()->SetRangeUser(20,400);
    res[i]->SetTitle(names[i].c_str());
    res[i]->SetLineColor(4);
    res[i]->GetXaxis()->SetTitle("p_{T}/GeV");
    res[i]->GetYaxis()->SetTitle("#sigma_{MC}/p_{T}");
    
    
    res[i]->Draw("AP");
    tf[i]->Draw("Same");    
    leg->Draw("Same");
    c1->SaveAs(("resforeta"+etabins[i]+".pdf").c_str());
    res[i]->SetName(("resforeta"+etabins[i]).c_str());
    res[i]->Write();
    resfunc[i]->SetName(("resfuncforeta"+etabins[i]).c_str());
    resfunc[i]->Write();
    tf[i]->SetName(("spring10resforeta"+etabins[i]).c_str());
    tf[i]->Write();
  }
  outfile->Close();
}

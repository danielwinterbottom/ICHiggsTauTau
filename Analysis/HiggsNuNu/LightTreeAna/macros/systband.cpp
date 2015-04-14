{

  std::string infolder="output_290315_sig";
  TString outfolder="output_withbands_290315";
  double nonshapeuncperc=0.1194;//0.1568; //0.1194
  double rationonshapeuncperc=0.1194; //0.09188;

  //Get in files
  TFile* centralfile=new TFile((infolder+"/nunu.root").c_str());
  TFile* jesupfile =new TFile((infolder+"/JESUP/nunu.root").c_str());
  TFile* jesdownfile =new TFile((infolder+"/JESDOWN/nunu.root").c_str());
  TFile* jerbetterfile =new TFile((infolder+"/JERBETTER/nunu.root").c_str());
  TFile* jerworsefile =new TFile((infolder+"/JERWORSE/nunu.root").c_str());
  TFile* uesupfile =new TFile((infolder+"/UESUP/nunu.root").c_str());
  TFile* uesdownfile =new TFile((infolder+"/UESDOWN/nunu.root").c_str());
  TFile* eleeffupfile =new TFile((infolder+"/ELEEFFUP/nunu.root").c_str());
  TFile* eleeffdownfile =new TFile((infolder+"/ELEEFFDOWN/nunu.root").c_str());
  TFile* mueffupfile =new TFile((infolder+"/MUEFFUP/nunu.root").c_str());
  TFile* mueffdownfile =new TFile((infolder+"/MUEFFDOWN/nunu.root").c_str());
  TFile* puupfile =new TFile((infolder+"/PUUP/nunu.root").c_str());
  TFile* pudownfile =new TFile((infolder+"/PUDOWN/nunu.root").c_str());

  unsigned nsysts=12;

  TFile* systfiles[12];
  systfiles[0]=jesupfile;
  systfiles[1]=jesdownfile;
  systfiles[2]=jerbetterfile;
  systfiles[3]=jerworsefile;
  systfiles[4]=uesupfile;
  systfiles[5]=uesdownfile;
  systfiles[6]=eleeffupfile;
  systfiles[7]=eleeffdownfile;
  systfiles[8]=mueffupfile;
  systfiles[9]=mueffdownfile;
  systfiles[10]=puupfile;
  systfiles[11]=pudownfile;

  //Make out file
  TFile* outfile=new TFile("outfile.root","RECREATE");
  outfile->cd();

  //Define shapes
  vector<TString> shape;
  shape.push_back("jet2_pt");
  shape.push_back("jet1_pt");
  shape.push_back("metnomuons");
  shape.push_back("met");
  shape.push_back("l1met");
  shape.push_back("dijet_M");
  shape.push_back("jetmetnomu_mindphi");
  shape.push_back("alljetsmetnomu_mindphi");
  shape.push_back("metnomu_significance");
  shape.push_back("dijet_sumeta");
  shape.push_back("ht");
  shape.push_back("jetunclet_mindphi");
  shape.push_back("metnomuunclet_dphi");
  shape.push_back("dijetmetnomu_scalarSum_pt");
  shape.push_back("dijetmetnomu_vectorialSum_pt");
  shape.push_back("n_jets_cjv_30");
  shape.push_back("n_jets_cjv_20EB_30EE");
  shape.push_back("dijet_dphi");
  shape.push_back("dijet_deta");
  shape.push_back("lep_mt");
  shape.push_back("dijetmetnomu_ptfraction");
  shape.push_back("ele1_pt");
  shape.push_back("mu1_pt");
  shape.push_back("jet_csv1");
  shape.push_back("jet_csv2");
  shape.push_back("jet_csv3");
  shape.push_back("central_tag_eta");
  shape.push_back("forward_tag_eta");
  shape.push_back("n_jets_30");
  shape.push_back("n_jets_15");



  //Loop over shapes
  for(unsigned iShape=0;iShape<shape.size();iShape++){
    TCanvas* canvas =(TCanvas*)centralfile->Get(("ControlPlots/"+shape[iShape]));

    THStack* centerstack=(THStack*)centralfile->Get(("stacksdir/stack"+shape[iShape]));
    TH1F* centerhist=centerstack->GetStack()->Last();

    THStack* syststacks[nsysts];
    TH1F* systhists[nsysts];
    for(unsigned iSyst=0;iSyst<nsysts;iSyst++){
      THStack* tmpstack=(THStack*)systfiles[iSyst]->Get(("stacksdir/stack"+shape[iShape]));
      syststacks[iSyst]=tmpstack;
      systhists[iSyst]=(TH1F*)syststacks[iSyst]->GetStack()->Last();
    }
      
    //Loop over all bins and get shape syst band
    TH1F* errorband=(TH1F*)(centerhist->Clone("errorband"));
    TH1F* ratioerrorband=(TH1F*)(centerhist->Clone("ratioerrorband"));
    for(unsigned iBin=0;iBin<centerhist->GetNbinsX()+1;iBin++){
      double centervalue=centerhist->GetBinContent(iBin);
      if(centervalue==0){
	errorband->SetBinError(iBin,0);
	errorband->SetBinContent(iBin,centervalue);
	ratioerrorband->SetBinError(iBin,0);
	ratioerrorband->SetBinContent(iBin,1);
	continue;
      }
      double systdeviations[6];
      for(unsigned iSyst=0;iSyst<nsysts;iSyst=iSyst+2){
	double updev=systhists[iSyst]->GetBinContent(iBin)-centervalue;
	double downdev=systhists[iSyst+1]->GetBinContent(iBin)-centervalue;
	if(fabs(updev)>fabs(downdev))systdeviations[iSyst/2]=updev;
	else systdeviations[iSyst/2]=downdev;
      }
      //Add up all contributions
      double totalsystdeviation=sqrt(pow(systdeviations[0],2)+pow(systdeviations[1],2)+pow(systdeviations[2],2)+pow(systdeviations[3],2)+pow(systdeviations[4],2)+pow(systdeviations[5],2));
      totalsystdeviation=sqrt(pow(totalsystdeviation,2)+pow(nonshapeuncperc*centervalue,2));
      double ratiototalsystdeviation=sqrt(pow(totalsystdeviation,2)+pow(rationonshapeuncperc*centervalue,2));

      errorband->SetBinError(iBin,totalsystdeviation);
      errorband->SetBinContent(iBin,centervalue);
      ratioerrorband->SetBinError(iBin,ratiototalsystdeviation/centervalue);
      ratioerrorband->SetBinContent(iBin,1);
    }

    TCanvas* c2=canvas->Clone(shape[iShape]);
    TPad* upper=c2->GetPrimitive("upper");
    TPad* lower=c2->GetPrimitive("lower");
    upper->cd();
    errorband->SetMarkerSize(0);
    errorband->SetFillColor(14);
    errorband->SetFillStyle(3013);                                                                                                             
    errorband->SetLineWidth(1);

    errorband->Draw("sameE2");

    lower->cd();
    ratioerrorband->SetMarkerSize(0);
    ratioerrorband->SetFillColor(14);
    ratioerrorband->SetFillStyle(3013);                                                                                                             
    ratioerrorband->SetLineWidth(1);
    ratioerrorband->Draw("sameE2");

    //Don't ask why there are so many canvases...
    c2->cd();
    c2->Update();
    c2->Write();

    TCanvas* c3=outfile->Get(shape[iShape]);
    c3->Print((outfolder+"/nunu_"+shape[iShape]+".pdf"));
    c3->Print((outfolder+"/nunu_"+shape[iShape]+".png"));
  }
}

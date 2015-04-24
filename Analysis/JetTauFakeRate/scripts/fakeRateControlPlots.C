void fakeRateControlPlots(){
	TFile *file1 = TFile::Open("SELECTION_Out_SmallEventTree_All.root");
	TTree *tree = dynamic_cast<TTree*>(file1->Get("EventTree2"));
	TBranch *branch_njets = tree->GetBranch("njets");
	TBranch *branch_ntaus = tree->GetBranch("ntaus");
	TBranch *branch_nvtx = tree->GetBranch("nvtx");
	TBranch *branch_jet_pt = tree->GetBranch("jet_sel_pt");
	TBranch *branch_jet_eta = tree->GetBranch("jet_sel_eta");
	TBranch *branch_jet_phi = tree->GetBranch("jet_sel_phi");
	TBranch *branch_tau_eta = tree->GetBranch("tau_sel_eta");
	TBranch *branch_tau_phi = tree->GetBranch("tau_sel_phi");
	TBranch *branch_tau_pt = tree->GetBranch("tau_sel_pt");
	TBranch *branch_jet_flavour = tree->GetBranch("jet_sel_flavour");
	TBranch *branch_tau_id_loose = tree->GetBranch("tau_id_loose");
	TBranch *branch_tau_id_med = tree->GetBranch("tau_id_med");
	TBranch *branch_tau_id_tight = tree->GetBranch("tau_id_tight");
	TBranch *branch_tau_dm = tree->GetBranch("tau_dm_find");

	Int_t njets; 
	Int_t ntaus;
	Int_t nvtx;
	std::vector<Double_t> *jetpt = new std::vector<Double_t>();
	std::vector<Double_t> *jeteta = new std::vector<Double_t>();
	std::vector<Double_t> *jetphi = new std::vector<Double_t>();
	std::vector<Int_t> *jetflavour = new std::vector<Int_t>();
	std::vector<Double_t> *taueta = new std::vector<Double_t>();
	std::vector<Double_t> *tauphi = new std::vector<Double_t>();
	std::vector<Double_t> *taupt = new std::vector<Double_t>();
	std::vector<Double_t> *tau_id_loose = new std::vector<Double_t>();
	std::vector<Double_t> *tau_id_med = new std::vector<Double_t>();
	std::vector<Double_t> *tau_id_tight = new std::vector<Double_t>();
	std::vector<Double_t> *tau_dm_find = new std::vector<Double_t>();
	Double_t deltaR1=0;
	Double_t deltaR2=0;
	Int_t jetnr;
	branch_njets->SetAddress(&njets);
	branch_ntaus->SetAddress(&ntaus);
	branch_nvtx->SetAddress(&nvtx);
	branch_jet_pt->SetAddress(&jetpt);
	branch_jet_eta->SetAddress(&jeteta);
	branch_jet_phi->SetAddress(&jetphi);
	branch_tau_eta->SetAddress(&taueta);
	branch_tau_phi->SetAddress(&tauphi);
	branch_tau_pt->SetAddress(&taupt);
	branch_tau_id_loose->SetAddress(&tau_id_loose);
	branch_tau_id_med->SetAddress(&tau_id_med);
	branch_tau_id_tight->SetAddress(&tau_id_tight);
	branch_tau_dm->SetAddress(&tau_dm_find);
	branch_jet_flavour->SetAddress(&jetflavour);

	TFile *foutput = new TFile("FakeRateControlPlots.root","RECREATE");
	TH1F * fJetPt = new TH1F("fJetPt",";p_{T}[GeV/c];N_{evts}",50,0,500);
	TH1F * fTauPt = new TH1F("fTauPt",";p_{T} [GeV/c];N_{evts}",50,0,500);
	TH1F * fTauPtDM = new TH1F("fTauPtDM",";p_{T} [GeV/c];N_{evts}",50,0,500);
	TH1F * fTauPtLoose = new TH1F("fTauPtLoose",";p_{T} [GeV/c];N_{evts}",50,0,500);
	TH1F * fTauPtMed = new TH1F("fTauPtMed",";p_{T} [GeV/c];N_{evts}",50,0,500);
	TH1F * fTauPtTight = new TH1F("fTauPtTight",";p_{T} [GeV/c];N_{evts}",50,0,500);

	TH1F * fJetEta = new TH1F("fJetEta",";#eta;N_{evts}",30,-2.5,2.5);
	TH1F * fTauEta = new TH1F("fTauEta",";#eta;N_{evts}",30,-2.5,2.5);
	TH1F * fTauEtaDM = new TH1F("fTauEtaDM",";#eta;N_{evts}",30,-2.5,2.5);
	TH1F * fTauEtaLoose = new TH1F("fTauEtaLoose",";eta;N_{evts}",30,-2.5,2.5);
	TH1F * fTauEtaMed = new TH1F("fTauEtaMed",";eta;N_{evts}",30,-2.5,2.5);
	TH1F * fTauEtaTight = new TH1F("fTauEtaTight",";eta;N_{evts}",30,-2.5,2.5);

	TH1F * fJetPhi = new TH1F("fJetPhi",";#phi;N_{evts}",20,-3.15,3.15);
	TH1F * fTauPhi = new TH1F("fTauPhi",";#phi;N_{evts}",20,-3.15,3.15);
	TH1F * fTauPhiDM = new TH1F("fTauPhiDM",";#phi;N_{evts}",20,-3.15,3.15);
	TH1F * fTauPhiLoose = new TH1F("fTauPhiLoose",";#phi;N_{evts}",20,-3.15,3.15);
	TH1F * fTauPhiMed = new TH1F("fTauPhiMed",";#phi;N_{evts}",20,-3.15,3.15);
	TH1F * fTauPhiTight = new TH1F("fTauPhiTight",";#phi;N_{evts}",20,-3.15,3.15);

	TH1F * fDeltaR = new TH1F("fDeltaR",";#Delta R;N_{evts}",20,0,0.3);
	TH1F * fDeltaRDM = new TH1F("fDeltaRDM",";#Delta R;N_{evts}",20,0,0.3);
	TH1F * fDeltaRLoose = new TH1F("fDeltaRLoose",";#Delta R;N_{evts}",20,0,0.3);
	TH1F * fDeltaRMed = new TH1F("fDeltaRMed",";#Delta R;N_{evts}",20,0,0.3);
	TH1F * fDeltaRTight = new TH1F("fDeltaRTight",";#Delta R;N_{evts}",20,0,0.3);

	TH1F * fJetFlavour = new TH1F("fJetFlavour",";JetFlavour;N_{evts}",50,-20,20);


	Int_t nEntries = tree->GetEntries();


	for(Int_t i = 0;i<nEntries;i++){
		if(i%10000==0) cout<<"AT EVENT "<<i<<endl;
		tree->GetEntry(i);
		for(Int_t k=0;k<ntaus;k++){
			fTauPt->Fill(taupt->at(k));
			fTauEta->Fill(taueta->at(k));
			fTauPhi->Fill(tauphi->at(k));
			if(tau_dm_find->at(k)>0.5){
				fTauPtDM->Fill(taupt->at(k));
				fTauEtaDM->Fill(taueta->at(k));
				fTauPhiDM->Fill(tauphi->at(k));
				if(tau_id_loose->at(k)>0.5){
					fTauPtLoose->Fill(taupt->at(k));
					fTauEtaLoose->Fill(taueta->at(k));
					fTauPhiLoose->Fill(tauphi->at(k));
				}
				if(tau_id_med->at(k)>0.5){
					fTauPtMed->Fill(taupt->at(k));
					fTauEtaMed->Fill(taueta->at(k));
					fTauPhiMed->Fill(tauphi->at(k));
				}
				if(tau_id_tight->at(k)>0.5){
					fTauPtTight->Fill(taupt->at(k));
					fTauEtaTight->Fill(taueta->at(k));
					fTauPhiTight->Fill(tauphi->at(k));
				}

			}
			deltaR1=1000;
			for(Int_t j=0;j<njets;j++){
				deltaR2=TMath::Sqrt(TMath::Power((jeteta->at(j)-taueta->at(k)),2)+TMath::Power((jetphi->at(j)-tauphi->at(k)),2));
				if(deltaR2<deltaR1){
					deltaR1=deltaR2;
				}
			}
			fDeltaR->Fill(deltaR1);
			if(tau_dm_find->at(k)>0.5){
				fDeltaRDM->Fill(deltaR1);
				if(tau_id_loose->at(k)>0.5) fDeltaRLoose->Fill(deltaR1);
				if(tau_id_med->at(k)>0.5) fDeltaRMed->Fill(deltaR1);
				if(tau_id_tight->at(k)>0.5) fDeltaRTight->Fill(deltaR1);
			}
		}


		for(Int_t j=0;j<njets;j++){
			fJetPt->Fill(jetpt->at(j));
			fJetEta->Fill(jeteta->at(j));
			fJetPhi->Fill(jetphi->at(j));
			fJetFlavour->Fill(jetflavour->at(j));
		}
	}


	foutput->cd();
	fJetPt->Write();
	fJetEta->Write();
	fJetPhi->Write();
	fJetFlavour->Write();
	fTauPt->Write();
	fTauEta->Write();
	fTauPhi->Write();
	fTauPtDM->Write();
	fTauEtaDM->Write();
	fTauPhiDM->Write();
	fTauPtLoose->Write();
	fTauEtaLoose->Write();
	fTauPhiLoose->Write();
	fTauPtMed->Write();
	fTauEtaMed->Write();
	fTauPhiMed->Write();
	fTauPtTight->Write();
	fTauEtaTight->Write();
	fTauPhiTight->Write();
	fDeltaR->Write();
	fDeltaRDM->Write();
	fDeltaRLoose->Write();
	fDeltaRMed->Write();
	fDeltaRTight->Write();
	foutput->Close();


}

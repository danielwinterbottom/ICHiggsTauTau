int Zignoringlepton(){
  //gSystem->Load("$ROOTSYS/test/libEvent");


  std::string filepath="../output_lighttree_topreweighted/";

  std::vector<std::string> zfiles;
  zfiles.push_back("MC_DYJetsToLL_PtZ-100-madgraph");
  zfiles.push_back("MC_DYJetsToLL");
  zfiles.push_back("MC_DY1JetsToLL");
  zfiles.push_back("MC_DY2JetsToLL");
  zfiles.push_back("MC_DY3JetsToLL");
  zfiles.push_back("MC_DY4JetsToLL");
  zfiles.push_back("MC_EWK-Z2j");

  for(unsigned ifile=0;ifile<zfiles.size();ifile++){
    std::cout<<"Processing: "<<zfiles[ifile]<<std::endl;
    //Get old file, old tree and set top branch address
    TFile *oldfile = new TFile((filepath+zfiles[ifile]+".root").c_str());
    TTree *oldtree = (TTree*)oldfile->Get("LightTree");
    Long64_t nentries = oldtree->GetEntries();
    int nvetoelectrons;
    int nselelectrons;
    int nvetomuons;
    int nselmuons;
    oldtree->SetBranchAddress("nvetoelectrons",&nvetoelectrons);
    oldtree->SetBranchAddress("nselelectrons",&nselelectrons);
    oldtree->SetBranchAddress("nvetomuons",&nvetomuons);
    oldtree->SetBranchAddress("nselmuons",&nselmuons);
    
    //Create a new file + a clone of old tree in new file
    TFile *newfile = new TFile((zfiles[ifile]+"iglep.root").c_str(),"recreate");
    TTree *newtree = oldtree->CloneTree(0);
    
    for (Long64_t i=0;i<nentries; i++) {
      oldtree->GetEntry(i);
      //if (event->GetNtrack() > 605) newtree->Fill();
      nvetoelectrons=0;
      nselelectrons=0;
      nvetomuons=0;
      nselmuons=0;
      newtree->Fill();
    //event->Clear();
    }
    newtree->Print();
    newtree->AutoSave();
    delete oldfile;
    delete newfile;
  }
}

void setBins(int &nxbins,double &xmin, double &xmax, int &nybins, double &ymin, double &ymax, int &nzbins, double &zmin, double &zmax, int &nmbins, double &mmin, double &mmax){
  int nbins = 50;
  nxbins = nbins; xmin = -1.; xmax = 1.; nybins = nbins; ymin = -1.; ymax = 1.; nzbins = nbins; zmin = -1.; zmax = 1.; nmbins = nbins; mmin = 0; mmax = 1.8;
}


std::vector<double> getValues(int bin){
  std::vector<double> v;
  int nxbins; double xmin; double xmax; int nybins; double ymin; double ymax; int nzbins; double zmin; double zmax; int nmbins; double mmin; double mmax;
  setBins(nxbins,xmin,xmax,nybins,ymin,ymax,nzbins,zmin,zmax,nmbins,mmin,mmax);
  
  int nbins = nxbins*nybins*nzbins*nmbins;
  bin--;
  double x = floor((bin)/(nbins/nxbins))*(xmax-xmin)/nxbins + xmin + (xmax-xmin)/(2*nxbins);
  bin -= floor(bin/(nbins/nxbins))*(nbins/nxbins);
  double y = floor(bin/(nzbins*nmbins))*(ymax-ymin)/nybins + ymin + (ymax-ymin)/(2*nybins);
  bin -= floor(bin/(nzbins*nmbins))*(nzbins*nmbins);
  double z = floor(bin/(nmbins))*(zmax-zmin)/nzbins + zmin + (zmax-zmin)/(2*nzbins);
  bin -= floor(bin/(nzbins))*(nzbins);
  double m = floor(bin)*(mmax-mmin)/nmbins + mmin + (mmax-mmin)/(2*nmbins);
  //std::cout << x << std::endl;
  //std::cout << y << std::endl;
  //std::cout << z << std::endl;
  //std::cout << m << std::endl;
  v = {x,y,z,m};
  return v;
}
        
int getBinNum(double xval, double yval, double zval, double mval){
  int nxbins; double xmin; double xmax; int nybins; double ymin; double ymax; int nzbins; double zmin; double zmax; int nmbins; double mmin; double mmax;
  setBins(nxbins,xmin,xmax,nybins,ymin,ymax,nzbins,zmin,zmax,nmbins,mmin,mmax);
  if (xval<xmin) xval = xmin;
  if (yval<ymin) yval = ymin;
  if (zval<zmin) zval = zmin;
  if (mval<mmin) mval = mmin;
  int m = floor((mval-mmin)/(mmax-mmin)*nmbins)+1;
  if (m>nmbins) m = nmbins;
  int z = floor((zval-zmin)/(zmax-zmin)*nzbins)*nmbins;
  if(z>nmbins*(nzbins-1)) z = nmbins*(nzbins-1);
  int y = floor((yval-ymin)/(ymax-ymin)*nybins)*nzbins*nmbins;
  if(y>nmbins*nzbins*(nybins-1)) y = nmbins*nzbins*(nybins-1);
  int x = floor((xval-xmin)/(xmax-xmin)*nxbins)*nybins*nzbins*nmbins;
  if(x>nmbins*nzbins*nybins*(nxbins-1)) x = nmbins*nzbins*nybins*(nxbins-1);
  //std::cout << x << std::endl;
  //std::cout << y << std::endl;
  //std::cout << z << std::endl;
  //std::cout << m << std::endl;

  return m+y+z+x;
}

void makeHist() {
std::vector<std::string> files = {
"/vols/cms/dw515/Offline/output/MSSM/tau_decay/DY1JetsToLL-LO_mt_2016.root",
"/vols/cms/dw515/Offline/output/MSSM/tau_decay/DY2JetsToLL-LO_mt_2016.root",
"/vols/cms/dw515/Offline/output/MSSM/tau_decay/DY3JetsToLL-LO_mt_2016.root",
"/vols/cms/dw515/Offline/output/MSSM/tau_decay/DY4JetsToLL-LO_mt_2016.root",
"/vols/cms/dw515/Offline/output/MSSM/tau_decay/DYJetsToLL-LO-ext1_mt_2016.root",
"/vols/cms/dw515/Offline/output/MSSM/tau_decay/DYJetsToLL-LO-ext2_mt_2016.root",
"/vols/cms/dw515/Offline/output/MSSM/tau_decay/DYJetsToLL_M-10-50-LO_mt_2016.root"};

TChain *c1 = new TChain("had_decay");
TChain *c2 = new TChain("lep_decay");

for(unsigned i=0; i<files.size(); ++i){
  c1->Add(files[i].c_str());
  c2->Add(files[i].c_str());
}

  int nxbins; double xmin; double xmax; int nybins; double ymin; double ymax; int nzbins; double zmin; double zmax; int nmbins; double mmin; double mmax;
  setBins(nxbins,xmin,xmax,nybins,ymin,ymax,nzbins,zmin,zmax,nmbins,mmin,mmax);
  int nbins = nmbins*nxbins*nybins*nzbins;
  TH1D *had_tau = new TH1D("had_tau","",nbins,1,nbins+1);
  TH1D *elec_tau = new TH1D("elec_tau","",nbins,1,nbins+1);
  TH1D *muon_tau = new TH1D("muon_tau","",nbins,1,nbins+1);

  double t_px_; double t_py_; double t_pz_; double t_m_; double l_px_; double l_py_; double l_pz_; double l_m_;

  c1->SetBranchAddress("t_px",&t_px_);
  c1->SetBranchAddress("t_py",&t_py_);
  c1->SetBranchAddress("t_pz",&t_pz_);
  c1->SetBranchAddress("decaytau_mass",&t_m_);

  c2->SetBranchAddress("l_px",&l_px_);
  c2->SetBranchAddress("l_py",&l_py_);
  c2->SetBranchAddress("l_pz",&l_pz_);
  c2->SetBranchAddress("decaytau_mass",&l_m_);
  

  for(unsigned i=0; i<c1->GetEntries();++i){
    c1->GetEntry(i);
    had_tau->Fill(getBinNum(t_px_,t_py_,t_pz_,t_m_));
  }
  for(unsigned i=0; i<c2->GetEntries();++i){
    c2->GetEntry(i);
    if (l_m_>0.05) muon_tau->Fill(getBinNum(l_px_,l_py_,l_pz_,l_m_));
    else elec_tau->Fill(getBinNum(l_px_,l_py_,l_pz_,l_m_));
  }
  
  //TH1D *had_test = new TH1D("had_test","",nbins,1,nbins+1);
  //gRandom = new TRandom3();

  //TH1D *m = new TH1D("m","",50,0,1.8);
  //for(unsigned i=1; i<=had_test->GetNbinsX(); ++i){
  //  double mass = getValues(i)[3];
  //  double old_content = m->GetBinContent(m->FindBin(mass));
  //  double new_content = old_content + had_test->GetBinContent(i);
  //  m->SetBinContent(m->FindBin(mass),new_content);
  //}
  //m->Draw();
  
  //TH1D *p = new TH1D("p","",50,-1,1);
  //for(unsigned i=1; i<=had_test->GetNbinsX(); ++i){
  //  double px = getValues(i)[0];
  //  double old_content = p->GetBinContent(p->FindBin(px));
  //  double new_content = old_content + had_test->GetBinContent(i);
  //  p->SetBinContent(p->FindBin(px),new_content);
  //}
  //p->Draw();
 

TFile *fout = new TFile("tau_decay_input.root","RECREATE");
fout->cd();
had_tau->Write();
elec_tau->Write();
muon_tau->Write();


}

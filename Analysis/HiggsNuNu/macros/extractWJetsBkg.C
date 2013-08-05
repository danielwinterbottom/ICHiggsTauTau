#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <iomanip>

#include "TMath.h"

using std::showpos;
using std::noshowpos;

//enum Selection {Trig,Lep,JetPair,DEta,MET,TightMjj,DPhiSIGNAL,DPhiQCD};
//enum Selection {Trig,Lep,JetPair,DEta,MET,TightMjj,DPhiSIGNAL_noCJV,DPhiQCD_noCJV,CJVfail,DPhiSIGNAL_CJVfail,DPhiQCD_CJVfail,CJVpass,DPhiSIGNAL_CJVpass,DPhiQCD_CJVpass};
enum Selection {Trig,Lep,JetPair,DEta,MET,TightMjj,DPhiSIGNAL_noCJV,DPhiQCD_noCJV,CJVpass,DPhiSIGNAL_CJVpass,DPhiQCD_CJVpass};
enum Sample {QCD,GJets,Top,TTbar,TTbarPowheg,SingleTop,TW,
	     WJets,QCD_WJets,EWK_WJets,QCD_WJets_enu,EWK_WJets_enu,QCD_WJets_munu,EWK_WJets_munu,QCD_WJets_taunu,EWK_WJets_taunu,
	     ZJets,ZJets_ll,ZJets_nunu,ZJets_vbf,ZJets_vbf_nunu,
	     VV,Data,Signal};

struct events {
  std::string sample;
  double number;
  double stat;
  double syst;
  double tauidSyst;
  std::pair<double,double> jerSyst;// = std::pair<double,double>(0,0);
  std::pair<double,double> jesSyst;
  std::pair<double,double> puSyst;

  double error() const{
    return sqrt(pow(stat,2)+pow(syst,2));
  }
  
  std::pair<double,double> totalError() const{
    return std::pair<double,double>(sqrt(pow(stat,2)+pow(totalSyst().first,2)),
				    -sqrt(pow(stat,2)+pow(totalSyst().second,2)));
  }
  
  std::pair<double,double> totalSyst() const{
    return std::pair<double,double>(sqrt(pow(tauidSyst,2)+pow(syst,2)+pow(jerSyst.first,2)+pow(jesSyst.first,2)+pow(puSyst.first,2)),
				    -sqrt(pow(tauidSyst,2)+pow(syst,2)+pow(jerSyst.second,2)+pow(jesSyst.second,2)+pow(puSyst.second,2)));
  }

  double systSym() const {
    return std::max(fabs(totalSyst().first),fabs(totalSyst().second));
  }

  void operator+=(const events & rhs){
    this->number += rhs.number;
    this->stat = sqrt(pow(this->stat,2)+pow(rhs.stat,2));
    this->syst = sqrt(pow(this->syst,2)+pow(rhs.syst,2));
  }

  void operator-=(const events & rhs){
    this->number -= rhs.number;
    this->stat = sqrt(pow(this->stat,2)+pow(rhs.stat,2));
    this->syst = sqrt(pow(this->syst,2)+pow(rhs.syst,2));
  }

};

double sumSqErr(events evt1, events evt2){
  if (evt1.number==0 || evt2.number==0) return 0;
  return sqrt(pow(evt1.error()/evt1.number,2)+pow(evt2.error()/evt2.number,2));
}

struct efficiency{
  std::string name;
  events num;
  events den;

  double eff() const{
    if (den.number==0) return 0;
    return num.number/den.number;
  }

  double error() const{
    if (den.number==0) return 0;
    //return 1/den.number*sqrt(num.number*(1-eff()));
    return eff()*sqrt(pow(num.error()/num.number,2)+pow(den.error()/den.number,2)-2*(num.error()*den.error()/(num.number*den.number)));
  }

};

std::ostream & operator<<(std::ostream & aOs, const events & evt){
  aOs //<< "$" << evt.sample << " $ & "
      << "$" <<  evt.number
      << " \\pm " << evt.stat << " (stat) \\pm " << evt.syst << " (mcStat) $";// \\\\"
  //<< std::endl;
  return aOs;
}

std::ostream & operator<<(std::ostream & aOs, const efficiency & eff){
  aOs //<< "$\\" << eff.name << " $ & " 
      << "$" <<  eff.eff()
      << " \\pm " << eff.error()
      << "$ ";//\\\\"
  //<< std::endl;
  return aOs;
}

std::istream & operator>>(std::istream & is, events & evt){
  is>>evt.sample; 
  is>>evt.number;
  is>>evt.stat; 
  return is; 
}

typedef std::vector < events > evtsArray;

std::istream & operator>>(std::istream & is, evtsArray & evtVec){

  events temp; 
  while(1){
    is>>temp; 
    if(is.eof()){
      break; 
    }
    evtVec.push_back(temp); 

  }
  return is; 
}

int extractWJetsBkg(){//main

  std::vector<std::string> lSelVecSignal;
  lSelVecSignal.push_back("HLTMetClean");
  lSelVecSignal.push_back("LeptonVeto");
  lSelVecSignal.push_back("JetPair");
  lSelVecSignal.push_back("DEta");
  lSelVecSignal.push_back("MET");
  lSelVecSignal.push_back("TightMjj");
  lSelVecSignal.push_back("DPhiSIGNAL_noCJV");
  lSelVecSignal.push_back("DPhiQCD_noCJV");
  lSelVecSignal.push_back("CJVpass");
  lSelVecSignal.push_back("DPhiSIGNAL_CJVpass");
  lSelVecSignal.push_back("DPhiQCD_CJVpass");

  std::vector<std::string> lSelVecControl;
  lSelVecControl.push_back("HLTMetClean");
  lSelVecControl.push_back("WSelection");
  lSelVecControl.push_back("JetPair");
  lSelVecControl.push_back("DEta");
  lSelVecControl.push_back("MET");
  lSelVecControl.push_back("TightMjj");
  lSelVecControl.push_back("DPhiSIGNAL_noCJV");
  lSelVecControl.push_back("DPhiQCD_noCJV");
  lSelVecControl.push_back("CJVpass");
  lSelVecControl.push_back("DPhiSIGNAL_CJVpass");
  lSelVecControl.push_back("DPhiQCD_CJVpass");

  std::vector<std::string> lSelVecTau;
  lSelVecTau.push_back("HLTMetClean");
  lSelVecTau.push_back("WSelection");
  lSelVecTau.push_back("JetPair");
  lSelVecTau.push_back("DEta");
  lSelVecTau.push_back("MET");
  lSelVecTau.push_back("TightMjj");
  lSelVecTau.push_back("DPhiSIGNAL_noCJV");
  lSelVecTau.push_back("DPhiQCD_noCJV");
  lSelVecTau.push_back("CJVpass");
  lSelVecTau.push_back("DPhiSIGNAL_CJVpass");
  lSelVecTau.push_back("DPhiQCD_CJVpass");

  std::vector<std::string> latex;
  latex.push_back("HLTMetClean");
  latex.push_back("LepSel");
  latex.push_back("JetPair");
  latex.push_back("DEta");
  latex.push_back("MET");
  latex.push_back("TightMjj");
  latex.push_back("SIG noCJV");
  latex.push_back("QCD noCJV");
  latex.push_back("CJVpass");
  latex.push_back("SIG CJVpass");
  latex.push_back("QCD CJVpass");

  //std::string lSuffix = "";
  const unsigned nWeights = 4;
  std::string lSuffix[nWeights] = {"","_pu","_pu_trig","_pu_trig_idiso"};

  bool doTaus = false;
  bool docrosschecktau=false;
  bool dojes = false;
  bool dojer = false;
  bool doWeights = false;

  const unsigned nSysts = 7;
  const unsigned nCh = 4;
  std::string lChannel[nCh] = {"nunu","enu","munu","taunu"};

  std::string TOPDIR = "../TABLES/";
  //std::string TOPDIR = "../oldanalysisruns/080713_taunominaltightlepiddiscr/TABLES/";

  const unsigned nSteps = lSelVecSignal.size();

  //reserve two slots for the puup and down
  std::string SYST[nSysts] = {"JESUP","JESDOWN","JERBETTER","JERWORSE","","",""};//Order so numbers from systematics can be saved to be put in central table as syst errors
  std::string lSuffixSyst[nSysts] = {"","","","","_puUp","_puDown",""};

  unsigned MET[3] = {130,0,70};

  for (unsigned iW(0); iW<(doWeights?nWeights:1); ++iW){//loop on different weights

    std::cout << " -- Processing weight " << lSuffix[iW] << std::endl;

  for (unsigned iMET(0); iMET<1; ++iMET){//loop on MET values

    std::cout << " -- Processing MET " << MET[iMET] << std::endl;

    //create variables to store systB numbers
    double systdiff[2][nCh-1][nSysts-1][4];
    double systperc[2][nCh-1][nSysts-1][4];
    double totalsystupshift[2][nCh-1][4];//[iqcd][ich][lnsmc,lncmc,lnsdata,lncdata]
    double totalsystdownshift[2][nCh-1][4];
    double totalsystupperc[2][nCh-1][4];
    double totalsystdownperc[2][nCh-1][4];
    bool fileExists[nCh][nSysts];
    for (unsigned iCh(0); iCh<nCh;++iCh){
      for (unsigned iSyst(0); iSyst<nSysts; ++iSyst){
	fileExists[iCh][iSyst] = true;
      }
    }

    events systevents[2][nCh-1][nSysts][4];//[iqcd][ich][isyst][lnsmc,lncmc,lnsdata,lncdata]
    events result_nocjv[nSysts];
    events result[nSysts];

    int isyststart=0;
    if(!dojes&&!dojer){
      isyststart=nSysts-1;
    }
    for (unsigned iSyst(isyststart); iSyst<nSysts; ++iSyst){//loop over different systematics NOTE TO JUST DO CENTRAL CHANGE to iSyst(nSysts-1) don't do iSyst<1
      std::cout<< "--Processing systematic " <<iSyst<<std::endl;
      std::ostringstream lFolder;
      lFolder << "MET" << MET[iMET] << "/" << SYST[iSyst] << "/";
      
      evtsArray lSel[nCh][nSteps];
      
	for (unsigned iS(0); iS<nSteps; ++iS){//loop on steps
	  
	  std::ifstream lTable;
	  std::ostringstream lName;
	  
	  
	  lName << TOPDIR << "/nunu/" << lFolder.str() << "/SummaryTable_" << lSelVecSignal[iS] << lSuffix[iW] << lSuffixSyst[iSyst]  << ".dat";
	  lTable.open(lName.str().c_str());
	  if(!lTable.is_open()){
	    cerr<<"Unable to open file: "<<lName.str()<<endl;
	    fileExists[0][iSyst] = false;
	    //return 1; 
	  }
	  else {
	    lTable>>lSel[0][iS];
	    lTable.close();
	  }
	  
	  lName.str("");
	  lName << TOPDIR << "/enu/" << lFolder.str() << "/SummaryTable_" << lSelVecControl[iS] << lSuffix[iW] << lSuffixSyst[iSyst] << ".dat";
	  lTable.open(lName.str().c_str());
	  if(!lTable.is_open()){
	    cerr<<"Unable to open file: "<<lName.str()<<endl;
	    fileExists[1][iSyst] = false;
	    //return 1; 
	  }
	  else {
	    lTable>>lSel[1][iS];
	    lTable.close();
	  }
	  
	  lName.str("");
	  lName << TOPDIR << "/munu/" << lFolder.str() << "/SummaryTable_" << lSelVecControl[iS] << lSuffix[iW] << lSuffixSyst[iSyst] << ".dat";
	  lTable.open(lName.str().c_str());
	  if(!lTable.is_open()){
	    cerr<<"Unable to open file: "<<lName.str()<<endl;
	    fileExists[2][iSyst] = false;
	    //return 1; 
	  }
	  else {
	    lTable>>lSel[2][iS];
	    lTable.close();
	  }

	  if (doTaus){
	    //if (iS==nSteps-1) continue;
	    lName.str("");
	    lName << TOPDIR << "/taunu/" << lFolder.str() << "/SummaryTable_" << lSelVecTau[iS] << lSuffix[iW] << lSuffixSyst[iSyst] << ".dat";
	    lTable.open(lName.str().c_str());
	    if(!lTable.is_open()){
	      cerr<<"Unable to open file: "<<lName.str()<<endl;
	      fileExists[3][iSyst] = false;
	      //return 1; 
	    }
	    else {
	      lTable>>lSel[3][iS];
	      lTable.close();
	    }
	  }
	}//loop on steps
	
	
	std::cout << " -- TABLES have been read in." << std::endl;

	//print Wjets components
	for (unsigned iCh(0); iCh< (doTaus ? 4 : 3); ++iCh){//loop on channel
	  if (!fileExists[iCh][iSyst]) continue;
	  std::cout << " -- Processing channel: " << lChannel[iCh] << std::endl;

	  std::ostringstream lName;
	  lName << TOPDIR <<"/" << lChannel[iCh] << "/MET" << MET[iMET] << "/" << SYST[iSyst] << "/WJetsTable" << lSuffix[iW] << lSuffixSyst[iSyst] << ".txt";
	  
	  std::ofstream lOutfile;
	  lOutfile.open(lName.str().c_str());
	  
	  lOutfile << "\\begin{tabular}{|l|c|c|c|c|c|c|}" << std::endl
		   << "\\hline" << std::endl
		   << "Step & QCD W$\\rightarrow e\\nu$ & EWK W$\\rightarrow e\\nu$ & QCD W$\\rightarrow\\mu\\nu$ & EWK W$\\rightarrow\\mu\\nu$ & QCD W$\\rightarrow\\tau\\nu$ & EWK W$\\rightarrow\\tau\\nu$  \\\\" << std::endl
		   << "\\hline" << std::endl;

	  for (unsigned iS(5); iS<nSteps; ++iS){//loop on steps

	    lOutfile << latex[iS];
	    lOutfile << std::setprecision(3) << " &  $"
		     << lSel[iCh][iS][QCD_WJets_enu].number  << "\\pm" << lSel[iCh][iS][QCD_WJets_enu].error() << "$ & $" 
		     << lSel[iCh][iS][EWK_WJets_enu].number  << "\\pm" << lSel[iCh][iS][EWK_WJets_enu].error() << "$ & $" 
		     << lSel[iCh][iS][QCD_WJets_munu].number  << "\\pm" << lSel[iCh][iS][QCD_WJets_munu].error()  << "$ & $" 
		     << lSel[iCh][iS][EWK_WJets_munu].number  << "\\pm" << lSel[iCh][iS][EWK_WJets_munu].error()  << "$ & $" 
		     << lSel[iCh][iS][QCD_WJets_taunu].number  << "\\pm" << lSel[iCh][iS][QCD_WJets_taunu].error() << "$ & $" 
		     << lSel[iCh][iS][EWK_WJets_taunu].number  << "\\pm" << lSel[iCh][iS][EWK_WJets_taunu].error() 
		     << "$ \\\\" << std::endl;
	  }//loop on steps
	  lOutfile << "\\hline" << std::endl
		   << "\\end{tabular}" << std::endl;
	  lOutfile.close();
	  
	  lName.str("");
	  lName << TOPDIR <<"/" << lChannel[iCh] << "/MET" << MET[iMET] << "/" << SYST[iSyst] << "/ZJetsTable" << lSuffix[iW] << lSuffixSyst[iSyst] << ".txt";
	  
	  lOutfile.open(lName.str().c_str());
	  
	  lOutfile << "\\begin{tabular}{|l|c|c|c|c|}" << std::endl
		   << "\\hline" << std::endl
		   << "Step & Z$\\rightarrow\\nu\\nu$ & EWK Z->nunu+2j  & Z$\\rightarrow$ll & EWK Z->ll+2j \\\\" << std::endl
		   << "\\hline" << std::endl;

	  for (unsigned iS(5); iS<nSteps; ++iS){//loop on steps

	    lOutfile << latex[iS];
	    lOutfile << std::setprecision(3) << " &  $"
		     << lSel[iCh][iS][ZJets_nunu].number  << "\\pm" << lSel[iCh][iS][ZJets_nunu].error() << "$ & $" 
		     << lSel[iCh][iS][ZJets_vbf_nunu].number  << "\\pm" << lSel[iCh][iS][ZJets_vbf_nunu].error() << "$ & $" 
		     << lSel[iCh][iS][ZJets_ll].number  << "\\pm" << lSel[iCh][iS][ZJets_ll].error() << "$ & $" 
		     << lSel[iCh][iS][ZJets_vbf].number-lSel[iCh][iS][ZJets_vbf_nunu].number << "\\pm" 
		     << sqrt(pow(lSel[iCh][iS][ZJets_vbf].error(),2)-pow(lSel[iCh][iS][ZJets_vbf_nunu].error(),2))
		     << "$ \\\\" << std::endl;
	  }//loop on steps
	  lOutfile << "\\hline" << std::endl
		   << "\\end{tabular}" << std::endl;
	  lOutfile.close();

	  lName.str("");
	  lName << TOPDIR <<"/" << lChannel[iCh] << "/MET" << MET[iMET] << "/" << SYST[iSyst] << "/TopTable" << lSuffix[iW] << lSuffixSyst[iSyst] << ".txt";
	  
	  lOutfile.open(lName.str().c_str());
	  
	  
	  lOutfile << "\\begin{tabular}{|l|c|c|c|c|}" << std::endl
		   << "\\hline" << std::endl
		   << "Step & $t\\bar{t}$ & $t\\bar{t}$ POWHEG & single top & $t,\\bar{t}\\rightarrow tW$  \\\\" << std::endl
		   << "\\hline" << std::endl;


	  for (unsigned iS(5); iS<nSteps; ++iS){//loop on steps

	    lOutfile << latex[iS];
	    lOutfile << std::setprecision(3) << " &  $"
		     << lSel[iCh][iS][TTbar].number  << "\\pm" << lSel[iCh][iS][TTbar].error()  << "$ & $" 
		     << lSel[iCh][iS][TTbarPowheg].number  << "\\pm" << lSel[iCh][iS][TTbarPowheg].error()  << "$ & $" 
		     << lSel[iCh][iS][SingleTop].number  << "\\pm" << lSel[iCh][iS][SingleTop].error()  << "$ & $" 
		     << lSel[iCh][iS][TW].number  << "\\pm" << lSel[iCh][iS][TW].error() << "$ \\\\" << std::endl;
	  }//loop on steps

	  lOutfile << "\\hline" << std::endl
		   << "\\end{tabular}" << std::endl;
	  lOutfile.close();
	  
	}//loop on channel
	

	if (doTaus){
	  if (!fileExists[3][iSyst] || !fileExists[0][iSyst]) continue;
	  std::cout << " -- Extracting tau results" << std::endl;

	  events nData = lSel[3][DPhiSIGNAL_noCJV][Data];
	  events nBkg = lSel[3][DPhiSIGNAL_noCJV][Top];
	  nBkg += lSel[3][DPhiSIGNAL_noCJV][ZJets];
	  nBkg += lSel[3][DPhiSIGNAL_noCJV][VV];
	  events nBkg_cjv = lSel[3][DPhiSIGNAL_CJVpass][Top];
	  nBkg_cjv += lSel[3][DPhiSIGNAL_CJVpass][ZJets];
	  nBkg_cjv += lSel[3][DPhiSIGNAL_CJVpass][VV];
	  efficiency eps_tau;
	  eps_tau.num = lSel[3][DPhiSIGNAL_noCJV][QCD_WJets_taunu];
	  eps_tau.num += lSel[3][DPhiSIGNAL_noCJV][EWK_WJets_taunu];
	  eps_tau.den = lSel[0][DPhiSIGNAL_noCJV][QCD_WJets_taunu];
	  eps_tau.den += lSel[0][DPhiSIGNAL_noCJV][EWK_WJets_taunu];
	  efficiency eps_tau_noew;
	  eps_tau_noew.num = lSel[3][DPhiSIGNAL_noCJV][QCD_WJets_taunu];
	  eps_tau_noew.den = lSel[0][DPhiSIGNAL_noCJV][QCD_WJets_taunu];
	  efficiency eps_tau_cjvpass;
	  eps_tau_cjvpass.num = lSel[3][DPhiSIGNAL_CJVpass][QCD_WJets_taunu];
	  eps_tau_cjvpass.num += lSel[3][DPhiSIGNAL_CJVpass][EWK_WJets_taunu];
	  eps_tau_cjvpass.den = lSel[0][DPhiSIGNAL_CJVpass][QCD_WJets_taunu];
	  eps_tau_cjvpass.den += lSel[0][DPhiSIGNAL_CJVpass][EWK_WJets_taunu];
	  efficiency eps_tau_cjvpass_noew;
	  eps_tau_cjvpass_noew.num = lSel[3][DPhiSIGNAL_CJVpass][QCD_WJets_taunu];
	  eps_tau_cjvpass_noew.den = lSel[0][DPhiSIGNAL_CJVpass][QCD_WJets_taunu];

	  efficiency eps_tau_cjv;
	  eps_tau_cjv.num = lSel[0][DPhiSIGNAL_CJVpass][QCD_WJets_taunu];
	  eps_tau_cjv.num += lSel[0][DPhiSIGNAL_CJVpass][EWK_WJets_taunu];
	  eps_tau_cjv.den.number = eps_tau.den.number;
	  eps_tau_cjv.den.stat = eps_tau.den.stat;
	  efficiency eps_tau_cjv_noew;
	  eps_tau_cjv_noew.num = lSel[0][DPhiSIGNAL_CJVpass][QCD_WJets_taunu];
	  eps_tau_cjv_noew.den.number = eps_tau.den.number;
	  eps_tau_cjv_noew.den.stat = eps_tau.den.stat;

	  events nDataW = nData;
	  nDataW -= nBkg;
	  if(docrosschecktau){
	    nDataW -= lSel[3][DPhiSIGNAL_noCJV][QCD_WJets_enu];
	    nDataW -= lSel[3][DPhiSIGNAL_noCJV][EWK_WJets_enu];
	  }
	  result_nocjv[iSyst] = nDataW;
	  result_nocjv[iSyst].number = nDataW.number/eps_tau.eff();
	  result_nocjv[iSyst].stat = nDataW.stat;
	  result_nocjv[iSyst].syst = result_nocjv[iSyst].number*sqrt(pow(eps_tau.error()/eps_tau.eff(),2));
	  result_nocjv[iSyst].tauidSyst = 0.08*result_nocjv[iSyst].number;

	  events effProduct;
	  effProduct.number = eps_tau_cjv.num.number / eps_tau.num.number;
	  effProduct.stat = effProduct.number*sqrt(pow(eps_tau_cjv.num.stat/eps_tau_cjv.num.number,2)+pow(eps_tau.num.stat/eps_tau.num.number,2));
	  result[iSyst] = nDataW;
	  result[iSyst].number = nDataW.number*effProduct.number;
	  result[iSyst].stat = result[iSyst].number*sqrt(pow(nDataW.stat/nDataW.number,2));

	  result[iSyst].syst=sqrt(pow(effProduct.stat/effProduct.number,2))*result[iSyst].number;
	  result[iSyst].tauidSyst = 0.08*result[iSyst].number;

	  events result_nocjv_noew = nDataW;
	  result_nocjv_noew.number = nDataW.number/eps_tau_noew.eff();
	  result_nocjv_noew.stat = nDataW.stat;
	  result_nocjv_noew.syst = nDataW.number*eps_tau_noew.error()/pow(eps_tau_noew.eff(),2);

	  events effProduct_noew;
	  effProduct_noew.number = eps_tau_cjv_noew.num.number / eps_tau_noew.num.number;
	  effProduct_noew.stat = effProduct_noew.number*sqrt(pow(eps_tau_cjv_noew.num.stat/eps_tau_cjv_noew.num.number,2)+pow(eps_tau_noew.num.stat/eps_tau_noew.num.number,2));
	  events result_noew = nDataW;
	  result_noew.number = nDataW.number*effProduct_noew.number;
	  result_noew.stat = result_noew.number*sqrt(pow(nDataW.stat/nDataW.number,2));

	  result_noew.syst=sqrt(pow(effProduct_noew.stat/effProduct_noew.number,2)+pow(0.08,2))*result_noew.number;

	  //AMM- commenting this for the moment
	  // 	  if(!docrosschecktau){
	  // 	    wenusyst=lSel[3][DPhiSIGNAL_noCJV][WJets_enu].number;
	  // 	    totsyst=sqrt(pow(multsyst,2)+pow(wenusyst,2));
	  // 	  }
	  // 	  else{
	  // 	    totsyst=multsyst;
	  // 	  }

	  std::ostringstream lName;
	  lName.str("");
	  lName << TOPDIR <<"/" << lChannel[3] << "/MET" << MET[iMET] << "/" << SYST[iSyst] << "/DataDrivenWJetsTable_signal" << lSuffix[iW] << lSuffixSyst[iSyst] << ".txt";
	  
 	  std::ofstream lOutfile;
 	  lOutfile.open(lName.str().c_str());
	  
 	  lOutfile << "\\begin{tabular}{|l|c|c|c|c|}" << std::endl
 		   << "\\hline" << std::endl
 		   << " & \\multicolumn{2}{c|}{no CJV} & \\multicolumn{2}{c|}{CJV} \\\\" << std::endl
 		   << "\\hline" << std::endl
 		   << " & $\\nu\\nu$ & $\\tau\\nu$ & $\\nu\\nu$ & $\\tau\\nu$ \\\\" << std::endl
		   << "\\hline" << std::endl
		   << std::setprecision(3)
 		   << "nData & - & $" << nData.number  << "\\pm " << nData.error()  << "$ & - & " << lSel[3][DPhiSIGNAL_CJVpass][Data] << "  \\\\" << std::endl
 		   << "nBkg &  & $" << nBkg.number  << "\\pm " << nBkg.error()  << "$ & - & " << nBkg_cjv << " \\\\" << std::endl
		   << "\\hline" << std::endl
 		   << "n(W$\\rightarrow \\tau \\nu$) & " << eps_tau.den << " & " << eps_tau.num << " & " << eps_tau_cjvpass.den << " & " <<eps_tau_cjvpass.num << " \\\\" << std::endl
		   << "\\hline" << std::endl
 		   << "$\\varepsilon_{tauID}$ & \\multicolumn{2}{c|}{" << eps_tau << "} & \\multicolumn{2}{c|}{" << eps_tau_cjvpass << "} \\\\" << std::endl
		   << "\\hline" << std::endl
 		   << "$\\varepsilon_{CJV}$ &  \\multicolumn{4}{c|}{ " << eps_tau_cjv << " }\\\\" <<  std::endl
		   << "\\hline" << std::endl
 		   << "W$\\rightarrow \\tau\\nu$ estimates & \\multicolumn{2}{c|}{ $" << result_nocjv[iSyst].number  << "\\pm " << result_nocjv[iSyst].stat << "(stat) \\pm " << result_nocjv[iSyst].syst << "(mcStat) $} & \\multicolumn{2}{c|}{ $" << result[iSyst].number  << "\\pm " << result[iSyst].stat << "(stat) \\pm " << result[iSyst].syst << "(mcStat) $} \\\\" << std::endl
		   << "\\hline" << std::endl
		   << "\\hline" << std::endl
 		   << " \\multicolumn{5}{|c|}{Without EWK W+2j included}\\\\" <<  std::endl
		   << "\\hline" << std::endl
		   << "n(W$\\rightarrow \\tau \\nu$) & " << eps_tau_noew.den << " & " << eps_tau_noew.num << " & " << eps_tau_cjvpass_noew.den << " & " <<eps_tau_cjvpass_noew.num << " \\\\" << std::endl
		   << "\\hline" << std::endl
 		   << "$\\varepsilon_{tauID}$ & \\multicolumn{2}{c|}{" << eps_tau_noew << "} & \\multicolumn{2}{c|}{" << eps_tau_cjvpass_noew << "} \\\\" << std::endl
		   << "\\hline" << std::endl
 		   << "$\\varepsilon_{CJV}$ &  \\multicolumn{4}{c|}{ " << eps_tau_cjv_noew << " }\\\\" <<  std::endl
		   << "\\hline" << std::endl
 		   << "W$\\rightarrow \\tau\\nu$ estimates & \\multicolumn{2}{c|}{ $" << result_nocjv_noew.number  << "\\pm " << result_nocjv_noew.stat << "(stat) \\pm " << result_nocjv_noew.syst << "(mcStat) $} & \\multicolumn{2}{c|}{ $" << result_noew.number  << "\\pm " << result_noew.stat << "(stat) \\pm " << result_noew.syst << "(mcStat) $} \\\\" << std::endl
 		   << "\\hline" << std::endl
 		   << "\\end{tabular}" << std::endl;
 	  lOutfile.close();

// 	    //<< "eps_tau_id:" << eps_tau.error()/eps_tau.eff()*result.number <<std::endl
// 	    //   << "eps_tau_cjv:" << eps_tau_cjv.error()/eps_tau_cjv.eff()*result.number<<std::endl
// 	    //   << "data/mc scale factor:"<<0.08*result.number<<std::endl
// 	    //      << "remaining W->enu:"<<wenusyst<<std::endl
 
	  
	}//do taus


	for (unsigned int iQCD(0); iQCD<2; ++iQCD){//DphiSignal or DphiQCD
	  if(iQCD==0)std::cout<<" ----- dphisignal -----"<<std::endl;
	  if(iQCD==1)std::cout<<" ----- dphiqcd -----"<<std::endl;
	  for (unsigned iCh(1); iCh<3; ++iCh){//loop on lep flavour

	    if (!fileExists[iCh][iSyst] || !fileExists[0][iSyst]) continue;

	    if(iCh==1)std::cout<<" - enu"<<std::endl;
	    if(iCh==2)std::cout<<" - munu"<<std::endl;

	    events lNCdata;
	    events lBkgCMC;
	    events lBkgSMC;
	    events lNCMC;
	    events lNSMC;
	    // std::cout << lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][Data] 
	    // 	    << " " << lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][Top]
	    // 	    << " " << lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][VV]
	    // 	    << " " << lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][ZJets]
	    // 	    << std::endl;
	    
	    lNCdata = lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][Data];
	    lNCdata -= lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][Top];
	    lNCdata -= lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][VV];
	    lNCdata -= lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][ZJets];
	    lBkgCMC = lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][Top];
	    lBkgCMC += lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][VV];
	    lBkgCMC += lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][ZJets];
	    lBkgCMC.sample = "$N_{EWKbkg}$";
	    lBkgSMC = lSel[0][DPhiSIGNAL_CJVpass+iQCD][Top];
	    lBkgSMC += lSel[0][DPhiSIGNAL_CJVpass+iQCD][VV];
	    lBkgSMC += lSel[0][DPhiSIGNAL_CJVpass+iQCD][ZJets];
	    lBkgSMC.sample = "$N_{EWKbkg}$";
	    
	    efficiency eps_lepveto_S;
	    eps_lepveto_S.name = "$\\varepsilon_{lepsel}$";
	    efficiency eps_VBF_S;
	    eps_VBF_S.name = "$\\varepsilon_{VBF}$";
	    efficiency eps_lepsel_C;
	    eps_lepsel_C.name = "$\\varepsilon_{lepsel}$";
	    efficiency eps_VBF_C;
	    eps_VBF_C.name = "$\\varepsilon_{VBF}$";
	    events lNSdata;
	    
	    unsigned shiftSample = 0;
	    if (iCh==1) shiftSample = 2;

	    lNSMC = lSel[0][DPhiSIGNAL_CJVpass+iQCD][QCD_WJets_munu-shiftSample];
	    lNSMC += lSel[0][DPhiSIGNAL_CJVpass+iQCD][EWK_WJets_munu-shiftSample];
	    lNCMC = lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][QCD_WJets_munu-shiftSample];
	    lNCMC += lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][EWK_WJets_munu-shiftSample];

	    if (iCh==2){
	      lNCdata.sample = "$N_{W\\rightarrow \\mu\\nu}^{data}$";
	      lNSdata.sample = "$N_{W\\rightarrow \\mu\\nu}^{data}$";
	      lNSMC.sample = "$N_{W\\rightarrow \\mu\\nu}^{MC}$";
	      lNCMC.sample = "$N_{W\\rightarrow \\mu\\nu}^{MC}$";
	    }
	    else {
	      lNCdata.sample = "$N_{W\\rightarrow e\\nu}^{data}$";
	      lNSdata.sample = "$N_{W\\rightarrow e\\nu}^{data}$";
	      lNSMC.sample = "$N_{W\\rightarrow e\\nu}^{MC}$";
	      lNCMC.sample = "$N_{W\\rightarrow e\\nu}^{MC}$";
	    }
	    
	    eps_lepveto_S.num = lSel[0][Lep][QCD_WJets_munu-shiftSample];
	    eps_lepveto_S.num += lSel[0][Lep][EWK_WJets_munu-shiftSample];
	    eps_lepveto_S.den = lSel[0][Trig][QCD_WJets_munu-shiftSample];
	    eps_lepveto_S.den += lSel[0][Trig][EWK_WJets_munu-shiftSample];
	    
	    eps_VBF_S.num = lSel[0][DPhiSIGNAL_CJVpass+iQCD][QCD_WJets_munu-shiftSample];
	    eps_VBF_S.num += lSel[0][DPhiSIGNAL_CJVpass+iQCD][EWK_WJets_munu-shiftSample];
	    eps_VBF_S.den = lSel[0][Lep][QCD_WJets_munu-shiftSample];
	    eps_VBF_S.den += lSel[0][Lep][EWK_WJets_munu-shiftSample];
	    
	    eps_lepsel_C.num = lSel[iCh][Lep][QCD_WJets_munu-shiftSample];
	    eps_lepsel_C.num += lSel[iCh][Lep][EWK_WJets_munu-shiftSample];
	    eps_lepsel_C.den = lSel[iCh][Trig][QCD_WJets_munu-shiftSample];
	    eps_lepsel_C.den += lSel[iCh][Trig][EWK_WJets_munu-shiftSample];
	    
	    eps_VBF_C.num = lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][QCD_WJets_munu-shiftSample];
	    eps_VBF_C.num += lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][EWK_WJets_munu-shiftSample];
	    eps_VBF_C.den = lSel[iCh][Lep][QCD_WJets_munu-shiftSample];
	    eps_VBF_C.den += lSel[iCh][Lep][EWK_WJets_munu-shiftSample];
	    
	    lNSdata.number = lNCdata.number * lNSMC.number / lNCMC.number;
	    lNSdata.stat = lNCdata.stat* lNSMC.number / lNCMC.number;
	    lNSdata.syst = lNSdata.number * sqrt(pow(lNCMC.stat/lNCMC.number,2)+pow(lNSMC.stat/lNSMC.number,2));
	    
	    
	    
	    //SYSTEMATIC INFORMATION
	    if(dojes&&dojer){
	      //save JES and JER numbers
	      systevents[iQCD][iCh][iSyst][0]=lNSMC;
	      systevents[iQCD][iCh][iSyst][1]=lNCMC;
	      systevents[iQCD][iCh][iSyst][2]=lNSdata;
	      systevents[iQCD][iCh][iSyst][3]=lNCdata;
	      if(iSyst==nSysts-1){//get differences
		for(int a=0;a<6;a++){//loop over jes up and down
		  if(a==0)std::cout<<"JESUP"<<std::endl;
		  if(a==1)std::cout<<"JESDOWN"<<std::endl;
		  if(a==2)std::cout<<"JERBETTER"<<std::endl;
		  if(a==3)std::cout<<"JERWORSE"<<std::endl;
		  if(a==4)std::cout<<"PUUP"<<std::endl;
		  if(a==5)std::cout<<"PUDOWN"<<std::endl;
		  for(int b=0;b<4;b++){//loop over lns/cmc/data to get differences and percentage differences
		    systdiff[iQCD][iCh][a][b]=(systevents[iQCD][iCh][a][b].number-systevents[iQCD][iCh][nSysts-1][b].number);
		    systperc[iQCD][iCh][a][b]=(systevents[iQCD][iCh][a][b].number-systevents[iQCD][iCh][nSysts-1][b].number)*100/systevents[iQCD][iCh][nSysts-1][b].number;
		    std::cout<<systevents[iQCD][iCh][a][b].number<<" "<<systevents[iQCD][iCh][nSysts-1][b].number<<" "<<systdiff[iQCD][iCh][a][b]<<std::endl;
		  }
		}
		//get max up shift and down shift
		for(int b=0;b<4;b++){
		  double maxjesupshift=std::max(std::max(systdiff[iQCD][iCh][0][b],0.),std::max(systdiff[iQCD][iCh][1][b],0.));
		  double maxjesdownshift=std::min(std::min(systdiff[iQCD][iCh][0][b],0.),std::min(systdiff[iQCD][iCh][1][b],0.));
		  double maxjerupshift=std::max(std::max(systdiff[iQCD][iCh][2][b],0.),std::max(systdiff[iQCD][iCh][3][b],0.));
		  double maxjerdownshift=std::min(std::min(systdiff[iQCD][iCh][2][b],0.),std::min(systdiff[iQCD][iCh][3][b],0.));
		  double maxpuupshift=std::max(std::max(systdiff[iQCD][iCh][4][b],0.),std::max(systdiff[iQCD][iCh][5][b],0.));
		  double maxpudownshift=std::min(std::min(systdiff[iQCD][iCh][4][b],0.),std::min(systdiff[iQCD][iCh][5][b],0.));
		  //add max jesup/downshift and jerup/downshift in quadrature
		  totalsystupshift[iQCD][iCh][b] = sqrt(maxjesupshift*maxjesupshift+maxjerupshift*maxjerupshift+maxpuupshift*maxpuupshift);
		  totalsystdownshift[iQCD][iCh][b] = -1*sqrt(maxjesdownshift*maxjesdownshift+maxjerdownshift*maxjerdownshift+maxpudownshift*maxpudownshift);
		  totalsystupperc[iQCD][iCh][b]=totalsystupshift[iQCD][iCh][b]*100/systevents[iQCD][iCh][nSysts-1][b].number;
		  totalsystdownperc[iQCD][iCh][b]=totalsystdownshift[iQCD][iCh][b]*100/systevents[iQCD][iCh][nSysts-1][b].number;
		}
	      }
	    }
	    
	    //TOTAL SUMMARY TABLE WITH TOTAL SYST ERRORS
	    std::ostringstream lName;
	    lName << TOPDIR << "/" << lChannel[iCh] << "/MET" << MET[iMET] << "/" << SYST[iSyst] ;

	    if (iQCD==0) lName << "/DataDrivenWJetsTable_signal" << lSuffix[iW] << lSuffixSyst[iSyst] << ".txt";
	    else if (iQCD==1) lName << "/DataDrivenWJetsTable_QCD" << lSuffix[iW] << lSuffixSyst[iSyst] << ".txt";
	    std::ofstream lOutfile;
	    lOutfile.open(lName.str().c_str());
	    lOutfile << "\\begin{tabular}{|l|c|c|}" << std::endl
		     << "\\hline" << std::endl
		     << " & Signal sample & Control sample \\\\" << std::endl
		     << "\\hline" << std::endl
		     << "$N_{data}$ & XXX & " << lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][Data].number << "\\\\" << std::endl;
	    lOutfile << std::setprecision(3)
		     << lBkgSMC.sample << " & " 
		     << " n/a & " 
		     << lBkgCMC << " \\\\" << std::endl
	      //<< eps_lepveto_S.name  << " & " 
	      //<< eps_lepveto_S  << " & " 
	      //<< eps_lepsel_C << " \\\\" << std::endl
	      //<< eps_VBF_S.name  << " & " 
	      //<< eps_VBF_S  << " & " 
	      //<< eps_VBF_C << " \\\\" << std::endl
		     << std::setprecision(3)
		     << lNSMC.sample  << " & " 
		     << lNSMC; 
	    if((dojer&&dojes)&&iSyst==nSysts-1){
	      lOutfile << " $ ^{ + " << totalsystupshift[iQCD][iCh][0] << " ( + " << totalsystupperc[iQCD][iCh][0] << "\\% ) } ";
	      lOutfile << " _{ " << totalsystdownshift[iQCD][iCh][0] << " ( " << totalsystdownperc[iQCD][iCh][0] << "\\% ) } $ ";
	    }
	    lOutfile << " & " 
		     << lNCMC; 
	    if((dojer&&dojes)&&iSyst==nSysts-1){
	      lOutfile << " $ ^{ + " << totalsystupshift[iQCD][iCh][1] << " ( + " << totalsystupperc[iQCD][iCh][1] << "\\% ) } ";
	      lOutfile << " _{ " << totalsystdownshift[iQCD][iCh][1] << " ( " << totalsystdownperc[iQCD][iCh][1] << "\\% ) } $ ";
	    }
	    lOutfile << " \\\\" << std::endl
		     << lNSdata.sample << " & " 
		     << "\\textcolor{red}{" << lNSdata;
	    if((dojer&&dojes)&&iSyst==nSysts-1){
	      lOutfile << " $ ^{ + " << totalsystupshift[iQCD][iCh][2] << " ( + " << totalsystupperc[iQCD][iCh][2] << "\\% ) } ";
	      lOutfile << " _{ " << totalsystdownshift[iQCD][iCh][2] << " ( " << totalsystdownperc[iQCD][iCh][2] << "\\% ) } $ ";
	    }
	    lOutfile << "} & " 
		     << lNCdata;
	    if((dojer&&dojes)&&iSyst==nSysts-1){
	      lOutfile << " $ ^{ + " << totalsystupshift[iQCD][iCh][3] << " ( + " << totalsystupperc[iQCD][iCh][3] << "\\% ) } ";
	      lOutfile << " _{ " << totalsystdownshift[iQCD][iCh][3] << " ( " << totalsystdownperc[iQCD][iCh][3] << "\\% ) } $ ";
	    }
	    lOutfile << " \\\\ " << std::endl 
		     << "\\hline" << std::endl
	      //<< "with Eff & " << result << " & \\\\" << std::endl
	      //	   << "\\hline" << std::endl
		     << "\\end{tabular}" << std::endl; 
	    lOutfile.close();

	    std::cout << "*** " << lSuffix[iW] << lSuffixSyst[iSyst] << ": result = " << lNSdata << " MC: " << lNSMC << std::endl;
	    
	  }//loop on lep flavour
	  
	}//loop on signal region

	//SUMMARY TABLES WITH SYST ERRORS SEPARATE
	if (iSyst == nSysts-1 && dojer && dojes){
	  for(int iSample=0;iSample<4;iSample++){
	    std::ostringstream lName;
	    lName << TOPDIR << "/MET" << MET[iMET] << "DataDrivenWJetsSystErrorTable";
	    if(iSample==0) lName <<"lNSMC";
	    if(iSample==1) lName <<"lNCMC";
	    if(iSample==2) lName <<"lNSdata";
	    if(iSample==3) lName <<"lNCdata";
	    lName << lSuffix[iW] << lSuffixSyst[iSyst] << ".txt";
	    std::ofstream lOutfile;
	    lOutfile.open(lName.str().c_str());
	    lOutfile << "\\begin{tabular}{l c c c c}" << std::endl
		     << "\\hline" << std::endl << " "
		     << systevents[0][1][nSysts-1][iSample].sample
		     << " & Electron $d\\phi < 1.0$ & Electron $d\\phi>2.6$ & Muon $d\\phi < 1.0$ & Muon $d\\phi > 2.6$ \\\\" << std::endl
		     << "\\hline" << std::endl;
	    lOutfile << std::setprecision(3)//central value
		     << "Central num. of events"  << " & " 
		     << systevents[0][1][nSysts-1][iSample].number << " & "
		     << systevents[1][1][nSysts-1][iSample].number << " & "
		     << systevents[0][2][nSysts-1][iSample].number << " & "
		     << systevents[1][2][nSysts-1][iSample].number << " \\\\" << std::endl;
	    lOutfile << "Statistical"  << " & " 
		     << "$\\pm$" << systevents[0][1][nSysts-1][iSample].error()*100/systevents[0][1][nSysts-1][iSample].number << "\\% & "
		     << "$\\pm$" << systevents[1][1][nSysts-1][iSample].error()*100/systevents[1][1][nSysts-1][iSample].number << "\\% & "
		     << "$\\pm$" << systevents[0][2][nSysts-1][iSample].error()*100/systevents[0][2][nSysts-1][iSample].number << "\\% & "
		     << "$\\pm$" << systevents[1][2][nSysts-1][iSample].error()*100/systevents[1][2][nSysts-1][iSample].number << "\\% \\\\" << std::endl;
	    lOutfile << "JESUP"  << " & "; 
	    lOutfile << showpos;
	    lOutfile << systperc[0][1][0][iSample] << "\\% & "
		     << systperc[1][1][0][iSample] << "\\% & "
		     << systperc[0][2][0][iSample] << "\\% & "
		     << systperc[1][2][0][iSample] << "\\% \\\\" << std::endl;
	    lOutfile << "JESDOWN"  << " & " 
		     << systperc[0][1][1][iSample] << "\\% & "
		     << systperc[1][1][1][iSample] << "\\% & "
		     << systperc[0][2][1][iSample] << "\\% & "
		     << systperc[1][2][1][iSample] << "\\% \\\\" << std::endl;
	    lOutfile << "JERBETTER"  << " & " 
		     << systperc[0][1][2][iSample] << "\\% & "
		     << systperc[1][1][2][iSample] << "\\% & "
		     << systperc[0][2][2][iSample] << "\\% & "
		     << systperc[1][2][2][iSample] << "\\% \\\\" << std::endl;
	    lOutfile << "JERWORSE"  << " & "
		     << systperc[0][1][3][iSample] << "\\% & "
		     << systperc[1][1][3][iSample] << "\\% & "
		     << systperc[0][2][3][iSample] << "\\% & "
		     << systperc[1][2][3][iSample] << "\\% \\\\" << std::endl;
	    lOutfile << "PUUP"  << " & "
		     << systperc[0][1][4][iSample] << "\\% & "
		     << systperc[1][1][4][iSample] << "\\% & "
		     << systperc[0][2][4][iSample] << "\\% & "
		     << systperc[1][2][4][iSample] << "\\% \\\\" << std::endl;
	    lOutfile << "PUDOWN"  << " & " 
		     << systperc[0][1][5][iSample] << "\\% & "
		     << systperc[1][1][5][iSample] << "\\% & "
		     << systperc[0][2][5][iSample] << "\\% & "
		     << systperc[1][2][5][iSample] << "\\% \\\\" << std::endl;
	    lOutfile << noshowpos;
	    lOutfile << "\\hline" << std::endl
		     << "\\end{tabular}" << std::endl; 
	    lOutfile << noshowpos;
	    lOutfile.close();
	  }
	}//if dojes/jer
	
	
    }//loop on systematics
    
    //print tau systematics
    std::cout << " -- Tau results with systematics" << std::endl;
    double jesUp = result_nocjv[0].number - result_nocjv[nSysts-1].number;
    double jesDown = result_nocjv[1].number - result_nocjv[nSysts-1].number;
    //select the maximum positive value
    result_nocjv[nSysts-1].jesSyst.first = std::max(std::max(jesUp,0.),std::max(jesDown,0.));
    //select the minimum negative value
    result_nocjv[nSysts-1].jesSyst.second = std::min(std::min(jesUp,0.),std::min(jesDown,0.));
    double jerBetter = result_nocjv[2].number - result_nocjv[nSysts-1].number;
    double jerWorse = result_nocjv[3].number - result_nocjv[nSysts-1].number;
    result_nocjv[nSysts-1].jerSyst.first = std::max(std::max(jerBetter,0.),std::max(jerWorse,0.));
    result_nocjv[nSysts-1].jerSyst.second = std::min(std::min(jerBetter,0.),std::min(jerWorse,0.));
    double puUp = result_nocjv[4].number - result_nocjv[nSysts-1].number;
    double puDown = result_nocjv[5].number - result_nocjv[nSysts-1].number;
    result_nocjv[nSysts-1].puSyst.first = std::max(std::max(puUp,0.),std::max(puDown,0.));
    result_nocjv[nSysts-1].puSyst.second = std::min(std::min(puUp,0.),std::min(puDown,0.));

    std::cout << result_nocjv[0].number << " "
	      << result_nocjv[1].number << " "
	      << result_nocjv[2].number << " "
	      << result_nocjv[3].number << " "
	      << result_nocjv[4].number << std::endl;
    std::cout << jesUp << " " << jesDown << " " << jerBetter << " " << jerWorse << std::endl;

    std::cout << "\\% Systematics & stat & MCstat & tauID & JES & JER & PU \\\\" << std::endl 
	      << "Result_noCJV & " 
	       << std::setprecision(3)
	      << result_nocjv[nSysts-1].stat << " & " 
	      << result_nocjv[nSysts-1].syst << " & "
	      << result_nocjv[nSysts-1].tauidSyst << " & "
	      << "$^{"<< result_nocjv[nSysts-1].jesSyst.first << "}_{"<< result_nocjv[nSysts-1].jesSyst.second << "}$ &  "
	      << "$^{"<< result_nocjv[nSysts-1].jerSyst.first << "}_{"<< result_nocjv[nSysts-1].jerSyst.second << "}$ & "
	      << "$^{"<< result_nocjv[nSysts-1].puSyst.first << "}_{"<< result_nocjv[nSysts-1].puSyst.second << "}$ \\\\"
	      << std::endl
	      << "\\% unc. & " 
	      << result_nocjv[nSysts-1].stat/result_nocjv[nSysts-1].number*100 << " & " 
	      << result_nocjv[nSysts-1].syst/result_nocjv[nSysts-1].number*100 << " & "
	      << result_nocjv[nSysts-1].tauidSyst/result_nocjv[nSysts-1].number*100 << " & "
	      << "$^{"<< result_nocjv[nSysts-1].jesSyst.first/result_nocjv[nSysts-1].number*100 << "}_{"<< result_nocjv[nSysts-1].jesSyst.second/result_nocjv[nSysts-1].number*100 << "}$ & "
	      << "$^{"<< result_nocjv[nSysts-1].jerSyst.first/result_nocjv[nSysts-1].number*100 << "}_{"<< result_nocjv[nSysts-1].jerSyst.second/result_nocjv[nSysts-1].number*100 << "}$ & "
	      << "$^{"<< result_nocjv[nSysts-1].puSyst.first/result_nocjv[nSysts-1].number*100 << "}_{"<< result_nocjv[nSysts-1].puSyst.second/result_nocjv[nSysts-1].number*100 << "}$ \\\\ "
	      << std::endl
	      << "Result_Nocjv = $" 
	      << result_nocjv[nSysts-1].number << " \\pm " 
	      << result_nocjv[nSysts-1].stat << " (stat) ^{" 
	      << result_nocjv[nSysts-1].totalSyst().first  << "}_{"
	      << result_nocjv[nSysts-1].totalSyst().second << "} (syst) $"
	      << std::endl;

    jesUp = result[0].number - result[nSysts-1].number;
    jesDown = result[1].number - result[nSysts-1].number;
    //select the maximum positive value
    result[nSysts-1].jesSyst.first = std::max(std::max(jesUp,0.),std::max(jesDown,0.));
    //select the minimum negative value
    result[nSysts-1].jesSyst.second = std::min(std::min(jesUp,0.),std::min(jesDown,0.));
    jerBetter = result[2].number - result[nSysts-1].number;
    jerWorse = result[3].number - result[nSysts-1].number;
    result[nSysts-1].jerSyst.first = std::max(std::max(jerBetter,0.),std::max(jerWorse,0.));
    result[nSysts-1].jerSyst.second = std::min(std::min(jerBetter,0.),std::min(jerWorse,0.));
    puUp = result[4].number - result[nSysts-1].number;
    puDown = result[5].number - result[nSysts-1].number;
    result[nSysts-1].puSyst.first = std::max(std::max(puUp,0.),std::max(puDown,0.));
    result[nSysts-1].puSyst.second = std::min(std::min(puUp,0.),std::min(puDown,0.));

    std::cout << "\\hline" << std::endl 
	      << "Result & " 
	      << result[nSysts-1].stat << " & " 
	      << result[nSysts-1].syst << " & "
	      << result[nSysts-1].tauidSyst << " & "
	      << "$^{"<< result[nSysts-1].jesSyst.first << "}_{"<< result[nSysts-1].jesSyst.second << "}$ & "
	      << "$^{"<< result[nSysts-1].jerSyst.first << "}_{"<< result[nSysts-1].jerSyst.second << "}$ & "
	      << "$^{"<< result[nSysts-1].puSyst.first << "}_{"<< result[nSysts-1].puSyst.second << "}$ \\\\"
	      << std::endl
	      << "\\% unc. & " 
	      << result[nSysts-1].stat/result[nSysts-1].number*100 << " & " 
	      << result[nSysts-1].syst/result[nSysts-1].number*100 << " & "
	      << result[nSysts-1].tauidSyst/result[nSysts-1].number*100 << " & "
	      << "$^{"<< result[nSysts-1].jesSyst.first/result[nSysts-1].number*100 << "}_{"<< result[nSysts-1].jesSyst.second/result[nSysts-1].number*100 << "}$ & "
	      << "$^{"<< result[nSysts-1].jerSyst.first/result[nSysts-1].number*100 << "}_{"<< result[nSysts-1].jerSyst.second/result[nSysts-1].number*100 << "}$ & "
	      << "$^{"<< result[nSysts-1].puSyst.first/result[nSysts-1].number*100 << "}_{"<< result[nSysts-1].puSyst.second/result[nSysts-1].number*100 << "}$ \\\\ "
	      << std::endl
	      << "Result = $" 
	      << result[nSysts-1].number << " \\pm " 
	      << result[nSysts-1].stat << " (stat) ^{" 
	      << result[nSysts-1].totalSyst().first  << "}_{"
	      << result[nSysts-1].totalSyst().second << "} (syst) $"
	      << std::endl;



    
  }//loop on MET values
  
  }//loop on weights
  
  return 0;
}//main
  

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
enum Selection {Trig,Lep,JetPair,DEta,MET,TightMjj,DPhiSIGNAL_noCJV,DPhiQCD_noCJV,CJVpass,CJVfail,DPhiSIGNAL_CJVpass,DPhiQCD_CJVpass};
enum Sample {QCD,GJets,Top,TTbar,SingleTop,TW,WJets,WJets_enu,WJets_munu,WJets_taunu,ZJets,ZJets_ll,ZJets_nunu,ZJets_vbf,VV,Data,Signal};


struct events {
  std::string sample;
  double number;
  double error;

  void operator+=(const events & rhs){
    this->number += rhs.number;
    this->error = sqrt(pow(this->error,2)+pow(rhs.error,2));
  }

  void operator-=(const events & rhs){
    this->number -= rhs.number;
    this->error = sqrt(pow(this->error,2)+pow(rhs.error,2));
  }

};

double sumSqErr(events evt1, events evt2){
  if (evt1.number==0 || evt2.number==0) return 0;
  return sqrt(pow(evt1.error/evt1.number,2)+pow(evt2.error/evt2.number,2));
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
    return eff()*sqrt(pow(num.error/num.number,2)+pow(den.error/den.number,2)-2*(num.error*den.error/(num.number*den.number)));
  }

};

std::ostream & operator<<(std::ostream & aOs, const events & evt){
  aOs //<< "$" << evt.sample << " $ & "
      << "$" <<  evt.number
      << " \\pm " << evt.error
      << "$";// \\\\"
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
  is>>evt.error; 
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
  lSelVecSignal.push_back("CJVfail");
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
  lSelVecControl.push_back("CJVfail");
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
  lSelVecTau.push_back("CJVfail");
  lSelVecTau.push_back("DPhiSIGNAL_CJVpass");
  lSelVecTau.push_back("DPhiQCD_CJVpass");

  //std::string lSuffix = "";
  const unsigned nWeights = 6;
  std::string lSuffix[nWeights] = {"","_pu","_puUp","_puDown","_pu_trig","_pu_trig_idiso"};

  bool doTaus = true;
  bool docrosschecktau=false;
  bool dojes = false;
  bool dojer = false;
  bool doWeights = false;

  //std::string TOPDIR = "../TABLES_mjj1200/";
  std::string TOPDIR = "../oldanalysisruns/080713_taunominaltightlepiddiscr/TABLES/";

  const unsigned nSteps = lSelVecSignal.size();

  std::string SYST[5] = {"JESUP","JESDOWN","JERBETTER","JERWORSE",""};//Order so numbers from systematics can be saved to be put in central table as syst errors

  unsigned MET[3] = {130,0,70};

  for (unsigned iW(0); iW<(doWeights?nWeights:1); ++iW){//loop on different weights

    std::cout << " -- Processing weight " << lSuffix[iW] << std::endl;

  for (unsigned iMET(0); iMET<1; ++iMET){//loop on MET values

    std::cout << " -- Processing MET " << MET[iMET] << std::endl;

    //create variables to store systB numbers
    events systevents[2][3][5][4];//[iqcd][ich][isyst][lnsmc,lncmc,lnsdata,lncdata]
    double systdiff[2][3][4][4];
    double systperc[2][3][4][4];
    double totalsystupshift[2][3][4];//[iqcd][ich][lnsmc,lncmc,lnsdata,lncdata]
    double totalsystdownshift[2][3][4];
    double totalsystupperc[2][3][4];
    double totalsystdownperc[2][3][4];


    int isyststart=0;
    if(!dojes&&!dojer){
      isyststart=4;
    }
    for (unsigned iSyst(isyststart); iSyst<5; ++iSyst){//loop over different systematics NOTE TO JUST DO CENTRAL CHANGE to iSyst(4) don't do iSyst<1
      std::cout<< "--Processing systematic " <<iSyst<<std::endl;
      std::ostringstream lFolder;
      lFolder << "MET" << MET[iMET] << "/" << SYST[iSyst] << "/";
      
      evtsArray lSel[4][nSteps];
      
	for (unsigned iS(0); iS<nSteps; ++iS){//loop on steps
	  
	  std::ifstream lTable;
	  std::ostringstream lName;
	  
	  
	  lName << TOPDIR << "/nunu/" << lFolder.str() << "/SummaryTable_" << lSelVecSignal[iS] << lSuffix[iW] << ".dat";
	  lTable.open(lName.str().c_str());
	  if(!lTable.is_open()){
	    cerr<<"Unable to open file: "<<lName.str()<<endl;
	    return 1; 
	  }
	  lTable>>lSel[0][iS];
	  lTable.close();
	  
	  lName.str("");
	  lName << TOPDIR << "/enu/" << lFolder.str() << "/SummaryTable_" << lSelVecControl[iS] << lSuffix[iW] << ".dat";
	  lTable.open(lName.str().c_str());
	  if(!lTable.is_open()){
	    cerr<<"Unable to open file: "<<lName.str()<<endl;
	    return 1; 
	  }
	  lTable>>lSel[1][iS];
	  lTable.close();
	  
	  lName.str("");
	  lName << TOPDIR << "/munu/" << lFolder.str() << "/SummaryTable_" << lSelVecControl[iS] << lSuffix[iW] << ".dat";
	  lTable.open(lName.str().c_str());
	  if(!lTable.is_open()){
	    cerr<<"Unable to open file: "<<lName.str()<<endl;
	    return 1; 
	  }
	  lTable>>lSel[2][iS];
	  lTable.close();

	  if (doTaus){
	    //if (iS==nSteps-1) continue;
	    lName.str("");
	    lName << TOPDIR << "/taunu/" << lFolder.str() << "/SummaryTable_" << lSelVecTau[iS] << lSuffix[iW] << ".dat";
	    lTable.open(lName.str().c_str());
	    if(!lTable.is_open()){
	      cerr<<"Unable to open file: "<<lName.str()<<endl;
	      return 1; 
	    }
	    lTable>>lSel[3][iS];
	    lTable.close();
	  }
	}//loop on steps
	
	
	//print Wjets components
	std::string lChannel[4] = {"nunu","enu","munu","taunu"};
	for (unsigned iCh(0); iCh< (doTaus ? 4 : 3); ++iCh){//loop on channel
	  std::ostringstream lName;
	  lName << TOPDIR <<"/" << lChannel[iCh] << "/MET" << MET[iMET] << "/" << SYST[iSyst] << "/WandZJetsTable" << lSuffix[iW] << ".txt";
	  
	  std::ofstream lOutfile;
	  lOutfile.open(lName.str().c_str());
	  
	  lOutfile << "\\begin{tabular}{|l|c|c|c||c|c|c|}" << std::endl
		   << "\\hline" << std::endl
		   << "Step & W$\\rightarrow e\\nu$ & W$\\rightarrow\\mu\\nu$ & W$\\rightarrow\\tau\\nu$ & Z$\\rightarrow\\nu\\nu$ & Z$\\rightarrow$ll & EWK Z+2j \\\\" << std::endl
		   << "\\hline" << std::endl
		   << "$\\Delta\\phi<1.0$ &  $" 
		   << lSel[iCh][DPhiSIGNAL_CJVpass][WJets_enu].number  << "\\pm" << lSel[iCh][DPhiSIGNAL_CJVpass][WJets_enu].error << "$ & $" 
		   << lSel[iCh][DPhiSIGNAL_CJVpass][WJets_munu].number  << "\\pm" << lSel[iCh][DPhiSIGNAL_CJVpass][WJets_munu].error  << "$ & $" 
		   << lSel[iCh][DPhiSIGNAL_CJVpass][WJets_taunu].number  << "\\pm" << lSel[iCh][DPhiSIGNAL_CJVpass][WJets_taunu].error << "$ & $" 
		   << lSel[iCh][DPhiSIGNAL_CJVpass][ZJets_nunu].number  << "\\pm" << lSel[iCh][DPhiSIGNAL_CJVpass][ZJets_nunu].error << "$ & $" 
		   << lSel[iCh][DPhiSIGNAL_CJVpass][ZJets_ll].number  << "\\pm" << lSel[iCh][DPhiSIGNAL_CJVpass][ZJets_ll].error << "$ & $" 
		   << lSel[iCh][DPhiSIGNAL_CJVpass][ZJets_vbf].number  << "\\pm" << lSel[iCh][DPhiSIGNAL_CJVpass][ZJets_vbf].error
		   << "$ \\\\" << std::endl
		   << "$\\Delta\\phi > 2.6$ & $" 
		   << lSel[iCh][DPhiQCD_CJVpass][WJets_enu].number  << "\\pm" << lSel[iCh][DPhiQCD_CJVpass][WJets_enu].error << "$ & $" 
		   << lSel[iCh][DPhiQCD_CJVpass][WJets_munu].number  << "\\pm" << lSel[iCh][DPhiQCD_CJVpass][WJets_munu].error  << "$ & $" 
		   << lSel[iCh][DPhiQCD_CJVpass][WJets_taunu].number  << "\\pm" << lSel[iCh][DPhiQCD_CJVpass][WJets_taunu].error << "$ & $" 
		   << lSel[iCh][DPhiQCD_CJVpass][ZJets_nunu].number  << "\\pm" << lSel[iCh][DPhiQCD_CJVpass][ZJets_nunu].error << "$ & $" 
		   << lSel[iCh][DPhiQCD_CJVpass][ZJets_ll].number  << "\\pm" << lSel[iCh][DPhiQCD_CJVpass][ZJets_ll].error << "$ & $" 
		   << lSel[iCh][DPhiQCD_CJVpass][ZJets_vbf].number  << "\\pm" << lSel[iCh][DPhiQCD_CJVpass][ZJets_vbf].error
		   << "$ \\\\" << std::endl
		   << "\\hline" << std::endl
		   << "\\end{tabular}" << std::endl;
	  lOutfile.close();

	  lName.str("");
	  lName << TOPDIR <<"/" << lChannel[iCh] << "/MET" << MET[iMET] << "/" << SYST[iSyst] << "/TopTable" << lSuffix[iW] << ".txt";
	  
	  lOutfile.open(lName.str().c_str());
	  

	  lOutfile << "\\begin{tabular}{|l|c|c|c|}" << std::endl
		   << "\\hline" << std::endl
		   << "Step & $t\\bar{t}$ & single top & $t,\\bar{t}\\rightarrow tW$  \\\\" << std::endl
		   << "\\hline" << std::endl
		   << "$\\Delta\\phi<1.0$ &  $"
		   << lSel[iCh][DPhiSIGNAL_CJVpass][TTbar].number  << "\\pm" << lSel[iCh][DPhiSIGNAL_CJVpass][TTbar].error  << "$ & $" 
		   << lSel[iCh][DPhiSIGNAL_CJVpass][SingleTop].number  << "\\pm" << lSel[iCh][DPhiSIGNAL_CJVpass][SingleTop].error  << "$ & $" 
		   << lSel[iCh][DPhiSIGNAL_CJVpass][TW].number  << "\\pm" << lSel[iCh][DPhiSIGNAL_CJVpass][TW].error << "$ \\\\" << std::endl;
	  lOutfile << "$\\Delta\\phi > 2.6$ & $" 
		   << lSel[iCh][DPhiQCD_CJVpass][TTbar].number  << "\\pm" << lSel[iCh][DPhiQCD_CJVpass][TTbar].error  << "$ & $" 
		   << lSel[iCh][DPhiQCD_CJVpass][SingleTop].number  << "\\pm" << lSel[iCh][DPhiQCD_CJVpass][SingleTop].error  << "$ & $" 
		   << lSel[iCh][DPhiQCD_CJVpass][TW].number  << "\\pm" << lSel[iCh][DPhiQCD_CJVpass][TW].error 
		   << "$ \\\\" << std::endl
		   << "\\hline" << std::endl
		   << "\\end{tabular}" << std::endl;
	  lOutfile.close();
	  
	}//loop on channel
	

	if (doTaus){

	  events nData = lSel[3][DPhiSIGNAL_noCJV][Data];
	  events nBkg = lSel[3][DPhiSIGNAL_noCJV][Top];
	  nBkg += lSel[3][DPhiSIGNAL_noCJV][ZJets];
	  nBkg += lSel[3][DPhiSIGNAL_noCJV][VV];
	  efficiency eps_tau;
	  eps_tau.num = lSel[3][DPhiSIGNAL_noCJV][WJets_taunu];
	  //eps_tau.den = lSel[0][DPhiSIGNAL_CJVpass_noCJV][WJets_taunu];
	  //Access number in signal region before CJV:
	  //Run interactively:
	  //cd output/nunu/MET130
	  //hadd MC_Wtaunu.root MC_W*JetsToLNu_taunu.root
	  //cd TightMjj
	  //dphijj->Scale(19600*37509./76102995)
	  //double err=0
	  // dphijj->IntegralAndError(0,32,err)
	  //dphijj->GetBinLowEdge(32)
	  //(const Double_t)9.73895999999999984e-01
	  //hardcode result:
	  //mjj 1200
	  //eps_tau.den.number = 131;
	  //eps_tau.den.error = 10;
	  eps_tau.den = lSel[0][DPhiSIGNAL_noCJV][WJets_taunu];
	  //eps_tau.den.number = 167;
	  //eps_tau.den.error = 11;

	  efficiency eps_tau_cjv;
	  eps_tau_cjv.num = lSel[0][DPhiSIGNAL_CJVpass][WJets_taunu];
	  eps_tau_cjv.den.number = eps_tau.den.number;
	  eps_tau_cjv.den.error = eps_tau.den.error;

	  events nDataW = nData;
	  nDataW -= nBkg;
	  if(docrosschecktau){
	    nDataW -= lSel[3][DPhiSIGNAL_noCJV][WJets_enu];
	  }
	  events result_nocjv = nDataW;
	  result_nocjv.number = nDataW.number/eps_tau.eff();
	  result_nocjv.error = sqrt(pow(nDataW.error/eps_tau.eff(),2)+pow(nDataW.number*eps_tau.error()/pow(eps_tau.eff(),2),2));

	  events result = nDataW;
	  result.number = nDataW.number*lSel[0][DPhiSIGNAL_CJVpass][WJets_taunu].number/lSel[3][DPhiSIGNAL_noCJV][WJets_taunu].number;
	  result.error = result.number*sqrt(pow(nDataW.error/nDataW.number,2));//+pow(lSel[0][DPhiSIGNAL_CJVpass][WJets_taunu].error/lSel[0][DPhiSIGNAL_CJVpass][WJets_taunu].number,2)+pow(lSel[3][DPhiSIGNAL_noCJV][WJets_taunu].error/lSel[3][DPhiSIGNAL_noCJV][WJets_taunu].number,2));

	  events crosscheck = result_nocjv;
	  crosscheck.number = result_nocjv.number*eps_tau_cjv.eff();
	  crosscheck.error = crosscheck.number*sqrt(pow(result_nocjv.error/result_nocjv.number,2)+pow(eps_tau_cjv.error()/eps_tau_cjv.eff(),2));

	  double totsyst=0.;
	  double wenusyst=0.;
	  double multsyst=sqrt(pow(lSel[0][DPhiSIGNAL_CJVpass][WJets_taunu].error/lSel[0][DPhiSIGNAL_CJVpass][WJets_taunu].number,2)+pow(lSel[3][DPhiSIGNAL_noCJV][WJets_taunu].error/lSel[3][DPhiSIGNAL_noCJV][WJets_taunu].number,2)+pow(0.08,2))*result.number;//sqrt(pow(eps_tau.error()/eps_tau.eff(),2)+pow(eps_tau_cjv.error()/eps_tau_cjv.eff(),2)+pow(0.08,2))*result.number;
	  //double multsyst=0.08*result.number;
	  if(!docrosschecktau){
	    wenusyst=lSel[3][DPhiSIGNAL_noCJV][WJets_enu].number;
	    totsyst=sqrt(pow(multsyst,2)+pow(wenusyst,2));
	  }
	  else{
	    totsyst=multsyst;
	  }

	  std::cout << "------ taunu estimates:---------" << std::endl
		    << "nData = $" << nData.number  << "\\pm " << nData.error  << "$" << std::endl
		    << "nBkg = $" << nBkg.number  << "\\pm " << nBkg.error  << "$" << std::endl
		    << "eff_tau = " << eps_tau << std::endl
	            << eps_tau.num <<std::endl
	            << eps_tau.den <<std::endl
		    << "*** result no CJV = $" << result_nocjv.number  << "\\pm " << result_nocjv.error << "$" << std::endl
		    << "eff_tau_cjv = " << eps_tau_cjv << std::endl
	            << eps_tau_cjv.num <<std::endl
	            << eps_tau_cjv.den <<std::endl
		    << "*** result CJV = $" << result.number  << "\\pm " << result.error << " \\pm " << totsyst<< "$" << std::endl
		    << "***crosscheck = $" << crosscheck.number << "\\pm " << crosscheck.error << "$" << std::endl
	            << "***syst breakdown:"<<std::endl
	            << "eps_tau_id:" << eps_tau.error()/eps_tau.eff()*result.number <<std::endl
	            << "eps_tau_cjv:" << eps_tau_cjv.error()/eps_tau_cjv.eff()*result.number<<std::endl
	            << "data/mc scale factor:"<<0.08*result.number<<std::endl
	            << "remaining W->enu:"<<wenusyst<<std::endl
		    << "****** MC Estimates: $" << eps_tau.den.number << "\\pm " << eps_tau.den.error << "$, $" << lSel[0][DPhiSIGNAL_noCJV][WJets_taunu].number << "\\pm " << lSel[0][DPhiSIGNAL_noCJV][WJets_taunu].error << "$" << std::endl
	    ;
	  
	}


	for (unsigned int iQCD(0); iQCD<2; ++iQCD){//DphiSignal or DphiQCD
	  if(iQCD==0)std::cout<<" ----- dphisignal -----"<<std::endl;
	  if(iQCD==1)std::cout<<" ----- dphiqcd -----"<<std::endl;
	  for (unsigned iCh(1); iCh<3; ++iCh){//loop on lep flavour
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
	    lBkgCMC.sample = "$N_{EWK}$";
	    lBkgSMC = lSel[0][DPhiSIGNAL_CJVpass+iQCD][Top];
	    lBkgSMC += lSel[0][DPhiSIGNAL_CJVpass+iQCD][VV];
	    lBkgSMC += lSel[0][DPhiSIGNAL_CJVpass+iQCD][ZJets];
	    lBkgSMC.sample = "$N_{EWK}$";
	    
	    efficiency eps_lepveto_S;
	    eps_lepveto_S.name = "$\\epsilon_{lepsel}$";
	    efficiency eps_VBF_S;
	    eps_VBF_S.name = "$\\epsilon_{VBF}$";
	    efficiency eps_lepsel_C;
	    eps_lepsel_C.name = "$\\epsilon_{lepsel}$";
	    efficiency eps_VBF_C;
	    eps_VBF_C.name = "$\\epsilon_{VBF}$";
	    events lNSdata;
	    
	    if (iCh==2) {
	      lNCdata.sample = "$N_{W\\rightarrow \\mu\\nu}^{data}$";
	      lNSdata.sample = "$N_{W\\rightarrow \\mu\\nu}^{data}$";
	      lNSMC = lSel[0][DPhiSIGNAL_CJVpass+iQCD][WJets_munu];
	      lNCMC = lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][WJets_munu];
	      lNSMC.sample = "$N_{W\\rightarrow \\mu\\nu}^{MC}$";
	      lNCMC.sample = "$N_{W\\rightarrow \\mu\\nu}^{MC}$";
	      eps_lepveto_S.num = lSel[0][Lep][WJets_munu];
	      eps_lepveto_S.den = lSel[0][Trig][WJets_munu];
	      
	      eps_VBF_S.num = lSel[0][DPhiSIGNAL_CJVpass+iQCD][WJets_munu];
	      eps_VBF_S.den = lSel[0][Lep][WJets_munu];
	      
	      eps_lepsel_C.num = lSel[iCh][Lep][WJets_munu];
	      eps_lepsel_C.den = lSel[iCh][Trig][WJets_munu];
	      
	      eps_VBF_C.num = lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][WJets_munu];
	      eps_VBF_C.den = lSel[iCh][Lep][WJets_munu];
	    }
	    else {
	      lNCdata.sample = "$N_{W\\rightarrow e\\nu}^{data}$";
	      lNSdata.sample = "$N_{W\\rightarrow e\\nu}^{data}$";
	      lNSMC = lSel[0][DPhiSIGNAL_CJVpass+iQCD][WJets_enu];
	      lNCMC = lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][WJets_enu];
	      lNSMC.sample = "$N_{W\\rightarrow e\\nu}^{MC}$";
	      lNCMC.sample = "$N_{W\\rightarrow e\\nu}^{MC}$";
	      eps_lepveto_S.num = lSel[0][Lep][WJets_enu];
	      eps_lepveto_S.den = lSel[0][Trig][WJets_enu];
	      
	      eps_VBF_S.num = lSel[0][DPhiSIGNAL_CJVpass+iQCD][WJets_enu];
	      eps_VBF_S.den = lSel[0][Lep][WJets_enu];
	      
	      eps_lepsel_C.num = lSel[iCh][Lep][WJets_enu];
	      eps_lepsel_C.den = lSel[iCh][Trig][WJets_enu];
	    
	      eps_VBF_C.num = lSel[iCh][DPhiSIGNAL_CJVpass+iQCD][WJets_enu];
	      eps_VBF_C.den = lSel[iCh][Lep][WJets_enu];
	    }
	    lNSdata.number = lNCdata.number * lNSMC.number / lNCMC.number;
	    lNSdata.error = lNSdata.number * sqrt(pow(lNCdata.error/lNCdata.number,2)+pow(lNCMC.error/lNCMC.number,2)+pow(lNSMC.error/lNSMC.number,2));
	    
	    
	    events result;
	    result.number = lNCdata.number *(eps_lepveto_S.eff()/eps_lepsel_C.eff())*(eps_VBF_S.eff()/eps_VBF_C.eff());
	    result.error = result.number * sqrt(pow(eps_lepveto_S.error()/eps_lepveto_S.eff(),2)+pow(eps_lepsel_C.error()/eps_lepsel_C.eff(),2)+pow(eps_VBF_S.error()/eps_VBF_S.eff(),2)+pow(eps_VBF_C.error()/eps_VBF_C.eff(),2)+pow(lNCdata.error/lNCdata.number,2));
	    
	    	    
	    //SYSTEMATIC INFORMATION
	    if(dojes&&dojer){
	      //save JES and JER numbers
	      systevents[iQCD][iCh][iSyst][0]=lNSMC;
	      systevents[iQCD][iCh][iSyst][1]=lNCMC;
	      systevents[iQCD][iCh][iSyst][2]=lNSdata;
	      systevents[iQCD][iCh][iSyst][3]=lNCdata;
	      if(iSyst==4){//get differences
		for(int a=0;a<4;a++){//loop over jes up and down
		  if(a==0)std::cout<<"JESUP"<<std::endl;
		  if(a==1)std::cout<<"JESDOWN"<<std::endl;
		  if(a==2)std::cout<<"JERBETTER"<<std::endl;
		  if(a==3)std::cout<<"JERWORSE"<<std::endl;
		  for(int b=0;b<4;b++){//loop over lns/cmc/data to get differences and percentage differences
		    systdiff[iQCD][iCh][a][b]=(systevents[iQCD][iCh][a][b].number-systevents[iQCD][iCh][4][b].number);
		    systperc[iQCD][iCh][a][b]=(systevents[iQCD][iCh][a][b].number-systevents[iQCD][iCh][4][b].number)*100/systevents[iQCD][iCh][2][b].number;
		    std::cout<<systevents[iQCD][iCh][a][b].number<<" "<<systevents[iQCD][iCh][4][b].number<<" "<<systdiff[iQCD][iCh][a][b]<<std::endl;
		  }
		}
		//get max up shift and down shift
		for(int b=0;b<4;b++){
		  double maxjesupshift=0.;
		  double maxjesdownshift=0;
		  double maxjerupshift=0.;
		  double maxjerdownshift=0.;
		  for(int a=0;a<2;a++){
		    if(systdiff[iQCD][iCh][a][b]<0){
		      if(systdiff[iQCD][iCh][a][b]<maxjesdownshift) maxjesdownshift=systdiff[iQCD][iCh][a][b];
		    }
		    if(systdiff[iQCD][iCh][a][b]>0){
		      if(systdiff[iQCD][iCh][a][b]>maxjesupshift) maxjesupshift=systdiff[iQCD][iCh][a][b];
		    }
		  }
		  for(int a=2;a<4;a++){
		    if(systdiff[iQCD][iCh][a][b]<0){
		      if(systdiff[iQCD][iCh][a][b]<maxjerdownshift) maxjerdownshift=systdiff[iQCD][iCh][a][b];
		    }
		    if(systdiff[iQCD][iCh][a][b]>0){
		      if(systdiff[iQCD][iCh][a][b]>maxjerupshift) maxjerupshift=systdiff[iQCD][iCh][a][b];
		    }
		  }
		
		//add max jesup/downshift and jerup/downshift in quadrature
		totalsystupshift[iQCD][iCh][b] = sqrt(maxjesupshift*maxjesupshift+maxjerupshift*maxjerupshift);
		totalsystdownshift[iQCD][iCh][b] = -1*sqrt(maxjesdownshift*maxjesdownshift+maxjerdownshift*maxjerdownshift);
		totalsystupperc[iQCD][iCh][b]=totalsystupshift[iQCD][iCh][b]*100/systevents[iQCD][iCh][4][b].number;
		totalsystdownperc[iQCD][iCh][b]=totalsystdownshift[iQCD][iCh][b]*100/systevents[iQCD][iCh][4][b].number;
		}
	      }
	    }
	    
	    //TOTAL SUMMARY TABLE WITH TOTAL SYST ERRORS
	    std::ostringstream lName;
	    lName << TOPDIR << "/" << lChannel[iCh] << "/MET" << MET[iMET] << "/" << SYST[iSyst] ;

	    if (iQCD==0) lName << "/DataDrivenWJetsTable_signal" << lSuffix[iW] << ".txt";
	    else if (iQCD==1) lName << "/DataDrivenWJetsTable_QCD" << lSuffix[iW] << ".txt";
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
	    if((dojer&&dojes)&&iSyst==4){
	      lOutfile << " $ ^{ + " << totalsystupshift[iQCD][iCh][0] << " ( + " << totalsystupperc[iQCD][iCh][0] << "\\% ) } ";
	      lOutfile << " _{ " << totalsystdownshift[iQCD][iCh][0] << " ( " << totalsystdownperc[iQCD][iCh][0] << "\\% ) } $ ";
	    }
	    lOutfile << " & " 
		     << lNCMC; 
	    if((dojer&&dojes)&&iSyst==4){
	      lOutfile << " $ ^{ + " << totalsystupshift[iQCD][iCh][1] << " ( + " << totalsystupperc[iQCD][iCh][1] << "\\% ) } ";
	      lOutfile << " _{ " << totalsystdownshift[iQCD][iCh][1] << " ( " << totalsystdownperc[iQCD][iCh][1] << "\\% ) } $ ";
	    }
	    lOutfile << " \\\\" << std::endl
		     << lNSdata.sample << " & " 
		     << "\\textcolor{red}{" << lNSdata;
	    if((dojer&&dojes)&&iSyst==4){
	      lOutfile << " $ ^{ + " << totalsystupshift[iQCD][iCh][2] << " ( + " << totalsystupperc[iQCD][iCh][2] << "\\% ) } ";
	      lOutfile << " _{ " << totalsystdownshift[iQCD][iCh][2] << " ( " << totalsystdownperc[iQCD][iCh][2] << "\\% ) } $ ";
	    }
	    lOutfile << "} & " 
		     << lNCdata;
	    if((dojer&&dojes)&&iSyst==4){
	      lOutfile << " $ ^{ + " << totalsystupshift[iQCD][iCh][3] << " ( + " << totalsystupperc[iQCD][iCh][3] << "\\% ) } ";
	      lOutfile << " _{ " << totalsystdownshift[iQCD][iCh][3] << " ( " << totalsystdownperc[iQCD][iCh][3] << "\\% ) } $ ";
	    }
	    lOutfile << " \\\\ " << std::endl 
		     << "\\hline" << std::endl
	      //<< "with Eff & " << result << " & \\\\" << std::endl
	      //	   << "\\hline" << std::endl
		     << "\\end{tabular}" << std::endl; 
	    lOutfile.close();

	    std::cout << "*** " << lSuffix[iW] << ": result = " << lNSdata << " MC: " << lNSMC << std::endl;
	    
	  }//loop on lep flavour
	  
	}//loop on signal region
	
	//SUMMARY TABLES WITH SYST ERRORS SEPARATE
	for(int iSample=0;iSample<4;iSample++){
	  std::ostringstream lName;
	  lName << TOPDIR << "/MET" << MET[iMET] << "DataDrivenWJetsSystErrorTable";
	  if(iSample==0) lName <<"lNSMC";
	  if(iSample==1) lName <<"lNCMC";
	  if(iSample==2) lName <<"lNSdata";
	  if(iSample==3) lName <<"lNCdata";
	  lName << lSuffix[iW] << ".txt";
	  std::ofstream lOutfile;
	  lOutfile.open(lName.str().c_str());
	  lOutfile << "\\begin{tabular}{l c c c c}" << std::endl
		   << "\\hline" << std::endl << " "
		   << systevents[0][1][4][iSample].sample
		   << " & Electron $d\\phi < 1.0$ & Electron $d\\phi>2.6$ & Muon $d\\phi < 1.0$ & Muon $d\\phi > 2.6$ \\\\" << std::endl
		   << "\\hline" << std::endl;
	  lOutfile << std::setprecision(3)//central value
		   << "Central num. of events"  << " & " 
		   << systevents[0][1][4][iSample].number << " & "
		   << systevents[1][1][4][iSample].number << " & "
	           << systevents[0][2][4][iSample].number << " & "
	           << systevents[1][2][4][iSample].number << " \\\\" << std::endl;
	  lOutfile << "Statistical"  << " & " 
		   << "$\\pm$" << systevents[0][1][4][iSample].error*100/systevents[0][1][4][iSample].number << "\\% & "
		   << "$\\pm$" << systevents[1][1][4][iSample].error*100/systevents[1][1][4][iSample].number << "\\% & "
	           << "$\\pm$" << systevents[0][2][4][iSample].error*100/systevents[0][2][4][iSample].number << "\\% & "
	           << "$\\pm$" << systevents[1][2][4][iSample].error*100/systevents[1][2][4][iSample].number << "\\% \\\\" << std::endl;
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
	  lOutfile << noshowpos;
	  lOutfile << "\\hline" << std::endl
		   << "\\end{tabular}" << std::endl; 
	  lOutfile << noshowpos;
	  lOutfile.close();
	}
	
	
    }//loop on systematics
    
  }//loop on MET values
  
  }//loop on weights

  return 0;
}//main
  

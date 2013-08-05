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

  double error() const{
    return sqrt(pow(stat,2)+pow(syst,2));
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
      << " \\pm " << evt.error()
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

  bool doTaus = true;
  bool dojes = false;
  bool dojer = false;

  std::string TOPDIR = "../TABLES/";
  //std::string TOPDIR = "../oldanalysisruns/080713_taunominaltightlepiddiscr/TABLES/";

  const unsigned nSteps = lSelVecSignal.size();

  std::string SYST[5] = {"JESUP","JESDOWN","JERBETTER","JERWORSE",""};//Order so numbers from systematics can be saved to be put in central table as syst errors

  unsigned MET[3] = {130,0,70};

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
	
	
	std::cout << " -- TABLES have been read in." << std::endl;

	//print Wjets components
	std::string lChannel[4] = {"nunu","enu","munu","taunu"};
	for (unsigned iCh(0); iCh< (doTaus ? 4 : 3); ++iCh){//loop on channel

	  std::cout << " -- Processing channel: " << lChannel[iCh] << std::endl;

	  std::ostringstream lName;
	  lName << TOPDIR <<"/" << lChannel[iCh] << "/MET" << MET[iMET] << "/" << SYST[iSyst] << "/WJetsTable" << lSuffix[iW] << ".txt";
	  
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
	  lName << TOPDIR <<"/" << lChannel[iCh] << "/MET" << MET[iMET] << "/" << SYST[iSyst] << "/ZJetsTable" << lSuffix[iW] << ".txt";
	  
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
	  lName << TOPDIR <<"/" << lChannel[iCh] << "/MET" << MET[iMET] << "/" << SYST[iSyst] << "/TopTable" << lSuffix[iW] << ".txt";
	  
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
	  events result_nocjv = nDataW;
	  result_nocjv.number = nDataW.number/eps_tau.eff();
	  result_nocjv.stat = nDataW.stat;
	  result_nocjv.syst = nDataW.number*eps_tau.error()/pow(eps_tau.eff(),2);

	  events effProduct;
	  effProduct.number = eps_tau_cjv.num.number / eps_tau.num.number;
	  effProduct.stat = effProduct.number*sqrt(pow(eps_tau_cjv.num.stat/eps_tau_cjv.num.number,2)+pow(eps_tau.num.stat/eps_tau.num.number,2));
	  events result = nDataW;
	  result.number = nDataW.number*effProduct.number;
	  result.stat = result.number*sqrt(pow(nDataW.stat/nDataW.number,2));

	  result.syst=sqrt(pow(effProduct.stat/effProduct.number,2)+pow(0.08,2))*result.number;

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
	  lName << TOPDIR <<"/" << lChannel[3] << "/MET" << MET[iMET] << "/" << SYST[iSyst] << "/DataDrivenWJetsTable_signal" << lSuffix[iW] << ".txt";
	  
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
 		   << "W$\\rightarrow \\tau\\nu$ estimates & \\multicolumn{2}{c|}{ $" << result_nocjv.number  << "\\pm " << result_nocjv.stat << "(stat) \\pm " << result_nocjv.syst << "(syst) $} & \\multicolumn{2}{c|}{ $" << result.number  << "\\pm " << result.stat << "(stat) \\pm " << result.syst << "(syst) $} \\\\" << std::endl
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
 		   << "W$\\rightarrow \\tau\\nu$ estimates & \\multicolumn{2}{c|}{ $" << result_nocjv_noew.number  << "\\pm " << result_nocjv_noew.stat << "(stat) \\pm " << result_nocjv_noew.syst << "(syst) $} & \\multicolumn{2}{c|}{ $" << result_noew.number  << "\\pm " << result_noew.stat << "(stat) \\pm " << result_noew.syst << "(syst) $} \\\\" << std::endl
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
	    lNSdata.stat = lNCdata.error();
	    lNSdata.syst = lNSdata.number * sqrt(pow(lNCMC.error()/lNCMC.number,2)+pow(lNSMC.error()/lNSMC.number,2));
	    
	    
	    
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
		   << "$\\pm$" << systevents[0][1][4][iSample].error()*100/systevents[0][1][4][iSample].number << "\\% & "
		   << "$\\pm$" << systevents[1][1][4][iSample].error()*100/systevents[1][1][4][iSample].number << "\\% & "
	           << "$\\pm$" << systevents[0][2][4][iSample].error()*100/systevents[0][2][4][iSample].number << "\\% & "
	           << "$\\pm$" << systevents[1][2][4][iSample].error()*100/systevents[1][2][4][iSample].number << "\\% \\\\" << std::endl;
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
  

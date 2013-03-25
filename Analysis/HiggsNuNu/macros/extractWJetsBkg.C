#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <iomanip>

#include "TMath.h"


enum Selection {Lep,JetPair,DEta,MET,TightMjj,DPhiSIGNAL,DPhiQCD};
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
    return 1/den.number*sqrt(num.number*(1-eff()));
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
  lSelVecSignal.push_back("LeptonVeto");
  lSelVecSignal.push_back("JetPair");
  lSelVecSignal.push_back("DEta");
  lSelVecSignal.push_back("MET");
  lSelVecSignal.push_back("TightMjj");
  lSelVecSignal.push_back("DPhiSIGNAL");
  lSelVecSignal.push_back("DPhiQCD");

  std::vector<std::string> lSelVecControl;
  lSelVecControl.push_back("WSelection");
  lSelVecControl.push_back("JetPair");
  lSelVecControl.push_back("DEta");
  lSelVecControl.push_back("MET");
  lSelVecControl.push_back("TightMjj");
  lSelVecControl.push_back("DPhiSIGNAL");
  lSelVecControl.push_back("DPhiQCD");

  std::string TOPDIR = "../TABLES/";

  const unsigned nSteps = lSelVecSignal.size();

  std::string SYST[3] = {"","JESUP","JESDOWN"};

  unsigned MET[3] = {130,0,70};

  for (unsigned iMET(0); iMET<3; ++iMET){//loop on MET values

    std::cout << " -- Processing MET " << MET[iMET] << std::endl;

      for (unsigned iSyst(0); iSyst<1; ++iSyst){//loop over different systematics                                                                                      
	std::cout<< "--Processing systematic " <<iSyst<<std::endl;
	std::ostringstream lFolder;
	lFolder << "MET" << MET[iMET] << "/" << SYST[iSyst] << "/";
        
	evtsArray lSel[3][nSteps];
	
	for (unsigned iS(0); iS<nSteps; ++iS){//loop on steps
	  
	  std::ifstream lTable;
	  std::ostringstream lName;
	  
	  
	  lName << TOPDIR << "/nunu/" << lFolder.str() << "/SummaryTable_" << lSelVecSignal[iS] << ".dat";
	  lTable.open(lName.str().c_str());
	  if(!lTable.is_open()){
	    cerr<<"Unable to open file: "<<lName.str()<<endl;
	    return 1; 
	  }
	  lTable>>lSel[0][iS];
	  lTable.close();
	  
	  lName.str("");
	  lName << TOPDIR << "/enu/" << lFolder.str() << "/SummaryTable_" << lSelVecControl[iS] << ".dat";
	  lTable.open(lName.str().c_str());
	  if(!lTable.is_open()){
	    cerr<<"Unable to open file: "<<lName.str()<<endl;
	    return 1; 
	  }
	  lTable>>lSel[1][iS];
	  lTable.close();
	  
	  lName.str("");
	  lName << TOPDIR << "/munu/" << lFolder.str() << "/SummaryTable_" << lSelVecControl[iS] << ".dat";
	  lTable.open(lName.str().c_str());
	  if(!lTable.is_open()){
	    cerr<<"Unable to open file: "<<lName.str()<<endl;
	    return 1; 
	  }
	  lTable>>lSel[2][iS];
	  lTable.close();
	  
	}//loop on steps
	
	
	//print Wjets components
	std::string lChannel[3] = {"nunu","enu","munu"};
	for (unsigned iCh(0); iCh<3; ++iCh){//loop on channel
	  std::ostringstream lName;
	  lName << TOPDIR <<"/" << lChannel[iCh] << "/MET" << MET[iMET] << "/" << SYST[iSyst] << "/WandZJetsTable.txt";
	  
	  std::ofstream lOutfile;
	  lOutfile.open(lName.str().c_str());
	  
	  lOutfile << "\\begin{tabular}{|l|c|c|c||c|c|c|}" << std::endl
		   << "\\hline" << std::endl
		   << "Step & W$\\rightarrow e\\nu$ & W$\\rightarrow\\mu\\nu$ & W$\\rightarrow\\tau\\nu$ & Z$\\rightarrow\\nu\\nu$ & Z$\\rightarrow$ll & EWK Z+2j \\\\" << std::endl
		   << "\\hline" << std::endl
		   << "$\\Delta\\phi<1.0$ &  $" 
		   << lSel[iCh][DPhiSIGNAL][WJets_enu].number  << "\\pm" << lSel[iCh][DPhiSIGNAL][WJets_enu].error << "$ & $" 
		   << lSel[iCh][DPhiSIGNAL][WJets_munu].number  << "\\pm" << lSel[iCh][DPhiSIGNAL][WJets_munu].error  << "$ & $" 
		   << lSel[iCh][DPhiSIGNAL][WJets_taunu].number  << "\\pm" << lSel[iCh][DPhiSIGNAL][WJets_taunu].error << "$ & $" 
		   << lSel[iCh][DPhiSIGNAL][ZJets_nunu].number  << "\\pm" << lSel[iCh][DPhiSIGNAL][ZJets_nunu].error << "$ & $" 
		   << lSel[iCh][DPhiSIGNAL][ZJets_ll].number  << "\\pm" << lSel[iCh][DPhiSIGNAL][ZJets_ll].error << "$ & $" 
		   << lSel[iCh][DPhiSIGNAL][ZJets_vbf].number  << "\\pm" << lSel[iCh][DPhiSIGNAL][ZJets_vbf].error
		   << "$ \\\\" << std::endl
		   << "$\\Delta\\phi > 2.6$ & $" 
		   << lSel[iCh][DPhiQCD][WJets_enu].number  << "\\pm" << lSel[iCh][DPhiQCD][WJets_enu].error << "$ & $" 
		   << lSel[iCh][DPhiQCD][WJets_munu].number  << "\\pm" << lSel[iCh][DPhiQCD][WJets_munu].error  << "$ & $" 
		   << lSel[iCh][DPhiQCD][WJets_taunu].number  << "\\pm" << lSel[iCh][DPhiQCD][WJets_taunu].error << "$ & $" 
		   << lSel[iCh][DPhiQCD][ZJets_nunu].number  << "\\pm" << lSel[iCh][DPhiQCD][ZJets_nunu].error << "$ & $" 
		   << lSel[iCh][DPhiQCD][ZJets_ll].number  << "\\pm" << lSel[iCh][DPhiQCD][ZJets_ll].error << "$ & $" 
		   << lSel[iCh][DPhiQCD][ZJets_vbf].number  << "\\pm" << lSel[iCh][DPhiQCD][ZJets_vbf].error
		   << "$ \\\\" << std::endl
		   << "\\hline" << std::endl
		   << "\\end{tabular}" << std::endl;
	  lOutfile.close();

	  lName.str("");
	  lName << TOPDIR <<"/" << lChannel[iCh] << "/MET" << MET[iMET] << "/" << SYST[iSyst] << "/TopTable.txt";
	  
	  lOutfile.open(lName.str().c_str());
	  

	  lOutfile << "\\begin{tabular}{|l|c|c|c|}" << std::endl
		   << "\\hline" << std::endl
		   << "Step & $t\\bar{t}$ & single top & $t,\\bar{t}\\rightarrow tW$  \\\\" << std::endl
		   << "\\hline" << std::endl
		   << "$\\Delta\\phi<1.0$ &  $"
		   << lSel[iCh][DPhiSIGNAL][TTbar].number  << "\\pm" << lSel[iCh][DPhiSIGNAL][TTbar].error  << "$ & $" 
		   << lSel[iCh][DPhiSIGNAL][SingleTop].number  << "\\pm" << lSel[iCh][DPhiSIGNAL][SingleTop].error  << "$ & $" 
		   << lSel[iCh][DPhiSIGNAL][TW].number  << "\\pm" << lSel[iCh][DPhiSIGNAL][TW].error << "$ \\\\" << std::endl;
	  lOutfile << "$\\Delta\\phi > 2.6$ & $" 
		   << lSel[iCh][DPhiQCD][TTbar].number  << "\\pm" << lSel[iCh][DPhiQCD][TTbar].error  << "$ & $" 
		   << lSel[iCh][DPhiQCD][SingleTop].number  << "\\pm" << lSel[iCh][DPhiQCD][SingleTop].error  << "$ & $" 
		   << lSel[iCh][DPhiQCD][TW].number  << "\\pm" << lSel[iCh][DPhiQCD][TW].error 
		   << "$ \\\\" << std::endl
		   << "\\hline" << std::endl
		   << "\\end{tabular}" << std::endl;
	  lOutfile.close();
	  
	}//loop on channel
	
	for (unsigned int iQCD(0); iQCD<2; ++iQCD){//loop on Mjj WP
	  
	  for (unsigned iCh(1); iCh<3; ++iCh){//loop on lep flavour
	    
	    events lNCdata;
	    events lBkgCMC;
	    events lBkgSMC;
	    events lNCMC;
	    events lNSMC;
	    // std::cout << lSel[iCh][DPhiSIGNAL+iQCD][Data] 
	    // 	    << " " << lSel[iCh][DPhiSIGNAL+iQCD][Top]
	    // 	    << " " << lSel[iCh][DPhiSIGNAL+iQCD][VV]
	    // 	    << " " << lSel[iCh][DPhiSIGNAL+iQCD][ZJets]
	    // 	    << std::endl;
	    
	    lNCdata = lSel[iCh][DPhiSIGNAL+iQCD][Data];
	    lNCdata -= lSel[iCh][DPhiSIGNAL+iQCD][Top];
	    lNCdata -= lSel[iCh][DPhiSIGNAL+iQCD][VV];
	    lNCdata -= lSel[iCh][DPhiSIGNAL+iQCD][ZJets];
	    lBkgCMC = lSel[iCh][DPhiSIGNAL+iQCD][Top];
	    lBkgCMC += lSel[iCh][DPhiSIGNAL+iQCD][VV];
	    lBkgCMC += lSel[iCh][DPhiSIGNAL+iQCD][ZJets];
	    lBkgCMC.sample = "$N_{EWK}$";
	    lBkgSMC = lSel[0][DPhiSIGNAL+iQCD][Top];
	    lBkgSMC += lSel[0][DPhiSIGNAL+iQCD][VV];
	    lBkgSMC += lSel[0][DPhiSIGNAL+iQCD][ZJets];
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
	      lNSMC = lSel[0][DPhiSIGNAL+iQCD][WJets_munu];
	      lNCMC = lSel[iCh][DPhiSIGNAL+iQCD][WJets_munu];
	      lNSMC.sample = "$N_{W\\rightarrow \\mu\\nu}^{MC}$";
	      lNCMC.sample = "$N_{W\\rightarrow \\mu\\nu}^{MC}$";
	      eps_lepveto_S.num = lSel[0][Lep][WJets_munu];
	      eps_lepveto_S.den = lSel[0][JetPair][WJets_munu];
	      
	      eps_VBF_S.num = lSel[0][DPhiSIGNAL+iQCD][WJets_munu];
	      eps_VBF_S.den = lSel[0][Lep][WJets_munu];
	      
	      eps_lepsel_C.num = lSel[iCh][Lep][WJets_munu];
	      eps_lepsel_C.den = lSel[iCh][JetPair][WJets_munu];
	      
	      eps_VBF_C.num = lSel[iCh][DPhiSIGNAL+iQCD][WJets_munu];
	      eps_VBF_C.den = lSel[iCh][Lep][WJets_munu];
	    }
	    else {
	      lNCdata.sample = "$N_{W\\rightarrow e\\nu}^{data}$";
	      lNSdata.sample = "$N_{W\\rightarrow e\\nu}^{data}$";
	      lNSMC = lSel[0][DPhiSIGNAL+iQCD][WJets_enu];
	      lNCMC = lSel[iCh][DPhiSIGNAL+iQCD][WJets_enu];
	      lNSMC.sample = "$N_{W\\rightarrow e\\nu}^{MC}$";
	      lNCMC.sample = "$N_{W\\rightarrow e\\nu}^{MC}$";
	      eps_lepveto_S.num = lSel[0][Lep][WJets_enu];
	      eps_lepveto_S.den = lSel[0][JetPair][WJets_enu];
	      
	      eps_VBF_S.num = lSel[0][DPhiSIGNAL+iQCD][WJets_enu];
	      eps_VBF_S.den = lSel[0][Lep][WJets_enu];
	      
	      eps_lepsel_C.num = lSel[iCh][Lep][WJets_enu];
	      eps_lepsel_C.den = lSel[iCh][JetPair][WJets_enu];
	    
	      eps_VBF_C.num = lSel[iCh][DPhiSIGNAL+iQCD][WJets_enu];
	      eps_VBF_C.den = lSel[iCh][Lep][WJets_enu];
	    }
	    lNSdata.number = lNCdata.number * lNSMC.number / lNCMC.number;
	    lNSdata.error = lNSdata.number * sqrt(pow(lNCdata.error/lNCdata.number,2)+pow(lNCMC.error/lNCMC.number,2)+pow(lNSMC.error/lNSMC.number,2));
	    
	    
	    events result;
	    result.number = lNCdata.number *(eps_lepveto_S.eff()/eps_lepsel_C.eff())*(eps_VBF_S.eff()/eps_VBF_C.eff());
	    result.error = result.number * sqrt(pow(eps_lepveto_S.error()/eps_lepveto_S.eff(),2)+pow(eps_lepsel_C.error()/eps_lepsel_C.eff(),2)+pow(eps_VBF_S.error()/eps_VBF_S.eff(),2)+pow(eps_VBF_C.error()/eps_VBF_C.eff(),2)+pow(lNCdata.error/lNCdata.number,2));
	    
	    std::ostringstream lName;
	    lName << TOPDIR << "/" << lChannel[iCh] << "/MET" << MET[iMET] << "/" << SYST[iSyst] ;
            
	    if (iQCD==0) lName << "/DataDrivenWJetsTable_signal.txt";
	    else if (iQCD==1) lName << "/DataDrivenWJetsTable_QCD.txt";
	    std::ofstream lOutfile;
	    lOutfile.open(lName.str().c_str());
	    lOutfile << "\\begin{tabular}{|l|c|c|}" << std::endl
		     << "\\hline" << std::endl
		     << " & Signal Region & Control Region \\\\" << std::endl
		     << "\\hline" << std::endl
		     << "$N_{data}$ & XXX & " << lSel[iCh][DPhiSIGNAL+iQCD][Data].number << "\\\\" << std::endl;
	    lOutfile << std::setprecision(3)
		     << lBkgSMC.sample << " & " 
		     << " n/a & " 
		     << lBkgCMC << " \\\\" << std::endl
		     << eps_lepveto_S.name  << " & " 
		     << eps_lepveto_S  << " & " 
		     << eps_lepsel_C << " \\\\" << std::endl
		     << eps_VBF_S.name  << " & " 
		     << eps_VBF_S  << " & " 
		     << eps_VBF_C << " \\\\" << std::endl
		     << std::setprecision(3)
		     << lNSMC.sample  << " & " 
		     << lNSMC << " & " 
		     << lNCMC << " \\\\" << std::endl
		     << lNSdata.sample << " & " 
		     << "\\textcolor{red}{" << lNSdata << "} & " 
		     << lNCdata << " \\\\ " << std::endl 
		   << "\\hline" << std::endl
	      //<< "with Eff & " << result << " & \\\\" << std::endl
	      //	   << "\\hline" << std::endl
		     << "\\end{tabular}" << std::endl; 
	    lOutfile.close();

	    
	  }//loop on lep flavour
	
	}//loop on signal region
	
      }//loop on systematics
    
  }//loop on MET values
  
  return 0;
}//main
  

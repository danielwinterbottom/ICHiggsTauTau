#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <iomanip>

#include "TMath.h"


enum Selection {JetPair,MET,LooseMjj,DEta,Lep,DPhi,TightMjj};
enum Sample {QCD,GJets,Top,WJets,WJets_enu,WJets_munu,WJets_taunu,ZJets,ZJets_ll,ZJets_nunu,ZJets_vbf,VV,Data,Signal};


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

double sumSq(events evt1, events evt2){
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
    return eff()*sumSq(num,den);
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
  lSelVecSignal.push_back("JetPair");
  lSelVecSignal.push_back("MET");
  lSelVecSignal.push_back("LooseMjj");
  lSelVecSignal.push_back("DEta");
  lSelVecSignal.push_back("LeptonVeto");
  lSelVecSignal.push_back("DPhi");
  lSelVecSignal.push_back("TightMjj");

  std::vector<std::string> lSelVecControl;
  lSelVecControl.push_back("JetPair");
  lSelVecControl.push_back("MET");
  lSelVecControl.push_back("LooseMjj");
  lSelVecControl.push_back("DEta");
  lSelVecControl.push_back("WSelection");
  lSelVecControl.push_back("DPhi");
  lSelVecControl.push_back("TightMjj");

  const unsigned nSteps = lSelVecSignal.size();
  const unsigned nMets = 1;
  //  unsigned MET[nMets] = {130,0,70};
  unsigned MET[nMets] = {130};

  for (unsigned iMET(0); iMET<nMets; ++iMET){//loop on MET values

      std::cout << " -- Processing MET " << MET[iMET] << std::endl;

    for (unsigned iQCD(0); iQCD<2; ++iQCD){//loop on signal region, just 0 and 1

      std::cout << " -- Processing signal region " << iQCD << std::endl;

      std::ostringstream lFolder;
      lFolder << "MET" << MET[iMET] << "/DOQCD" << iQCD << "/";

      evtsArray lSel[3][nSteps];
      
      for (unsigned iS(0); iS<nSteps; ++iS){//loop on steps

	std::ifstream lTable;
	std::ostringstream lName;
	lName << "../TABLES/nunu/" << lFolder.str() << "/SummaryTable_" << lSelVecSignal[iS] << ".dat";
	lTable.open(lName.str().c_str());
	if(!lTable.is_open()){
	  cerr<<"Unable to open file: "<<lName.str()<<endl;
	  return 1; 
	}
	lTable>>lSel[0][iS];
	lTable.close();
	
	lName.str("");
	lName << "../TABLES/enu/" << lFolder.str() << "/SummaryTable_" << lSelVecControl[iS] << ".dat";
	lTable.open(lName.str().c_str());
	if(!lTable.is_open()){
	  cerr<<"Unable to open file: "<<lName.str()<<endl;
	  return 1; 
	}
	lTable>>lSel[1][iS];
	lTable.close();
	
	lName.str("");
	lName << "../TABLES/munu/" << lFolder.str() << "/SummaryTable_" << lSelVecControl[iS] << ".dat";
	lTable.open(lName.str().c_str());
	if(!lTable.is_open()){
	  cerr<<"Unable to open file: "<<lName.str()<<endl;
	  return 1; 
	}
	lTable>>lSel[2][iS];
	lTable.close();
	
      }//loop on steps


      if (iQCD==2) continue;

      //print Wjets components
      std::string lChannel[3] = {"nunu","enu","munu"};
      for (unsigned iCh(0); iCh<3; ++iCh){//loop on channel
	std::ostringstream lName;
	lName << "../TABLES/" << lChannel[iCh] << "/MET" << MET[iMET] << "/DOQCD" << iQCD << "/WandZJetsTable.txt";
	std::ofstream lOutfile;
	lOutfile.open(lName.str().c_str());

	lOutfile << "\\begin{tabular}{|l|c|c|c||c|c|c|}" << std::endl
		 << "\\hline" << std::endl
		 << "Step & W$\\rightarrow e\\nu$ & W$\\rightarrow\\mu\\nu$ & W$\\rightarrow\\tau\\nu$ & Z$\\rightarrow\\nu\\nu$ & Z$\\rightarrow$ll & EWK Z+2j  \\\\" << std::endl
		 << "\\hline" << std::endl
		 << "LooseMjj+DPhi &  $" 
		 << lSel[iCh][DPhi][WJets_enu].number  << "\\pm" << lSel[iCh][DPhi][WJets_enu].error << "$ & $" 
		 << lSel[iCh][DPhi][WJets_munu].number  << "\\pm" << lSel[iCh][DPhi][WJets_munu].error  << "$ & $" 
		 << lSel[iCh][DPhi][WJets_taunu].number  << "\\pm" << lSel[iCh][DPhi][WJets_taunu].error << "$ & $" 
		 << lSel[iCh][DPhi][ZJets_nunu].number  << "\\pm" << lSel[iCh][DPhi][ZJets_nunu].error << "$ & $" 
		 << lSel[iCh][DPhi][ZJets_ll].number  << "\\pm" << lSel[iCh][DPhi][ZJets_ll].error << "$ & $" 
		 << lSel[iCh][DPhi][ZJets_vbf].number  << "\\pm" << lSel[iCh][DPhi][ZJets_vbf].error 
		 << "$ \\\\" << std::endl
		 << "TightMjj+DPhi & $" 
		 << lSel[iCh][DPhi+1][WJets_enu].number  << "\\pm" << lSel[iCh][DPhi+1][WJets_enu].error << "$ & $" 
		 << lSel[iCh][DPhi+1][WJets_munu].number  << "\\pm" << lSel[iCh][DPhi+1][WJets_munu].error  << "$ & $" 
		 << lSel[iCh][DPhi+1][WJets_taunu].number  << "\\pm" << lSel[iCh][DPhi+1][WJets_taunu].error << "$ & $" 
		 << lSel[iCh][DPhi+1][ZJets_nunu].number  << "\\pm" << lSel[iCh][DPhi+1][ZJets_nunu].error << "$ & $" 
		 << lSel[iCh][DPhi+1][ZJets_ll].number  << "\\pm" << lSel[iCh][DPhi+1][ZJets_ll].error << "$ & $" 
		 << lSel[iCh][DPhi+1][ZJets_vbf].number  << "\\pm" << lSel[iCh][DPhi+1][ZJets_vbf].error 
		 << "$ \\\\" << std::endl
		 << "\\hline" << std::endl
		 << "\\end{tabular}" << std::endl;
	
	lOutfile.close();

      }//loop on channel

      for (unsigned int iMjj(0); iMjj<2; ++iMjj){//loop on Mjj WP
	
	for (unsigned iCh(1); iCh<3; ++iCh){//loop on lep flavour
	  
	  events lNCdata;
	  events lBkgCMC;
	  events lBkgSMC;
	  events lNCMC;
	  events lNSMC;
	  // std::cout << lSel[iCh][DPhi+iMjj][Data] 
	  // 	    << " " << lSel[iCh][DPhi+iMjj][Top]
	  // 	    << " " << lSel[iCh][DPhi+iMjj][VV]
	  // 	    << " " << lSel[iCh][DPhi+iMjj][ZJets]
	  // 	    << std::endl;

	  lNCdata = lSel[iCh][DPhi+iMjj][Data];
	  lNCdata -= lSel[iCh][DPhi+iMjj][Top];
	  lNCdata -= lSel[iCh][DPhi+iMjj][VV];
	  lNCdata -= lSel[iCh][DPhi+iMjj][ZJets];
	  lBkgCMC = lSel[iCh][DPhi+iMjj][Top];
	  lBkgCMC += lSel[iCh][DPhi+iMjj][VV];
	  lBkgCMC += lSel[iCh][DPhi+iMjj][ZJets];
	  lBkgCMC.sample = "$N_{EWK}$";
	  lBkgSMC = lSel[0][DPhi+iMjj][Top];
	  lBkgSMC += lSel[0][DPhi+iMjj][VV];
	  lBkgSMC += lSel[0][DPhi+iMjj][ZJets];
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
	    lNSMC = lSel[0][DPhi+iMjj][WJets_munu];
	    lNCMC = lSel[iCh][DPhi+iMjj][WJets_munu];
	    lNSMC.sample = "$N_{W\\rightarrow \\mu\\nu}^{MC}$";
	    lNCMC.sample = "$N_{W\\rightarrow \\mu\\nu}^{MC}$";
	    eps_lepveto_S.num = lSel[0][DPhi+iMjj][WJets_munu];
	    eps_lepveto_S.den = lSel[0][DEta][WJets_munu];
	    
	    eps_VBF_S.num = lSel[0][DEta][WJets_munu];
	    eps_VBF_S.den = lSel[0][JetPair][WJets_munu];
	    
	    eps_lepsel_C.num = lSel[iCh][DPhi+iMjj][WJets_munu];
	    eps_lepsel_C.den = lSel[iCh][DEta][WJets_munu];
	    
	    eps_VBF_C.num = lSel[iCh][DEta][WJets_munu];
	    eps_VBF_C.den = lSel[iCh][JetPair][WJets_munu];
	  }
	  else {
	    lNCdata.sample = "$N_{W\\rightarrow e\\nu}^{data}$";
	    lNSdata.sample = "$N_{W\\rightarrow e\\nu}^{data}$";
	    lNSMC = lSel[0][DPhi+iMjj][WJets_enu];
	    lNCMC = lSel[iCh][DPhi+iMjj][WJets_enu];
	    lNSMC.sample = "$N_{W\\rightarrow e\\nu}^{MC}$";
	    lNCMC.sample = "$N_{W\\rightarrow e\\nu}^{MC}$";
	    eps_lepveto_S.num = lSel[0][DPhi+iMjj][WJets_enu];
	    eps_lepveto_S.den = lSel[0][DEta][WJets_enu];
	    
	    eps_VBF_S.num = lSel[0][DEta][WJets_enu];
	    eps_VBF_S.den = lSel[0][JetPair][WJets_enu];
	    
	    eps_lepsel_C.num = lSel[iCh][DPhi+iMjj][WJets_enu];
	    eps_lepsel_C.den = lSel[iCh][DEta][WJets_enu];
	    
	    eps_VBF_C.num = lSel[iCh][DEta][WJets_enu];
	    eps_VBF_C.den = lSel[iCh][JetPair][WJets_enu];
	  }
	  lNSdata.number = lNCdata.number * lNSMC.number / lNCMC.number;
	  lNSdata.error = lNSdata.number * sqrt(pow(lNCdata.error/lNCdata.number,2)+pow(lNCMC.error/lNCMC.number,2)+pow(lNSMC.error/lNSMC.number,2));
	  

	  events result;
	  result.number = lNCdata.number *(eps_lepveto_S.eff()/eps_lepsel_C.eff())*(eps_VBF_S.eff()/eps_VBF_C.eff());
	  result.error = result.number * sqrt(pow(eps_lepveto_S.error()/eps_lepveto_S.eff(),2)+pow(eps_lepsel_C.error()/eps_lepsel_C.eff(),2)+pow(eps_VBF_S.error()/eps_VBF_S.eff(),2)+pow(eps_VBF_C.error()/eps_VBF_C.eff(),2)+pow(lNCdata.error/lNCdata.number,2));

	  std::ostringstream lName;
	  lName << "../TABLES/" << lChannel[iCh] << "/MET" << MET[iMET] << "/DOQCD" << iQCD ;
	  if (iMjj==0) lName << "/DataDrivenWJetsTable_looseMjj.txt";
	  else if (iMjj==1) lName << "/DataDrivenWJetsTable_tightMjj.txt";
	  std::ofstream lOutfile;
	  lOutfile.open(lName.str().c_str());
	  lOutfile << "\\begin{tabular}{|l|c|c|}" << std::endl
		   << "\\hline" << std::endl
		   << " & Signal Region & Control Region \\\\" << std::endl
		   << "\\hline" << std::endl;
	  lOutfile << std::setprecision(2)
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
	
      }//loop on Mjj WP

    }//loop on signal region

  }//loop on MET values

  return 0;
}//main

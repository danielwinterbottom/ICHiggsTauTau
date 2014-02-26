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
enum Selection {Trig,Lep,JetPair,DEta,MET,TightMjj,DPhiSIGNAL_noCJV,DPhiQCD_noCJV,CJVpass,DPhiSIGNAL_CJVpass,DPhiQCD_CJVpass,GenNoFilters,GenZMassFiltered};
enum Sample {QCD,GJets,Top,TTbar,TTbarPowheg,SingleTop,TW,
	     WJets,QCD_WJets,EWK_WJets,QCD_WJets_enu,EWK_WJets_enu,QCD_WJets_munu,EWK_WJets_munu,QCD_WJets_taunu,EWK_WJets_taunu,
	     ZJets,ZJets_ll,ZJets_nunu,ZJets_vbf,ZJets_vbf_nunu,
	     VV,Data,Signal};

struct events {
  std::string sample;
  double number;
  double stat;
  double syst;
  std::pair<double,double> jerSyst;// = std::pair<double,double>(0,0);
  double jerBetter;
  double jerWorse;
  std::pair<double,double> jesSyst;
  double jesUp;
  double jesDown;
  std::pair<double,double> puSyst;
  double puUp;
  double puDown;
  std::pair<double,double> eleeffSyst;
  double eleeffUp;
  double eleeffDown;
  std::pair<double,double> mueffSyst;
  double mueffUp;
  double mueffDown;

  double error() const{
    return sqrt(pow(stat,2)+pow(syst,2));
  }
  
  std::pair<double,double> totalError() const{
    return std::pair<double,double>(sqrt(pow(stat,2)+pow(totalSyst().first,2)),
				    -sqrt(pow(stat,2)+pow(totalSyst().second,2)));
  }
  
  std::pair<double,double> totalSyst() const{
    return std::pair<double,double>(sqrt(pow(syst,2)+pow(jerSyst.first,2)+pow(jesSyst.first,2)+pow(puSyst.first,2)+pow(eleeffSyst.first,2)+pow(mueffSyst.first,2)),
				    -sqrt(pow(syst,2)+pow(jerSyst.second,2)+pow(jesSyst.second,2)+pow(puSyst.second,2)+pow(eleeffSyst.second,2)+pow(mueffSyst.second,2)));
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

int extractZJetsBkg(){//main

  //OPTIONS
  std::string TOPDIR = "../TABLES/";//"../TABLESzbkgptcutcorrected/";
  bool dojes = false;
  bool dojer = false;
  bool doeleerr = false;
  bool domuerr = false;
  bool doWeights = false;
  bool verbose = true;
  bool dooldincorrecteff = false;

  double RQCD = 5.616;//Cross-section ratio
  double RQCDerr = 0.023;//Absolute error on cross-section ratio
  double REWK = 1.4803;//1.5625;
  double REWKerr = 0;
  double sigmaincmumuQCD = 3503700/3;
  double sigmaincmumuEWK = 304; //888/3;
  
  //SETUP
  std::vector<std::string> lSelVecSignal;
  lSelVecSignal.push_back("HLTMetClean");
  lSelVecSignal.push_back("IgnoreLeptons");
  lSelVecSignal.push_back("JetPair");
  lSelVecSignal.push_back("DEta");
  lSelVecSignal.push_back("MET");
  lSelVecSignal.push_back("TightMjj");
  lSelVecSignal.push_back("DPhiSIGNAL_noCJV");
  lSelVecSignal.push_back("DPhiQCD_noCJV");
  lSelVecSignal.push_back("CJVpass");
  lSelVecSignal.push_back("DPhiSIGNAL_CJVpass");
  lSelVecSignal.push_back("DPhiQCD_CJVpass");
  lSelVecSignal.push_back("GenNoFilters");
  lSelVecSignal.push_back("GenZMassFiltered");

  std::vector<std::string> lSelVecControl;
  lSelVecControl.push_back("HLTMetClean");
  lSelVecControl.push_back("ZSelection");
  lSelVecControl.push_back("JetPair");
  lSelVecControl.push_back("DEta");
  lSelVecControl.push_back("MET");
  lSelVecControl.push_back("TightMjj");
  lSelVecControl.push_back("DPhiSIGNAL_noCJV");
  lSelVecControl.push_back("DPhiQCD_noCJV");
  lSelVecControl.push_back("CJVpass");
  lSelVecControl.push_back("DPhiSIGNAL_CJVpass");
  lSelVecControl.push_back("DPhiQCD_CJVpass");
  lSelVecControl.push_back("GenNoFilters");
  lSelVecControl.push_back("GenZMassFiltered");
  

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
  latex.push_back("GenNoFilters");
  latex.push_back("GenZMassFiltered");

  //std::string lSuffix = "";
  const unsigned nWeights = 4;
  std::string lSuffixName[nWeights] = {"Central","_pu","_pu_trig","_pu_trig_idiso"};
  std::string lSuffix[nWeights] = {"","_pu","_pu_trig","_pu_trig_idiso"};

  const unsigned nSysts = 11;
  std::string SYSTNAME[nSysts]= {"JESUP","JESDOWN","JERBETTER","JERWORSE","ELEEFFUP","ELEEFFDOWN","MUEFFUP","MUEFFDOWN","PUUP","PUDOWN","CENTRAL"};//Order so numbers from systematics can be saved to be put in central table as syst errors two slots saved for pu weights and last is central
  std::string SYST[nSysts]= {"JESUP","JESDOWN","JERBETTER","JERWORSE","ELEEFFUP","ELEEFFDOWN","MUEFFUP","MUEFFDOWN","","",""};//Order so numbers from systematics can be saved to be put in central table as syst errors two slots saved for pu weights and last is central
  std::string lSuffixSyst[nSysts] = {"","","","","","","","","_puUp","_puDown",""};

  const unsigned nCh = 2;
  std::string lChannel[nCh] = {"nunuiglep","mumu"};

  const unsigned nSteps = lSelVecSignal.size();


  unsigned MET[3] = {130,0,70};

  for (unsigned iW(0); iW<(doWeights?nWeights:1); ++iW){//LOOP OVER WEIGHTS
    std::cout << "Processing weight: " << lSuffixName[iW] << std::endl;
    
    for (unsigned iMET(0); iMET<1; ++iMET){//loop on MET values
      
      std::cout << "  Processing MET cut: " << MET[iMET] <<" GeV"<< std::endl;
      
      bool fileExists[nCh][nSysts];
      for (unsigned iCh(0); iCh<nCh;++iCh){
	for (unsigned iSyst(0); iSyst<nSysts; ++iSyst){
	  fileExists[iCh][iSyst] = true;
	}
      }
      
      //VARIABLES TO STORE EVENT NUMBERS
      events result[nSysts];
      events resultQCD[nSysts];
      events resultEWK[nSysts];

      for (unsigned iSyst(0); iSyst<nSysts; ++iSyst){//loop over different systematics if not doing jes and jet automatically only does central
	if(!dojes){
	  if(iSyst==0||iSyst==1)continue;
	}
	if(!dojer){
	  if(iSyst==2||iSyst==3)continue;
	}
	if(!doeleerr){
	  if(iSyst==4||iSyst==5)continue;
	}
	if(!domuerr){
	  if(iSyst==6||iSyst==7)continue;
	}
	std::cout<< "    Processing systematic: " <<SYSTNAME[iSyst]<<std::endl;
	
	std::ostringstream lFolder;
	lFolder << "MET" << MET[iMET] << "/" << SYST[iSyst] << "/";
	
	evtsArray lSel[nCh][nSteps];


	for (unsigned iS(0); iS<nSteps; ++iS){//loop on steps
	  
	  std::ifstream lTable;

	  //READ SUMMARY TABLES INTO LSEL
	  for (unsigned iCh(0); iCh<nCh; ++iCh){//loop on channel
	    std::ostringstream lName;
	    if(iCh==0){
	      lName << TOPDIR << "/"<< lChannel[iCh] <<"/" << lFolder.str() << "/SummaryTable_" << lSelVecSignal[iS] << lSuffix[iW] << lSuffixSyst[iSyst]  << ".dat";
	    }
	    if(iCh==1){
	      lName << TOPDIR << "/"<< lChannel[iCh] <<"/" << lFolder.str() << "/SummaryTable_" << lSelVecControl[iS] << lSuffix[iW] << lSuffixSyst[iSyst] << ".dat";
	    }
	    lTable.open(lName.str().c_str());
	    if(!lTable.is_open()){
	      std::cerr<<"Unable to open file: "<<lName.str()<<std::endl;
	      fileExists[iCh][iSyst] = false;
	      //return 1; 
	    }
	    else {
	      lTable>>lSel[iCh][iS];
	      lTable.close();
	    }
	  }//end loop on channels
	}//end loop on steps
	
	//std::cout << "    TABLES have been read in." << std::endl;
	
	for (unsigned iCh(0); iCh<nCh; ++iCh){//loop on channel
	  if (!fileExists[iCh][iSyst]) continue;
	  //std::cout << "        Processing channel: " << lChannel[iCh] << std::endl;
	  
	  std::ostringstream lName;
	  std::ofstream lOutfile;
	  //print Zjets components to ZJetsTable
	  lName.str("");
	  lName << TOPDIR <<"/" << lChannel[iCh] << "/MET" << MET[iMET] << "/" << SYST[iSyst] << "/ZJetsTable" << lSuffix[iW] << lSuffixSyst[iSyst] << ".txt";
	  
	  lOutfile.open(lName.str().c_str());
	  
	  lOutfile << "\\begin{tabular}{|l|c|c|c|}" << std::endl
		   << "\\hline" << std::endl
		   << "Step & Z$\\rightarrow\\nu\\nu$  & Z$\\rightarrow$ll & EWK Z \\\\" << std::endl
		   << "\\hline" << std::endl;

	  for (unsigned iS(5); iS<nSteps; ++iS){//loop on steps
	    lOutfile << latex[iS];
	    lOutfile << std::setprecision(3) << " &  $"
		     << lSel[iCh][iS][ZJets_nunu].number  << "\\pm" << lSel[iCh][iS][ZJets_nunu].error() << "$ & $" 
		     << lSel[iCh][iS][ZJets_ll].number  << "\\pm" << lSel[iCh][iS][ZJets_ll].error() << "$ & $" 
		     << lSel[iCh][iS][ZJets_vbf].number << "\\pm" << lSel[iCh][iS][ZJets_vbf].error()
		     << "$ \\\\" << std::endl;
	  }//loop on steps
	  lOutfile << "\\hline" << std::endl
		   << "\\end{tabular}" << std::endl;
	  lOutfile.close();

	}//end loop on channel

	if(!fileExists[0][iSyst] || !fileExists[1][iSyst]) continue;
	if(verbose)std::cout << "Extracting Znunu results:"<<std::endl;

	//Get relevant variables
	events nCData = lSel[1][DPhiSIGNAL_CJVpass][Data];
	std::cout<<"nCData & "<<nCData<<std::endl;
	events nCBkg = lSel[1][DPhiSIGNAL_CJVpass][Top];
	nCBkg += lSel[1][DPhiSIGNAL_CJVpass][VV];
	std::cout<<"nCBkg & "<<nCBkg<<std::endl;

	events nSMC = lSel[0][DPhiSIGNAL_CJVpass][ZJets_ll];
	nSMC += lSel[0][DPhiSIGNAL_CJVpass][ZJets_vbf];
	events nSMCQCD = lSel[0][DPhiSIGNAL_CJVpass][ZJets_ll];
	events nSMCEWK = lSel[0][DPhiSIGNAL_CJVpass][ZJets_vbf];
	std::cout<<"nSMC QCD & "<<nSMCQCD<<std::endl;
	std::cout<<"nSMC EWK & "<<nSMCEWK<<std::endl;
	std::cout<<"nSMC Total & "<<nSMC<<std::endl;

	

	events nCMC = lSel[1][DPhiSIGNAL_CJVpass][ZJets_ll];
	nCMC += lSel[1][DPhiSIGNAL_CJVpass][ZJets_vbf];
	events nCMCQCD = lSel[1][DPhiSIGNAL_CJVpass][ZJets_ll];
	events nCMCEWK = lSel[1][DPhiSIGNAL_CJVpass][ZJets_vbf];
	std::cout<<"nCMCQCD Total &  "<<nCMCQCD<<std::endl;
	std::cout<<"nCMCEWK Total &  "<<nCMCEWK<<std::endl;
	std::cout<<"nCMC Total &  "<<nCMC<<std::endl;

	events nGen = lSel[0][GenNoFilters][ZJets_ll];
	nGen += lSel[0][GenNoFilters][ZJets_vbf];
	events nGenQCD = lSel[0][GenNoFilters][ZJets_ll];
	events nGenEWK = lSel[0][GenNoFilters][ZJets_vbf];
	std::cout<<"nGen QCD & "<<nGenQCD<<std::endl;
	std::cout<<"nGen EWK &  "<<nGenEWK<<std::endl;
	std::cout<<"nGen Total  $ "<<nGen<<std::endl;

	events nGenZMassFiltered = lSel[0][GenZMassFiltered][ZJets_ll];
	nGenZMassFiltered += lSel[0][GenZMassFiltered][ZJets_vbf];
	events nGenZMassFilteredQCD = lSel[0][GenZMassFiltered][ZJets_ll];
	events nGenZMassFilteredEWK = lSel[0][GenZMassFiltered][ZJets_vbf];
	std::cout<<"nGenZMassFiltered QCD  $ "<<nGenZMassFilteredQCD<<std::endl;
	std::cout<<"nGenZMassFiltered EWK  $ "<<nGenZMassFilteredEWK<<std::endl;
	std::cout<<"nGenZMassFiltered Total  $ "<<nGenZMassFiltered<<std::endl;


	//Calculate result and error
	if(dooldincorrecteff){
	  result[iSyst].sample="$N_{Z\rightarrow\\mu\\mu}^{Data}$";
	  result[iSyst].number=(nCData.number-nCBkg.number)*nSMC.number/nCMC.number*nGenZMassFiltered.number/nGen.number*RQCD;
	  result[iSyst].stat=result[iSyst].number*(nCData.stat/nCData.number);
	  result[iSyst].syst=result[iSyst].number*sqrt(pow(nCBkg.stat,2)+pow(nSMC.stat/nSMC.number,2)+pow(nCMC.stat/nCMC.number,2));
	  //!!DO R ERROR AND SYSTS
	  std::cout<<result[iSyst].number<<" \\pm "<<result[iSyst].stat<<" (stat.) \\pm "<<result[iSyst].syst<<" (MC stat.)"<<std::endl;
	}
	else{
	  result[iSyst].sample="$N_{Z\rightarrow\\mu\\mu}^{Data}$";
	  double sigmaincnunuewk=sigmaincmumuEWK*REWK;
	  double effsvbfewk=nSMCEWK.number/nGenZMassFilteredEWK.number;
	  double sigmaincnunuqcd=sigmaincmumuQCD*RQCD;
	  double effsvbfqcd=nSMCQCD.number/nGenZMassFilteredQCD.number;
	  double effcvbfewk=nCMCEWK.number/nGenEWK.number;
	  double effcvbfqcd=nCMCQCD.number/nGenQCD.number;
	  
	  result[iSyst].number=(nCData.number-nCBkg.number)*(sigmaincnunuewk*effsvbfewk+sigmaincnunuqcd*effsvbfqcd)/(sigmaincmumuEWK*effcvbfewk+sigmaincmumuQCD*effcvbfqcd);
	  result[iSyst].stat=result[iSyst].number*(nCData.stat/nCData.number);
	  result[iSyst].syst=result[iSyst].number*sqrt(pow(nCBkg.stat,2)+pow(nSMCQCD.stat/nSMCQCD.number,2)+pow(nSMCEWK.stat/nSMCEWK.number,2)+pow(nCMCQCD.stat/nCMCQCD.number,2)+pow(nCMCEWK.stat/nCMCEWK.number,2)+pow(nGenQCD.stat/nGenQCD.number,2)+pow(nGenEWK.stat/nGenEWK.number,2)+pow(nGenZMassFilteredQCD.stat/nGenZMassFilteredQCD.number,2)+pow(nGenZMassFilteredEWK.stat/nGenZMassFilteredEWK.number,2));
	  //!!DO R ERROR AND SYSTS
	  std::cout<<result[iSyst].number<<" \\pm "<<result[iSyst].stat<<" (stat.) \\pm "<<result[iSyst].syst<<" (MC stat.)"<<std::endl;
	}

	if(verbose){
	  double sigmaincnunuewk=sigmaincmumuEWK*REWK;
	  double effsvbfewk=nSMCEWK.number/nGenZMassFilteredEWK.number;
	  double numewk = sigmaincnunuewk*effsvbfewk;
	  double sigmaincnunuqcd=sigmaincmumuQCD*RQCD;
	  double effsvbfqcd=nSMCQCD.number/nGenZMassFilteredQCD.number;
	  double numqcd = sigmaincnunuqcd*effsvbfqcd;
	  std::cout<<"sigma(nunu)_ewk: "<<sigmaincnunuewk<<" eff^VBF_S(ewk): "<<effsvbfewk<<" sigma(nunu)_qcd: "<<sigmaincnunuqcd<<" eff^VBF_S(qcd): "<<effsvbfqcd<<std::endl; 
	  std::cout<<"ewk numerator cont.: "<<numewk<<" qcd numerator cont.: "<<numqcd<<std::endl;
	  
	  double effcvbfewk=nCMCEWK.number/nGenEWK.number;
	  double denewk = sigmaincmumuEWK*effcvbfewk;
	  double effcvbfqcd=nCMCQCD.number/nGenQCD.number;
	  double denqcd = sigmaincmumuQCD*effcvbfqcd;
	  std::cout<<"sigma(mumu)_ewk: "<<sigmaincmumuEWK<<" eff^VBF_C(ewk): "<<effcvbfewk<<" sigma(mumu)_qcd: "<<sigmaincmumuQCD<<" eff^VBF_C(qcd): "<<effcvbfqcd<<std::endl; 
	  std::cout<<"ewk denominator cont.: "<<denewk<<" qcd denominator cont.: "<<denqcd<<std::endl;
	}
      }//end loop on systs

      if(dojes){
	double jesUp = result[0].number - result[nSysts-1].number;
	double jesDown = result[1].number - result[nSysts-1].number;
	result[nSysts-1].jesUp = jesUp;
	result[nSysts-1].jesDown = jesDown;
	result[nSysts-1].jesSyst.first = std::max(std::max(jesUp,0.),std::max(jesDown,0.));
	result[nSysts-1].jesSyst.second = std::min(std::min(jesUp,0.),std::min(jesDown,0.));
      }
      if(dojer){
	double jerBetter = result[2].number - result[nSysts-1].number;
	double jerWorse = result[3].number - result[nSysts-1].number;
	result[nSysts-1].jerBetter = jerBetter;
	result[nSysts-1].jerWorse = jerWorse;
	result[nSysts-1].jerSyst.first = std::max(std::max(jerBetter,0.),std::max(jerWorse,0.));
	result[nSysts-1].jerSyst.second = std::min(std::min(jerBetter,0.),std::min(jerWorse,0.));
      }
      if(doeleerr){
	double eleeffUp = result[4].number - result[nSysts-1].number;
	double eleeffDown = result[5].number - result[nSysts-1].number;
	result[nSysts-1].eleeffUp = eleeffUp;
	result[nSysts-1].eleeffDown = eleeffDown;
	result[nSysts-1].eleeffSyst.first = std::max(std::max(eleeffUp,0.),std::max(eleeffDown,0.));
	result[nSysts-1].eleeffSyst.second = std::min(std::min(eleeffUp,0.),std::min(eleeffDown,0.));    
      }
      if(domuerr){
	double mueffUp = result[6].number - result[nSysts-1].number;
	double mueffDown = result[7].number - result[nSysts-1].number;
	result[nSysts-1].mueffUp = mueffUp;
	result[nSysts-1].mueffDown = mueffDown;
	result[nSysts-1].mueffSyst.first = std::max(std::max(mueffUp,0.),std::max(mueffDown,0.));
	result[nSysts-1].mueffSyst.second = std::min(std::min(mueffUp,0.),std::min(mueffDown,0.));    
      }
      double puUp = result[8].number - result[nSysts-1].number;
      double puDown = result[9].number - result[nSysts-1].number;
      result[nSysts-1].puUp = puUp;
      result[nSysts-1].puDown = puDown;
      result[nSysts-1].puSyst.first = std::max(std::max(puUp,0.),std::max(puDown,0.));
      result[nSysts-1].puSyst.second = std::min(std::min(puUp,0.),std::min(puDown,0.));    
      
      //!!OUTPUT THINGS
      std::cout<<result[nSysts-1].number<<std::endl;
    }//end loop on met
  }//end loop on weights
  return 0;
}//end program
	//!!DONE UP TO HERE AFTERWARDS IS BEING DEVELOPED

	//!!END OF STUFF IN PROGRESS

	/*!!TO BE DONE:
	 */


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
enum Selection {Trig,Lep,JetPair,DEta,MET,TightMjj,DPhiSIGNAL_noCJV,DPhiQCD_noCJV,CJVpass,CJVfail,DPhiSIGNAL_CJVpass,DPhiQCD_CJVpass,DPhiSIGNAL_CJVfail,DPhiQCD_CJVfail,GenNoFilters,GenZMassFiltered};
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
  events operator*=(const double &rhs){
    this->number = this->number*rhs;
    this->stat = this->stat*rhs;
    this->syst = this->syst*rhs;
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

int extractQCDBkg(){//main

  //OPTIONS
  std::string TOPDIR = "../TABLESqcd/";//"../TABLES/";//
  bool dojes = false;
  bool dojer = false;
  bool doeleerr = false;
  bool domuerr = false;
  bool doWeights = false;
  bool verbose = true;
  bool dooldincorrecteff = false;

  double RQCD = 5.651;//Cross-section ratio                                                                                                         
  double RQCDerr = 0.023;//Absolute error on cross-section ratio                                                                                        
  double sigmaincmumuQCD = 3503700./3;
  double sigmaincnunuQCD = sigmaincmumuQCD*RQCD;
  double sigmaincmumuEWK = 303; //888/3;                                                                                                                   
  double sigmaincnunuEWK = 460*3;
  double lumi=19.5003

  //SETUP
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
  lSelVecSignal.push_back("DPhiSIGNAL_CJVfail");
  lSelVecSignal.push_back("DPhiQCD_CJVfail");
  lSelVecSignal.push_back("GenNoFilters");
  lSelVecSignal.push_back("GenZMassFiltered");


  std::vector<std::string> lSelVecControl;
  lSelVecControl.push_back("HLTMetClean");
  lSelVecControl.push_back("IgnoreLeptons");
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
  lSelVecControl.push_back("DPhiSIGNAL_CJVfail");
  lSelVecControl.push_back("DPhiQCD_CJVfail");
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
  latex.push_back("CJVfail");
  latex.push_back("SIG CJVpass");
  latex.push_back("QCD CJVpass");
  latex.push_back("SIG CJVfail");
  latex.push_back("QCD CJVfail");
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

  const unsigned nCh = 4;
  std::string lChannel[nCh] = {"nunu","nunulowmet","nunuiglep","nunulowmetiglep"};

  const unsigned nSteps = lSelVecSignal.size();


  unsigned MET[3] = {130,0,70};

  for (unsigned iW(0); iW<(doWeights?nWeights:1); ++iW){//LOOP OVER WEIGHTS
    std::cout << "Processing weight: " << lSuffixName[iW] << std::endl;
    
    //VARIABLES TO STORE EVENT NUMBERS
    for (unsigned iMET(0); iMET<1; ++iMET){//loop on MET values

      events result[nSysts];
      events resultQCD[nSysts];
      events resultEWK[nSysts];
      
      std::cout << "  Processing MET cut: " << MET[iMET] <<" GeV"<< std::endl;
      
      bool fileExists[nCh][nSysts];
      for (unsigned iCh(0); iCh<nCh;++iCh){
	for (unsigned iSyst(0); iSyst<nSysts; ++iSyst){
	  fileExists[iCh][iSyst] = true;
	}
      }
      
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
	    if(iCh==0||iCh==1){
	      lName << TOPDIR << "/"<< lChannel[iCh] <<"/" << lFolder.str() << "/SummaryTable_" << lSelVecSignal[iS] << lSuffix[iW] << lSuffixSyst[iSyst]  << ".dat";
 	    }
 	    if(iCh==2||iCh==3){
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
	
	if(!fileExists[0][iSyst]||!fileExists[1][iSyst]) continue;
	if(verbose)std::cout << "Extracting QCD results:"<<std::endl;

	//Get relevant variables
	events nZllEWK[3];
	events nZEWKGEN[3];
	events nZllQCD[3];
	events nZQCDGEN[3];
	events nZnnEWK[3];
	events nZnnQCD[3];
	events nZ[3];

	events nData[3];
	
	for(int iReg=0;iReg<3;iReg++){
	  int chan;
	  int cjvpass;
	  string RegName;
	  if(iReg==0){
	    chan=1;
	    cjvpass=DPhiSIGNAL_CJVfail;
	    RegName="A";
	  }
	  if(iReg==1){
	    chan=0;
	    cjvpass=DPhiSIGNAL_CJVfail;
	    RegName="B";
	  }
	  if(iReg==2){
	    chan=1;
	    cjvpass=DPhiSIGNAL_CJVpass;
	    RegName="C";
	  }

	  else std::cout<<"ERROR: iReg out of range"<<std::endl;

	  //CALCULATE Z BACKGROUND
	  nZllEWK[iReg] = lSel[chan+2][cjvpass][ZJets_vbf];
	  nZEWKGEN[iReg] = lSel[chan+2][GenZMassFiltered][ZJets_vbf];
	  nZllQCD[iReg] = lSel[chan+2][cjvpass][ZJets_ll];
	  nZQCDGEN[iReg] = lSel[chan+2][GenZMassFiltered][ZJets_qcd];
	  nZnnEWK[iReg].number=nZllEWK[iReg].number/nZEWKGEN[iReg].number*sigmaincnunuEWK*lumi;
	  nZnnEWK[iReg].stat=sqrt(pow(nZllEWK[iReg].stat/nZEWK[iReg].number,2)+pow(nZEWKGEN[iReg].stat/nZEWKGEN[iReg].number,2))*nZEWK[iReg].number/nZEWKGEN[iReg].number*sigmaincnunuEWK*lumi;
	  nZnnQCD[iReg].number=nZllQCD[iReg].number/nZQCDGEN[iReg].number*sigmaincnunuQCD*lumi;
	  nZnnQCD[iReg].stat=sqrt(pow(nZllQCD[iReg].stat/nZQCD[iReg].number,2)+pow(nZQCDGEN[iReg].stat/nZQCDGEN[iReg].number,2))*nZQCD[iReg].number/nZQCDGEN[iReg].number*sigmaincnunuQCD*lumi;
	  events nZ[iReg]=nZnnEWK[iReg];
	  nZ[iReg]+=nZnnQCD[iReg];
	  nZ[iReg]+=nZllEWK[iReg];
	  nZ[iReg]+=nZllQCD[iReg];
										   
	  nData[iReg]=lSel[chan][cjvpass][Data];
	  std::cout<<"n"<<RegName<<"Data & "<<nData[iReg]<std::endl;
	  nBkg[iReg]=lSel[chan][cjvpass][Top];
	  nBkg[iReg] += lSel[chan][cjvpass][VV];
	  nBkg[iReg] += lSel[chan][cjvpass][WJets];
	  nBkg[iReg] += nZ[iReg];
	  std::cout<<"n"<<RegName<<"Bkg & "<<nBkg[iReg]<<std::endl;
	}

	//OLD IMPLEMENTATION
// 	events nAData = lSel[1][DPhiSIGNAL_CJVfail][Data];
// 	std::cout<<"nAData & "<<nAData<<std::endl;
// 	events nABkg = lSel[1][DPhiSIGNAL_CJVfail][Top];
// 	nABkg += lSel[1][DPhiSIGNAL_CJVfail][VV];
// 	nABkg += lSel[1][DPhiSIGNAL_CJVfail][WJets];
// 	nABkg += nAZ;
// 	std::cout<<"nABkg & "<<nABkg<<std::endl;

// 	events nBData = lSel[0][DPhiSIGNAL_CJVfail][Data];
// 	std::cout<<"nBData & "<<nBData<<std::endl;
// 	events nBBkg = lSel[0][DPhiSIGNAL_CJVfail][Top];
// 	nBBkg += lSel[0][DPhiSIGNAL_CJVfail][VV];
// 	nBBkg += lSel[0][DPhiSIGNAL_CJVfail][ZJets];
// 	nBBkg += lSel[0][DPhiSIGNAL_CJVfail][WJets];
// 	std::cout<<"nBBkg & "<<nBBkg<<std::endl;

// 	events nCData = lSel[1][DPhiSIGNAL_CJVpass][Data];
// 	std::cout<<"nCData & "<<nCData<<std::endl;
// 	events nCBkg = lSel[1][DPhiSIGNAL_CJVpass][Top];
// 	nCBkg += lSel[1][DPhiSIGNAL_CJVpass][VV];
// 	nCBkg += lSel[1][DPhiSIGNAL_CJVpass][ZJets];
// 	nCBkg += lSel[1][DPhiSIGNAL_CJVpass][WJets];
// 	std::cout<<"nCBkg & "<<nCBkg<<std::endl;

	//Calculate result and error
	result[iSyst].sample="$N_{QCD}^{Data}$";
		
	result[iSyst].number=(nData[1].number-nBkg[1].number)*(nData[2].number-nBkg[2].number)/(nData[0].number-nBkg[0].number);
	result[iSyst].stat=result[iSyst].number*sqrt((pow(nData[1].stat,2)+pow(nData[2].stat,2))/pow((nData[1].number+nData[2].number),2)+pow(nData[0].stat/nData[0].number,2));
	result[iSyst].syst=result[iSyst].number*sqrt((pow(nBkg[1].stat,2)+pow(nBkg[2].stat,2))/pow((nBkg[1].number+nBkg[2].number),2)+pow(nBkg[0].stat/nBkg[0].number,2));
	std::cout<<result[iSyst].number<<" \\pm "<<result[iSyst].stat<<" (stat.) \\pm "<<result[iSyst].syst<<" (MC stat.)"<<std::endl;
	

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



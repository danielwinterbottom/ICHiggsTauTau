#include <iostream>
#include <string>
#include <fstream>
#include <vector>






int mcfmStudy(){


  const unsigned nS = 3;
  double xs[nS] = {181,51.1,23.04};
  double nEvts[nS] = {21852156.0, 11015445.0, 6402827.0};
  double lData = 19617;

  double lNorm[nS];

  unsigned nInMassRange[nS];
  unsigned nZjj[nS];
  unsigned nZjj_VBF[nS];
  unsigned nZjj_VBF_MET[nS];
  unsigned nZjj_VBF_MET_DPhi[nS];
  unsigned nZmm[nS];
  unsigned nZjjmm_noDR[nS];
  unsigned nZjjmm[nS];
  unsigned nZjjmm_VBF[nS];
  unsigned nZjjmm_VBF_MET[nS];
  unsigned nZjjmm_VBF_MET_DPhi[nS];

  const unsigned nP = 4;

  double nZjj_tot[nP][2];
  double nZjjmm_tot[nP][2];

  string sel[nP] = {"jj","VBF","MET","DPhi"};

  double eff[2][nP-1][2];
  double ratio[nP-1][2];

  nInMassRange[0] = 6753765;
  nZjj[0] = 551395;
  nZjj_VBF[0] = 1638;
  nZjj_VBF_MET[0] = 264;
  nZjj_VBF_MET_DPhi[0] = 73;
  nZmm[0] = 2905975;
  nZjjmm_noDR[0] = 260223;
  nZjjmm[0] = 230808;
  nZjjmm_VBF[0] = 507;
  nZjjmm_VBF_MET[0] = 153;
  nZjjmm_VBF_MET_DPhi[0] = 45;
  
  nInMassRange[1] = 3409452;
  nZjj[1] = 819405;
  nZjj_VBF[1] = 3912;
  nZjj_VBF_MET[1] = 657;
  nZjj_VBF_MET_DPhi[1] = 144;
  nZmm[1] = 1500917;
  nZjjmm_noDR[1] = 386327;
  nZjjmm[1] = 344017;
  nZjjmm_VBF[1] = 1226;
  nZjjmm_VBF_MET[1] = 318;
  nZjjmm_VBF_MET_DPhi[1] = 85;

  nInMassRange[2] = 1981254;
  nZjj[2] = 1018916;
  nZjj_VBF[2] = 8636;
  nZjj_VBF_MET[2] = 1697;
  nZjj_VBF_MET_DPhi[2] = 382;
  nZmm[2] = 891311;
  nZjjmm_noDR[2] = 478114;
  nZjjmm[2] = 425094;
  nZjjmm_VBF[2] = 2790;
  nZjjmm_VBF_MET[2] = 860;
  nZjjmm_VBF_MET_DPhi[2] = 222;


  for (unsigned iP(0); iP<nP; ++iP){
    for (unsigned iE(0); iE<2; ++iE){
      nZjj_tot[iP][iE] = 0;
      nZjjmm_tot[iP][iE] = 0;
    }
  }

  for (unsigned iS(0); iS<nS; ++iS){//loop on samples

    lNorm[iS] = lData*xs[iS]/nEvts[iS];
    nZjj_tot[0][0] += nZjj[iS]*lNorm[iS];
    nZjj_tot[0][1] += pow(sqrt(nZjj[iS])*lNorm[iS],2);
    nZjj_tot[1][0] += nZjj_VBF[iS]*lNorm[iS];
    nZjj_tot[1][1] += pow(sqrt(nZjj_VBF[iS])*lNorm[iS],2);
    nZjj_tot[2][0] += nZjj_VBF_MET[iS]*lNorm[iS];
    nZjj_tot[2][1] += pow(sqrt(nZjj_VBF_MET[iS])*lNorm[iS],2);
    nZjj_tot[3][0] += nZjj_VBF_MET_DPhi[iS]*lNorm[iS];
    nZjj_tot[3][1] += pow(sqrt(nZjj_VBF_MET_DPhi[iS])*lNorm[iS],2);

    nZjjmm_tot[0][0] += nZjjmm[iS]*lNorm[iS];
    nZjjmm_tot[0][1] += pow(sqrt(nZjjmm[iS])*lNorm[iS],2);
    nZjjmm_tot[1][0] += nZjjmm_VBF[iS]*lNorm[iS];
    nZjjmm_tot[1][1] += pow(sqrt(nZjjmm_VBF[iS])*lNorm[iS],2);
    nZjjmm_tot[2][0] += nZjjmm_VBF_MET[iS]*lNorm[iS];
    nZjjmm_tot[2][1] += pow(sqrt(nZjjmm_VBF_MET[iS])*lNorm[iS],2);
    nZjjmm_tot[3][0] += nZjjmm_VBF_MET_DPhi[iS]*lNorm[iS];
    nZjjmm_tot[3][1] += pow(sqrt(nZjjmm_VBF_MET_DPhi[iS])*lNorm[iS],2);

  }//loop on samples


  for (unsigned iP(0); iP<nP; ++iP){
    nZjj_tot[iP][1] = sqrt(nZjj_tot[iP][1]);
    nZjjmm_tot[iP][1] = sqrt(nZjjmm_tot[iP][1]);

    if (iP>0){
      eff[0][iP-1][0] = nZjj_tot[iP][0]/nZjj_tot[0][0];
      eff[1][iP-1][0] = nZjjmm_tot[iP][0]/nZjjmm_tot[0][0];

      eff[0][iP-1][1] = eff[0][iP-1][0]*sqrt(pow(nZjj_tot[iP][1]/nZjj_tot[iP][0],2)+pow(nZjj_tot[0][1]/nZjj_tot[0][0],2)-2*nZjj_tot[iP][1]*nZjj_tot[0][1]/(nZjj_tot[iP][0]*nZjj_tot[0][0]));
      eff[1][iP-1][1] = eff[1][iP-1][0]*sqrt(pow(nZjjmm_tot[iP][1]/nZjjmm_tot[iP][0],2)+pow(nZjjmm_tot[0][1]/nZjjmm_tot[0][0],2)-2*nZjjmm_tot[iP][1]*nZjjmm_tot[0][1]/(nZjjmm_tot[iP][0]*nZjjmm_tot[0][0]));

      ratio[iP-1][0] = eff[0][iP-1][0]/eff[1][iP-1][0];
      ratio[iP-1][1] = ratio[iP-1][0]*sqrt(pow(eff[0][iP-1][1]/eff[0][iP-1][0],2)+pow(eff[1][iP-1][1]/eff[1][iP-1][0],2));

    }
  } 


  std::cout << " & Signal & Control & Ratio \\\\" << std::endl
	    << "\\hline" << std::endl;
  for (unsigned iP(0); iP<nP-1; ++iP){
    
    std::cout << sel[iP+1] << " & $" << eff[0][iP][0] << " \\pm " << eff[0][iP][1] << "$" 
	      << " & $"  << eff[1][iP][0] << " \\pm " << eff[1][iP][1] << "$" 
	      << " & $"  << ratio[iP][0] << " \\pm " << ratio[iP][1] << "$" 
	      << std::endl;
	       
  }

  std::cout << "\\hline" << std::endl;

  return 0;

}//main

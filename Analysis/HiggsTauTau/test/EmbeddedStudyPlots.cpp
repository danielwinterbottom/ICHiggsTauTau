#include <iostream>
#include <vector>
#include <map>
#include "TCanvas.h"
#include "TH1F.h"

#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"


using namespace std;
using namespace ic;

int main(int argc, char* argv[]){
	TH1::AddDirectory(false);
	HTTAnalysis ana(channel::mt, "2012", 1);
	ana.ReadTrees("output/Moriond_2012_NTuples");
	ana.ParseParamFile("scripts/Moriond_params_2012.dat");
	TCanvas canv("demo","demo");

	std::string os_sel = "os && mt_1 < 20.";
	std::string boost_high = "";

	TH1F result = ana.GetShape("m_vis(15,0,150)","DYJetsToTauTauSoup",os_sel, boost_high,"wt");
	std::cout << Integral(&result) << "  " << Error(&result) << std::endl;

	auto stats = ana.GetRate("DYJetsToTauTauSoup","os && mt_1 < 20.", "n_jet>=1","wt");
	std::cout << stats.first << "  " << stats.second << std::endl;
	result.Scale(stats.first / Integral(&result));
	std::cout << Integral(&result) << "  " << Error(&result) << std::endl;
	result.Draw();
	canv.SaveAs("demo.pdf");

  return 0;
}


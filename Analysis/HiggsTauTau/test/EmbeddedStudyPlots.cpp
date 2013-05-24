#include <iostream>
#include <vector>
#include <map>
#include "TCanvas.h"
#include "TH1F.h"
#include "boost/bind.hpp"

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

	std::string os_sel 	= "os && mt_1 < 20.";
	std::string cat_vbf = "n_jet>=2 && mjj>500. && jdeta>3.5";
	std::string cat_vbf_loose = "n_jet>=2 && mjj>200. && jdeta>2.0";
	std::string var = "m_vis(15,0,150)";
	std::string wt = "wt";

	//// Data
	auto data_norm = ana.GetRate("Data", os_sel, cat_vbf, wt);
	TH1F data_hist = ana.GetShape(var, "Data", os_sel, cat_vbf, wt);
	SetNorm(&data_hist, data_norm.first);
	std::cout << "Data: " << data_norm.first << "  " << data_norm.second << std::endl;

	// BlindHistogram(&data, 60., 120.);

	//// Top
	auto top_norm = ana.GetLumiScaledRate("TT", os_sel, cat_vbf, wt);
	TH1F top_hist = ana.GetLumiScaledShape(var, "TT", os_sel, cat_vbf_loose, wt);
	SetNorm(&top_hist, top_norm.first);
	std::cout << "TOP:  " << top_norm.first << "  " << top_norm.second << std::endl;

	auto zll_norm = ana.GetLumiScaledRate("DYJetsToLLSoup", os_sel, cat_vbf, wt);
	TH1F zll_hist = ana.GetLumiScaledShape(var, "DYJetsToLLSoup", os_sel, cat_vbf_loose, wt);
	SetNorm(&zll_hist, zll_norm.first);
	std::cout << "ZLL:  " << zll_norm.first << "  " << zll_norm.second << std::endl;

	//// Diboson
	std::vector<std::string> vv_samples = {
		"WWJetsTo2L2Nu", "WZJetsTo2L2Q", "WZJetsTo3LNu",
		"ZZJetsTo2L2Nu", "ZZJetsTo2L2Q", "ZZJetsTo4L",
		"T-tW", "Tbar-tW"
	};
	auto vv_norm = ana.GetLumiScaledRate(vv_samples, os_sel, cat_vbf, wt);
	TH1F vv_hist = ana.GetLumiScaledShape(var, vv_samples, os_sel, cat_vbf_loose, wt);
	SetNorm(&vv_hist, vv_norm.first);
	std::cout << "VV:   " << vv_norm.first << "  " << vv_norm.second << std::endl;

	auto ztt_norm = ana.GetRateViaRefEfficiency("Embedded", "DYJetsToTauTauSoup", "os", "", os_sel, cat_vbf, wt);
	TH1F ztt_shape = ana.GetShape(var, "Embedded", os_sel, cat_vbf, wt);
	SetNorm(&ztt_shape, ztt_norm.first);
	std::cout << "ZTT:  " << ztt_norm.first << "  " << ztt_norm.second << std::endl;

	std::vector<std::string> w_sub_samples = {
		"WWJetsTo2L2Nu", "WZJetsTo2L2Q", "WZJetsTo3LNu",
		"ZZJetsTo2L2Nu", "ZZJetsTo2L2Q", "ZZJetsTo4L",
		"T-tW", "Tbar-tW", "TT", "DYJetsToTauTauSoup", "DYJetsToLLSoup"
	};
	auto w_norm = ana.GetRateViaWMethod("WJetsToLNuSoup", cat_vbf_loose, "mt_1>60. && mt_1<120.", "mt_1<20.", 
			"Data", cat_vbf, "os && mt_1>60. && mt_1<120.", w_sub_samples, wt, { 
		{"Embedded", boost::bind(&HTTAnalysis::GetRateViaRefEfficiency, &ana, "Embedded", "DYJetsToTauTauSoup", "os", "", "os && mt_1>60. && mt_1<120.", cat_vbf, wt)}
	});
	std::cout << "W:    " << w_norm.first << "  " << w_norm.second << std::endl;

	std::vector<std::string> qcd_sub_samples = {
		"WWJetsTo2L2Nu", "WZJetsTo2L2Q", "WZJetsTo3LNu",
		"ZZJetsTo2L2Nu", "ZZJetsTo2L2Q", "ZZJetsTo4L",
		"T-tW", "Tbar-tW", "TT", "DYJetsToTauTauSoup", "DYJetsToLLSoup",
		"WJetsToLNuSoup"
	};
	std::map<std::string, std::function<std::pair<double,double>()>> dummy;



	auto qcd_norm = ana.GetRateViaQCDMethod(std::make_pair(1.06,0.), "Data", "!os && mt_1<20.", cat_vbf, qcd_sub_samples, wt, {
		{"WJetsToLNuSoup", [&]()->HTTAnalysis::Value {
			return ana.GetRateViaWMethod("WJetsToLNuSoup", cat_vbf_loose, "mt_1>60. && mt_1<120.", "mt_1<20.", 
				"Data", cat_vbf, "!os && mt_1>60. && mt_1<120.", w_sub_samples, wt, dummy);} 
		}
	});

	TH1F qcd_shape = ana.GetShapeViaQCDMethod(var, "Data", "!os && mt_1<20.", cat_vbf, qcd_sub_samples, wt, {
		{"WJetsToLNuSoup", [&]()->HTTAnalysis::Value {
			return ana.GetRateViaWMethod("WJetsToLNuSoup", cat_vbf_loose, "mt_1>60. && mt_1<120.", "mt_1<20.", 
				"Data", cat_vbf, "!os && mt_1>60. && mt_1<120.", w_sub_samples, wt, dummy);} 
		}
	});

	std::cout << "QCD:  " << qcd_norm.first << "  " << qcd_norm.second << std::endl;


	qcd_shape.Draw();
	canv.SaveAs("demo.pdf");

  return 0;
}


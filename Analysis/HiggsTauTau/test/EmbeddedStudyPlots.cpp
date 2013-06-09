#include <iostream>
#include <vector>
#include <map>
#include "TCanvas.h"
#include "TH1F.h"
#include "boost/bind.hpp"

#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"


using namespace std;
using namespace ic;

int main(int argc, char* argv[]){
	TH1::AddDirectory(false);

	HTTAnalysis ana(channel::mt, "2012", 2);
	ana.ReadTrees("output/Moriond_2012_NTuples");
	ana.ParseParamFile("scripts/Moriond_params_2012.dat");

	HTTAnalysis::HistoMap hmap;
	ana.FillHistoMap(hmap, 1, "m_vis(25,0,250)", "os && mt_1<20.", "pt_2>100. && pt_tt>100", "wt", "");
	TFile datacard("datacard.root","RECREATE");
	datacard.cd();
	gDirectory->mkdir("muTau_0jet_high");
	gDirectory->cd("muTau_0jet_high");

	for (auto iter : hmap) {
		iter.second.first.SetTitle(iter.first.c_str());
		iter.second.first.SetName(iter.first.c_str());
		iter.second.first.Write();
	}
	datacard.Close();


	// TCanvas canv("demo","demo");

	// std::string os_sel 	= "os && mt_1 < 20.";
	// std::string cat_vbf = "";
	// std::string cat_vbf_loose = "";
	// std::string var = "m_vis(15,0,150)";
	// /*
	// 	alias concept:
	// 	m_vis[0,10,20,30,40,50,60,70,80,90,100,150,200,250,300,350]
	// 	m_vis[$smfinebins]
	// 	categories: "@et_boost_high@" --> "n_jets>=1 && met>30. && !$et_vbf$"
	// 	or "njets>=1 && nbjets==0 && !("+vbf+")";

	// */
	// std::string wt = "wt";

	// //// Data
	// auto data_norm = ana.GetRate("Data", os_sel, cat_vbf, wt);
	// TH1F data_hist = ana.GetShape(var, "Data", os_sel, cat_vbf, wt);
	// SetNorm(&data_hist, data_norm.first);
	// HTTAnalysis::PrintValue("Data", data_norm);

	// // BlindHistogram(&data, 60., 120.);

	// //// Top
	// auto top_norm = ana.GetLumiScaledRate("TTJets", os_sel, cat_vbf, wt);
	// TH1F top_hist = ana.GetLumiScaledShape(var, "TT", os_sel, cat_vbf_loose, wt);
	// SetNorm(&top_hist, top_norm.first);
	// HTTAnalysis::PrintValue("Top", top_norm);


	// auto zll_norm = ana.GetLumiScaledRate("DYJetsToLLSoup", os_sel, cat_vbf, wt);
	// TH1F zll_hist = ana.GetLumiScaledShape(var, "DYJetsToLLSoup", os_sel, cat_vbf_loose, wt);
	// SetNorm(&zll_hist, zll_norm.first);
	// HTTAnalysis::PrintValue("ZLL", zll_norm);

	// //// Diboson
	// std::vector<std::string> vv_samples = {
	// 	"WWJetsTo2L2Nu", "WZJetsTo2L2Q", "WZJetsTo3LNu",
	// 	"ZZJetsTo2L2Nu", "ZZJetsTo2L2Q", "ZZJetsTo4L",
	// 	"T-tW", "Tbar-tW"
	// };
	// // just have this vector defined somewhere or try an alias system:
	// // vector<string> x = {"@diboson","blah"} -->
	// auto vv_norm = ana.GetLumiScaledRate(vv_samples, os_sel, cat_vbf, wt);
	// TH1F vv_hist = ana.GetLumiScaledShape(var, vv_samples, os_sel, cat_vbf_loose, wt);
	// SetNorm(&vv_hist, vv_norm.first);
	// HTTAnalysis::PrintValue("VV", vv_norm);

	// auto ztt_norm = ana.GetRateViaRefEfficiency("Embedded", "DYJetsToTauTauSoup", "os", "", os_sel, cat_vbf, wt);
	// TH1F ztt_shape = ana.GetShape(var, "Embedded", os_sel, cat_vbf, wt);
	// SetNorm(&ztt_shape, ztt_norm.first);
	// HTTAnalysis::PrintValue("ZTT", ztt_norm);

	// std::vector<std::string> w_sub_samples = {
	// 	"WWJetsTo2L2Nu", "WZJetsTo2L2Q", "WZJetsTo3LNu",
	// 	"ZZJetsTo2L2Nu", "ZZJetsTo2L2Q", "ZZJetsTo4L",
	// 	"T-tW", "Tbar-tW", "TT", "DYJetsToTauTauSoup", "DYJetsToLLSoup"
	// };
	// auto w_norm = ana.GetRateViaWMethod("WJetsToLNuSoup", cat_vbf_loose, "mt_1>60. && mt_1<120.", "mt_1<20.", 
	// 		"Data", cat_vbf, "os && mt_1>60. && mt_1<120.", w_sub_samples, wt, { 
	// 	{"Embedded", boost::bind(&HTTAnalysis::GetRateViaRefEfficiency, &ana, "Embedded", "DYJetsToTauTauSoup", "os", "", "os && mt_1>60. && mt_1<120.", cat_vbf, wt)}
	// });
	// HTTAnalysis::PrintValue("W", w_norm);

	// std::vector<std::string> qcd_sub_samples = {
	// 	"WWJetsTo2L2Nu", "WZJetsTo2L2Q", "WZJetsTo3LNu",
	// 	"ZZJetsTo2L2Nu", "ZZJetsTo2L2Q", "ZZJetsTo4L",
	// 	"T-tW", "Tbar-tW", "TT", "DYJetsToTauTauSoup", "DYJetsToLLSoup",
	// 	"WJetsToLNuSoup"
	// };
	// std::map<std::string, std::function<std::pair<double,double>()>> dummy;



	// auto qcd_norm = ana.GetRateViaQCDMethod(std::make_pair(1.06,0.), "Data", "!os && mt_1<20.", cat_vbf, qcd_sub_samples, wt, {
	// 	{"WJetsToLNuSoup", [&]()->HTTAnalysis::Value {
	// 		return ana.GetRateViaWMethod("WJetsToLNuSoup", cat_vbf_loose, "mt_1>60. && mt_1<120.", "mt_1<20.", 
	// 			"Data", cat_vbf, "!os && mt_1>60. && mt_1<120.", w_sub_samples, wt, dummy);} 
	// 	}
	// });

	// TH1F qcd_shape = ana.GetShapeViaQCDMethod(var, "Data", "!os && mt_1<20.", cat_vbf, qcd_sub_samples, wt, {
	// 	{"WJetsToLNuSoup", [&]()->HTTAnalysis::Value {
	// 		return ana.GetRateViaWMethod("WJetsToLNuSoup", cat_vbf_loose, "mt_1>60. && mt_1<120.", "mt_1<20.", 
	// 			"Data", cat_vbf, "!os && mt_1>60. && mt_1<120.", w_sub_samples, wt, dummy);} 
	// 	}
	// });

	// // GetRateViaRefEfficiency("Special_3_Data","QCD", "N/A", "N/A", "os && mt_1<20.", cat_vbf, wt,
	// // QCD : GetRateViaQCDMethod ( ... , WJetsToLNuSoup: GetRateViaWMethod ( ... , )  )

	// HTTAnalysis::PrintValue("QCD", qcd_norm);

	// auto total_norm = top_norm;
	// total_norm = HTTAnalysis::ValueAdd(total_norm, zll_norm);
	// total_norm = HTTAnalysis::ValueAdd(total_norm, vv_norm);
	// total_norm = HTTAnalysis::ValueAdd(total_norm, ztt_norm);
	// total_norm = HTTAnalysis::ValueAdd(total_norm, w_norm);
	// total_norm = HTTAnalysis::ValueAdd(total_norm, qcd_norm);
	// HTTAnalysis::PrintValue("Total", total_norm);

	// auto data_mc_ratio = HTTAnalysis::ValueDivide(data_norm, total_norm);
	// HTTAnalysis::PrintValue("Data/MC", data_mc_ratio);


	// qcd_shape.Draw();
	// canv.SaveAs("demo.pdf");

  return 0;
}


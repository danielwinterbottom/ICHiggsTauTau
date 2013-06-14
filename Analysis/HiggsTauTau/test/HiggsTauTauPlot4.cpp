#include <iostream>
#include <vector>
#include "TH1F.h"
#include "boost/program_options.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTPlotTools.h"

#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"

using namespace std;
using namespace ic;
namespace po = boost::program_options;

int main(int argc, char* argv[]){

	string cfg;															      // The configuration file
	string paramfile;															// The paramters files									
	string folder;																// Folder containing input files
	string channel_str;             							// Channel, e.g. et
	unsigned method;															// Use background methods for chosen category
	string var;																		// Use background methods for chosen category
	string cat;																		// Use background methods for chosen category
	bool is_2012;																	// false = 7 TeV, true = 8 TeV
	unsigned verbosity;														// Verbose output, useful for diagnostic purposes
	bool is_paper;                                // Tweaking some things for the paper
	string datacard;             									// Channel, e.g. et
	string set_alias;															// A string like alias1:value1,alias2:value2 etc
	string sm_masses_str;															// A string like alias1:value1,alias2:value2 etc
	string mssm_masses_str;														// A string like alias1:value1,alias2:value2 etc
	string syst_tau_scale;
	string add_sm_background;
	// Program options
  po::options_description preconfig("Pre-Configuration");
  preconfig.add_options()("cfg", po::value<std::string>(&cfg)->required());
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
  	("paramfile",           po::value<string>(&paramfile)->required())
  	("folder",              po::value<string>(&folder)->required())
  	("channel",             po::value<string>(&channel_str)->required())
  	("method",           		po::value<unsigned>(&method)->required())
	  ("var",              		po::value<string>(&var)->required())
	  ("cat",             		po::value<string>(&cat)->default_value(""))
	  ("is_2012",             po::value<bool>(&is_2012)->required())
	  ("verbosity",           po::value<unsigned>(&verbosity)->default_value(0))
	  ("is_paper",            po::value<bool>(&is_paper)->default_value(false))
	  ("datacard",            po::value<string>(&datacard)->default_value(""))
	  ("set_alias",           po::value<string>(&set_alias)->default_value(""))
	  ("sm_masses",           po::value<string>(&sm_masses_str)->default_value(""))
	  ("mssm_masses",         po::value<string>(&mssm_masses_str)->default_value(""))
	  ("syst_tau_scale",      po::value<string>(&syst_tau_scale)->default_value(""))
	  ("add_sm_background",   po::value<string>(&add_sm_background)->default_value(""));

	HTTPlot plot;
	config.add(plot.GenerateOptions(""));
	po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
	po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
	po::notify(vm);


	std::string reduced_var = var;
	std::size_t begin_var = reduced_var.find("[");
	if (begin_var != reduced_var.npos) reduced_var.erase(begin_var, reduced_var.npos);
	begin_var = reduced_var.find("(");
	if (begin_var != reduced_var.npos) reduced_var.erase(begin_var, reduced_var.npos);
	if (plot.plot_name() == "") {
		plot.set_plot_name(reduced_var+"_"+datacard+"_"+channel_str+(is_2012 ? "_2012":"_2011"));
	}
	TH1::AddDirectory(false);

	HTTAnalysis ana(String2Channel(channel_str), is_2012 ? "2012" : "2011", verbosity);
	
	// Check if any aliases need to be re-defined
	std::vector<std::pair<std::string, std::string>> alias_vec;
	if (set_alias != "") {
		std::vector<std::string> alias_list;
		boost::split(alias_list, set_alias, boost::is_any_of(","));
		for (unsigned i = 0; i < alias_list.size(); ++i) {
			std::vector<std::string> alias_part;
			boost::split(alias_part, alias_list[i], boost::is_any_of(":"));
			if (alias_part.size() == 2) alias_vec.push_back(std::make_pair(alias_part[0],alias_part[1]));
		}
	}
	for (auto const& a : alias_vec) ana.SetAlias(a.first, a.second);

	std::vector<std::string> sm_masses;
	if (sm_masses_str != "") boost::split(sm_masses, sm_masses_str, boost::is_any_of(","));
	std::vector<std::string> mssm_masses;
	if (mssm_masses_str != "") boost::split(mssm_masses, mssm_masses_str, boost::is_any_of(","));
	ana.AddSMSignalSamples(sm_masses);
	ana.AddMSSMSignalSamples(mssm_masses);

	ana.ReadTrees(folder);
	ana.ParseParamFile(paramfile);

	HTTAnalysis::HistValueMap hmap;

	std::string sel = ana.ResolveAlias("os_sel");
	cat = ana.ResolveAlias(cat);

	ana.FillHistoMap(hmap, method, var, sel, cat, "wt", "");
	ana.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "", 1.0);
	if (add_sm_background != "") ana.FillSMSignal(hmap, {add_sm_background}, var, sel, cat, "wt", "_SM", "");
	ana.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "", 1.0);

	if (syst_tau_scale != "") {
		HTTAnalysis ana_tscale_down(String2Channel(channel_str), is_2012 ? "2012" : "2011", verbosity);
		for (auto const& a : alias_vec) ana_tscale_down.SetAlias(a.first, a.second);
		ana_tscale_down.AddSMSignalSamples(sm_masses);
		ana_tscale_down.AddMSSMSignalSamples(mssm_masses);
		ana_tscale_down.ReadTrees(folder+"/TSCALE_DOWN", folder);
		ana_tscale_down.ParseParamFile(paramfile);

		ana_tscale_down.FillHistoMap(hmap, method, var, sel, cat, "wt", "_"+syst_tau_scale+"Down");
		ana_tscale_down.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "_"+syst_tau_scale+"Down", 1.0);
		if (add_sm_background != "") ana_tscale_down.FillSMSignal(hmap, {add_sm_background}, var, sel, cat, "wt", "_SM", "_"+syst_tau_scale+"Down");
		ana_tscale_down.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "_"+syst_tau_scale+"Down", 1.0);

		HTTAnalysis ana_tscale_up(String2Channel(channel_str), is_2012 ? "2012" : "2011", verbosity);
		for (auto const& a : alias_vec) ana_tscale_up.SetAlias(a.first, a.second);
		ana_tscale_up.AddSMSignalSamples(sm_masses);
		ana_tscale_up.AddMSSMSignalSamples(mssm_masses);
		ana_tscale_up.ReadTrees(folder+"/TSCALE_UP", folder);
		ana_tscale_up.ParseParamFile(paramfile);
		ana_tscale_up.FillHistoMap(hmap, method, var, sel, cat, "wt", "_"+syst_tau_scale+"Up");
		ana_tscale_up.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "_"+syst_tau_scale+"Up", 1.0);
		if (add_sm_background != "") ana_tscale_up.FillSMSignal(hmap, {add_sm_background}, var, sel, cat, "wt", "_SM", "_"+syst_tau_scale+"Up");
		ana_tscale_up.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "_"+syst_tau_scale+"Up", 1.0);
	}


	// **** Can run on hmap
	// if (method == 6 || method == 7) {
	//   double top_norm_diff = (method == 7) ? (0.053*ztt_norm) : (0.015*ztt_norm);
	//   std::cout << "TOP ZTT Subtraction fraction: " << (top_norm_diff/top_norm) << std::endl;
	//   top_norm = top_norm - top_norm_diff;
	//   top_shape.hist_ptr()->Scale(top_norm / Integral(top_shape.hist_ptr()));
	// }

	// **** Can run on hmap
	// if (method == 12) {
	//   double top_norm_diff = (0.015*ztt_norm);
	//   std::cout << "TOP ZTT Subtraction fraction: " << (top_norm_diff/top_norm) << std::endl;
	//   top_norm = top_norm - top_norm_diff;
	//   top_shape.hist_ptr()->Scale(top_norm / Integral(top_shape.hist_ptr()));
	// }

	// **** Can run on hmap
	// for (int j = 0; j < qcd_hist->GetNbinsX(); ++j) {
	//   if (qcd_hist->GetBinContent(j+1) < 0) {
	//     // qcd_hist->SetBinContent(j+1,0.0);
	//     // qcd_hist->SetBinError(j+1, 0.0);
	//     qcd_hist->SetBinContent(j+1,0.0000001);
	//     qcd_hist->SetBinError(j+1, 1.0);
	//   }
	// }
	// if (qcd_norm <= 0.0) qcd_norm = 0.0000001;

	// **** would need a dedicated function call in HTTAnalysis
	// qcd_norm = qcd_category;
	// // Inverted electron isolation
	// qcd_hist = (TH1F*)(plots[Token("Special_24_Data",cat,ss_sel)].hist_ptr()->Clone());
	// TH1F *fr_hist = (TH1F*)(plots[Token("Special_20_Data",cat,os_sel)].hist_ptr()->Clone());
	// fr_hist->Add((TH1F*)(plots[Token("Special_21_Data",cat,os_sel)].hist_ptr()->Clone()) ,1.0);
	// fr_hist->Add((TH1F*)(plots[Token("Special_22_Data",cat,os_sel)].hist_ptr()->Clone()) ,-1.0);
	// em_0jet_high_Up = (TH1F*)qcd_hist->Clone(TString("Fakes_CMS_htt_FakeShape_em_0jet_high_")+(is_2012 ? "8":"7")+"TeVUp");
	// em_0jet_high_Down = (TH1F*)fr_hist->Clone(TString("Fakes_CMS_htt_FakeShape_em_0jet_high_")+(is_2012 ? "8":"7")+"TeVDown");
	// qcd_hist->Scale(0.2 / Integral (qcd_hist)); // 0.2
	// fr_hist->Scale(0.8 / Integral (fr_hist)); // 0.8
	// qcd_hist->Add(fr_hist);//Fakes_CMS_htt_FakeShape_em_0jet_high_8TeVUp
	// qcd_hist->Scale(qcd_norm / Integral (qcd_hist));
	// em_0jet_high_Up->Scale(qcd_norm / Integral (em_0jet_high_Up));
	// em_0jet_high_Down->Scale(qcd_norm / Integral (em_0jet_high_Down));

	// **** Can do this with the hmap
	// if (channel == channel::mt && method == 2) {
	//   h1 = (TH1F*)(qcd_hist->Clone());
	//   h2 = (TH1F*)h1->Clone(TString("QCD_CMS_htt_QCDShape_mutau_boost_low_")+(is_2012 ? "8":"7")+"TeVUp");
	//   h3 = (TH1F*)h1->Clone(TString("QCD_CMS_htt_QCDShape_mutau_boost_low_")+(is_2012 ? "8":"7")+"TeVDown");
	//   float x;
	//   float y;
	//   float c;
	//   float cUp;
	//   float cDown;
	//   for(int i=1;i<h1->GetNbinsX();++i){
	//     x = h1->GetXaxis()->GetBinCenter(i);
	//     if(x < (is_2012 ? 70 : 50) ){
	//       y = h1->GetBinContent(i);
	//       c = 1.15;  // or whatever other correction we want to apply
	//       cUp = 1.3;
	//       cDown = 1.0;
	//       h1->SetBinContent(i,y*c);
	//       h2->SetBinContent(i,y*cUp);
	//       h3->SetBinContent(i,y*cDown);
	//     }  
	//   }
	//   qcd_hist = h1;
	//   h2->Scale( qcd_norm / Integral(h2) );
	//   h3->Scale( qcd_norm / Integral(h3) );
	// }s

	// *** Have to create another HTTAnalysis and run standard procedure
	// if (tau_scale_mode == 1 && channel != channel::em) append = "_CMS_scale_t_"+dc_mode_label_alt+(is_2012 ? "_8":"_7")+"TeVDown";
	// if (tau_scale_mode == 2 && channel != channel::em) append = "_CMS_scale_t_"+dc_mode_label_alt+(is_2012 ? "_8":"_7")+"TeVUp";
	// if (tau_scale_mode == 1 && channel == channel::em) append = string("_CMS_scale_e")+(is_2012 ? "_8":"_7")+"TeVDown";
	// if (tau_scale_mode == 2 && channel == channel::em) append = string("_CMS_scale_e")+(is_2012 ? "_8":"_7")+"TeVUp";

	// qcd_hist->Scale( qcd_norm / Integral(qcd_hist) );

	// *** Can do this with hmap
	//  VH->Scale(1./ (parser.GetParam<double>("XS_WH_ZH_TTH_HToTauTau_M-"+signal_masses[i])));
	// if (Integral(VH) < 0.0000001) {
	//   std::cout << "Histogram " << "WH_ZH_TTH_HToTauTau_M-"+signal_masses[i] << ":" << os_sel << " appears to be empty, adding small value in central bin!" << std::endl;
	//   VH->SetBinContent(1, 0.0000001);
	// }



	if (datacard != "") {
		std::string dc_mode_label;
		if (channel_str == "et") 			dc_mode_label = "eleTau";
		if (channel_str == "mt") 			dc_mode_label = "muTau";
		if (channel_str == "mtmet") 	dc_mode_label = "muTauMet";
		if (channel_str == "em") 			dc_mode_label = "emu";
		std::string tfile_name = "datacard_"+reduced_var+"_"+datacard+"_"+channel_str+(is_2012 ? "_2012":"_2011")+".root";
		TFile dc_file(tfile_name.c_str(),"RECREATE");
		dc_file.cd();
		gDirectory->mkdir((dc_mode_label+"_"+datacard).c_str());
		gDirectory->cd((dc_mode_label+"_"+datacard).c_str());

		for (auto iter : hmap) {
			iter.second.first.SetTitle(iter.first.c_str());
			iter.second.first.SetName(iter.first.c_str());
			iter.second.first.Write();
		}
		std::cout << "[HiggsTauTauPlot4] writing datacard input " << tfile_name << std::endl;
		dc_file.Close();
	}
	ana.FillSMSignal(hmap, sm_masses, var, sel, cat, "wt", "", "");
	ana.FillMSSMSignal(hmap, mssm_masses, var, sel, cat, "wt", "", "");
	plot.GeneratePlot(hmap);



  return 0;
}


#include <iostream>
#include <vector>
#include "TH1F.h"
#include "boost/program_options.hpp"
#include "boost/regex.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTRun2AnalysisTools.h"
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
  unsigned use_status_flags;
	unsigned verbosity;														// Verbose output, useful for diagnostic purposes
	bool do_ss;                            		    // Tweaking some things for the paper
	string datacard;             									// Channel, e.g. et
	vector<string> set_alias;											// A string like alias1:value1,alias2:value2 etc
	string shift_backgrounds;
	bool auto_titles=true;
  bool calc_vertex_weights;
  double qcd_os_ss_factor;
  bool interpolate;

	// Program options
  po::options_description preconfig("Pre-Configuration");
  preconfig.add_options()("cfg", po::value<std::string>(&cfg)->required());
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
  po::notify(vm);
  po::options_description config("Configuration");
  config.add_options()
  	("paramfile",               po::value<string>(&paramfile)->required())
  	("folder",                  po::value<string>(&folder)->required())
  	("channel",                 po::value<string>(&channel_str)->required())
  	("method",           		    po::value<unsigned>(&method)->required())
	  ("var",              		    po::value<string>(&var)->required())
	  ("cat",             		    po::value<string>(&cat)->default_value(""))
    ("use_status_flags",        po::value<unsigned>(&use_status_flags)->default_value(1))
	  ("verbosity",               po::value<unsigned>(&verbosity)->default_value(0))
	  ("do_ss", 	                po::value<bool>(&do_ss)->default_value(false))
	  ("interpolate", 	          po::value<bool>(&interpolate)->default_value(false))
	  ("datacard",                po::value<string>(&datacard)->default_value(""))
	  ("set_alias",               po::value<vector<string>>(&set_alias)->composing())
    ("calc_vertex_weights",     po::value<bool>(&calc_vertex_weights)->default_value(false))
	  ("qcd_os_ss_factor",  	    po::value<double>(&qcd_os_ss_factor)->default_value(1.06));


	HTTPlot plot;
	config.add(plot.GenerateOptions("")); // The string here is a prefix for the options parameters
	po::store(po::command_line_parser(argc, argv).options(config).allow_unregistered().run(), vm);
	po::store(po::parse_config_file<char>(cfg.c_str(), config), vm);
	po::notify(vm);

	std::cout << "-----------------------------------------------------------------------------------" << std::endl;
	std::cout << "ZllPlot" << std::endl;
	std::cout << "-----------------------------------------------------------------------------------" << std::endl;
	std::cout << boost::format(param_fmt()) % "paramfile" 	% paramfile;
	std::cout << boost::format(param_fmt()) % "cfg" 				% cfg;
	std::cout << boost::format(param_fmt()) % "folder" 			% folder;
	std::cout << boost::format(param_fmt()) % "channel"     % channel_str;
	std::cout << boost::format(param_fmt()) % "variable" 		% var;
	std::cout << boost::format(param_fmt()) % "method" 		  % method;
	std::cout << boost::format(param_fmt()) % "category"    % cat;
	std::cout << boost::format(param_fmt()) % "datacard"    % datacard;
	std::cout << "-----------------------------------------------------------------------------------" << std::endl;

	// ************************************************************************
	// Extract variable name and set output filename
	// ************************************************************************
	std::string reduced_var = var;
	std::size_t begin_var = reduced_var.find("[");
	if (begin_var != reduced_var.npos) reduced_var.erase(begin_var, reduced_var.npos);
	begin_var = reduced_var.find("(");
	if (begin_var != reduced_var.npos) reduced_var.erase(begin_var, reduced_var.npos);
	if (plot.plot_name() == "") {
		plot.set_plot_name(reduced_var+"_"+datacard+"_"+channel_str+("_2015"));
	}
	TH1::AddDirectory(false);

	// ************************************************************************
	// Load alias overrides
	// ************************************************************************
	std::vector<std::pair<std::string, std::string>> alias_vec;
	for (unsigned i = 0; i < set_alias.size(); ++i) {
		std::vector<std::string> alias_part;
		boost::split(alias_part, set_alias[i], boost::is_any_of(":"));
		if (alias_part.size() == 2) alias_vec.push_back(std::make_pair(alias_part[0],alias_part[1]));
		std::cout << "[ZllPlot] Setting alias: " << alias_part[0] << " --> \"" << alias_part[1] << "\"" << std::endl;
	}

	// ************************************************************************
	// Setup HTTRun2Analysis 
	// ************************************************************************
	HTTRun2Analysis ana(String2Channel(channel_str), "2015", use_status_flags, verbosity);
    ana.SetQCDRatio(qcd_os_ss_factor);
    if (do_ss) ana.SetQCDRatio(1.0);
	for (auto const& a : alias_vec) ana.SetAlias(a.first, a.second);
    //if (is_2012 && (check_ztt_top_frac || sub_ztt_top_shape)) ana.AddSample("Embedded-TTJets_FullLeptMGDecays");
	ana.ReadTrees(folder);
	ana.ParseParamFile(paramfile);

	HTTRun2Analysis::HistValueMap hmap;

//	std::string sel = " "+ana.ResolveAlias("sel");
	std::string sel;
    if (channel_str != "wmnu") sel = "os && "+ana.ResolveAlias("sel");
    else sel = ana.ResolveAlias("sel");
	if (do_ss) {
		sel = "!os && "+ana.ResolveAlias("sel");
		ana.SetAlias("w_os", "!os");
		ana.SetAlias("w_sdb_os", "!os");
		ana.SetAlias("w_vbf_os", "!os");
		ana.SetAlias("w_shape_os", "!os");
	}
	cat = ana.ResolveAlias(cat);

  std::string morph_var = "m_sv(70,0,350)";
  std::string sig_var = var;
	ana.FillHistoMap(hmap, method, var, sel, cat, "wt", "");



	// ************************************************************************
	// Write datacard
	// ************************************************************************
	if (datacard != "") {
		std::string dc_mode_label;
		if (channel_str == "zee") 			dc_mode_label = "Zee";
		if (channel_str == "zmm") 			dc_mode_label = "Zmm";
		std::string tfile_name = "datacard_"+reduced_var+"_"+datacard+"_"+channel_str+("_2015")+".root";
		TFile dc_file(tfile_name.c_str(),"RECREATE");
		dc_file.cd();
		gDirectory->mkdir((dc_mode_label+"_"+datacard).c_str());
		gDirectory->cd((dc_mode_label+"_"+datacard).c_str());

		for (auto iter : hmap) {
			iter.second.first.SetTitle(iter.first.c_str());
			iter.second.first.SetName(iter.first.c_str());
			iter.second.first.Write();
		}
		std::cout << "[ZllPlot] Writing datacard input " << tfile_name << std::endl;
		dc_file.Close();
	}

	
    // ************************************************************************
	// Shift backgrounds
	// ************************************************************************
	vector<string> shift_strs;
	boost::split(shift_strs, shift_backgrounds, boost::is_any_of(","));
	if (shift_strs.size() > 0) {
		std::cout << "[ZllPlot] Shifting background yields... " << std::endl;
		for (auto shift : shift_strs) {
			std::vector<std::string> shift_part;
			boost::split(shift_part, shift, boost::is_any_of(":"));
			if (shift_part.size() != 2) continue;
			string regex_str = shift_part[0]/*+".*"*/;
			boost::regex rgx = boost::regex(regex_str);
			double shift_val = boost::lexical_cast<double>(shift_part[1]);
			for (auto & entry : hmap) {
				if (boost::regex_match(entry.first, rgx)) {
					std::cout << "Scaling " << entry.first << " by " << shift_val << std::endl;
					entry.second.first.Scale(shift_val);
					entry.second.second.first *= shift_val; // the yield
					entry.second.second.second *= shift_val; // the yield error
				}
			}
		}
	}
	
	// ************************************************************************
	// Deduce titles
	// ************************************************************************
	if (auto_titles) {
		double pb_lumi = ana.GetLumi();
		string com = "13";
        plot.set_lumi_label((boost::format("%.1f pb^{-1} at %s TeV") % pb_lumi % com).str());
        plot.set_cms_label("CMS");
        plot.set_cms_extra("Preliminary");
        std::string channel_fmt = ""; 
		if (channel_str == "zee") 		channel_fmt = "Z#rightarrowee";
		if (channel_str == "zmm") 		channel_fmt = "Z#rightarrow#mu#mu";
		if (channel_str == "wmnu") 		channel_fmt = "W#rightarrow#mu#nu";
    ic::TextElement text(channel_fmt,0.05,0.16,0.96);

    //ic::TextElement text2("#splitline{Same-sign}{region}",0.05,0.65,0.5);
    plot.AddTextElement(text);
    //plot.AddTextElement(text2);
	}

  if (calc_vertex_weights){

    TH1F * data = &hmap["data_obs"].first;
    TH1F *data_test = (TH1F*)data->Clone("data_test");
    TH1F * nvtx_weights = new TH1F("nvtx_weights","nvtx_weights",101,-0.5,100.5);
    data_test->Scale(1./data_test->Integral(1,data_test->GetNbinsX()));
    std::cout<<data_test->Integral()<<std::endl;
    vector<string>bkgs = {"ZLL","W","TT","VV"};

    double bckg_yield=0;
    for (int i = 1; i<= data_test->GetNbinsX(); ++i){
      for (auto bkg : bkgs) bckg_yield += hmap[bkg].first.GetBinContent(i);
    }
    std::cout <<bckg_yield<<std::endl;
    for (int i = 1; i <= data_test->GetNbinsX(); ++i){
      double bkg_tot = 0;
      for (auto bkg : bkgs) bkg_tot += hmap[bkg].first.GetBinContent(i);
      bkg_tot = (double)bkg_tot/bckg_yield;
    if(bkg_tot !=0){
      nvtx_weights->SetBinContent(nvtx_weights->FindBin(data_test->GetBinCenter(i)),(data_test->GetBinContent(i)/bkg_tot)); 
      std::cout<<"PVMin "<<data_test->GetBinCenter(i)<<" data/bkg "<<(data_test->GetBinContent(i))/(bkg_tot)<<std::endl;
   }

   }
   std::string weightfilename = "VertexWeightDistribution_"+channel_str+".root";
   TFile *fileout = new TFile(weightfilename.c_str(),"RECREATE");
     nvtx_weights->Write();
   fileout->Close();
  }

	plot.GeneratePlot(hmap);

  return 0;
}


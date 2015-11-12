#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTRun2AnalysisTools.h"
#include <iostream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include "boost/range/algorithm.hpp"
#include "boost/range/algorithm_ext.hpp"
#include "boost/filesystem.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SimpleParamParser.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/th1fmorph.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TEfficiency.h"
#include "TEntryList.h"
#include "TMath.h"
#include "TH1.h"
#include "TLegend.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooRealVar.h"
#include "RooAddPdf.h"
#include "RooPlot.h"

namespace ic {

  HTTRun2Analysis::HTTRun2Analysis(ic::channel ch, std::string year, int use_status_flags, int verbosity) : ch_(ch), year_(year), use_status_flags_(use_status_flags), verbosity_(verbosity)  {
    lumi_ = 1.;
    qcd_os_ss_factor_ = 1.06;
    using boost::range::push_back;
    // Define some sensible defaults
/*    sample_names_ = {
//      "WZJetsTo3LNu",
      "SingleElectron-2015B-17Jul",
      "SingleMuon-2015B-17Jul",
      "Tau-2015B-17Jul",
      "MuonEG-2015B-17Jul",
      "SingleElectron-2015C-prompt",
      "SingleMuon-2015C-prompt",
      "Tau-2015C-prompt",
      "MuonEG-2015C-prompt",
      "SingleElectron-2015D-prompt",
      "SingleMuon-2015D-prompt",
      "MuonEG-2015D-prompt",
      "Tau-2015D-prompt",
      "QCDMuEnr",
      "QCDFlat",
      "T-tW",
      "Tbar-tW",
      "T-tW-AOD",
      "Tbar-tW-AOD",
      "TTJets",
      "TT",
      "DYJetsToLL",
      "TT-AOD",
      "DYJetsToLL-AOD",
      "WWTo2L2Nu",
      "WWTo4Q",
      "ZZTo4L",
      "WWToLNuQQ",
      "WZTo1L1Nu2Q",
      "WWinclusive", 
      "WZinclusive", 
      "ZZinclusive",
      "WWinclusive-AOD", 
      "WZinclusive-AOD", 
      "ZZinclusive-AOD",
      "DYJetsToTauTau",
    };
    if (ch_ != channel::em) {
      push_back(sample_names_, std::vector<std::string>{
        "WJetsToLNu-AOD",
        "WJetsToLNu",
        "DYJetsToLL-L", 
        "DYJetsToLL-J",
        "DYJetsToTauTau-L",
        "DYJetsToTauTau-JJ",
      });
    }
    if (ch_ == channel::et) {
      push_back(sample_names_, std::vector<std::string>{
       "SingleElectron-2015B-17Jul"
      });
    }
    if (ch_ == channel::mt) {
      push_back(sample_names_, std::vector<std::string>{
        "SingleMuon-2015B-17Jul"
      });
    }
    if (ch_ == channel::em) {
      push_back(sample_names_, std::vector<std::string>{
        "MuonEG-2015B-17Jul"
      });
    }
    if (ch_ == channel::tt) {
      push_back(sample_names_, std::vector<std::string>{
        "Tau-2015B-17Jul"
      });
    }*/   

    //Sample splitting
    if (ch_ == channel::et){
      alias_map_["ztt_sel"] = "(gen_match_2==5)";
      alias_map_["zl_sel"] = "(gen_match_2<5)";
      alias_map_["zj_sel"] = "(gen_match_2==6)";
    }
    if (ch_ == channel::mt){
      alias_map_["ztt_sel"] = "(gen_match_2==5)";
      alias_map_["zl_sel"] = "(gen_match_2<5)";
      alias_map_["zj_sel"] = "(gen_match_2==6)";
    }
    if (ch_ == channel::tt){
      alias_map_["ztt_sel"] = "(gen_match_1==5&&gen_match_2==5)";
      alias_map_["zl_sel"] = "(gen_match_2<5)";
      alias_map_["zj_sel"] = "(gen_match_2==6||gen_match_1==6)";
    }
    if (ch_ == channel::em ){
      alias_map_["ztt_sel"] = "(gen_match_1>2 && gen_match_2>3)";
      alias_map_["zll_sel"] = "(gen_match_1<3 || gen_match_2<4)";    
    }
    if (ch_ == channel::zee||ch_==channel::zmm||ch_ ==channel::wmnu){
      alias_map_["ztt_sel"] = "";
      alias_map_["zll_sel"] = "1.";
    }

    if (ch_ == channel::et || ch_ == channel::mt) {
      // SM Categories
      alias_map_["inclusive"]         = "(iso_1<0.1&&iso_2<1.5 && antiele_2 && antimu_2 && !leptonveto)";
      // Categories for iso studies
      alias_map_["incnoiso"]         = "(iso_2<1.5 && antiele_2 && antimu_2 && !leptonveto)";
      alias_map_["incnoisonolv"]     = "(iso_2<1.5 && antiele_2 && antimu_2)";
      alias_map_["db03iso0p1"]            = "(iso_1_db03<0.1&&"+alias_map_["incnoiso"]+")";
      alias_map_["db03iso0p11"]            = "(iso_1_db03<0.11&&"+alias_map_["incnoiso"]+")";
      alias_map_["db03iso0p12"]            = "(iso_1_db03<0.12&&"+alias_map_["incnoiso"]+")";
      alias_map_["db03iso0p13"]            = "(iso_1_db03<0.13&&"+alias_map_["incnoiso"]+")";
      alias_map_["db03iso0p09"]           = "(iso_1_db03<0.09&&"+alias_map_["incnoiso"]+")";
      alias_map_["db03iso0p08"]           = "(iso_1_db03<0.08&&"+alias_map_["incnoiso"]+")";
      alias_map_["db03iso0p07"]           = "(iso_1_db03<0.07&&"+alias_map_["incnoiso"]+")";
      alias_map_["ea03iso0p1"]            = "(iso_1_ea03<0.1&&"+alias_map_["incnoiso"]+")";
      alias_map_["ea03iso0p11"]            = "(iso_1_ea03<0.11&&"+alias_map_["incnoiso"]+")";
      alias_map_["ea03iso0p12"]            = "(iso_1_ea03<0.12&&"+alias_map_["incnoiso"]+")";
      alias_map_["ea03iso0p13"]            = "(iso_1_ea03<0.13&&"+alias_map_["incnoiso"]+")";
      alias_map_["ea03iso0p14"]            = "(iso_1_ea03<0.14&&"+alias_map_["incnoiso"]+")";
      alias_map_["ea03iso0p15"]            = "(iso_1_ea03<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["ea03iso0p16"]            = "(iso_1_ea03<0.16&&"+alias_map_["incnoiso"]+")";
      alias_map_["ea03iso0p17"]            = "(iso_1_ea03<0.17&&"+alias_map_["incnoiso"]+")";
      alias_map_["ea03iso0p18"]            = "(iso_1_ea03<0.18&&"+alias_map_["incnoiso"]+")";
      alias_map_["ea03iso0p09"]           = "(iso_1_ea03<0.09&&"+alias_map_["incnoiso"]+")";
      alias_map_["ea03iso0p08"]           = "(iso_1_ea03<0.08&&"+alias_map_["incnoiso"]+")";
      alias_map_["ea03iso0p07"]           = "(iso_1_ea03<0.07&&"+alias_map_["incnoiso"]+")";
      alias_map_["db03allchiso0p1"]            = "(iso_1_db03allch<0.1&&"+alias_map_["incnoiso"]+")";
      alias_map_["db03allchiso0p11"]            = "(iso_1_db03allch<0.11&&"+alias_map_["incnoiso"]+")";
      alias_map_["db03allchiso0p12"]            = "(iso_1_db03allch<0.12&&"+alias_map_["incnoiso"]+")";
      alias_map_["db03allchiso0p13"]            = "(iso_1_db03allch<0.13&&"+alias_map_["incnoiso"]+")";
      alias_map_["db03allchiso0p09"]           = "(iso_1_db03allch<0.09&&"+alias_map_["incnoiso"]+")";
      alias_map_["db03allchiso0p08"]           = "(iso_1_db03allch<0.08&&"+alias_map_["incnoiso"]+")";
      alias_map_["db03allchiso0p07"]           = "(iso_1_db03allch<0.07&&"+alias_map_["incnoiso"]+")";
      alias_map_["db04allchiso0p1"]            = "(iso_1_db04allch<0.1&&"+alias_map_["incnoiso"]+")";
      alias_map_["db04allchiso0p11"]            = "(iso_1_db04allch<0.11&&"+alias_map_["incnoiso"]+")";
      alias_map_["db04allchiso0p12"]            = "(iso_1_db04allch<0.12&&"+alias_map_["incnoiso"]+")";
      alias_map_["db04allchiso0p13"]            = "(iso_1_db04allch<0.13&&"+alias_map_["incnoiso"]+")";
      alias_map_["db04allchiso0p14"]            = "(iso_1_db04allch<0.14&&"+alias_map_["incnoiso"]+")";
      alias_map_["db04allchiso0p15"]            = "(iso_1_db04allch<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["db04allchiso0p16"]            = "(iso_1_db04allch<0.16&&"+alias_map_["incnoiso"]+")";
      alias_map_["db04allchiso0p17"]            = "(iso_1_db04allch<0.17&&"+alias_map_["incnoiso"]+")";
      alias_map_["db04allchiso0p18"]            = "(iso_1_db04allch<0.18&&"+alias_map_["incnoiso"]+")";
      alias_map_["db04allchiso0p09"]           = "(iso_1_db04allch<0.09&&"+alias_map_["incnoiso"]+")";
      alias_map_["db04allchiso0p08"]           = "(iso_1_db04allch<0.08&&"+alias_map_["incnoiso"]+")";
      alias_map_["db04allchiso0p07"]           = "(iso_1_db04allch<0.07&&"+alias_map_["incnoiso"]+")";
     // alias_map_["inclusive"]         = "(iso_1<0.1 && iso_2<1.5 && antiele_2 && antimu_2 && !leptonveto)";
      //alias_map_["qcd_loose_shape"]         = "(iso_1>0.2 && iso_1<0.5  && iso_2<10 && antiele_2 && antimu_2)";
      alias_map_["qcd_loose_shape"]         = "(iso_1>0.2 && iso_1<0.5  && iso_2<10 && antiele_2 && antimu_2 && !leptonveto)";
      //Categories can be added using inclusive alias as follows:
      alias_map_["vbf"] = "(n_jets>=2 && n_jetsingap==0 && mjj>500 && jdeta>3.5)";
      alias_map_["1jet"] = "(!("+alias_map_["vbf"]+")"+"&& n_jets>=1 && n_bjets==0)";
      alias_map_["vbf"] += "&&" + alias_map_["inclusive"];
      alias_map_["1jet"] += "&&" + alias_map_["inclusive"];
    } else if (ch_ == channel::tt) {
      // SM Categories
      //alias_map_["inclusive"]         = "iso_1<1.0 && iso_2<1.0 && antiele_1 && antimu_1 && antiele_2 && antimu_2";
      alias_map_["inclusive"]         = "iso_1<1.0 && iso_2<1.0 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto";
      //alias_map_["qcd_loose_shape"]   = "iso_1>1.0 && iso_2>1.0 && antiele_1 && antimu_1 && antiele_2 && antimu_2";
      alias_map_["qcd_loose_shape"]   = "iso_1>1.0 && iso_2>1.0 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto";
    } else if (ch_ == channel::em) {
      // SM Categories
      alias_map_["inclusive"]         = "iso_1<0.15 && iso_2<0.15 && !leptonveto";
      alias_map_["incnoiso"]         = "!leptonveto";
      alias_map_["incnoisowmu"]         = "iso_2<0.15 && !leptonveto";
      alias_map_["incnoisowe"]         = "iso_1<0.15 && !leptonveto";
      alias_map_["incnoisonolvwmu"]         = "iso_2<0.15";
      alias_map_["incnoisonolvwe"]         = "iso_1<0.15";
      alias_map_["edb03iso0p15"]            = "(iso_1_db03<0.15&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03iso0p16"]            = "(iso_1_db03<0.16&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03iso0p17"]            = "(iso_1_db03<0.17&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03iso0p18"]            = "(iso_1_db03<0.18&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03iso0p19"]            = "(iso_1_db03<0.18&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03iso0p20"]            = "(iso_1_db03<0.19&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03iso0p21"]            = "(iso_1_db03<0.21&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03iso0p12"]           = "(iso_1_db03<0.12&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03iso0p13"]           = "(iso_1_db03<0.13&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03iso0p14"]           = "(iso_1_db03<0.14&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03iso0p15"]            = "(iso_2_db03<0.15&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03iso0p16"]            = "(iso_2_db03<0.16&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03iso0p17"]            = "(iso_2_db03<0.17&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03iso0p18"]            = "(iso_2_db03<0.18&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03iso0p19"]            = "(iso_2_db03<0.19&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03iso0p20"]            = "(iso_2_db03<0.20&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03iso0p21"]            = "(iso_2_db03<0.21&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03iso0p12"]           = "(iso_2_db03<0.12&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03iso0p13"]           = "(iso_2_db03<0.13&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03iso0p14"]           = "(iso_2_db03<0.14&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["eea03iso0p15"]            = "(iso_1_ea03<0.15&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["eea03iso0p16"]            = "(iso_1_ea03<0.16&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["eea03iso0p17"]            = "(iso_1_ea03<0.17&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["eea03iso0p18"]            = "(iso_1_ea03<0.18&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["eea03iso0p19"]            = "(iso_1_ea03<0.19&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["eea03iso0p20"]            = "(iso_1_ea03<0.20&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["eea03iso0p21"]            = "(iso_1_ea03<0.21&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["eea03iso0p12"]           = "(iso_1_ea03<0.12&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["eea03iso0p13"]           = "(iso_1_ea03<0.13&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["eea03iso0p14"]           = "(iso_1_ea03<0.14&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mea03iso0p15"]            = "(iso_2_ea03<0.15&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mea03iso0p16"]            = "(iso_2_ea03<0.16&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mea03iso0p17"]            = "(iso_2_ea03<0.17&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mea03iso0p18"]            = "(iso_2_ea03<0.18&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mea03iso0p19"]            = "(iso_2_ea03<0.19&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mea03iso0p20"]            = "(iso_2_ea03<0.20&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mea03iso0p21"]            = "(iso_2_ea03<0.21&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mea03iso0p12"]           = "(iso_2_ea03<0.12&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mea03iso0p13"]           = "(iso_2_ea03<0.13&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mea03iso0p14"]           = "(iso_2_ea03<0.14&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03allchiso0p15"]            = "(iso_1_db03allch<0.15&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03allchiso0p16"]            = "(iso_1_db03allch<0.16&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03allchiso0p17"]            = "(iso_1_db03allch<0.17&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03allchiso0p18"]            = "(iso_1_db03allch<0.18&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03allchiso0p19"]            = "(iso_1_db03allch<0.19&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03allchiso0p20"]            = "(iso_1_db03allch<0.20&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03allchiso0p21"]            = "(iso_1_db03allch<0.21&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03allchiso0p12"]           = "(iso_1_db03allch<0.12&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03allchiso0p13"]           = "(iso_1_db03allch<0.13&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb03allchiso0p14"]           = "(iso_1_db03allch<0.14&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03allchiso0p15"]            = "(iso_2_db03allch<0.15&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03allchiso0p16"]            = "(iso_2_db03allch<0.16&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03allchiso0p17"]            = "(iso_2_db03allch<0.17&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03allchiso0p18"]            = "(iso_2_db03allch<0.18&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03allchiso0p19"]            = "(iso_2_db03allch<0.19&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03allchiso0p20"]            = "(iso_2_db03allch<0.20&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03allchiso0p21"]            = "(iso_2_db03allch<0.21&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03allchiso0p12"]           = "(iso_2_db03allch<0.12&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03allchiso0p13"]           = "(iso_2_db03allch<0.13&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb03allchiso0p14"]           = "(iso_2_db03allch<0.14&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb04allchiso0p15"]            = "(iso_1_db04allch<0.15&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb04allchiso0p16"]            = "(iso_1_db04allch<0.16&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb04allchiso0p17"]            = "(iso_1_db04allch<0.17&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb04allchiso0p18"]            = "(iso_1_db04allch<0.18&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb04allchiso0p19"]            = "(iso_1_db04allch<0.19&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb04allchiso0p20"]            = "(iso_1_db04allch<0.20&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb04allchiso0p21"]            = "(iso_1_db04allch<0.21&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb04allchiso0p12"]           = "(iso_1_db04allch<0.12&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb04allchiso0p13"]           = "(iso_1_db04allch<0.13&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["edb04allchiso0p14"]           = "(iso_1_db04allch<0.14&&iso_2<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb04allchiso0p15"]            = "(iso_2_db04allch<0.15&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb04allchiso0p16"]            = "(iso_2_db04allch<0.16&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb04allchiso0p17"]            = "(iso_2_db04allch<0.17&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb04allchiso0p18"]            = "(iso_2_db04allch<0.18&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb04allchiso0p19"]            = "(iso_2_db04allch<0.19&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb04allchiso0p20"]            = "(iso_2_db04allch<0.20&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb04allchiso0p21"]            = "(iso_2_db04allch<0.21&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb04allchiso0p12"]           = "(iso_2_db04allch<0.12&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb04allchiso0p13"]           = "(iso_2_db04allch<0.13&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";
      alias_map_["mdb04allchiso0p14"]           = "(iso_2_db04allch<0.14&&iso_1<0.15&&"+alias_map_["incnoiso"]+")";

      //alias_map_["qcd_loose_shape"]         = "(iso_1>0.2&&iso_1<0.5  && iso_2>0.2&&iso_2<0.5)";
      alias_map_["qcd_loose_shape"]         = "(iso_1>0.2&&iso_1<0.5  && iso_2>0.2&&iso_2<0.5 && !leptonveto)";
    } else if (ch_ == channel::zmm || ch_ == channel::zee) {
      alias_map_["inclusive"]         = "(iso_1<0.1 && iso_2<0.1)";
    } else if (ch_ == channel::wmnu) {
      alias_map_["inclusive"]         = "(iso_1<0.1)";
    }
    
    // Selection control regions
    alias_map_["sel"]                    = "1";
    alias_map_["w_sdb"]                  = "mt_1>80.";
    alias_map_["w_sdb_os"]               = "os";
    alias_map_["w_vbf_sdb"]              = "mt_1>60. && mt_1<120.";
    alias_map_["w_os"]                   = "os";
    alias_map_["w_vbf_os"]               = "os";
    alias_map_["w_ss"]                   = "!os";
    alias_map_["w_shape_os"]             = "os"; 

    if (ch_ == channel::em) {
    // Inclusive region for e-mu fake rate normalisation
      // Sideband region for e-mu SS fake shape estimation
      alias_map_["ss"]                        = "!os";
      alias_map_["sel"]                       = "1";
    }

    alias_map_["ZTT_Shape_Sample"]  = "DYJetsToLL_M-50-LO";

    // Samples to combine for diboson contribution
    samples_alias_map_["vv_samples"] = {
     "T-tW", "Tbar-tW", "T-t","Tbar-t",
     "WWToLNuQQ","WWTo2L2Nu",
     "ZZTo2L2Nu","ZZTo2L2Q","ZZTo4L",
     "WZTo2L2Q","WZTo3LNu","WZTo1L3Nu","WZTo1L1Nu2Q"
    };

/*  if(ch_==channel::em){
    samples_alias_map_["vv_samples"] = {
//     "WZJetsTo3LNu",
     "T-tW", "Tbar-tW", "WWinclusive","WZinclusive", "ZZinclusive"//,"WWTo2L2Nu","WWTo4Q","WZTo1L1Nu2Q","ZZTo4L"
    //,"WWTo2L2Nu","WWTo4Q","WZTo1L1Nu2Q","ZZTo4L"
    };
  }*/



    samples_alias_map_["top_samples"] = {
     "TT-ext"
    };

 
   samples_alias_map_["ztt_shape_samples"]={
    "DYJetsToLL_M-50-LO"//, "DYJetsToLL10-50"
   };

   samples_alias_map_["data_samples"] = {
   "SingleElectron-2015D-prompt"
   };
   if(ch_==channel::et || ch_==channel::zee){
     samples_alias_map_["data_samples"] = {
      "SingleElectron-2015D-Oct05","SingleElectron-2015D-promptv4"
     };
   }
   if(ch_==channel::mt || ch_==channel::zmm || ch_==channel::wmnu){
     samples_alias_map_["data_samples"] = {
     "SingleMuon-2015D-Oct05","SingleMuon-2015D-promptv4"
     };
   }
   if(ch_==channel::tt){
     samples_alias_map_["data_samples"] = {
      "Tau-2015D-Oct05","Tau-2015D-promptv4"
     };
   }
   if(ch_==channel::em){
    samples_alias_map_["data_samples"] = {
      "MuonEG-2015D-Oct05","MuonEG-2015D-promptv4"
    };
   }



  if(use_status_flags_){
    samples_alias_map_["zj_samples"] = {
     "DYJetsToLL_M-50-LO",
    };

   samples_alias_map_["ztt_samples"]={
     "DYJetsToLL_M-50-LO",
   };
   samples_alias_map_["zl_samples"] = {
     "DYJetsToLL_M-50-LO",
   };
   samples_alias_map_["zll_samples"] = {
     "DYJetsToLL_M-50-LO",
   };



  } else {
     samples_alias_map_["zj_samples"] = {
     "DYJetsToTauTau-JJ","DYJetsToLL-J",
//     "DYJetsToTauTau10-50-JJ", "DYJetsToLL10-50-J"
    };
   samples_alias_map_["ztt_samples"]={
     "DYJetsToTauTau"//,"DYJetsToTauTau10-50"
  };
   samples_alias_map_["zl_samples"] = {
     "DYJetsToLL-L"//,"DYJetsToLL10-50-L"
   };
   if(ch_ == channel::em){
     samples_alias_map_["zll_samples"] = {
     "DYJetsToLL"
     };
   }
 }  
  
 if(ch_!=channel::em && use_status_flags_){
  samples_alias_map_["qcd_sub_samples"] = {
   "DYJetsToLL_M-50-LO", 
   "T-tW", "Tbar-tW", "T-t","Tbar-t",
   "WWToLNuQQ","WWTo2L2Nu",
   "ZZTo2L2Nu","ZZTo2L2Q","ZZTo4L",
   "WZTo2L2Q","WZTo3LNu","WZTo1L3Nu","WZTo1L1Nu2Q",
   "WJetsToLNu-LO","TT-ext"
   };



  }
  

 if(ch_!=channel::em && !use_status_flags_){
  samples_alias_map_["qcd_sub_samples"] = {
   "DYJetsToTauTau-JJ","DYJetsToLL-J","DYJetsToTauTau","DYJetsToLL-L", 
   "T-tW", "Tbar-tW", "WWinclusive","WZinclusive", "ZZinclusive",//"WWTo2L2Nu","WWTo4Q","WZTo1L1Nu2Q","ZZTo4L",
   "WJetsToLNu","TT"
   };

  }

 if(ch_==channel::em && !use_status_flags_){
  samples_alias_map_["qcd_sub_samples"] = {
   "DYJetsToLL","DYJetsToTauTau",
   "T-tW", "Tbar-tW", "WWinclusive","WZinclusive", "ZZinclusive",//"WWTo2L2Nu","WWTo4Q","WZTo1L1Nu2Q","ZZTo4L",
   "TT"
   };
  }
  

 if(ch_==channel::em && use_status_flags_){
  samples_alias_map_["qcd_sub_samples"] = {
   "DYJetsToLL_M-50-LO",
   "T-tW", "Tbar-tW", "T-t","Tbar-t",
   "WWToLNuQQ","WWTo2L2Nu",
   "ZZTo2L2Nu","ZZTo2L2Q","ZZTo4L",
   "WZTo2L2Q","WZTo3LNu","WZTo1L3Nu","WZTo1L1Nu2Q",
   "TT-ext","WJetsToLNu-LO"
   };

  }

if(use_status_flags_){
  samples_alias_map_["w_sub_samples"] = {
   "DYJetsToLL_M-50-LO",
   "T-tW", "Tbar-tW", "T-t","Tbar-t",
   "WWToLNuQQ","WWTo2L2Nu",
   "ZZTo2L2Nu","ZZTo2L2Q","ZZTo4L",
   "WZTo2L2Q","WZTo3LNu","WZTo1L3Nu","WZTo1L1Nu2Q",
   "TT-ext"
   };

  }else{
  samples_alias_map_["w_sub_samples"] = {
   "DYJetsToLL-L","DYJetsToTauTau","DYJetsToLL-J","DYJetsToTauTau-JJ"
   "T-tW", "Tbar-tW", "WWinclusive","WZinclusive", "ZZinclusive",//"WWTo2L2Nu","WWTo4Q","WZTo1L1Nu2Q","ZZTo4L",
   "TT"
   };

}

samples_alias_map_["wjets_samples"] = {
  "WJetsToLNu-LO"
 };

sample_names_={};
push_back(sample_names_,this->ResolveSamplesAlias("ztt_samples"));
push_back(sample_names_,this->ResolveSamplesAlias("zl_samples"));
push_back(sample_names_,this->ResolveSamplesAlias("zj_samples"));
push_back(sample_names_,this->ResolveSamplesAlias("vv_samples"));
push_back(sample_names_,this->ResolveSamplesAlias("wjets_samples"));
if(ch_ == channel::em){
  push_back(sample_names_,this->ResolveSamplesAlias("zll_samples"));
}
push_back(sample_names_,this->ResolveSamplesAlias("top_samples"));
push_back(sample_names_,this->ResolveSamplesAlias("data_samples"));

  }

  void HTTRun2Analysis::SetQCDRatio(double const& ratio){
    qcd_os_ss_factor_ = ratio;
  }

  void HTTRun2Analysis::AddSample(std::string const& sample) {
    sample_names_.push_back(sample);
  }

  void HTTRun2Analysis::ParseParamFile(std::string const& file) {
    SimpleParamParser parser;
    parser.ParseFile(file);
    std::cout << "[HTTRun2Analysis::ParseParamFile] Extracting sample info from file " << file << std::endl;
    lumi_ = parser.GetParam<double>("LUMI_2015_"+Channel2String(ch_));
    std::cout << "[HTTRun2Analysis::ParseParamFile] Integrated luminosity set to " << lumi_ << " /pb" << std::endl;
//    if (verbosity_ > 1) std::cout << boost::format("%-25s %152 %15.3f %15.3f %15.3f\n") % "Sample" % "Events" % "Cross Section" % "Sample Lumi" % "Rel. Lumi";
    if (verbosity_ > 1) std::cout << "-----------------------------------------------------------------------------------------\n";
    for (auto sample : sample_names_) {
      std::string lookup = sample;
      if (sample.find("Special") != sample.npos) {
        size_t found = sample.find('_');
        found = sample.find('_',found+1);
        lookup = sample.substr(found+1);
      }
      if (parser.ParamExists("XS_"+sample) && parser.ParamExists("EVT_"+sample)) {
        double evt = parser.GetParam<double>("EVT_"+sample);
        double xs = parser.GetParam<double>("XS_"+sample);
        if (xs <= 0) continue;
        sample_info_[sample] = std::make_pair(evt, xs);
      //  if (verbosity_ > 1) std::cout << boost::format("%-25s %15i %15.3f %15.3f %15.3f\n") % sample % unsigned(evt+0.5) % xs % (evt/xs) % (evt/(xs*lumi_));
      }
    }
  }

  void HTTRun2Analysis::AddSMSignalSamples(std::vector<std::string> masses) {
    for (auto m : masses) {
        sample_names_.push_back("GluGluHToTauTau_M-"+m);
        //sample_names_.push_back("VBF_HToTauTau_M-"+m");
    }
  }

  void HTTRun2Analysis::AddMSSMSignalSamples(std::vector<std::string> masses) {
    for (auto m : masses) {
        sample_names_.push_back("SUSYGluGluToHToTauTau_M-"+m);
//      sample_names_.push_back("SUSYBBHToTauTau_M-"+m);
    }
  }

  void HTTRun2Analysis::ReadTrees(std::string const& folder, std::string const& fallback_folder) {
    std::cout << "[HTTRun2Analysis::ReadTrees] Reading input files..." << std::endl;
    std::vector<std::string> result_summary;
    for (auto name : sample_names_) {
      // The input file is folder + sample name + channel + year
      std::string input_filename = folder+"/"+name+"_"+Channel2String(ch_)+"_"+year_+".root";
      std::string label = name;
      TFile *tmp_file = nullptr;
      if (boost::filesystem::exists(input_filename)) tmp_file = TFile::Open(input_filename.c_str());
      if (!tmp_file && fallback_folder != "") {
        if (verbosity_ > 1) std::cout << "[HTTRun2Analysis::ReadTrees] " << input_filename << " not found, trying fallback folder" << std::endl;
        input_filename = fallback_folder+"/"+name+"_"+Channel2String(ch_)+"_"+year_+".root";
        if (boost::filesystem::exists(input_filename)) tmp_file = TFile::Open(input_filename.c_str());
      }
      if (!tmp_file) {
        std::cout << "[HTTRun2Analysis::ReadTrees] Warning: " << input_filename << " cannot be opened" << std::endl;
        continue;
      }
      if (verbosity_ > 1) result_summary.push_back((boost::format("%-70s %s %-30s\n") % input_filename % "-->" % label).str());
      gDirectory->cd("/");
      TTree *tmp_tree = dynamic_cast<TTree*>(gDirectory->Get("ntuple"));
      if (!tmp_tree) {
        std::cerr << "[HTTRun2Analysis::ReadTrees] Warning: Unable to extract TTree from file " << input_filename << std::endl;
        continue;        
      }
      tmp_tree->SetEstimate(100000);
      tfiles_[label] = tmp_file;
      ttrees_[label] = tmp_tree;
    }
    for (auto str : result_summary) std::cout << str;
  }

  double HTTRun2Analysis::GetLumiScale(std::string const& sample) {
    auto it = sample_info_.find(sample);
    if (it != sample_info_.end()) {
      double evt = it->second.first;
      double xs = it->second.second;
      return ((xs*lumi_)/evt);
    } else {
      return 1.0;
    }
  }

  double HTTRun2Analysis::GetLumiScaleFixedXS(std::string const& sample, double xs) {
      auto it = sample_info_.find(sample);
    if (it != sample_info_.end()) {
      double evt = it->second.first;;
      return ((xs*lumi_)/evt);
    } else {
      return 1.0;
    }
  }


  HTTRun2Analysis::HistValuePair HTTRun2Analysis::GenerateData(unsigned /*method*/, std::string var, std::string sel, std::string cat, std::string wt) {
    auto data_norm = this->GetRate(this->ResolveSamplesAlias("data_samples"), sel, cat, wt);
    TH1F data_hist = this->GetShape(var, this->ResolveSamplesAlias("data_samples"), sel, cat, wt);
    SetNorm(&data_hist, data_norm.first);
    return std::make_pair(data_hist, data_norm);
  }



  HTTRun2Analysis::HistValuePair HTTRun2Analysis::GenerateZTT(unsigned /*method*/, std::string var, std::string sel, std::string cat, std::string wt) {
    if (verbosity_) std::cout << "[HTTRun2Analysis::GenerateZTT] --------------------------------------------------------\n";
    Value ztt_norm;
    //ztt_norm = this->GetRateViaRefEfficiency(this->ResolveAlias("ZTT_Eff_Sample"), "DYJetsToLL", "os", this->ResolveAlias("inclusive"), sel, cat, wt);
    std::vector<std::string> ztt_samples = this->ResolveSamplesAlias("ztt_samples");
    std::vector<std::string> ztt_shape_samples = this->ResolveSamplesAlias("ztt_samples");
    ztt_norm = this->GetLumiScaledRate(ztt_samples, sel, cat, wt) ;
    TH1F ztt_hist = this->GetLumiScaledShape(var, ztt_samples, sel, cat, wt);
    if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
      % this->ResolveAlias("ZTT_Shape_Sample") % sel % cat % wt;
    SetNorm(&ztt_hist, ztt_norm.first);
    if (ch_ != channel::em && !use_status_flags_) {
      auto ztt_leptonic_norm = this->GetLumiScaledRate("DYJetsToTauTau-L", sel, cat, wt);
      TH1F ztt_leptonic_hist = this->GetLumiScaledShape(var, "DYJetsToTauTau-L", sel, cat, wt);
      SetNorm(&ztt_leptonic_hist, ztt_leptonic_norm.first);
      if (verbosity_) PrintValue("ZTT-Leptonic", ztt_leptonic_norm); 
      ztt_norm = ValueAdd(ztt_norm, ztt_leptonic_norm);
      ztt_hist.Add(&ztt_leptonic_hist);
/*
      auto ztt_hadronic_norm = this->GetLumiScaledRate("DYJetsToTauTau-JJ"+dy_soup_, sel, cat, wt);
      TH1F ztt_hadronic_hist = this->GetLumiScaledShape(var, "DYJetsToTauTau-JJ"+dy_soup_, sel, cat, wt);
      SetNorm(&ztt_hadronic_hist, ztt_hadronic_norm.first);
      if (verbosity_) PrintValue("ZTT-Hadronic", ztt_hadronic_norm);
      ztt_norm = ValueAdd(ztt_norm, ztt_hadronic_norm);
      ztt_hist.Add(&ztt_hadronic_hist);
*/
    }

    return std::make_pair(ztt_hist, ztt_norm);
  }
  
  HTTRun2Analysis::HistValuePair HTTRun2Analysis::GenerateZL(unsigned /*method*/, std::string var, std::string sel, std::string cat, std::string wt) {
    if (verbosity_) std::cout << "[HTTRun2Analysis::GenerateZL --------------------------------------------------------\n";
    Value zl_norm;
    //ztt_norm = this->GetRateViaRefEfficiency(this->ResolveAlias("ZTT_Eff_Sample"), "DYJetsToLL", "os", this->ResolveAlias("inclusive"), sel, cat, wt);
    std::vector<std::string> zl_samples = this->ResolveSamplesAlias("zl_samples");
    zl_norm = this->GetLumiScaledRate(zl_samples, sel, cat, wt) ;
    TH1F zl_hist = this->GetLumiScaledShape(var, zl_samples, sel, cat, wt);
    if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
      % "DYJetsToLL" % sel % cat % wt;
    SetNorm(&zl_hist, zl_norm.first);
    return std::make_pair(zl_hist, zl_norm);
  }
  
  HTTRun2Analysis::HistValuePair HTTRun2Analysis::GenerateZJ(unsigned /*method*/, std::string var, std::string sel, std::string cat, std::string wt) {
    if (verbosity_) std::cout << "[HTTRun2Analysis::GenerateZJ --------------------------------------------------------\n";
    Value zj_norm;
    //ztt_norm = this->GetRateViaRefEfficiency(this->ResolveAlias("ZTT_Eff_Sample"), "DYJetsToLL", "os", this->ResolveAlias("inclusive"), sel, cat, wt);
    std::vector<std::string> zj_samples = this->ResolveSamplesAlias("zj_samples");
    if (verbosity_) {
      std::cout << "zj_samples: ";
      for (unsigned i = 0; i < zj_samples.size(); ++i) {
        std::cout << zj_samples[i];
        if (i != zj_samples.size()-1) std::cout << ", ";
      }
      std::cout << std::endl;
    }
    zj_norm = this->GetLumiScaledRate(zj_samples, sel, cat, wt) ;
    TH1F zj_hist = this->GetLumiScaledShape(var, zj_samples, sel, cat, wt);
    if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
      % "zj_samples" % sel % cat % wt;
    SetNorm(&zj_hist, zj_norm.first);
    return std::make_pair(zj_hist, zj_norm);
  }
  
  HTTRun2Analysis::HistValuePair HTTRun2Analysis::GenerateZLL(unsigned /*method*/, std::string var, std::string sel, std::string cat, std::string wt) {
    if (verbosity_) std::cout << "[HTTRun2Analysis::GenerateZLL --------------------------------------------------------\n";
    Value zl_norm;
    std::vector<std::string> zll_samples = this->ResolveSamplesAlias("zll_samples");
    zl_norm = this->GetLumiScaledRate(zll_samples, sel, cat, wt) ;
    TH1F zl_hist = this->GetLumiScaledShape(var, zll_samples, sel, cat, wt);
    if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
      % "DYJetsToLL" % sel % cat % wt;
    SetNorm(&zl_hist, zl_norm.first);
    return std::make_pair(zl_hist, zl_norm);
  }


  HTTRun2Analysis::HistValuePair HTTRun2Analysis::GenerateTOP(unsigned /*method*/, std::string var, std::string sel, std::string cat, std::string wt) {
    if (verbosity_) std::cout << "[HTTRun2Analysis::GenerateTOP] --------------------------------------------------------\n";
    std::vector<std::string> top_samples = this->ResolveSamplesAlias("top_samples");
    if (verbosity_) {
      std::cout << "top_samples: ";
      for (unsigned i = 0; i < top_samples.size(); ++i) {
        std::cout << top_samples[i];
        if (i != top_samples.size()-1) std::cout << ", ";
      }
      std::cout << std::endl;
    }
    auto top_norm = this->GetLumiScaledRate(top_samples, sel, cat, wt);
    std::string top_shape_cat = cat;
    TH1F top_hist = this->GetLumiScaledShape(var, top_samples, sel, top_shape_cat, wt);
    // TH1F top_hist = this->GetLumiScaledShape(var, top_shape_sample, sel, top_shape_cat, wt);
    if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
      % "top_samples" % sel % top_shape_cat % wt;
    SetNorm(&top_hist, top_norm.first);
    return std::make_pair(top_hist, top_norm);
  }

  HTTRun2Analysis::HistValuePair HTTRun2Analysis::GenerateVV(unsigned /*method*/, std::string var, std::string sel, std::string cat, std::string wt) {
    if (verbosity_) std::cout << "[HTTRun2Analysis::GenerateVV] ---------------------------------------------------------\n";
    std::vector<std::string> vv_samples = this->ResolveSamplesAlias("vv_samples");
    auto vv_norm = this->GetLumiScaledRate(vv_samples, sel, cat, wt);
    std::string vv_shape_cat = cat;
    TH1F vv_hist = this->GetLumiScaledShape(var, vv_samples, sel, vv_shape_cat, wt);
    if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
      % "vv_samples" % sel % vv_shape_cat % wt;
    SetNorm(&vv_hist, vv_norm.first);
    return std::make_pair(vv_hist, vv_norm);
  }

  HTTRun2Analysis::HistValuePair HTTRun2Analysis::GenerateW(unsigned method, std::string var, std::string sel, std::string cat, std::string wt) {
    if (verbosity_) std::cout << "[HTTRun2Analysis::GenerateW] ----------------------------------------------------------\n";
    std::vector<std::string> w_sub_samples = this->ResolveSamplesAlias("w_sub_samples");
    std::string w_extrap_cat = cat;
    std::string w_extrp_sdb_sel = this->ResolveAlias("w_os")+" && "+this->ResolveAlias("w_sdb");
    std::string w_extrp_sig_sel = this->ResolveAlias("w_os")+" && "+this->ResolveAlias("sel");
    std::string w_sdb_sel = this->ResolveAlias("w_sdb_os")+" && "+this->ResolveAlias("w_sdb");
    
    Value w_norm;
    std::vector<std::string> wjets_samples = this->ResolveSamplesAlias("wjets_samples");
    if(method == 8 || method == 9) {
      w_norm = this->GetLumiScaledRate(wjets_samples, sel, cat, wt);
    } else if(method == 10 || method == 11){
     w_norm = this->GetRateViaWMethod(wjets_samples.at(0), w_extrap_cat, w_extrp_sdb_sel, w_extrp_sig_sel, 
        this->ResolveSamplesAlias("data_samples"), cat, w_sdb_sel, w_sub_samples, wt, ValueFnMap());
    }
    
    std::string w_shape_cat = cat;
    std::string w_shape_sel = this->ResolveAlias("w_shape_os") + " && " + this->ResolveAlias("sel");
    TH1F w_hist = this->GetShape(var, wjets_samples, w_shape_sel, w_shape_cat, wt);
    //if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
     // % wjets_samples % w_shape_sel % w_shape_cat % wt;
    SetNorm(&w_hist, w_norm.first);
    return std::make_pair(w_hist, w_norm);
  }

  HTTRun2Analysis::HistValuePair HTTRun2Analysis::GenerateQCD(unsigned method, std::string var, std::string /*sel*/, std::string cat, std::string wt) {
    if (verbosity_) std::cout << "[HTTRun2Analysis::GenerateQCD] --------------------------------------------------------\n";
    Value qcd_norm;
    TH1F qcd_hist;
//    if (ch_ != channel::em) {
      std::vector<std::string> qcd_sub_samples;
      qcd_sub_samples = this->ResolveSamplesAlias("qcd_sub_samples");
      
      std::vector<std::string> w_sub_samples;
       if(ch_ != channel::em){
          w_sub_samples = this->ResolveSamplesAlias("w_sub_samples");
       }
      std::string qcd_sdb_sel = "!os && " + this->ResolveAlias("sel");
      std::string w_extrp_sdb_sel = this->ResolveAlias("w_ss")+" && "+this->ResolveAlias("w_sdb");
      std::string w_extrp_sig_sel = this->ResolveAlias("w_ss")+" && "+this->ResolveAlias("sel");
      std::string w_sdb_sel = "!os && "+this->ResolveAlias("w_sdb");
      std::string qcd_cat = cat;
      //Work out the W norm in the SS region separately if it is data driven
      Value w_ss_norm;
      std::vector<std::string> wjets_samples = this->ResolveSamplesAlias("wjets_samples");
      if(ch_ != channel::em) {
        if(method == 10 || method == 11) { 
          w_ss_norm = this->GetRateViaWMethod(wjets_samples.at(0), qcd_cat, w_extrp_sdb_sel, w_extrp_sig_sel, 
              this->ResolveSamplesAlias("data_samples"), qcd_cat, w_sdb_sel, w_sub_samples, wt, ValueFnMap());
        } else {
          w_ss_norm = std::make_pair(0.0,0.0);
        }
      } else {
        w_ss_norm = std::make_pair(0.0,0.0);
      }
      
      if(method == 8 || method == 9) {
        qcd_norm = this->GetRateViaQCDMethod(std::make_pair(qcd_os_ss_factor_,0.), this->ResolveSamplesAlias("data_samples"), qcd_sdb_sel, qcd_cat, qcd_sub_samples, wt, ValueFnMap());
      } else if (method == 10 || method == 11) {
        qcd_norm = this->GetRateViaQCDMethod(std::make_pair(qcd_os_ss_factor_,0.), this->ResolveSamplesAlias("data_samples"), qcd_sdb_sel, qcd_cat, qcd_sub_samples, wt,{
          {wjets_samples.at(0), [&]()->HTTRun2Analysis::Value {
            return w_ss_norm;}
          }
        });
      } 
      if (qcd_norm.first <= 0.0) {
        double default_rate = 0.0000001;
        std::cout << "[HTTRun2Analysis::GenerateQCD] Warning, QCD rate is negative (" 
          << qcd_norm.first << "), setting to " << default_rate << " and maintaining error" << std::endl;
        qcd_norm.first = default_rate;
      }
      std::string qcd_shape_cat = cat;
      if (method == 8) {
        qcd_hist = this->GetShapeViaQCDMethod(var, this->ResolveSamplesAlias("data_samples"), qcd_sdb_sel, qcd_shape_cat, qcd_sub_samples, wt, ValueFnMap());
      } else if(method == 10) {
        qcd_hist = this->GetShapeViaQCDMethod(var, this->ResolveSamplesAlias("data_samples"), qcd_sdb_sel, qcd_shape_cat, qcd_sub_samples, wt, {
        {wjets_samples.at(0), [&]()->HTTRun2Analysis::Value {
            return w_ss_norm;} 
          }
        });
      } else {
        if (method == 9 || method == 11) qcd_shape_cat = this->ResolveAlias("qcd_loose_shape");
        qcd_hist = this->GetShape(var, this->ResolveSamplesAlias("data_samples"), qcd_sdb_sel, qcd_shape_cat, wt);
//        if (verbosity_) std::cout << "Shape: " << boost::format("%s,'%s','%s','%s'\n")
 //         % this->ResolveSamplesAlias("data_samples") % qcd_sdb_sel % qcd_shape_cat % wt;
      }
    SetNorm(&qcd_hist, qcd_norm.first);
    return std::make_pair(qcd_hist, qcd_norm);
  }

  HTTRun2Analysis::HistValuePair HTTRun2Analysis::GenerateSignal(std::string sample, std::string var, std::string sel, std::string cat, std::string wt, double xs) {
    Value signal_norm;
    if (xs > 0) {
      if (verbosity_ > 1) std::cout << "[HTTRun2Analysis::GenerateSignal] " << sample << " scaled to lumi using cross section " << xs << " pb" << std::endl;
      signal_norm = GetRate(sample, sel, cat, wt);
      signal_norm = ValueProduct(signal_norm, std::make_pair(this->GetLumiScaleFixedXS(sample, xs), 0.0));
    } else {
      signal_norm = GetLumiScaledRate(sample, sel, cat, wt);
    }
    TH1F signal_shape = this->GetShape(var, sample, sel, cat, wt);
    SetNorm(&signal_shape, signal_norm.first);
    return std::make_pair(signal_shape, signal_norm);
  }

  void HTTRun2Analysis::FillSMSignal(HistValueMap & hmap, 
                    std::vector<std::string> const& masses,
                    std::string const& var,
                    std::string const& sel,
                    std::string const& cat,
                    std::string const& wt,
                    std::string const& infix,
                    std::string const& postfix,
                    double fixed_xs) {
    for (auto const& m : masses) {
        hmap["ggH"+infix+m+postfix] = this->GenerateSignal("GluGluHToTauTau_M-"+m,    var, sel, cat, wt, fixed_xs);
//      hmap["qqH"+infix+m+postfix] = this->GenerateSignal("VBF_HToTauTau_M-"+m,        var, sel, cat, wt, fixed_xs);
    }
  }
  

  void HTTRun2Analysis::FillMSSMSignal(HistValueMap & hmap, 
                    std::vector<std::string> const& masses,
                    std::string const& var,
                    std::string const& sel,
                    std::string const& cat,
                    std::string const& wt,
                    std::string const& infix,
                    std::string const& postfix,
                    double fixed_xs) {
    for (auto const& m : masses) {
        hmap["ggH"+infix+m+postfix] = this->GenerateSignal("SUSYGluGluToHToTauTau_M-"+m, var, sel, cat, wt, fixed_xs);
//      hmap["bbH"+infix+m+postfix] = this->GenerateSignal("SUSYBBHToTauTau_M-"+m,       var, sel, cat, wt, fixed_xs);
    }
  }


  void HTTRun2Analysis::FillHistoMap(HistValueMap & hmap, unsigned method,
                        std::string var,
                        std::string sel,
                        std::string cat,
                        std::string wt,
                        std::string postfix) {
    Value total_bkr;
    // Data
    auto data_pair = this->GenerateData(method, var, sel, cat, wt);
    PrintValue("data_obs"+postfix, data_pair.second);
    hmap["data_obs"+postfix] = data_pair;
    // Top
    auto top_pair = this->GenerateTOP(method, var, sel, cat, wt);
    std::string top_map_label = "TT";
    //std::string top_map_label = (ch_ == channel::em) ? "ttbar" : "TT";
    PrintValue(top_map_label+postfix, top_pair.second);
    total_bkr = ValueAdd(total_bkr, top_pair.second);
    hmap[top_map_label+postfix] = top_pair;
    TH1F total_hist = hmap[top_map_label+postfix].first; 
    // Diboson
    auto vv_pair = this->GenerateVV(method, var, sel, cat, wt);
    std::string vv_map_label =  "VV";
    //std::string vv_map_label = (ch_ == channel::em) ? "EWK" : "VV";
    PrintValue(vv_map_label+postfix, vv_pair.second);
    total_bkr = ValueAdd(total_bkr, vv_pair.second);
    hmap[vv_map_label+postfix] = vv_pair;
    total_hist.Add(&hmap[vv_map_label+postfix].first,1.0);
    // Z->ll
    if (ch_ != channel::em && ch_!= channel::zee && ch_!= channel::zmm && ch_!=channel::wmnu) {
      std::string zl_sel, zj_sel;
      if(use_status_flags_){
      zl_sel= sel+"&&"+this->ResolveAlias("zl_sel");
      zj_sel= sel+"&&"+this->ResolveAlias("zj_sel");
      } else {
      zl_sel = sel;
      zj_sel = sel;
      }
      auto zl_pair = this->GenerateZL(method, var, zl_sel, cat, wt);
      auto zj_pair = this->GenerateZJ(method, var, zj_sel, cat, wt);
      Value zll_norm = ValueAdd(zl_pair.second, zj_pair.second);
      TH1F zll_hist = zl_pair.first;
      zll_hist.Add(&zj_pair.first);
      PrintValue("ZLL"+postfix, zll_norm);
      PrintValue("ZL"+postfix, zl_pair.second);
      PrintValue("ZJ"+postfix, zj_pair.second);
      total_bkr = ValueAdd(total_bkr, zll_norm);
      hmap["ZLL"+postfix] = std::make_pair(zll_hist, zll_norm);
      hmap["ZL"+postfix]  = zl_pair;
      hmap["ZJ"+postfix]  = zj_pair;
      total_hist.Add(&hmap["ZLL"+postfix].first,1.0);
    } else {
      std::string zll_sel;
      if(use_status_flags_){
      zll_sel = sel+"&&"+this->ResolveAlias("zll_sel");
      }else zll_sel=sel;
      auto zll_pair = this->GenerateZLL(method, var, zll_sel, cat, wt);
      std::string zll_map_label = "ZLL";
      PrintValue(zll_map_label+postfix, zll_pair.second);
      total_bkr = ValueAdd(total_bkr, zll_pair.second);
      hmap[zll_map_label+postfix] = zll_pair;
      total_hist.Add(&hmap[zll_map_label+postfix].first,1.0);
    }
    // Z->tautau
    if(ch_!= channel::zee && ch_!= channel::zmm && ch_!=channel::wmnu) {
      std::string ztt_sel;
      if(use_status_flags_){
        ztt_sel = sel+"&&"+this->ResolveAlias("ztt_sel");
      } else ztt_sel = sel;
      auto ztt_pair = this->GenerateZTT(method, var, ztt_sel, cat, wt);
      std::string ztt_map_label = "ZTT";
      //std::string ztt_map_label = (ch_ == channel::em) ? "Ztt" : "ZTT";
      PrintValue(ztt_map_label+postfix, ztt_pair.second);
      total_bkr = ValueAdd(total_bkr, ztt_pair.second);
      hmap[ztt_map_label+postfix] = ztt_pair;
      total_hist.Add(&hmap[ztt_map_label+postfix].first,1.0);
    }    // W+jets
//    if (ch_ != channel::em) {
      auto w_pair = this->GenerateW(method, var, sel, cat, wt);
      PrintValue("W"+postfix, w_pair.second);
      total_bkr = ValueAdd(total_bkr, w_pair.second);
      hmap["W"+postfix] = w_pair;
      total_hist.Add(&hmap["W"+postfix].first,1.0);
 //   }
    // QCD/Fakes
    if(ch_!= channel::zee && ch_!= channel::zmm && ch_!=channel::wmnu) {
      auto qcd_pair = this->GenerateQCD(method, var, sel, cat, wt);
      std::string qcd_map_label = "QCD";
      //std::string qcd_map_label = (ch_ == channel::em) ? "Fakes" : "QCD";
      PrintValue(qcd_map_label+postfix, qcd_pair.second);
      total_bkr = ValueAdd(total_bkr, qcd_pair.second);
      hmap[qcd_map_label+postfix] = qcd_pair;
    }
    // Print the total background yield
    PrintValue("Total"+postfix, total_bkr);
    //Until there is data, fill the data with sum of the backgrounds
//    hmap["data_obs"+postfix] = std::make_pair(total_hist,total_bkr);
    return;
  }

  std::string HTTRun2Analysis::BuildCutString(std::string const& selection,
      std::string const& category,
      std::string const& weight) {
    std::string full_selection;
    if (weight != "" && (selection != "" || category != "")) full_selection += "( ";

    if (selection != "")                    full_selection += ("(" + selection + ")");
    if (selection != "" && category != "")  full_selection += " && ";
    if (category != "")                     full_selection += ("(" + category + ")");
    if (weight != "" && (selection != "" || category != "")) full_selection += " ) * ";
    if (weight != "") full_selection += ("("+weight+")");
    return full_selection;                                      
  }

  std::string HTTRun2Analysis::BuildVarString(std::string const& variable) {
    std::string full_variable = variable;
    if (full_variable.find_last_of("(") != full_variable.npos 
        && full_variable.find("[") == full_variable.npos
        && full_variable.find("]") == full_variable.npos) {
      full_variable.insert(full_variable.find_last_of("("),">>htemp");
    }
    return full_variable;
  }


  TH1F HTTRun2Analysis::GetShape(std::string const& variable,
                                       std::string const& sample, 
                                       std::string const& selection, 
                                       std::string const& category, 
                                       std::string const& weight) {
    TH1::SetDefaultSumw2(true);
    std::string full_variable = BuildVarString(variable);
    std::size_t begin_var = full_variable.find("[");
    std::size_t end_var   = full_variable.find("]");
    TH1F *htemp = nullptr;
    if (begin_var != full_variable.npos && end_var != full_variable.npos) {
      std::string binning = full_variable.substr(begin_var+1, end_var-begin_var-1);
      std::vector<std::string> string_vec;
      boost::split(string_vec, binning, boost::is_any_of(","));
      std::vector<double> bin_vec;
      for (auto str : string_vec) bin_vec.push_back(boost::lexical_cast<double>(str));
      TH1::AddDirectory(true);
      htemp = new TH1F("htemp","htemp", bin_vec.size()-1, &(bin_vec[0]));
      TH1::AddDirectory(false);
      full_variable.erase(begin_var, full_variable.npos);
      full_variable += ">>htemp";
    }
    std::string full_selection = BuildCutString(selection, category, weight);
    // std::cout << full_selection << std::endl;
    // std::cout << full_variable << std::endl;
    TH1::AddDirectory(true);
    //In the case of an empty sample, can return htemp. This is only created above if the []
    //binning is used, so we have to create an empty version in the case of () binning.
    if(ttrees_[sample]->GetEntries() == 0) {
      std::size_t begin_var = full_variable.find_last_of("(");
      std::size_t end_var   = full_variable.find_last_of(")");
      if (begin_var != full_variable.npos && end_var != full_variable.npos) {
        std::string binning = full_variable.substr(begin_var+1, end_var-begin_var-1);
        std::vector<std::string> string_vec;
        boost::split(string_vec, binning, boost::is_any_of(","));
        std::vector<double> bin_vec;
        for (auto str : string_vec) bin_vec.push_back(boost::lexical_cast<double>(str));
        htemp = new TH1F("htemp","htemp", bin_vec[0],bin_vec[1],bin_vec[bin_vec.size()-1]);
      }
      return (*htemp);
    }
    ttrees_[sample]->Draw(full_variable.c_str(), full_selection.c_str(), "goff");
    TH1::AddDirectory(false);
    htemp = (TH1F*)gDirectory->Get("htemp");
    TH1F result = (*htemp);
    gDirectory->Delete("htemp;*");
    auto rate = GetRate(sample, selection, category, weight);
    SetNorm(&result, rate.first);
    return result;
  }

  TH1F HTTRun2Analysis::GetShape(std::string const& variable,
                                       std::vector<std::string> const& samples, 
                                       std::string const& selection, 
                                       std::string const& category, 
                                       std::string const& weight) {

    TH1F result = GetShape(variable, samples.at(0), selection, category, weight);
    if (samples.size() > 1) {
      for (unsigned i = 1; i < samples.size(); ++i) {
        TH1F tmp = GetShape(variable, samples.at(i), selection, category, weight);
        result.Add(&tmp);
      }
    }
    return result;
  }


  TH1F HTTRun2Analysis::GetLumiScaledShape(std::string const& variable,
                                       std::string const& sample, 
                                       std::string const& selection, 
                                       std::string const& category, 
                                       std::string const& weight) {
    TH1F result = GetShape(variable, sample, selection, category, weight);
    result.Scale(GetLumiScale(sample));
    return result;
  }

  TH1F HTTRun2Analysis::GetLumiScaledShape(std::string const& variable,
                                       std::vector<std::string> const& samples, 
                                       std::string const& selection, 
                                       std::string const& category, 
                                       std::string const& weight) {

    TH1F result = GetLumiScaledShape(variable, samples.at(0), selection, category, weight);
    if (samples.size() > 1) {
      for (unsigned i = 1; i < samples.size(); ++i) {
        TH1F tmp = GetLumiScaledShape(variable, samples.at(i), selection, category, weight);
        result.Add(&tmp);
      }
    }
    return result;
  }


  std::pair<double, double> HTTRun2Analysis::GetRate(std::string const& sample, 
                                      std::string const& selection, 
                                      std::string const& category, 
                                      std::string const& weight) {
    if(verbosity_>1){ std::cout << "--GetRate-- Sample:\"" << sample << "\" Selection:\"" << selection << "\" Category:\"" 
      << category << "\" Weight:\"" << weight << "\"" << std::endl;}
    std::string full_selection = BuildCutString(selection, category, weight);
    TH1::AddDirectory(true);
    //If the tree is empty, return 0
    if(ttrees_[sample]->GetEntries() == 0) return std::make_pair(0,0);
    ttrees_[sample]->Draw("0.5>>htemp(1,0,1)", full_selection.c_str(), "goff");
    TH1::AddDirectory(false);
    TH1F *htemp = (TH1F*)gDirectory->Get("htemp");
    auto result = std::make_pair(Integral(htemp), Error(htemp));
    gDirectory->Delete("htemp;*");
    return result;
  }

  std::pair<double, double> HTTRun2Analysis::GetRate(std::vector<std::string> const& samples, 
                                      std::string const& selection, 
                                      std::string const& category, 
                                      std::string const& weight) {
    auto result = GetRate(samples.at(0),selection,category,weight);
    double err_sqr = result.second*result.second;
    if(samples.size() > 1){
      for(unsigned i = 1; i < samples.size(); ++i){
        auto tmp = GetRate(samples.at(i),selection,category,weight);
        result.first += tmp.first;
        err_sqr += (tmp.second*tmp.second);
      }
    }
    result.second = sqrt(err_sqr);
    return result;
  }


  std::pair<double, double> HTTRun2Analysis::GetLumiScaledRate(std::string const& sample, 
                                      std::string const& selection, 
                                      std::string const& category, 
                                      std::string const& weight) {
    auto result = GetRate(sample, selection, category, weight);
    double sf = GetLumiScale(sample);
    result.first *= sf;
    result.second *= sf;
    return result;
  }
  std::pair<double, double> HTTRun2Analysis::GetLumiScaledRate(std::vector<std::string> const& samples, 
                                      std::string const& selection, 
                                      std::string const& category, 
                                      std::string const& weight) {
    auto result = GetLumiScaledRate(samples.at(0), selection, category, weight);
    double err_sqr = result.second * result.second;
    if (samples.size() > 1) {
      for (unsigned i = 1; i < samples.size(); ++i) {
        auto tmp = GetLumiScaledRate(samples.at(i), selection, category, weight);
        result.first += tmp.first;
        err_sqr += (tmp.second * tmp.second);
      }
    }
    result.second = sqrt(err_sqr);
    return result;
  }

  std::pair<double, double> HTTRun2Analysis::SampleEfficiency(std::string const& sample, 
                          std::string const& ref_selection, 
                          std::string const& ref_category,
                          std::string const& target_selection, 
                          std::string const& target_category,  
                          std::string const& weight) {
    auto num = GetRate(sample, target_selection, target_category, weight);
    auto den = GetRate(sample, ref_selection, ref_category, weight);
    double num_eff = std::pow(num.first / num.second, 2.0) ;
    unsigned num_eff_rounded = unsigned(num_eff+0.5);
    double den_eff = std::pow(den.first / den.second, 2.0) ;
    unsigned den_eff_rounded = unsigned(den_eff+0.5);
    double eff = num.first / den.first;
    TEfficiency teff;
    double eff_err_up   = teff.ClopperPearson(den_eff_rounded,num_eff_rounded,0.683,1)-(num_eff/den_eff);
    double eff_err_down = (num_eff/den_eff)-teff.ClopperPearson(den_eff_rounded,num_eff_rounded,0.683,0);
    double eff_err = (eff_err_up/(num_eff/den_eff)) * eff;
    if (num.first == 0.0) {
      std::cout << "[HTTRun2Analysis::SampleEfficiency] Numerator is zero, setting error to zero" << std::endl;
      eff_err = 0.0;
    }
    auto result = std::make_pair(eff, eff_err);
    if (verbosity_ > 0) {
      std::cout << "[HTTRun2Analysis::SampleEfficiency]" << std::endl;
      std::cout << "Numerator:   " << boost::format("%s,'%s','%s','%s'\n") % sample % target_selection
                % target_category % weight;
      std::cout << "Denominator: " << boost::format("%s,'%s','%s','%s'\n") % sample % ref_selection
                % ref_category % weight;
      PrintValue("Numerator",num);
      PrintValue("Denominator",den);
      std::cout << "Effective Numerator:   " << num_eff_rounded << std::endl;
      std::cout << "Effective Denominator: " << den_eff_rounded << std::endl;
      std::cout << "Error down (relative): " << eff_err_down/(num_eff/den_eff) << std::endl;
      std::cout << "Error up   (relative): " << eff_err_up/(num_eff/den_eff) << std::endl;
    }
    return result;
  }

  HTTRun2Analysis::Value HTTRun2Analysis::SampleRatio(std::string const& sample, 
                          std::string const& ref_selection, 
                          std::string const& ref_category,
                          std::string const& target_selection, 
                          std::string const& target_category,  
                          std::string const& weight) {
    Value num = GetRate(sample, target_selection, target_category, weight);
    Value den = GetRate(sample, ref_selection, ref_category, weight);
    return ValueDivide(num, den);
  }

  HTTRun2Analysis::Value HTTRun2Analysis::GetRateViaRefEfficiency(std::string const& target_sample, 
                          std::string const& ref_sample,
                          std::string const& ref_selection, 
                          std::string const& ref_category,
                          std::string const& target_selection, 
                          std::string const& target_category,  
                          std::string const& weight) {
    if (verbosity_) {
      std::cout << "[HTTRun2Analysis::GetRateViaRefEfficiency]\n";
      std::cout << "ReferenceRate:   " << boost::format("%s,'%s','%s','%s'\n") % ref_sample % ref_selection 
                % ref_category % weight;
      std::cout << "Efficiency:      " << boost::format("%s,'%s','%s','%s'\n") %target_sample % target_selection 
                % target_category % weight;
    }
    auto ref_rate = GetLumiScaledRate(ref_sample, ref_selection, ref_category, weight);
    if (verbosity_) PrintValue("ReferenceRate", ref_rate);
    auto target_eff = SampleEfficiency(target_sample, ref_selection, ref_category, target_selection, target_category, weight);
    if (verbosity_) PrintValue("Efficiency", target_eff);
    return ValueProduct(ref_rate, target_eff);
  }

  HTTRun2Analysis::Value HTTRun2Analysis::GetRateViaFakesMethod(std::string const& sel,
                              std::string const& cat, 
                              std::string const& wt) {
    auto e_fakes = this->GetRate("Special_20_Data", sel, cat, wt);
    auto m_fakes = this->GetRate("Special_21_Data", sel, cat, wt);
    auto em_fakes = this->GetRate("Special_22_Data", sel, cat, wt);
    if (verbosity_) PrintValue("Electron Fakes", e_fakes);
    if (verbosity_) PrintValue("Muon Fakes", m_fakes);
    if (verbosity_) PrintValue("Double Fakes", em_fakes);
    auto norm = ValueAdd(e_fakes, m_fakes);
    norm = ValueSubtract(norm, em_fakes);
    return norm;
  }


  HTTRun2Analysis::Value HTTRun2Analysis::GetRateViaWMethod(std::string const& w_sample,
                          std::string const& ratio_cat,
                          std::string const& ratio_control_sel,
                          std::string const& ratio_signal_sel,
                          std::vector<std::string> const& data_sample,
                          std::string const& cat,
                          std::string const& control_sel,
                          std::vector<std::string> const& sub_samples,
                          std::string const& wt,
                          std::map<std::string, std::function<Value()>> dict
                          ) {
    if (verbosity_) {
      std::cout << "[HTTRun2Analysis::GetRateViaWMethod]\n";
      std::cout << "ExtrapFactor:   " << boost::format("%s,'%s'/'%s','%s','%s'\n") % w_sample % ratio_signal_sel 
                % ratio_control_sel % ratio_cat % wt;
//      std::cout << "Sideband:       " << boost::format("%s,'%s','%s','%s'\n") % data_sample % control_sel % cat % wt;
    }
    Value ratio = SampleRatio(w_sample, ratio_control_sel, ratio_cat, ratio_signal_sel, ratio_cat, wt);
    Value data_control = GetRate(data_sample, control_sel, cat, wt);
 //   if (verbosity_) PrintValue(data_sample, data_control);
    Value total_bkg;
    for (unsigned i = 0; i < sub_samples.size(); ++i) {
      Value bkr;
      if (dict.count(sub_samples[i])) {
        bkr = ((*dict.find(sub_samples[i])).second)(); // find and evaluate function
      } else {
        bkr = GetLumiScaledRate(sub_samples[i], control_sel, cat, wt);
      }
      if (verbosity_) PrintValue("-"+sub_samples[i], bkr);
      double new_err = std::sqrt((total_bkg.second * total_bkg.second) + (bkr.second * bkr.second));
      total_bkg.first += bkr.first;
      total_bkg.second = new_err;
    }
    if (verbosity_) PrintValue("TotalBkg", total_bkg);
    double w_control_err = std::sqrt((total_bkg.second * total_bkg.second) + (data_control.second * data_control.second));
    Value w_control(data_control.first - total_bkg.first, w_control_err);
    if (verbosity_) PrintValue("WSideband", w_control);
    if (verbosity_) PrintValue("ExtrapFactor", ratio);
    Value w_signal = ValueProduct(w_control, ratio);
    return w_signal;
  }

  HTTRun2Analysis::Value HTTRun2Analysis::GetRateViaQCDMethod(HTTRun2Analysis::Value const& ratio,
                          std::vector<std::string> const& data_sample,
                          std::string const& control_selection,
                          std::string const& category,
                          std::vector<std::string> const& sub_samples,
                          std::string const& weight,
                          std::map<std::string, std::function<Value()>> dict
                          ) {
    if (verbosity_) {
      std::cout << "[HTTRun2Analysis::GetRateViaQCDMethod]\n";
//      std::cout << "Sideband:       " << boost::format("%s,'%s','%s','%s'\n") % data_sample % control_selection % category % weight;
    }
    Value data_control = GetRate(data_sample, control_selection, category, weight);
//    if (verbosity_) PrintValue(data_sample, data_control);
    Value total_bkg;
    for (unsigned i = 0; i < sub_samples.size(); ++i) {
      Value bkr;
      if (dict.count(sub_samples[i])) {
        bkr = ((*dict.find(sub_samples[i])).second)(); // find and evaluate function
      } else {
        bkr = GetLumiScaledRate(sub_samples[i], control_selection, category, weight);
      }
      if (verbosity_) PrintValue("-"+sub_samples[i], bkr);
      double new_err = std::sqrt((total_bkg.second * total_bkg.second) + (bkr.second * bkr.second));
      total_bkg.first += bkr.first;
      total_bkg.second = new_err;
    }
    if (verbosity_) PrintValue("TotalBkg", total_bkg);
    double qcd_control_err = std::sqrt((total_bkg.second * total_bkg.second) + (data_control.second * data_control.second));
    Value qcd_control(data_control.first - total_bkg.first, qcd_control_err);
    if (verbosity_) PrintValue("QCDSideband", qcd_control);
    Value qcd_signal = ValueProduct(qcd_control, ratio);
    if (verbosity_) PrintValue("OS/SS Factor", ratio);
    return qcd_signal;
  }

  TH1F HTTRun2Analysis::GetShapeViaQCDMethod(std::string const& variable,
                          std::vector<std::string> const& data_sample,
                          std::string const& selection,
                          std::string const& category,
                          std::vector<std::string> const& sub_samples,
                          std::string const& weight,
                          std::map<std::string, std::function<Value()>> dict
                          ) {
    if (verbosity_) {
      std::cout << "[HTTRun2Analysis::GetShapeViaQCDMethod]\n";
//      std::cout << "Sideband:       " << boost::format("%s,'%s','%s','%s'\n") % data_sample % selection % category % weight;

    }
    TH1F result = GetLumiScaledShape(variable, data_sample, selection, category, weight);
    for (unsigned i = 0; i < sub_samples.size(); ++i) {
      if (dict.count(sub_samples[i])) {
        Value bkr_rate = ((*dict.find(sub_samples[i])).second)(); // find and evaluate function
        TH1F tmp = GetShape(variable, sub_samples.at(i), selection, category, weight);
        SetNorm(&tmp, bkr_rate.first);
        result.Add(&tmp, -1.);
      } else {
      TH1F tmp = GetLumiScaledShape(variable, sub_samples[i], selection, category, weight);
      result.Add(&tmp, -1.);
     }
    }
    return result;
  }


  TH1F HTTRun2Analysis::GetShapeViaFakesMethod(std::string const& var,
                              std::string const& sel,
                              std::string const& cat,
                              std::string const& wt) {
    TH1F fr_e_hist =  this->GetShape(var, "Special_20_Data", sel, cat, wt);
    TH1F fr_m_hist =  this->GetShape(var, "Special_21_Data", sel, cat, wt);
    TH1F fr_em_hist = this->GetShape(var, "Special_22_Data", sel, cat, wt);
    fr_e_hist.Add(&fr_m_hist,1.0);
    fr_e_hist.Add(&fr_em_hist,-1.0);
    return fr_e_hist;
  }

  HTTRun2Analysis::Value HTTRun2Analysis::ValueProduct(Value const& p1, Value const& p2) {
    if (p1.first == 0.0 || p2.first == 0.0) {
      //if (verbosity_ > 0) std::cout << "[HTTRun2Analysis::ValueProduct] At least one value is zero, returning 0.0 +/- 0.0" << std::endl;
      return std::make_pair(0.0, 0.0);
    }
    double f = p1.first * p2.first;
    double a_sqrd = std::pow(p1.second / p1.first, 2.0);
    double b_sqrd = std::pow(p2.second / p2.first, 2.0);
    double f_err = f * sqrt( a_sqrd + b_sqrd );
    return std::make_pair(f, f_err);
  }
  HTTRun2Analysis::Value HTTRun2Analysis::ValueDivide(Value const& p1, Value const& p2) {
    if (p1.first == 0.0 && p2.first == 0.0) {
      std::cout << "[HTTRun2Analysis::ValueDivide] Numerator and denominator both zero, returning 0.0 +/- 0.0" << std::endl;
      return std::make_pair(0.0, 0.0);
    }
    if (p1.first == 0.0) {
      //if (verbosity_ > 0) std::cout << "[HTTRun2Analysis::ValueDivide] Numerator is zero, returning 0.0 +/- 0.0" << std::endl;
      return std::make_pair(0.0, 0.0);
    }
    if (p2.first == 0.0) {
      std::cout << "[HTTRun2Analysis::ValueDivide] Denominator is zero, returning 0.0 +/- 0.0" << std::endl;
      return std::make_pair(0.0, 0.0);
    }
    double f = p1.first / p2.first;
    double a_sqrd = std::pow(p1.second / p1.first, 2.0);
    double b_sqrd = std::pow(p2.second / p2.first, 2.0);
    double f_err = f * sqrt( a_sqrd + b_sqrd );
    return std::make_pair(f, f_err);
  }
  HTTRun2Analysis::Value HTTRun2Analysis::ValueAdd(Value const& p1, Value const& p2) {
    double f = p1.first + p2.first;
    double f_err = sqrt( p1.second*p1.second + p2.second*p2.second );
    return std::make_pair(f, f_err);
  }
  HTTRun2Analysis::Value HTTRun2Analysis::ValueSubtract(Value const& p1, Value const& p2) {
    double f = p1.first - p2.first;
    double f_err = sqrt( p1.second*p1.second + p2.second*p2.second );
    return std::make_pair(f, f_err);
  }

  void HTTRun2Analysis::PrintValue(std::string const& label, Value const& val) {
    std::cout << (boost::format("%-45s %10.3f +/-   %10.3f  (%.4f)") % (label+":") % val.first % val.second % (val.second/val.first)) << std::endl;
  }

  std::string HTTRun2Analysis::ResolveAlias(std::string const& al) {
    if (alias_map_.count(al)) {
      return alias_map_.find(al)->second;
    } else {
      std::cerr << "Warning in <HTTRun2Analysis::ResolveAlias>: No alias for " << al << " found" << std::endl;
      return al;
    }
  }

  std::vector<std::string> HTTRun2Analysis::ResolveSamplesAlias(std::string const& al) {
    if (samples_alias_map_.count(al)) {
      return samples_alias_map_.find(al)->second;
    } else {
      std::cerr << "Warning in <HTTRun2Analysis::ResolveSamplesAlias>: No alias for " << al << " found" << std::endl;
      return std::vector<std::string>();
    }
  }

  void HTTRun2Analysis::SetAlias(std::string const& al, std::string const& val) {
    alias_map_[al] = val;
  }

  double HTTRun2Analysis::KolmogorovTest(std::string const& variable,
      std::string const& sample1, std::string const& selection1, std::string const& category1,
      std::string const& sample2, std::string const& selection2, std::string const& category2,
      std::string const& weight) {
    std::cout << "[HTTRun2Analysis::KolmogorovTest] Calculating statistic for shapes:" << std::endl;
    std::cout << "[1] " << boost::format("%s,'%s','%s','%s'\n") % sample1 % selection1 % category1 % weight;
    std::cout << "[2] " << boost::format("%s,'%s','%s','%s'\n") % sample2 % selection2 % category2 % weight;
    std::string full1 = BuildCutString(selection1, category1, weight);
    std::string full2 = BuildCutString(selection2, category2, weight);
    TH1::AddDirectory(true);
    ttrees_[sample1]->Draw(">>elist", full1.c_str(), "entrylist");
    TEntryList *elist1 = (TEntryList*)gDirectory->Get("elist");
    TH1::AddDirectory(false);
    unsigned entries1 = elist1->GetN();
    double x1;
    double wt1;
    std::vector<std::pair<double,double>> a(entries1,std::make_pair(0.,0.));
    ttrees_[sample1]->SetBranchAddress(variable.c_str(), &x1);
    ttrees_[sample1]->SetBranchAddress(weight.c_str(), &wt1);
    for (unsigned i = 0; i < entries1; ++i) {
      unsigned entry = elist1->GetEntry(i);
      ttrees_[sample1]->GetEntry(entry);
      a[i].first = x1;
      a[i].second = wt1;
    }
    gDirectory->Delete("elist;*");
    TH1::AddDirectory(true);
    ttrees_[sample2]->Draw(">>elist", full2.c_str(), "entrylist");
    TEntryList *elist2 = (TEntryList*)gDirectory->Get("elist");
    TH1::AddDirectory(false);
    unsigned entries2 = elist2->GetN();
    double x2;
    double wt2;
    std::vector<std::pair<double,double>> b(entries2,std::make_pair(0.,0.));
    ttrees_[sample2]->SetBranchAddress(variable.c_str(), &x2);
    ttrees_[sample2]->SetBranchAddress(weight.c_str(), &wt2);
    for (unsigned i = 0; i < entries2; ++i) {
      unsigned entry = elist2->GetEntry(i);
      ttrees_[sample2]->GetEntry(entry);
      b[i].first = x2;
      b[i].second = wt2;
    }
    gDirectory->Delete("elist;*");
    std::sort(a.begin(), a.end(), [](const std::pair<double, double>& first, const std::pair<double, double>& second)
      {
        return first.first < second.first;
      });
    std::sort(b.begin(), b.end(), [](const std::pair<double, double>& first, const std::pair<double, double>& second)
      {
        return first.first < second.first;
      });
    double prob = -1;
    int na = a.size();
    int nb = b.size();
    double rna = 0.;
    double rnb = 0.;
    for (unsigned i = 0; i < a.size(); ++i) rna += a[i].second;
    for (unsigned i = 0; i < b.size(); ++i) rnb += b[i].second;

    /* 
      The implementation below shamelessly stolen from:
      http://root.cern.ch/root/html/TMath.html#TMath:KolmogorovTest
      and modified to take into account weighted events.  Quite possible
      that this is not statistically valid, but seems to behave well enough
      when weights are close to unity.
    */
    // double rna = a.size();
    // double rnb = b.size();
    std::cout << "Entries: (1) " << rna << " (2) " << rnb << std::endl;
    // double sa  = 1./rna;
    // double sb  = 1./rnb;
    double rdiff = 0;
    double rdmax = 0;
    int ia = 0;
    int ib = 0;
    bool ok = false;
    for (int i=0;i<na+nb;i++) {
      if (a[ia].first < b[ib].first) {
         rdiff -= a[ia].second/rna;
         ia++;
         if (ia >= na) {ok = true; break;}
      } else if (a[ia].first > b[ib].first) {
         rdiff += b[ib].second/rnb;
         ib++;
         if (ib >= nb) {ok = true; break;}
      } else {
         // special cases for the ties
         double x = a[ia].first;
         while(a[ia].first == x && ia < na) {
            rdiff -= a[ia].second/rna;
            ia++;
         }
         while(b[ib].first == x && ib < nb) {
            rdiff += b[ib].second/rnb;
            ib++;
         }
         if (ia >= na) {ok = true; break;}
         if (ib >= nb) {ok = true; break;}
      }
      rdmax = TMath::Max(rdmax,TMath::Abs(rdiff));
    }
    if (ok) {
      rdmax = TMath::Max(rdmax,TMath::Abs(rdiff));
      Double_t z = rdmax * TMath::Sqrt(rna*rnb/(rna+rnb));
      prob = TMath::KolmogorovProb(z);
    }
    std::cout << " Kolmogorov Probability = " << prob << ", rmax=" << rdmax << std::endl;
    return prob;
  }



}


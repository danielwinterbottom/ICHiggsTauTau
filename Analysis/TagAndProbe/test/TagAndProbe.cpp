#include <iostream>
#include <vector>
#include <string>
#include "boost/bind.hpp"
#include "boost/program_options.hpp"
#include "TSystem.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/AnalysisBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/TagAndProbe/interface/ElectronTagAndProbe.h"
#include "UserCode/ICHiggsTauTau/Analysis/TagAndProbe/interface/MuonTagAndProbe.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/PileupWeight.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/SimpleFilter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/SimpleCounter.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/LumiMask.h"

namespace po = boost::program_options;

int main(int argc, char* argv[]){
    using namespace ic;

    bool isdata;
    bool iselec;
    bool do_skim;
    bool split_pm_eta;
    bool second_trigger;
    bool idiso_only;
    bool trg_only;
    bool VH;
    bool ltmet;
    bool IsoMu24;
    std::string era, eraB;
    std::string configfile, outname,outnametrg,outnametrgB, filelist, outfolder;
    std::string skim_path="";
    std::string input_prefix;
    int max_events;
    int run_low, run_low_B;
    int run_high, run_high_B;
    std::vector<int> pt_bins;
    std::vector<double> eta_bins;
    std::vector<int> pt_bins_fine;
    std::vector<double> eta_bins_fine;
    std::vector<int> vtx_bins_fine;
    std::vector<int> pt_bins_trg;
    std::vector<int> pt_bins_trg_B;
   

    po::options_description preconfig("Pre-Configuration");
    preconfig.add_options()
      ("cfg", po::value<std::string>(&configfile)->required(), 
            "configuration file");
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(preconfig).allow_unregistered().run(), vm);
    po::notify(vm);
    po::options_description config("Configuration");
    config.add_options()
      ("max_events", po::value<int>(&max_events)->default_value(-1), "maximum number of events to process")
      ("run_low", po::value<int>(&run_low)->default_value(0), "lower bound of runs to use")
      ("run_low_B", po::value<int>(&run_low_B)->default_value(0), "lower bound of runs to use for second trigger")
      ("run_high", po::value<int>(&run_high)->default_value(10000000), "upper bound of runs to use")
      ("run_high_B", po::value<int>(&run_high_B)->default_value(10000000), "upper bound of runs to use for second trigger")
      ("pt_bins", po::value<std::vector<int> >(&pt_bins)->multitoken(), "ptbins") 
      ("eta_bins", po::value<std::vector<double> >(&eta_bins)->multitoken(), "etabins") 
      ("pt_bins_fine", po::value<std::vector<int> >(&pt_bins_fine)->multitoken(), "ptbins for fine binning") 
      ("eta_bins_fine", po::value<std::vector<double> >(&eta_bins_fine)->multitoken(), "etabins for fine binning") 
      ("vtx_bins_fine", po::value<std::vector<int> >(&vtx_bins_fine)->multitoken(), "vtxbins for fine binning") 
      ("pt_bins_trg", po::value<std::vector<int> >(&pt_bins_trg)->multitoken(), "ptbins for trigger turn on") 
      ("pt_bins_trg_B", po::value<std::vector<int> >(&pt_bins_trg_B)->multitoken(), "ptbins for second trigger turn on") 
      ("era", po::value<std::string>(&era)->default_value("2012"), "era")
      ("eraB", po::value<std::string>(&eraB)->default_value(era), "era for second trigger")
      ("is_data", po::value<bool>(&isdata)->required(), "0=mc, 1=data")
      ("split_pm_eta", po::value<bool>(&split_pm_eta)->default_value(false), "true separates positive and negative eta in the trigger")
      ("do_skim", po::value<bool>(&do_skim)->default_value(false), "true runs in skim mode")
      ("skim_path", po::value<std::string>(&skim_path), "output folder for skims")
      ("idiso_only", po::value<bool>(&idiso_only)->default_value(false), "to rerun just id and iso")
      ("VH", po::value<bool>(&VH)->default_value(false), "to just run the numbers for VH")
      ("ltmet", po::value<bool>(&ltmet)->default_value(false), "to run for lepton+tau+MET channels")
      ("IsoMu24", po::value<bool>(&IsoMu24)->default_value(false), "to run for IsoMu24 efficiency measurement")
      ("trg_only", po::value<bool>(&trg_only)->default_value(false), "to rerun just trigger")
      ("second_trigger", po::value<bool>(&second_trigger)->default_value(false), "allows a measurement of a second trigger if there are two in era")
      ("is_elec", po::value<bool>(&iselec)->required(), "0=muons, 1=electrons")
      ("filelist", po::value<std::string>(&filelist)->required(), "input filelist")
      ("input_prefix", po::value<std::string>(&input_prefix)->default_value(""))
      ("outfolder", po::value<std::string>(&outfolder)->default_value(""), "output folder");
    po::store(po::command_line_parser(argc, argv).
            options(config).allow_unregistered().run(), vm);
    po::store(po::parse_config_file<char>(configfile.c_str(), config), vm);
    po::notify(vm);
    
    //note:the outfolder must exist otherwise the code will seg fault.
    ofstream ptbins, etabins, ptbinsfine, etabinsfine, vtxbinsfine, ptbinstrg, ptbinstrgB;


    //Open a file for all the id/iso histograms, a file for the trigger histograms, and files
    //containing the binning used for each case.
    std::string data;
    if(isdata) data="data";
    else data="MC";
    if(iselec)
    {
        outname="eeTandPIdIso"+data+".root";
        outnametrg="eeTandPtrg"+data+".root";
        outnametrgB="eeTandPtrgB"+data+".root";
        if(!idiso_only)
        {
            ptbinstrg.open((outfolder+"/electron_triggerptbins.txt").c_str());
            ptbinstrgB.open((outfolder+"/electron_triggerBptbins.txt").c_str());
        }
        if(!trg_only)
        {
            ptbins.open((outfolder+"/electron_ptbins.txt").c_str());
            ptbinsfine.open((outfolder+"/electron_ptbinsfine.txt").c_str());
            etabinsfine.open((outfolder+"/electron_etabinsfine.txt").c_str());
            vtxbinsfine.open((outfolder+"/electron_vtxbinsfine.txt").c_str());
        }
    }
    else
    {
        outname="mumuTandPIdIso"+data+".root";
        outnametrg="mumuTandPtrg"+data+".root";
        outnametrgB="mumuTandPtrgB"+data+".root";
        if(!idiso_only)
        {
            ptbinstrg.open((outfolder+"/muon_triggerptbins.txt").c_str());
            ptbinstrgB.open((outfolder+"/muon_triggerBptbins.txt").c_str());
        }
        if(!trg_only)
        {
            ptbins.open((outfolder+"/muon_ptbins.txt").c_str());
            ptbinsfine.open((outfolder+"/muon_ptbinsfine.txt").c_str());
            etabinsfine.open((outfolder+"/muon_etabinsfine.txt").c_str());
            vtxbinsfine.open((outfolder+"/muon_vtxbinsfine.txt").c_str());
        }
    }
    if(!trg_only)
    {
        for(unsigned i=0; i<pt_bins.size(); i++)
        {
            ptbins << pt_bins[i] <<std::endl;
        }
        for(unsigned i=0; i<pt_bins_fine.size(); i++)
        {
            ptbinsfine << pt_bins_fine[i] <<std::endl;
        }
        for(unsigned i=0; i<eta_bins.size(); i++)
        {
            etabins << eta_bins[i] <<std::endl;
        }
        for(unsigned i=0; i<eta_bins_fine.size(); i++)
        {
            etabinsfine << eta_bins_fine[i] <<std::endl;
        }
        for(unsigned i=0; i<vtx_bins_fine.size(); i++)
        {
            vtxbinsfine << vtx_bins_fine[i] <<std::endl;
        }
    }
    if(!idiso_only)
    {
        for(unsigned i=0; i<pt_bins_trg.size(); i++)
        {
            ptbinstrg << pt_bins_trg[i] <<std::endl;
        }
        if(second_trigger)
        {
            for(unsigned i=0; i<pt_bins_trg_B.size(); i++)
            {
                ptbinstrgB << pt_bins_trg_B[i] <<std::endl;
            }
        }
    }
    if(!trg_only)
    {
        ptbins.close();
        etabins.close();
        ptbinsfine.close();
        etabinsfine.close();
        vtxbinsfine.close();
    }
    if(!idiso_only)
    {
        ptbinstrg.close();
        ptbinstrgB.close();
    }
    
    
    // Load necessary libraries for ROOT I/O of custom classes
    gSystem->Load("libFWCoreFWLite.dylib");
    gSystem->Load("libUserCodeICHiggsTauTau.dylib");
    AutoLibraryLoader::enable();
    // Build a vector of input files
    std::vector<std::string> files = ParseFileLines(filelist);
    for (unsigned i = 0; i < files.size(); ++i) files[i] = input_prefix + files[i];
    
    // Create ROOT output fileservice
    fwlite::TFileService *fsid=NULL, *fstrg=NULL, *fstrg2=NULL;
    if(!trg_only && !do_skim)
    {
        if (outfolder == "") {
        fsid = new fwlite::TFileService(outname.c_str());
        } else {
        fsid = new fwlite::TFileService((outfolder+"/"+outname).c_str());
        }
    }
    if(!idiso_only && !do_skim)
    {
        if (outfolder == "") {
        fstrg = new fwlite::TFileService(outname.c_str());
        } else {
        fstrg = new fwlite::TFileService((outfolder+"/"+outnametrg).c_str());
        }
       
        if (outfolder == "") {
        fstrg2 = new fwlite::TFileService(outname.c_str());
        } else {
        fstrg2 = new fwlite::TFileService((outfolder+"/"+outnametrgB).c_str());
        }
    }
  
  std::string data_pu_path = "data/pileup/Data_Pileup_2011_HCP-500bins.root";
  std::string mc_pu_path = "data/pileup/MC_Fall11_PU_S6-500bins.root";
  if (era == "2012A" || era=="2012B" || era=="2012")
  {
      data_pu_path = "data/pileup/Data_Pileup_2012_ReReco-600bins.root";
      mc_pu_path = "data/pileup/MC_Summer12_PU_S10-600bins.root";
  }
  TH1D data_pu  = GetFromTFile<TH1D>(data_pu_path, "/", "pileup");
  TH1D mc_pu    = GetFromTFile<TH1D>(mc_pu_path, "/", "pileup");
 
  std::string data_json;
  if (era == "2012A" || era == "2012B" || era == "2012C" || era == "2012D") data_json = "data/json/data_2012_moriond.txt" ;
  else data_json           =  "data/json/json_data_2011.txt";

//==============================================================================================
  // Create analysis object
    ic::AnalysisBase analysis(
        "TagAndProbe",        // Analysis name
        files,                // Input files
        "icEventProducer/EventTree", // TTree name
        max_events);          // Max. events to process (-1 = all)
  if (do_skim && skim_path != "") analysis.DoSkimming(skim_path);

 //----------------------Generic Modules------------------------------------------  
    LumiMask lumiMask = LumiMask("LumiMask")
      .set_produce_output_jsons("")
      .set_input_file(data_json);

    PileupWeight pileupWeight = PileupWeight
      ("PileupWeight").set_data(&data_pu).set_mc(&mc_pu).set_print_weights(false);

//==============================================================================================
//==========================================ELECTRONS=============================================
//==============================================================================================


//----------------------Skimming Modules------------------------------------------  
   SimpleCounter<GenParticle> zEEFilter = SimpleCounter<GenParticle>("ZToEESelector")
     .set_input_label("genParticles")
     .set_predicate(
       (bind(&GenParticle::status, _1) == 3) && 
       (bind(abs,(bind(&GenParticle::pdgid, _1))) == 11))
     .set_min(2);


    SimpleFilter<Electron> electronTagFilter = SimpleFilter<Electron>
    ("electronTagFilter")
    .set_input_label("electrons")
    .set_predicate(boost::bind(ElectronHTTIdIso, _1, 1)
                   && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                   && (boost::bind(MinPtMaxEta, _1, 20, 2.1)))
    .set_min(1);
    
    
    SimpleCounter<Electron> electronCounter = SimpleCounter<Electron>
    ("electronCounter")
    .set_predicate( (boost::bind(DummyFunction<Electron>, _1)) )
    .set_input_label("electrons")
    .set_min(2);
 
//==============================================================================================
 //----------------------Id/Iso for ETau------------------------------------------  

    ElectronTagAndProbe electronIDTagAndProbe("electronIDTagAndProbe");
    electronIDTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/electron_id_count").c_str())
    .set_data(isdata)
    .set_tag_predicate(boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate( (boost::bind(MinPtMaxEta, _1, 10, 2.1)))
    .set_passprobe_predicate((boost::bind(ElectronHTTIdIso, _1, 1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045))
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(0)
    .set_era(era);

    ElectronTagAndProbe electronIsoTagAndProbe("electronIsoTagAndProbe");
    electronIsoTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/electron_iso_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1) 
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(  boost::bind(ElectronHTTIdIso, _1, 1) 
                        && (boost::bind(MinPtMaxEta, _1, 10, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045) )
    .set_passprobe_predicate(boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(1)
    .set_era(era);
    
    ElectronTagAndProbe electronIDIsoTagAndProbe("electronIDIsoTagAndProbe");
    electronIDIsoTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/electron_idiso_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 10, 2.1)) 
    .set_passprobe_predicate((boost::bind(ElectronHTTIdIso, _1, 1)) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1) )
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(2)
    .set_era(era);
   
//==============================================================================================
 //----------------------Id/Iso for ETau+MET------------------------------------------  
    ElectronTagAndProbe electronltmetIDTagAndProbe("electronltmetIDTagAndProbe");
    electronltmetIDTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/electron_id_count").c_str())
    .set_data(isdata)
    .set_tag_predicate(boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate( (boost::bind(MinPtMaxEta, _1, 10, 2.1)))
    .set_passprobe_predicate((boost::bind(ElectronHTTIdIso, _1, 1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045))
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(0)
    .set_era(era);

    ElectronTagAndProbe electronltmetIsoTagAndProbe("electronltmetIsoTagAndProbe");
    electronltmetIsoTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/electron_iso_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1) 
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(  boost::bind(ElectronHTTIdIso, _1, 1) 
                        && (boost::bind(MinPtMaxEta, _1, 10, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045) )
    .set_passprobe_predicate(boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(1)
    .set_era(era);
    
    ElectronTagAndProbe electronltmetIDIsoTagAndProbe("electronltmetIDIsoTagAndProbe");
    electronltmetIDIsoTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/electron_idiso_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 10, 2.1)) 
    .set_passprobe_predicate((boost::bind(ElectronHTTIdIso, _1, 1)) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1) )
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(2)
    .set_era(era);
    

//==============================================================================================
 //----------------------Id/Iso for VH electrons------------------------------------------  
    ElectronTagAndProbe electronVHIDTagAndProbe("electronVHIDTagAndProbe");
    electronVHIDTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/electron_id_count").c_str())
    .set_data(isdata)
    .set_tag_predicate(boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate( (boost::bind(MinPtMaxEta, _1, 10, 2.1)))
    .set_passprobe_predicate((boost::bind(ElectronHTTVHID, _1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045))
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(0)
    .set_era(era);

    ElectronTagAndProbe electronVHIsoTagAndProbe("electronVHIsoTagAndProbe");
    electronVHIsoTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/electron_iso_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1) 
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(  boost::bind(ElectronHTTVHID, _1) 
                        && (boost::bind(MinPtMaxEta, _1, 10, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045) )
    .set_passprobe_predicate(boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.3)
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(1)
    .set_era(era);
    
    ElectronTagAndProbe electronVHIDIsoTagAndProbe("electronVHIDIsoTagAndProbe");
    electronVHIDIsoTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/electron_idiso_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 10, 2.1)) 
    .set_passprobe_predicate((boost::bind(ElectronHTTVHID, _1)) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.3) )
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(2)
    .set_era(era);
     
//==============================================================================================
 //----------------------Fine Id/Iso for ETau------------------------------------------  
    ElectronTagAndProbe electronIDFineTagAndProbe("electronIDFineTagAndProbe");
    electronIDFineTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/electron_idbinned_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 10, 2.1))
    .set_passprobe_predicate( (boost::bind(ElectronHTTIdIso, _1, 1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045))
    .set_pt_bins(pt_bins_fine)
    .set_eta_bins(eta_bins_fine)
    .set_vtx_bins(vtx_bins_fine)
    .set_mode(10)
    .set_era(era);

    ElectronTagAndProbe electronIsoFineTagAndProbe("electronIsoFineTagAndProbe");
    electronIsoFineTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/electron_isobinned_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(  boost::bind(ElectronHTTIdIso, _1, 1) 
                        && boost::bind(MinPtMaxEta, _1, 10, 2.1)
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045) )
    .set_passprobe_predicate((boost::bind(ElectronHTTIdIso, _1, 1)) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1) )
    .set_pt_bins(pt_bins_fine)
    .set_eta_bins(eta_bins_fine)
    .set_vtx_bins(vtx_bins_fine)
    .set_mode(11)
    .set_era(era);
    
    ElectronTagAndProbe electronIDIsoFineTagAndProbe("electronIDIsoFineTagAndProbe");
    electronIDIsoFineTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/electron_idisobinned_count").c_str())
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 10, 2.1))
    .set_passprobe_predicate((boost::bind(ElectronHTTIdIso, _1, 1)) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1))
    .set_data(isdata)
    .set_pt_bins(pt_bins_fine)
    .set_eta_bins(eta_bins_fine)
    .set_vtx_bins(vtx_bins_fine)
    .set_mode(12)
    .set_era(era);

//==============================================================================================
 //----------------------Fine Id/Iso for ETau+MET------------------------------------------  
    ElectronTagAndProbe electronltmetIDFineTagAndProbe("electronltmetIDFineTagAndProbe");
    electronltmetIDFineTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/electron_idbinned_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 10, 2.1))
    .set_passprobe_predicate( (boost::bind(ElectronHTTIdIso, _1, 1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045))
    .set_pt_bins(pt_bins_fine)
    .set_eta_bins(eta_bins_fine)
    .set_vtx_bins(vtx_bins_fine)
    .set_mode(10)
    .set_era(era);

    ElectronTagAndProbe electronltmetIsoFineTagAndProbe("electronltmetIsoFineTagAndProbe");
    electronltmetIsoFineTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/electron_isobinned_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(  boost::bind(ElectronHTTIdIso, _1, 1) 
                        && boost::bind(MinPtMaxEta, _1, 10, 2.1)
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045) )
    .set_passprobe_predicate((boost::bind(ElectronHTTIdIso, _1, 1)) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1) )
    .set_pt_bins(pt_bins_fine)
    .set_eta_bins(eta_bins_fine)
    .set_vtx_bins(vtx_bins_fine)
    .set_mode(11)
    .set_era(era);
    
    ElectronTagAndProbe electronltmetIDIsoFineTagAndProbe("electronltmetIDIsoFineTagAndProbe");
    electronltmetIDIsoFineTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/electron_idisobinned_count").c_str())
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 10, 2.1))
    .set_passprobe_predicate((boost::bind(ElectronHTTIdIso, _1, 1)) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1))
    .set_data(isdata)
    .set_pt_bins(pt_bins_fine)
    .set_eta_bins(eta_bins_fine)
    .set_vtx_bins(vtx_bins_fine)
    .set_mode(12)
    .set_era(era);

//==============================================================================================
 //----------------------Trigger for ETau------------------------------------------  
    
    ElectronTagAndProbe electronTrgATagAndProbe("electronTrgATagAndProbe");
    electronTrgATagAndProbe
    .set_fs(fstrg)
    .set_output_name((outfolder+"/electron_trg_count").c_str())
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 10, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045))
    .set_data(isdata)
    .set_pt_bins(pt_bins_trg)
    .set_run_low(run_low)
    .set_run_high(run_high)
    .set_eta_bins(eta_bins)
    .set_mode(3)
    .set_era(era);
    
    ElectronTagAndProbe electronTrgBTagAndProbe("electronTrgBTagAndProbe");
    electronTrgBTagAndProbe
    .set_fs(fstrg2)
    .set_output_name((outfolder+"/electron_trgB_count").c_str())
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 10, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045))
    .set_data(isdata)
    .set_pt_bins(pt_bins_trg_B)
    .set_run_low(run_low_B)
    .set_run_high(run_high_B)
    .set_eta_bins(eta_bins)
    .set_mode(3)
    .set_era(eraB);
 
//==============================================================================================
 //----------------------Trigger for ETau+MET------------------------------------------  
    ElectronTagAndProbe electronltmetTrgTagAndProbe("electronltmetTrgTagAndProbe");
    electronltmetTrgTagAndProbe
    .set_fs(fstrg)
    .set_output_name((outfolder+"/electron_trg_count").c_str())
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 5, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045))
    .set_data(isdata)
    .set_pt_bins(pt_bins_trg)
    .set_run_low(run_low)
    .set_run_high(run_high)
    .set_eta_bins(eta_bins)
    .set_mode(4)
    .set_era(era);

//==============================================================================================
//==========================================MUONS=============================================
//==============================================================================================

 //----------------------Skimming Modules------------------------------------------  

   SimpleCounter<GenParticle> zMuMuFilter = SimpleCounter<GenParticle>("ZToMuMuSelector")
     .set_input_label("genParticles")
     .set_predicate(
       (bind(&GenParticle::status, _1) == 3) && 
       (bind(abs,(bind(&GenParticle::pdgid, _1))) == 13))
     .set_min(2);

    SimpleFilter<Muon> muonTagFilter = SimpleFilter<Muon>
    ("muonTagFilter")
    .set_predicate(boost::bind(MuonTight, _1)
                   && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                   && (boost::bind(MinPtMaxEta, _1, 20, 2.1)) )
    .set_input_label("muonsPFlow")
    .set_min(1);
    
    SimpleCounter<Muon> muonCounter = SimpleCounter<Muon>
    ("muonCounter")
    .set_predicate( (boost::bind(DummyFunction<Muon>, _1)) )
    .set_input_label("muonsPFlow")
    .set_min(2);

//==============================================================================================
//----------------------Id/Iso for MuTau------------------------------------------  
     
    MuonTagAndProbe muonIDTagAndProbe("muonIDTagAndProbe");
    muonIDTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/muon_id_count").c_str())
    .set_data(isdata)
    .set_tag_predicate(boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate( (boost::bind(MinPtMaxEta, _1, 10, 2.1)))
    .set_passprobe_predicate((boost::bind(MuonTight, _1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045))
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(0)
    .set_era(era);

    MuonTagAndProbe muonIsoTagAndProbe("muonIsoTagAndProbe");
    muonIsoTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/muon_iso_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1) 
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(  boost::bind(MuonTight, _1) 
                        && (boost::bind(MinPtMaxEta, _1, 10, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045) )
    .set_passprobe_predicate(boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(1)
    .set_era(era);
    
    MuonTagAndProbe muonIDIsoTagAndProbe("muonIDIsoTagAndProbe");
    muonIDIsoTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/muon_idiso_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 10, 2.1)) 
    .set_passprobe_predicate((boost::bind(MuonTight, _1)) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1) )
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(2)
    .set_era(era);
    
//==============================================================================================
//----------------------Id/Iso for VH muons------------------------------------------  
    MuonTagAndProbe muonVHIDTagAndProbe("muonVHIDTagAndProbe");
    muonVHIDTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/muon_id_count").c_str())
    .set_data(isdata)
    .set_tag_predicate(boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate( (boost::bind(MinPtMaxEta, _1, 10, 2.1)))
    .set_passprobe_predicate((boost::bind(MuonTight, _1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045))
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(0)
    .set_era(era);

    MuonTagAndProbe muonVHIsoTagAndProbe("muonVHIsoTagAndProbe");
    muonVHIsoTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/muon_iso_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1) 
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(  boost::bind(MuonTight, _1) 
                        && (boost::bind(MinPtMaxEta, _1, 10, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045) )
    .set_passprobe_predicate(boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.3)
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(1)
    .set_era(era);
    
    MuonTagAndProbe muonVHIDIsoTagAndProbe("muonVHIDIsoTagAndProbe");
    muonVHIDIsoTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/muon_idiso_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 10, 2.1)) 
    .set_passprobe_predicate((boost::bind(MuonTight, _1)) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.3) )
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(2)
    .set_era(era);
    
//==============================================================================================
//----------------------Id/Iso for MuTau+MET------------------------------------------  
 
    MuonTagAndProbe muonltmetIDTagAndProbe("muonltmetIDTagAndProbe");
    muonltmetIDTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/muon_id_count").c_str())
    .set_data(isdata)
    .set_tag_predicate(boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate( (boost::bind(MinPtMaxEta, _1, 10, 2.1)))
    .set_passprobe_predicate((boost::bind(MuonTight, _1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045))
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(0)
    .set_era(era);

    MuonTagAndProbe muonltmetIsoTagAndProbe("muonltmetIsoTagAndProbe");
    muonltmetIsoTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/muon_iso_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1) 
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(  boost::bind(MuonTight, _1) 
                        && (boost::bind(MinPtMaxEta, _1, 10, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045) )
    .set_passprobe_predicate(boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(1)
    .set_era(era);
    
    MuonTagAndProbe muonltmetIDIsoTagAndProbe("muonltmetIDIsoTagAndProbe");
    muonltmetIDIsoTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/muon_idiso_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 10, 2.1)) 
    .set_passprobe_predicate((boost::bind(MuonTight, _1)) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1) )
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(2)
    .set_era(era);

//==============================================================================================
 //----------------------Fine Id/Iso for MuTau------------------------------------------  

    MuonTagAndProbe muonIDFineTagAndProbe("muonIDFineTagAndProbe");
    muonIDFineTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/muon_idbinned_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 10, 2.1))
    .set_passprobe_predicate( (boost::bind(MuonTight, _1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045))
    .set_pt_bins(pt_bins_fine)
    .set_eta_bins(eta_bins_fine)
    .set_vtx_bins(vtx_bins_fine)
    .set_mode(10)
    .set_era(era);

    MuonTagAndProbe muonIsoFineTagAndProbe("muonIsoFineTagAndProbe");
    muonIsoFineTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/muon_isobinned_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(  boost::bind(MuonTight, _1) 
                        && boost::bind(MinPtMaxEta, _1, 10, 2.1)
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045) )
    .set_passprobe_predicate((boost::bind(MuonTight, _1)) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1) )
    .set_pt_bins(pt_bins_fine)
    .set_eta_bins(eta_bins_fine)
    .set_vtx_bins(vtx_bins_fine)
    .set_mode(11)
    .set_era(era);
    
    MuonTagAndProbe muonIDIsoFineTagAndProbe("muonIDIsoFineTagAndProbe");
    muonIDIsoFineTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/muon_idisobinned_count").c_str())
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 10, 2.1))
    .set_passprobe_predicate((boost::bind(MuonTight, _1)) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1))
    .set_data(isdata)
    .set_pt_bins(pt_bins_fine)
    .set_eta_bins(eta_bins_fine)
    .set_vtx_bins(vtx_bins_fine)
    .set_mode(12)
    .set_era(era);
 
//==============================================================================================
 //----------------------Fine Id/Iso for MuTau+MET------------------------------------------  
    
    MuonTagAndProbe muonltmetIDFineTagAndProbe("muonltmetIDFineTagAndProbe");
    muonltmetIDFineTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/muon_idbinned_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 10, 2.1))
    .set_passprobe_predicate( (boost::bind(MuonTight, _1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045))
    .set_pt_bins(pt_bins_fine)
    .set_eta_bins(eta_bins_fine)
    .set_vtx_bins(vtx_bins_fine)
    .set_mode(10)
    .set_era(era);

    MuonTagAndProbe muonltmetIsoFineTagAndProbe("muonltmetIsoFineTagAndProbe");
    muonltmetIsoFineTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/muon_isobinned_count").c_str())
    .set_data(isdata)
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(  boost::bind(MuonTight, _1) 
                        && boost::bind(MinPtMaxEta, _1, 10, 2.1)
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045) )
    .set_passprobe_predicate((boost::bind(MuonTight, _1)) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1) )
    .set_pt_bins(pt_bins_fine)
    .set_eta_bins(eta_bins_fine)
    .set_vtx_bins(vtx_bins_fine)
    .set_mode(11)
    .set_era(era);
    
    MuonTagAndProbe muonltmetIDIsoFineTagAndProbe("muonltmetIDIsoFineTagAndProbe");
    muonltmetIDIsoFineTagAndProbe
    .set_fs(fsid)
    .set_output_name((outfolder+"/muon_idisobinned_count").c_str())
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 10, 2.1))
    .set_passprobe_predicate((boost::bind(MuonTight, _1)) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1))
    .set_data(isdata)
    .set_pt_bins(pt_bins_fine)
    .set_eta_bins(eta_bins_fine)
    .set_vtx_bins(vtx_bins_fine)
    .set_mode(12)
    .set_era(era);
 
//==============================================================================================
 //----------------------Trigger for MuTau------------------------------------------  
    MuonTagAndProbe muonTrgATagAndProbe("muonTrgATagAndProbe");
    muonTrgATagAndProbe
    .set_fs(fstrg)
    .set_output_name((outfolder+"/muon_trg_count").c_str())
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 10, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045))
    .set_data(isdata)
    .set_pt_bins(pt_bins_trg)
    .set_split_pm_eta(split_pm_eta)
    .set_run_low(run_low)
    .set_run_high(run_high)
    .set_eta_bins(eta_bins)
    .set_mode(3)
    .set_era(era);

    MuonTagAndProbe muonTrgBTagAndProbe("muonTrgBTagAndProbe");
    muonTrgBTagAndProbe
    .set_fs(fstrg2)
    .set_output_name((outfolder+"/muon_trgB_count").c_str())
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 10, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045))
    .set_data(isdata)
    .set_pt_bins(pt_bins_trg_B)
    .set_split_pm_eta(split_pm_eta)
    .set_run_low(run_low_B)
    .set_run_high(run_high_B)
    .set_eta_bins(eta_bins)
    .set_mode(3)
    .set_era(eraB);

//==============================================================================================
 //----------------------Trigger for MuTau+MET------------------------------------------  
    MuonTagAndProbe muonltmetTrgTagAndProbe("muonltmetTrgTagAndProbe");
    muonltmetTrgTagAndProbe
    .set_fs(fstrg)
    .set_output_name((outfolder+"/muon_trg_count").c_str())
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 5, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045))
    .set_data(isdata)
    .set_pt_bins(pt_bins_trg)
    .set_split_pm_eta(split_pm_eta)
    .set_run_low(run_low)
    .set_run_high(run_high)
    .set_eta_bins(eta_bins)
    .set_mode(4)
    .set_era(era);

//==============================================================================================
 //----------------------Trigger for IsoMu24------------------------------------------  
    MuonTagAndProbe muonIsoMu24TrgTagAndProbe("muonIsoMu24TrgTagAndProbe");
    muonIsoMu24TrgTagAndProbe
    .set_fs(fstrg)
    .set_output_name((outfolder+"/muon_trg_count").c_str())
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 5, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045))
    .set_data(isdata)
    .set_pt_bins(pt_bins_trg)
    .set_split_pm_eta(split_pm_eta)
    .set_run_low(run_low)
    .set_run_high(run_high)
    .set_eta_bins(eta_bins)
    .set_mode(5)
    .set_era(era);


//==============================================================================================

    if(!do_skim)
    {
        if(iselec)
        {
            if(!isdata) analysis.AddModule(&pileupWeight);
            if (isdata) analysis.AddModule(&lumiMask);
            if(VH)
            {
                analysis.AddModule(&electronVHIDTagAndProbe);
                analysis.AddModule(&electronVHIsoTagAndProbe);
                analysis.AddModule(&electronVHIDIsoTagAndProbe);
            }
            if(!trg_only && !VH && !ltmet)
            {
                analysis.AddModule(&electronIDTagAndProbe);
                analysis.AddModule(&electronIsoTagAndProbe);
                analysis.AddModule(&electronIDIsoTagAndProbe);
                analysis.AddModule(&electronIDFineTagAndProbe);
                analysis.AddModule(&electronIsoFineTagAndProbe);
                analysis.AddModule(&electronIDIsoFineTagAndProbe);
            }
            if(!idiso_only && !VH && !ltmet)
            {
                analysis.AddModule(&electronTrgATagAndProbe);
                    if(second_trigger) analysis.AddModule(&electronTrgBTagAndProbe);
            }
            if(ltmet && !trg_only)
            {
                analysis.AddModule(&electronltmetTrgTagAndProbe);
                analysis.AddModule(&electronltmetIDTagAndProbe);
                analysis.AddModule(&electronltmetIsoTagAndProbe);
                analysis.AddModule(&electronltmetIDIsoTagAndProbe);
                analysis.AddModule(&electronltmetIDFineTagAndProbe);
                analysis.AddModule(&electronltmetIsoFineTagAndProbe);
            }
            if(ltmet && trg_only)
            {
                analysis.AddModule(&electronltmetIDIsoFineTagAndProbe);
            }
        }
        if(!iselec)
        {
            if(!isdata) analysis.AddModule(&pileupWeight);
            if (isdata) analysis.AddModule(&lumiMask);
            if(VH)
            {
                analysis.AddModule(&muonVHIDTagAndProbe);
                analysis.AddModule(&muonVHIsoTagAndProbe);
                analysis.AddModule(&muonVHIDIsoTagAndProbe);
            }
            if(!trg_only && !VH && !ltmet)
            {
                analysis.AddModule(&muonIDTagAndProbe);
                analysis.AddModule(&muonIsoTagAndProbe);
                analysis.AddModule(&muonIDIsoTagAndProbe);
                analysis.AddModule(&muonIDFineTagAndProbe);
                analysis.AddModule(&muonIsoFineTagAndProbe);
                analysis.AddModule(&muonIDIsoFineTagAndProbe);
            }
            if(!idiso_only && !VH && !ltmet && !IsoMu24)
            {
                analysis.AddModule(&muonTrgATagAndProbe); 
                    if(second_trigger) analysis.AddModule(&muonTrgBTagAndProbe);
            }
            if(ltmet && !trg_only)
            {
                analysis.AddModule(&muonltmetIDTagAndProbe);
                analysis.AddModule(&muonltmetIsoTagAndProbe);
                analysis.AddModule(&muonltmetIDIsoTagAndProbe);
                analysis.AddModule(&muonltmetIDFineTagAndProbe);
                analysis.AddModule(&muonltmetIsoFineTagAndProbe);
                analysis.AddModule(&muonltmetIDIsoFineTagAndProbe);
            }
            if(ltmet && !idiso_only)
            {
                analysis.AddModule(&muonltmetTrgTagAndProbe);
            }
            if(IsoMu24)
            {
                analysis.AddModule(&muonIsoMu24TrgTagAndProbe);
            }
        }
    }
    if(do_skim)
    {
        if(iselec)
        {
            if(!isdata) analysis.AddModule(&zEEFilter);
            analysis.AddModule(&electronCounter);
            analysis.AddModule(&electronTagFilter);
   //         analysis.AddModule(&electronProbeFilter);
        }
        else
        {
            if(!isdata) analysis.AddModule(&zMuMuFilter);
            analysis.AddModule(&muonCounter);
            analysis.AddModule(&muonTagFilter);
     //       analysis.AddModule(&muonProbeFilter);
        }
    }


    analysis.RunAnalysis();
   
    if(!do_skim)
    {
        if(!trg_only) delete fsid;
        if(!idiso_only)
        {
            delete fstrg;
            delete fstrg2;
        }
    }

    return 0;
}

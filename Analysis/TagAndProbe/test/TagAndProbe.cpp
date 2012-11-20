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
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/ElectronTagAndProbe.h"
#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/MuonTagAndProbe.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"

namespace po = boost::program_options;

int main(int argc, char* argv[]){
    using namespace ic;

    bool isdata;
    bool iselec;
    bool split_pm_eta;
    bool second_trigger;
    bool idiso_only;
    bool trg_only;
    std::string era, eraB;
    std::string configfile, outname,outnametrg,outnametrgB, filelist, outfolder;
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
      ("idiso_only", po::value<bool>(&idiso_only)->default_value(false), "to rerun just id and iso")
      ("trg_only", po::value<bool>(&trg_only)->default_value(false), "to rerun just trigger")
      ("second_trigger", po::value<bool>(&second_trigger)->default_value(false), "allows a measurement of a second trigger if there are two in era")
      ("is_elec", po::value<bool>(&iselec)->required(), "0=muons, 1=electrons")
      ("filelist", po::value<std::string>(&filelist)->required(), "input filelist")
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
    // Create ROOT output fileservice
    fwlite::TFileService *fsid, *fstrg, *fstrg2;
    if(!trg_only)
    {
        if (outfolder == "") {
        fsid = new fwlite::TFileService(outname.c_str());
        } else {
        fsid = new fwlite::TFileService((outfolder+"/"+outname).c_str());
        }
    }
    if(!idiso_only)
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

  // Create analysis object
    ic::AnalysisBase analysis(
        "TagAndProbe",        // Analysis name
        files,                // Input files
        "icEventProducer",    // TTree path
        "EventTree",          // TTree name
        max_events);          // Max. events to process (-1 = all)


    ElectronTagAndProbe electronIDTagAndProbe("electronIDTagAndProbe");
    electronIDTagAndProbe
    .set_fs(fsid)
    .set_output_name("electron_id_count")
    .set_data(isdata)
    .set_tag_predicate(boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate( (boost::bind(MinPtMaxEta, _1, 15, 2.1)))
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
    .set_output_name("electron_iso_count")
    .set_data(isdata)
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1) 
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(  boost::bind(ElectronHTTIdIso, _1, 1) 
                        && (boost::bind(&Electron::pt, _1) > 15)
                        && (boost::bind(fabs, (boost::bind(&Electron::eta, _1))) < 2.1)
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
    .set_output_name("electron_idiso_count")
    .set_data(isdata)
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 15, 2.1)) 
    .set_passprobe_predicate((boost::bind(ElectronHTTIdIso, _1, 1)) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1) )
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(2)
    .set_era(era);
    
    
    ElectronTagAndProbe electronIDFineTagAndProbe("electronIDFineTagAndProbe");
    electronIDFineTagAndProbe
    .set_fs(fsid)
    .set_output_name("electron_idbinned_count")
    .set_data(isdata)
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 15, 2.1))
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
    .set_output_name("electron_isobinned_count")
    .set_data(isdata)
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(  boost::bind(ElectronHTTIdIso, _1, 1) 
                        && boost::bind(MinPtMaxEta, _1, 15, 2.1)
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
    .set_output_name("electron_idisobinned_count")
    .set_tag_predicate( boost::bind(ElectronHTTIdIso, _1, 1)
                        && (boost::bind(PF04IsolationVal<Electron>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Electron::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Electron::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 15, 2.1))
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
    
    ElectronTagAndProbe electronTrgATagAndProbe("electronTrgATagAndProbe");
    electronTrgATagAndProbe
    .set_fs(fstrg)
    .set_output_name("electron_trg_count")
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
    .set_output_name("electron_trgB_count")
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
   
    MuonTagAndProbe muonIDTagAndProbe("muonIDTagAndProbe");
    muonIDTagAndProbe
    .set_fs(fsid)
    .set_output_name("muon_id_count")
    .set_data(isdata)
    .set_tag_predicate(boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate( (boost::bind(MinPtMaxEta, _1, 15, 2.1)))
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
    .set_output_name("muon_iso_count")
    .set_data(isdata)
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1) 
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(  boost::bind(MuonTight, _1) 
                        && (boost::bind(&Muon::pt, _1) > 15)
                        && (boost::bind(fabs, (boost::bind(&Muon::eta, _1))) < 2.1)
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
    .set_output_name("muon_idiso_count")
    .set_data(isdata)
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 15, 2.1)) 
    .set_passprobe_predicate((boost::bind(MuonTight, _1)) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1)) ) < 0.2) 
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1) ))< 0.045) 
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1) )
    .set_pt_bins(pt_bins)
    .set_eta_bins(eta_bins)
    .set_mode(2)
    .set_era(era);
    
    
    MuonTagAndProbe muonIDFineTagAndProbe("muonIDFineTagAndProbe");
    muonIDFineTagAndProbe
    .set_fs(fsid)
    .set_output_name("muon_idbinned_count")
    .set_data(isdata)
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 15, 2.1))
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
    .set_output_name("muon_isobinned_count")
    .set_data(isdata)
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045) )
    .set_probe_predicate(  boost::bind(MuonTight, _1) 
                        && boost::bind(MinPtMaxEta, _1, 15, 2.1)
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
    .set_output_name("muon_idisobinned_count")
    .set_tag_predicate( boost::bind(MuonTight, _1)
                        && (boost::bind(PF04IsolationVal<Muon>, _1, 0.5) < 0.1)
                        && (boost::bind(MinPtMaxEta, _1, 20, 2.1))
                        && (boost::bind(fabs, (boost::bind(&Muon::dz_vertex, _1))) < 0.2)
                        && (boost::bind(fabs, (boost::bind(&Muon::dxy_vertex, _1))) < 0.045))
    .set_probe_predicate(boost::bind(MinPtMaxEta, _1, 15, 2.1))
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
    
    MuonTagAndProbe muonTrgATagAndProbe("muonTrgATagAndProbe");
    muonTrgATagAndProbe
    .set_fs(fstrg)
    .set_output_name("muon_trg_count")
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
    .set_output_name("muon_trg_count")
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



    if(iselec)
    {
        if(!trg_only)
        {
            analysis.AddModule(&electronIDTagAndProbe);
            analysis.AddModule(&electronIsoTagAndProbe);
            analysis.AddModule(&electronIDIsoTagAndProbe);
            analysis.AddModule(&electronIDFineTagAndProbe);
            analysis.AddModule(&electronIsoFineTagAndProbe);
            analysis.AddModule(&electronIDIsoFineTagAndProbe);
        }
        if(!idiso_only)
        {
            analysis.AddModule(&electronTrgATagAndProbe);
                if(second_trigger) analysis.AddModule(&electronTrgBTagAndProbe);
        }
    }
    if(!iselec)
    {
        if(!trg_only)
        {
            analysis.AddModule(&muonIDTagAndProbe);
            analysis.AddModule(&muonIsoTagAndProbe);
            analysis.AddModule(&muonIDIsoTagAndProbe);
            analysis.AddModule(&muonIDFineTagAndProbe);
            analysis.AddModule(&muonIsoFineTagAndProbe);
            analysis.AddModule(&muonIDIsoFineTagAndProbe);
        }
        if(!idiso_only)
        {
            analysis.AddModule(&muonTrgATagAndProbe); 
                if(second_trigger) analysis.AddModule(&muonTrgBTagAndProbe);
        }
    }

    analysis.RunAnalysis();
    if(!trg_only) delete fsid;
    if(!idiso_only)
    {
        delete fstrg;
        delete fstrg2;
    }

    return 0;
}

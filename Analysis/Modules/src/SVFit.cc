#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/SVFit.h"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "boost/lexical_cast.hpp"
#include <boost/algorithm/string.hpp>
#include <stdlib.h>

#include "boost/filesystem.hpp"



namespace ic {

  SVFit::SVFit(std::string const& name) : ModuleBase(name), channel_(channel::et) {
    //Add option to specify full path
    mode_ = 0;
    op_ = 0;
    // 1: Produce scripts
    // 2: Read mass from .out files
    file_counter = 0;
    event_counter = 0;
    split_ = 2500;
    outname_ = "svfit_test";
    fullpath_ = "SVFIT_2012/";
  }

  SVFit::~SVFit() {
    ;
  }

  int SVFit::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for SVFit" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    if (channel_ == channel::et) mode_ = 0;
    if (channel_ == channel::mt) mode_ = 1;
    if (channel_ == channel::mtmet) mode_ = 1;
    if (channel_ == channel::em) mode_ = 2;
    std::cout << "SVFit Mode: " << op_ << std::endl;
    std::cout << "Dilepton Label: " << dilepton_label_ << std::endl;
    std::cout << "MET Label: " << met_label_ << std::endl;

    std::string sub(".root");
    std::string::size_type foundpos = outname_.find(sub);
    if ( foundpos != std::string::npos )
      outname_.erase(outname_.begin() + foundpos, outname_.begin() + foundpos + sub.length());
    outname_ += "_SVFIT";
    boost::filesystem::path folder_p(outname_);
    std::cout << "Using directory: " << outname_ << std::endl;
    if (fullpath_ == "") {
      std::cout << "No path specified for SVFit directories, using the working directory:" << std::endl;
      fullpath_ = boost::filesystem::current_path().string();
      std::cout << "--:" << fullpath_ << std::endl;
    } else {
      std::cout << "Path for SVFit directory specifed:" << std::endl;
      std::cout << "--:" << fullpath_ << std::endl;
    }
    total_path_ = operator/(fullpath_, folder_p);
    boost::filesystem::create_directories(total_path_);
    std::cout << "Total path:" << std::endl;
    std::cout << "--:" << total_path_.string() << std::endl;
    //system(("mkdir -p "+cwd+"/"+outname_).c_str());

    if (op_ == 1) outFileSubmit.open((total_path_.string()+"/submit.sh").c_str());

    if (op_ == 2) {
      boost::filesystem::directory_iterator it(total_path_);
      for (; it != boost::filesystem::directory_iterator(); ++it) {
        std::string path = it->path().string();
        if (path.find(".out") != path.npos) {
          std::cout << "Reading file: " << path << std::endl;
          std::ifstream file;
          file.open(path.c_str());
          if (!file.is_open()) {
            std::cerr << "Error, file cannot be read!" << std::endl;
            exit(1);
          }
          std::string line = "";
        while(std::getline(file, line)) {//while loop through lines
          std::vector<std::string> vec_format;
          boost::split(vec_format, line, boost::is_any_of(":"));
          mass_map[vec_format[0]] = boost::lexical_cast<double>(vec_format[1]);
        }
      }
    }
  }

  return 0;
}

int SVFit::Execute(TreeEvent *event) {

    std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(dilepton_label_);
    Candidate* lepton = dilepton.at(0)->GetCandidate("lepton1"); //electron in el-mu
    Candidate* tau = dilepton.at(0)->GetCandidate("lepton2"); // muon in el-mu
    Met const* pfMetMVA = event->GetPtr<Met>(met_label_);
    if (op_ == 1) {
      if (event_counter%split_ == 0) {
        if (outFile.is_open()) {
          outFile.close();
          WriteRunScript();
          outFileSubmit << "bsub -o /dev/null -e /dev/null -q 8nh svfit_" << boost::lexical_cast<std::string>(file_counter - 1) << ".sh" << std::endl;
        }
        outFile.open((total_path_.string()+"/svfit_"+boost::lexical_cast<std::string>(file_counter)).c_str());
        ++file_counter;
      }
      EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
      int run = eventInfo->run();
      int evt = eventInfo->event();
      double l1_pt = lepton->pt(); 
      double l1_eta = lepton->eta(); 
      double l1_phi = lepton->phi(); 
      double l1_energy = lepton->energy(); 
      double l2_pt = tau->pt(); 
      double l2_eta = tau->eta(); 
      double l2_phi = tau->phi(); 
      double l2_energy = tau->energy(); 
      double met_pt = pfMetMVA->pt();
      double met_eta = pfMetMVA->eta();
      double met_phi = pfMetMVA->phi();
      double met_energy = pfMetMVA->energy();
      double xx_sig = pfMetMVA->xx_sig();
      double yx_sig = pfMetMVA->yx_sig();
      double xy_sig = pfMetMVA->xy_sig();
      double yy_sig = pfMetMVA->yy_sig();
      outFile << run << "."
      << evt << ":"
      << mode_ << ":"
      << l1_pt << ":"
      << l1_eta << ":"
      << l1_phi << ":"
      << l1_energy << ":"
      << l2_pt << ":"
      << l2_eta << ":"
      << l2_phi << ":"
      << l2_energy << ":"
      << met_pt << ":"
      << met_eta << ":"
      << met_phi << ":"
      << met_energy << ":"
      << xx_sig << ":"
      << yx_sig << ":"
      << xy_sig << ":"
      << yy_sig << std::endl;
      ++event_counter;
      return 0;
    }

    if (op_ == 2) {
      EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
      int run = eventInfo->run();
      int evt = eventInfo->event();
      std::string label = boost::lexical_cast<std::string>(run)+"."+boost::lexical_cast<std::string>(evt);
      std::map<std::string, double>::const_iterator it = mass_map.find(label);
      double mass = 0;
      if (it != mass_map.end()) {
        mass = it->second;
        event->Add("svfitMass", mass);
      } else {
        std::cout << "Warning, SVFit mass for " << label << " not found!" << std::endl;
        event->Add("svfitMass", double(-100.0));
      }
    }

    return 0;
  }
  int SVFit::PostAnalysis() {

    if (outFile.is_open()) {
      outFile.close();
      system(("chmod +x "+total_path_.string()+"/submit.sh").c_str());
      WriteRunScript();
      outFileSubmit << "bsub -o /dev/null -e /dev/null -q 8nh svfit_" << boost::lexical_cast<std::string>(file_counter - 1) << ".sh" << std::endl;
      outFileSubmit.close();
    }
    return 0;
  }

  void SVFit::PrintInfo() {
    ;
  }

  void SVFit::WriteRunScript() {
    std::ofstream runscript;
    std::string name = (total_path_.string()+"/svfit_"+boost::lexical_cast<std::string>(file_counter-1));
    runscript.open((name+".sh").c_str());
    runscript << "cd " + boost::filesystem::system_complete(total_path_).string()  << std::endl;
    runscript << "export SCRAM_ARCH=slc5_amd64_gcc462" << std::endl;
    runscript << "eval `scramv1 runtime -sh`" << std::endl;
    std::string cmssw_base = getenv("CMSSW_BASE");
    runscript << "source "+cmssw_base+"/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/scripts/setup_libs.sh" << std::endl;
    runscript << cmssw_base+"/src/UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/bin/SVFit " << ("svfit_"+boost::lexical_cast<std::string>(file_counter-1)) << " &> " << ("svfit_"+boost::lexical_cast<std::string>(file_counter-1)) << ".out" << std::endl;
    runscript.close();
    system(("chmod +x "+name+".sh").c_str());

  }

}

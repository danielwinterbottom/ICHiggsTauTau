#include <string>
#include "CombineTools/interface/CombineHarvester.h"
#include "CombineTools/interface/Observation.h"
#include "CombineTools/interface/Process.h"
#include "CombineTools/interface/HelperFunctions.h"
#include "CombineTools/interface/HttSystematics.h"
#include "TH1F.h"
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
#include "RooFitResult.h"
#include "boost/format.hpp"
#include "boost/bind.hpp"
#include "boost/assign/list_of.hpp"

using boost::assign::map_list_of;
using boost::bind;
using std::string;
using std::vector;
using std::pair;
using std::tuple;
using std::make_tuple;
using std::make_pair;

using ch::Process;
using ch::Observation;
using ch::Nuisance;
using ch::CombineHarvester;

int main() {
  CombineHarvester cb;

  vector<pair<int, string>> mt_cats_7 = {
      make_pair(1, "muTau_0jet_medium"),
      make_pair(2, "muTau_0jet_high"),
      make_pair(3, "muTau_1jet_medium"),
      make_pair(4, "muTau_1jet_high_lowhiggs"),
      make_pair(5, "muTau_1jet_high_mediumhiggs"),
      make_pair(6, "muTau_vbf")};

  vector<pair<int, string>> mt_cats_8 = {
      make_pair(1, "muTau_0jet_medium"),
      make_pair(2, "muTau_0jet_high"),
      make_pair(3, "muTau_1jet_medium"),
      make_pair(4, "muTau_1jet_high_lowhiggs"),
      make_pair(5, "muTau_1jet_high_mediumhiggs"),
      make_pair(6, "muTau_vbf_loose"),
      make_pair(7, "muTau_vbf_tight")};

  vector<string> masses = {"90",  "95",  "100", "105", "110", "115",
                           "120", "125", "130", "135", "140", "145"};

  std::cout << "Adding observations...";
  cb.AddObservations({"*"}, {"htt"}, {"7TeV"}, {"mt"}, mt_cats_7);
  cb.AddObservations({"*"}, {"htt"}, {"8TeV"}, {"mt"}, mt_cats_8);
  std::cout << " done\n";

  std::cout << "Adding background processes...";
  cb.AddProcesses({"*"}, {"htt"}, {"7TeV"}, {"mt"},
      {"ZTT", "W", "QCD", "ZL", "ZJ", "TT", "VV"}, mt_cats_7, false);
  cb.AddProcesses({"*"}, {"htt"}, {"8TeV"}, {"mt"},
      {"ZTT", "W", "QCD", "ZL", "ZJ", "TT", "VV"}, mt_cats_8, false);
  std::cout << " done\n";

  std::cout << "Adding signal processes...";
  cb.AddProcesses(masses, {"htt"}, {"7TeV"}, {"mt"},
      {"ggH", "qqH", "WH", "ZH"}, mt_cats_7, true);
  cb.AddProcesses(masses, {"htt"}, {"8TeV"}, {"mt"},
      {"ggH", "qqH", "WH", "ZH"}, mt_cats_8, true);
  std::cout << " done\n";

  std::cout << "Adding systematic uncertainties...";
  ch::AddDefaultSystematics(&cb);
  std::cout << " done\n";

  std::cout << "Extracting histograms from input root files...";
  cb.cp().era(true, {"7TeV"}).backgrounds().ExtractShapes(
      "data/demo/htt_mt.inputs-sm-7TeV-hcg.root", "$CHANNEL/$PROCESS",
      "$CHANNEL/$PROCESS_$SYSTEMATIC");
  cb.cp().era(true, {"8TeV"}).backgrounds().ExtractShapes(
      "data/demo/htt_mt.inputs-sm-8TeV-hcg.root", "$CHANNEL/$PROCESS",
      "$CHANNEL/$PROCESS_$SYSTEMATIC");
  cb.cp().era(true, {"7TeV"}).signals().ExtractShapes(
      "data/demo/htt_mt.inputs-sm-7TeV-hcg.root", "$CHANNEL/$PROCESS$MASS",
      "$CHANNEL/$PROCESS$MASS_$SYSTEMATIC");
  cb.cp().era(true, {"8TeV"}).signals().ExtractShapes(
      "data/demo/htt_mt.inputs-sm-8TeV-hcg.root", "$CHANNEL/$PROCESS$MASS",
      "$CHANNEL/$PROCESS$MASS_$SYSTEMATIC");
  std::cout << " done\n";

  std::cout << "Scaling signal process rates...\n";
  map<string, TGraph> xs;
  for (std::string const& e : {"7TeV", "8TeV"}) {
    for (std::string const& p : {"ggH", "qqH", "WH", "ZH"}) {
      ch::ParseTable(&xs, "data/xsecs_brs/"+p+"_"+e+"_YR3.txt", {p+"_"+e});
    }
  }
  ch::ParseTable(&xs, "data/xsecs_brs/htt_YR3.txt", {"htt"});

  for (std::string const& e : {"7TeV", "8TeV"}) {
    for (std::string const& p : {"ggH", "qqH", "WH", "ZH"}) {
      std::cout << "Scaling for process " << p << " and era " << e << "\n";

      cb.cp().process(true, {p}).era(true, {e}).ForEachProc(
          bind(ch::ScaleProcessRate, _1, &xs, p+"_"+e, "htt"));
    }
  }

  std::cout << "Setting standardised bin names...";
  cb.ForEachObs(ch::SetStandardBinName<ch::Observation>);
  cb.ForEachProc(ch::SetStandardBinName<ch::Process>);
  cb.ForEachNus(ch::SetStandardBinName<ch::Nuisance>);
  std::cout << " done\n";

  // cb.cp().nus_type(true, {"shape"}).PrintAll();

  std::cout << "Generating bbb uncertainties...\n";
  cb.cp().process(true, {"ZTT", "W", "QCD"}).AddBinByBin(0.1, true, &cb);
  std::cout << "...done\n";

  set<string> bins = cb.GenerateSetFromObs<string>(mem_fn(&Observation::bin));
  TFile output("htt_mt.input.root", "RECREATE");
  for (auto b : bins) {
    for (auto m : masses) {
      cout << "Writing datacard for bin: " << b << " and mass: " << m
                << "\n";
      cb.cp().bin(true, {b}).mass(true, {m, "*"}).WriteDatacard(
          b + "_" + m + ".txt", output);
    }
  }

  /* Still to do:
     [x] adding systematics
     [ ] adding workspaces, data and pdfs
     [x] scaling signal processes
     [ ] merging bin errors
     [x] adding bbb uncertainties
     [ ] interpolated signal points (with fb templates)
     [ ] template function to assign common properties between objects
  */

}


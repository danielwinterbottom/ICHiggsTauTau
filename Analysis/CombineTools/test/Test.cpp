#include <string>
#include "CombineTools/interface/CombineHarvester.h"
#include "CombineTools/interface/Observation.h"
#include "CombineTools/interface/Process.h"
#include "CombineTools/interface/HelperFunctions.h"
#include "TH1F.h"
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
#include "RooFitResult.h"
#include "boost/format.hpp"

int main() {
  ch::CombineHarvester test;
  // test.ParseDatacard("CMSDAS/hgg_8TeV_MVA_cat0145.txt", "hgg", "8TeV", "hgg", 0, "125");
  // test.ParseDatacard("CMSDAS/htt_mt_5_8TeV.txt", "htt", "8TeV", "mt", 5, "125");
  //test.ParseDatacard("CMSDAS/hzz4l_4muS_8TeV_0.txt", "hzz", "8TeV", "4mu", 0, "125");
  // test.PrintAll();
  // test.WriteDatacard("hgg_testcard.txt", "hgg_testfile.root");

  std::vector<std::pair<int, std::string>> mt_cats_7 = {
    {1, "muTau_0jet_medium"},
    {2, "muTau_0jet_high"},
    {3, "muTau_1jet_medium"},
    {4, "muTau_1jet_high_lowhiggs"},
    {5, "muTau_1jet_high_mediumhiggs"},
    {6, "muTau_vbf"}
  };

  std::vector<std::pair<int, std::string>> mt_cats_8 = {
    {1, "muTau_0jet_medium"},
    {2, "muTau_0jet_high"},
    {3, "muTau_1jet_medium"},
    {4, "muTau_1jet_high_lowhiggs"},
    {5, "muTau_1jet_high_mediumhiggs"},
    {6, "muTau_vbf_loose"},
    {7, "muTau_vbf_tight"}
  };

  std::vector<std::string> masses =
      {"90", "95", "100", "105", "110", "115", "120",
       "125", "130", "135", "140", "145"};

  test.AddObservations({"*"}, {"htt"}, {"7TeV"}, {"mt"}, mt_cats_7);
  test.AddObservations({"*"}, {"htt"}, {"8TeV"}, {"mt"}, mt_cats_8);

  test.AddProcesses({"*"}, {"htt"}, {"7TeV"}, {"mt"},
      {"ZTT", "W", "QCD", "ZL", "ZJ", "TT", "VV"}, mt_cats_7, false);
  test.AddProcesses({"*"}, {"htt"}, {"8TeV"}, {"mt"},
      {"ZTT", "W", "QCD", "ZL", "ZJ", "TT", "VV"}, mt_cats_8, false);

  test.AddProcesses(masses, {"htt"}, {"7TeV"}, {"mt"},
      {"ggH", "qqH", "WH", "ZH"}, mt_cats_7, true);
  test.AddProcesses(masses, {"htt"}, {"8TeV"}, {"mt"},
      {"ggH", "qqH", "WH", "ZH"}, mt_cats_8, true);

  test.shallow_copy().era(true, {"7TeV"}).backgrounds()
      .ExtractShapes("htt_mt.input_7TeV.root","$CHANNEL/$PROCESS");
  test.shallow_copy().era(true, {"8TeV"}).backgrounds()
      .ExtractShapes("htt_mt.input_8TeV.root","$CHANNEL/$PROCESS");

  test.shallow_copy().era(true, {"7TeV"}).signals()
      .ExtractShapes("htt_mt.input_7TeV.root","$CHANNEL/$PROCESS$MASS");
  test.shallow_copy().era(true, {"8TeV"}).signals()
      .ExtractShapes("htt_mt.input_8TeV.root","$CHANNEL/$PROCESS$MASS");

  test.ForEachObs(ch::SetStandardBinName<ch::Observation>);
  test.ForEachProc(ch::SetStandardBinName<ch::Process>);

  test.PrintAll();
  std::set<std::string> bins = test.GenerateSetFromObs<std::string>(std::mem_fn(&ch::Observation::bin));
  TFile output("htt_mt.input.root","RECREATE");
  for (auto b : bins) {
    for (auto m : masses) {
      test.shallow_copy().bin(true, {b}).mass(true, {m, "*"})
          .WriteDatacard(b+"_"+m+".txt", output);
    }
  }

  /* Still to do:
     [ ] adding systematics
     [ ] adding workspaces, data and pdfs
     [ ] scaling signal processes
     [ ] merging bin errors
     [ ] adding bbb uncertainties
     [ ] interpolated signal points (with fb templates)
  */


}

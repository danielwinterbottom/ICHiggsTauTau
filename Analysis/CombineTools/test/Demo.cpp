#include <string>
#include <map>
#include <set>
#include <iostream>
#include <vector>
#include "CombineTools/interface/CombineHarvester.h"
#include "CombineTools/interface/HighLevelFunctions.h"
#include "CombineTools/interface/HelperFunctions.h"
#include "CombineTools/interface/HttSystematics.h"

using namespace std;

int main() {
  ch::CombineHarvester cb;

  // cb.SetVerbosity(1);

  typedef vector<pair<int, string>> Categories;
  map<string, Categories> cats;

  cats["7TeV"] = {
      {1, "muTau_0jet_medium"},
      {2, "muTau_0jet_high"},
      {3, "muTau_1jet_medium"},
      {4, "muTau_1jet_high_lowhiggs"},
      {5, "muTau_1jet_high_mediumhiggs"},
      {6, "muTau_vbf"}};

  cats["8TeV"] = {
      {1, "muTau_0jet_medium"},
      {2, "muTau_0jet_high"},
      {3, "muTau_1jet_medium"},
      {4, "muTau_1jet_high_lowhiggs"},
      {5, "muTau_1jet_high_mediumhiggs"},
      {6, "muTau_vbf_loose"},
      {7, "muTau_vbf_tight"}};

  vector<string> masses = ch::MassesFromRange("110-145:5");

  for (string era : {"7TeV", "8TeV"}) {
    cout << "------ " << era << " ------\n";
    cout << ">> Adding observations...";
    cb.AddObservations({"*"}, {"htt"}, {era}, {"mt"}, cats[era]);
    cout << " done\n";

    cout << ">> Adding background processes...";
    cb.AddProcesses({"*"}, {"htt"}, {era}, {"mt"},
        {"ZTT", "W", "QCD", "ZL", "ZJ", "TT", "VV"}, cats[era], false);
    cout << " done\n";

    cout << ">> Adding signal processes...";
    cb.AddProcesses(masses, {"htt"}, {era}, {"mt"},
        {"ggH", "qqH", "WH", "ZH"}, cats[era], true);
    cout << " done\n";
    cout << "------------------\n";
  }

  cout << ">> Adding systematic uncertainties...";
  ch::AddDefaultSystematics(&cb);
  cout << " done\n";

  cout << ">> Extracting histograms from input root files...";
  for (string era : {"7TeV", "8TeV"}) {
    cb.cp().era({era}).backgrounds().ExtractShapes(
        "data/demo/htt_mt.inputs-sm-"+era+"-hcg.root",
        "$BIN/$PROCESS",
        "$BIN/$PROCESS_$SYSTEMATIC");
    cb.cp().era({era}).signals().ExtractShapes(
        "data/demo/htt_mt.inputs-sm-"+era+"-hcg.root",
        "$BIN/$PROCESS$MASS",
        "$BIN/$PROCESS$MASS_$SYSTEMATIC");
  }
  cout << " done\n";

  cout << ">> Scaling signal process rates...\n";
  for (string const& e : {"7TeV", "8TeV"}) {
    for (string const& p : {"ggH", "qqH", "WH", "ZH"}) {
      map<string, TGraph> xs;
      ch::ParseTable(&xs, "data/xsecs_brs/"+p+"_"+e+"_YR3.txt", {p+"_"+e});
      ch::ParseTable(&xs, "data/xsecs_brs/htt_YR3.txt", {"htt"});
      cout << ">>>> Scaling for process " << p << " and era " << e << "\n";
      cb.cp().process({p}).era({e}).ForEachProc(
          bind(ch::ScaleProcessRate, _1, &xs, p+"_"+e, "htt"));
    }
  }

  cout << ">> Setting standardised bin names...";
  ch::SetStandardBinNames(&cb);
  cout << " done\n";

  cout << ">> Merging bin errors...";
  cb.cp().bin_id({0, 1, 2, 3, 4}).process({"W", "QCD"})
      .MergeBinErrors(0.1, 0.4);
  cb.cp().bin_id({5}).era({"7TeV"}).process({"W"})
      .MergeBinErrors(0.1, 0.4);
  cb.cp().bin_id({5, 6}).era({"8TeV"}).process({"W"})
      .MergeBinErrors(0.1, 0.4);
  cb.cp().bin_id({7}).era({"8TeV"}).process({"W", "ZTT"})
      .MergeBinErrors(0.1, 0.4);
  cout << "done\n";

  cout << ">> Generating bbb uncertainties...";
  cb.cp().bin_id({0, 1, 2, 3, 4}).process({"W", "QCD"})
      .AddBinByBin(0.1, true, &cb);
  cb.cp().bin_id({5}).era({"7TeV"}).process({"W"})
      .AddBinByBin(0.1, true, &cb);
  cb.cp().bin_id({5, 6}).era({"8TeV"}).process({"W"})
      .AddBinByBin(0.1, true, &cb);
  cb.cp().bin_id({7}).era({"8TeV"}).process({"W", "ZTT"})
      .AddBinByBin(0.1, true, &cb);
  cout << "done\n";

  set<string> bins = cb.bin_set();
  TFile output("htt_mt.input.root", "RECREATE");
  for (auto b : bins) {
    for (auto m : masses) {
      cout << ">> Writing datacard for bin: " << b << " and mass: " << m
                << "\r" << flush;
      cb.cp().bin({b}).mass({m, "*"}).WriteDatacard(
          b + "_" + m + ".txt", output);
    }
  }
  cout << "\n>> Writing combined datacard\n";
  cb.cp().mass({"*", "125"}).WriteDatacard("htt_mt_125_combined.txt", output);
  cout << ">> Done!\n";

  output.Close();
}

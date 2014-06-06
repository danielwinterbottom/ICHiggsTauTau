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
#include "boost/bind.hpp"
#include "boost/assign/list_of.hpp"

using boost::assign::map_list_of;
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
  // cb.ParseDatacard("CMSDAS/hgg_8TeV_MVA_cat0145.txt", "hgg", "8TeV", "hgg", 0, "125");
  // cb.ParseDatacard("CMSDAS/htt_mt_5_8TeV.txt", "htt", "8TeV", "mt", 5, "125");
  //cb.ParseDatacard("CMSDAS/hzz4l_4muS_8TeV_0.txt", "hzz", "8TeV", "4mu", 0, "125");
  // cb.PrintAll();
  // cb.WriteDatacard("hgg_testcard.txt", "hgg_testfile.root");

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
  cb.cp().signals().AddSyst<string>(&cb,
      "lumi_$ERA", "lnN", &Process::era, map_list_of
      ("7TeV", 1.022)("8TeV", 1.026));

  cb.cp().AddSyst<string>(&cb,
      "pdf_gg", "lnN", &Process::process, map_list_of
      ("ggH", 1.097));

  cb.cp().AddSyst<string>(&cb,
      "pdf_qqbar", "lnN", &Process::process, map_list_of
      ("qqH", 1.036)("WH",  1.04)("ZH",  1.04));

  cb.cp().process(true, {"ggH"}).AddSyst<int>(&cb,
      "QCDscale_ggH", "lnN", &Process::bin_id, map_list_of
      (1, 1.08)(2, 1.08));

  cb.cp().process(true, {"ggH"}).AddSyst<int>(&cb,
      "QCDscale_ggH1in", "lnN", &Process::bin_id, map_list_of
      (3, 1.105)(4, 1.095)(5, 1.195));

  cb.cp().process(true, {"ggH"}).AddSyst<int>(&cb,
      "QCDscale_ggH2in", "lnN", &Process::bin_id, map_list_of
      (6, 1.228)(7, 1.307));

  cb.cp().process(true, {"qqH"}).AddSyst<int>(&cb,
      "QCDscale_qqH", "lnN", &Process::bin_id, map_list_of
      (1, 1.022)
      (2, 1.023)
      (3, 1.015)
      (4, 1.013)
      (5, 1.015)
      (6, 1.018)
      (7, 1.031));

  cb.cp().process(true, {"WH", "ZH"}).AddSyst<int>(&cb,
      "QCDscale_VH", "lnN", &Process::bin_id, map_list_of
      (1, 1.010)
      (2, 1.010)
      (3, 1.040)
      (4, 1.040)
      (5, 1.040)
      (6, 1.040)
      (7, 1.040));

  cb.cp().process(true, {"qqH", "WH", "ZH"}).AddSyst<int>(&cb,
      "UEPS", "lnN", &Process::bin_id, map_list_of
      (1, 1.050)
      (2, 1.060)
      (3, 1.007)
      (4, 0.996)
      (5, 1.006)
      (6, 0.988)
      (7, 0.986));
  cb.cp().process(true, {"ggH"}).AddSyst<int>(&cb,
      "UEPS", "lnN", &Process::bin_id, map_list_of
      (1, 1.013)
      (2, 1.028)
      (3, 0.946)
      (4, 0.954)
      (5, 0.983)
      (6, 0.893)
      (7, 0.881));

  cb.cp()
      .process(true, {"ggH", "qqH", "WH", "ZH", "ZTT", "ZL", "ZJ", "TT", "VV"})
      .AddSyst<string>(&cb,
      "CMS_eff_m", "lnN", &Process::era, map_list_of
      ("7TeV", 1.02)("8TeV", 1.02));

  cb.cp()
      .process(true, {"ggH", "qqH", "WH", "ZH", "ZTT", "TT", "VV"})
      .AddSyst<string>(&cb,
      "CMS_eff_t_$CHANNEL_$ERA", "lnN", &Process::era, map_list_of
      ("7TeV", 1.08)("8TeV", 1.08));

  cb.cp()
      .process(true, {"ggH", "qqH", "WH", "ZH", "ZTT", "TT", "VV"})
      .AddSyst<tuple<string, int>>(&cb,
      "CMS_eff_t_$CHANNEL_medium_$ERA", "lnN",
      [](Process* p){ return make_tuple(p->era(), p->bin_id()); },
      {
        {make_tuple("7TeV",  1),      1.03},
        {make_tuple("7TeV",  3),      1.03},
        {make_tuple("7TeV",  6),      1.01},
        {make_tuple("8TeV",  1),      1.03},
        {make_tuple("8TeV",  3),      1.03},
        {make_tuple("8TeV",  6),      1.01},
        {make_tuple("8TeV",  7),      1.01}
      });

  cb.cp()
      .process(true, {"ggH", "qqH", "WH", "ZH", "ZTT", "TT", "VV"})
      .AddSyst<tuple<string, int>>(&cb,
      "CMS_eff_t_$CHANNEL_high_$ERA", "lnN",
      [](Process* p){ return make_tuple(p->era(), p->bin_id()); },
      {
        {make_tuple("7TeV",  2),      1.030},
        {make_tuple("7TeV",  4),      1.030},
        {make_tuple("7TeV",  5),      1.030},
        {make_tuple("7TeV",  6),      1.012},
        {make_tuple("8TeV",  2),      1.030},
        {make_tuple("8TeV",  4),      1.030},
        {make_tuple("8TeV",  5),      1.030},
        {make_tuple("8TeV",  6),      1.012},
        {make_tuple("8TeV",  7),      1.015}
      });

  cb.cp()
      .process(true, {"ggH", "qqH", "WH", "ZH", "ZTT"})
      .AddSyst<string>(&cb,
      "CMS_scale_t_mutau_$ERA", "shape", &Process::era, map_list_of
      ("7TeV", 1.00)("8TeV", 1.00));

  cb.cp().AddSyst<tuple<string, int, string>>(&cb,
      "CMS_scale_j_$ERA", "lnN",
      [](Process* p){ return make_tuple(p->era(), p->bin_id(), p->process()); },
      {
        {make_tuple("7TeV", 1, "ggH"),     0.98},
        {make_tuple("7TeV", 1, "qqH"),     0.86},
        {make_tuple("7TeV", 1, "WH"),      0.97},
        {make_tuple("7TeV", 1, "ZH"),      0.97},
        {make_tuple("7TeV", 1, "TT"),      0.99},
        {make_tuple("7TeV", 1, "VV"),      0.98},

        {make_tuple("8TeV", 1, "ggH"),     0.98},
        {make_tuple("8TeV", 1, "qqH"),     0.92},
        {make_tuple("8TeV", 1, "WH"),      0.96},
        {make_tuple("8TeV", 1, "ZH"),      0.96},
        {make_tuple("8TeV", 1, "ZL"),      0.99},
        {make_tuple("8TeV", 1, "ZJ"),      0.98},
        {make_tuple("8TeV", 1, "TT"),      0.95},
        {make_tuple("8TeV", 1, "VV"),      0.98},

        {make_tuple("7TeV", 2, "ggH"),     0.98},
        {make_tuple("7TeV", 2, "qqH"),     0.86},
        {make_tuple("7TeV", 2, "WH"),      0.97},
        {make_tuple("7TeV", 2, "ZH"),      0.97},
        {make_tuple("7TeV", 2, "TT"),      0.99},
        {make_tuple("7TeV", 2, "VV"),      0.98},

        {make_tuple("8TeV", 2, "ggH"),     0.98},
        {make_tuple("8TeV", 2, "qqH"),     0.92},
        {make_tuple("8TeV", 2, "WH"),      0.88},
        {make_tuple("8TeV", 2, "ZH"),      0.88},
        {make_tuple("8TeV", 2, "ZL"),      0.99},
        {make_tuple("8TeV", 2, "ZJ"),      0.97},
        {make_tuple("8TeV", 2, "TT"),      0.84},
        {make_tuple("8TeV", 2, "VV"),      0.97},

        {make_tuple("7TeV", 3, "ggH"),     1.05},
        {make_tuple("7TeV", 3, "qqH"),     1.01},
        {make_tuple("7TeV", 3, "WH"),      1.05},
        {make_tuple("7TeV", 3, "ZH"),      1.05},
        {make_tuple("7TeV", 3, "VV"),      1.02},

        {make_tuple("8TeV", 3, "ggH"),     1.04},
        {make_tuple("8TeV", 3, "qqH"),     0.99},
        {make_tuple("8TeV", 3, "WH"),      1.01},
        {make_tuple("8TeV", 3, "ZH"),      1.01},
        {make_tuple("8TeV", 3, "ZL"),      1.02},
        {make_tuple("8TeV", 3, "ZJ"),      1.02},
        {make_tuple("8TeV", 3, "VV"),      1.03},

        {make_tuple("7TeV", 4, "ggH"),     1.03},
        {make_tuple("7TeV", 4, "qqH"),     1.02},
        {make_tuple("7TeV", 4, "WH"),      1.03},
        {make_tuple("7TeV", 4, "ZH"),      1.03},
        {make_tuple("7TeV", 4, "TT"),      1.01},
        {make_tuple("7TeV", 4, "VV"),      1.04},

        {make_tuple("8TeV", 4, "ggH"),     1.04},
        {make_tuple("8TeV", 4, "WH"),      1.03},
        {make_tuple("8TeV", 4, "ZH"),      1.03},
        {make_tuple("8TeV", 4, "ZL"),      1.02},
        {make_tuple("8TeV", 4, "ZJ"),      1.02},
        {make_tuple("8TeV", 4, "VV"),      1.02},

        {make_tuple("7TeV", 5, "ggH"),     1.03},
        {make_tuple("7TeV", 5, "qqH"),     1.02},
        {make_tuple("7TeV", 5, "WH"),      1.03},
        {make_tuple("7TeV", 5, "ZH"),      1.03},
        {make_tuple("7TeV", 5, "TT"),      1.01},
        {make_tuple("7TeV", 5, "VV"),      1.04},

        {make_tuple("8TeV", 5, "ggH"),     1.02},
        {make_tuple("8TeV", 5, "WH"),      1.01},
        {make_tuple("8TeV", 5, "ZH"),      1.01},
        {make_tuple("8TeV", 5, "TT"),      0.97},
        {make_tuple("8TeV", 5, "VV"),      1.03},

        {make_tuple("7TeV", 6, "ggH"),     1.20},
        {make_tuple("7TeV", 6, "qqH"),     1.05},
        {make_tuple("7TeV", 6, "WH"),      1.20},
        {make_tuple("7TeV", 6, "ZH"),      1.20},
        {make_tuple("7TeV", 6, "TT"),      1.10},
        {make_tuple("7TeV", 6, "VV"),      1.15},

        {make_tuple("8TeV", 6, "ggH"),     1.10},
        {make_tuple("8TeV", 6, "qqH"),     1.04},
        {make_tuple("8TeV", 6, "WH"),      1.15},
        {make_tuple("8TeV", 6, "ZH"),      1.15},
        {make_tuple("8TeV", 6, "ZL"),      1.05},
        {make_tuple("8TeV", 6, "ZJ"),      1.10},
        {make_tuple("8TeV", 6, "TT"),      1.05},
        {make_tuple("8TeV", 6, "VV"),      1.08},

        {make_tuple("8TeV", 7, "ggH"),     1.06},
        {make_tuple("8TeV", 7, "qqH"),     1.03},
        {make_tuple("8TeV", 7, "WH"),      1.15},
        {make_tuple("8TeV", 7, "ZH"),      1.15},
        {make_tuple("8TeV", 7, "ZL"),      1.05},
        {make_tuple("8TeV", 7, "TT"),      1.05},
        {make_tuple("8TeV", 7, "VV"),      1.10}
      });

  cb.cp().AddSyst<tuple<string, int, string>>(&cb,
      "CMS_htt_scale_met_$ERA", "lnN",
      [](Process* p){ return make_tuple(p->era(), p->bin_id(), p->process()); },
      {
        {make_tuple("7TeV", 1, "ggH"),     1.05},
        {make_tuple("7TeV", 1, "qqH"),     1.05},
        {make_tuple("7TeV", 1, "WH"),      1.05},
        {make_tuple("7TeV", 1, "ZH"),      1.05},
        {make_tuple("7TeV", 1, "TT"),      1.07},
        {make_tuple("7TeV", 1, "W"),       1.07},
        {make_tuple("7TeV", 1, "ZL"),      1.05},
        {make_tuple("7TeV", 1, "ZJ"),      1.05},

        {make_tuple("8TeV", 1, "TT"),      1.05},
        {make_tuple("8TeV", 1, "W"),       1.01},

        {make_tuple("7TeV", 2, "ggH"),     1.05},
        {make_tuple("7TeV", 2, "qqH"),     1.05},
        {make_tuple("7TeV", 2, "WH"),      1.05},
        {make_tuple("7TeV", 2, "ZH"),      1.05},
        {make_tuple("7TeV", 2, "TT"),      1.07},
        {make_tuple("7TeV", 2, "W"),       1.07},
        {make_tuple("7TeV", 2, "ZL"),      1.05},
        {make_tuple("7TeV", 2, "ZJ"),      1.05},

        {make_tuple("8TeV", 2, "ggH"),     1.01},
        {make_tuple("8TeV", 2, "qqH"),     1.01},
        {make_tuple("8TeV", 2, "WH"),      1.01},
        {make_tuple("8TeV", 2, "ZH"),      1.01},
        {make_tuple("8TeV", 2, "TT"),      1.04},
        {make_tuple("8TeV", 2, "W"),       1.01},
        {make_tuple("8TeV", 2, "ZL"),      1.02},

        {make_tuple("7TeV", 3, "ggH"),     1.05},
        {make_tuple("7TeV", 3, "qqH"),     1.05},
        {make_tuple("7TeV", 3, "WH"),      1.05},
        {make_tuple("7TeV", 3, "ZH"),      1.05},
        {make_tuple("7TeV", 3, "TT"),      1.07},
        {make_tuple("7TeV", 3, "W"),       1.07},
        {make_tuple("7TeV", 3, "ZL"),      1.05},
        {make_tuple("7TeV", 3, "ZJ"),      1.05},

        {make_tuple("8TeV", 3, "ggH"),     0.99},
        {make_tuple("8TeV", 3, "qqH"),     0.99},
        {make_tuple("8TeV", 3, "WH"),      0.99},
        {make_tuple("8TeV", 3, "ZH"),      0.99},
        {make_tuple("8TeV", 3, "TT"),      1.02},
        {make_tuple("8TeV", 3, "W"),       1.01},
        {make_tuple("8TeV", 3, "ZL"),      1.02},

        {make_tuple("7TeV", 4, "ggH"),     1.05},
        {make_tuple("7TeV", 4, "qqH"),     1.05},
        {make_tuple("7TeV", 4, "WH"),      1.05},
        {make_tuple("7TeV", 4, "ZH"),      1.05},
        {make_tuple("7TeV", 4, "TT"),      1.07},
        {make_tuple("7TeV", 4, "W"),       1.07},
        {make_tuple("7TeV", 4, "ZL"),      1.05},
        {make_tuple("7TeV", 4, "ZJ"),      1.05},

        {make_tuple("8TeV", 4, "ggH"),     0.99},
        {make_tuple("8TeV", 4, "qqH"),     0.99},
        {make_tuple("8TeV", 4, "WH"),      0.99},
        {make_tuple("8TeV", 4, "ZH"),      0.99},
        {make_tuple("8TeV", 4, "TT"),      1.02},
        {make_tuple("8TeV", 4, "W"),       1.01},
        {make_tuple("8TeV", 4, "ZL"),      1.03},

        {make_tuple("7TeV", 5, "ggH"),     1.05},
        {make_tuple("7TeV", 5, "qqH"),     1.05},
        {make_tuple("7TeV", 5, "WH"),      1.05},
        {make_tuple("7TeV", 5, "ZH"),      1.05},
        {make_tuple("7TeV", 5, "TT"),      1.07},
        {make_tuple("7TeV", 5, "W"),       1.07},
        {make_tuple("7TeV", 5, "ZL"),      1.05},
        {make_tuple("7TeV", 5, "ZJ"),      1.05},

        {make_tuple("8TeV", 5, "ggH"),     0.99},
        {make_tuple("8TeV", 5, "qqH"),     0.99},
        {make_tuple("8TeV", 5, "WH"),      0.99},
        {make_tuple("8TeV", 5, "ZH"),      0.99},
        {make_tuple("8TeV", 5, "TT"),      1.01},
        {make_tuple("8TeV", 5, "W"),       1.03},
        {make_tuple("8TeV", 5, "ZL"),      1.02},
        {make_tuple("8TeV", 5, "ZJ"),      0.98},

        {make_tuple("7TeV", 6, "ggH"),     1.05},
        {make_tuple("7TeV", 6, "qqH"),     1.05},
        {make_tuple("7TeV", 6, "WH"),      1.05},
        {make_tuple("7TeV", 6, "ZH"),      1.05},
        {make_tuple("7TeV", 6, "TT"),      1.07},
        {make_tuple("7TeV", 6, "W"),       1.07},
        {make_tuple("7TeV", 6, "ZL"),      1.05},
        {make_tuple("7TeV", 6, "ZJ"),      1.05},

        {make_tuple("8TeV", 6, "ggH"),     0.99},
        {make_tuple("8TeV", 6, "qqH"),     0.99},
        {make_tuple("8TeV", 6, "WH"),      0.99},
        {make_tuple("8TeV", 6, "ZH"),      0.99},
        {make_tuple("8TeV", 6, "TT"),      1.04},
        {make_tuple("8TeV", 6, "W"),       1.04},
        {make_tuple("8TeV", 6, "ZL"),      1.03},
        {make_tuple("8TeV", 6, "ZJ"),      1.03},

        {make_tuple("8TeV", 7, "ggH"),     0.99},
        {make_tuple("8TeV", 7, "qqH"),     0.99},
        {make_tuple("8TeV", 7, "WH"),      0.99},
        {make_tuple("8TeV", 7, "ZH"),      0.99},
        {make_tuple("8TeV", 7, "TT"),      1.03},
        {make_tuple("8TeV", 7, "W"),       1.03},
        {make_tuple("8TeV", 7, "ZL"),      1.03},
        {make_tuple("8TeV", 7, "ZJ"),      1.03},
      });

  cb.cp().AddSyst<tuple<string, int, string>>(&cb,
    "CMS_eff_b_$ERA", "lnN",
    [](Process* p){ return make_tuple(p->era(), p->bin_id(), p->process()); },
    {
      {make_tuple("7TeV", 1, "TT"),      0.90},
      {make_tuple("7TeV", 2, "TT"),      0.90},
      {make_tuple("7TeV", 3, "TT"),      0.90},
      {make_tuple("7TeV", 3, "VV"),      0.98},
      {make_tuple("7TeV", 4, "TT"),      0.90},
      {make_tuple("7TeV", 4, "VV"),      0.98},
      {make_tuple("7TeV", 5, "TT"),      0.90},
      {make_tuple("7TeV", 5, "VV"),      0.98},
      {make_tuple("7TeV", 6, "TT"),      0.90},
      {make_tuple("8TeV", 1, "TT"),      0.98},
      {make_tuple("8TeV", 2, "TT"),      0.96},
      {make_tuple("8TeV", 3, "TT"),      0.96},
      {make_tuple("8TeV", 4, "TT"),      0.96},
      {make_tuple("8TeV", 5, "TT"),      0.94},
      {make_tuple("8TeV", 6, "TT"),      0.94},
      {make_tuple("8TeV", 7, "TT"),      0.94}
    });

  cb.cp().process(true, {"ZTT", "ZL", "ZJ"})
      .AddSyst<string>(&cb,
      "CMS_htt_zttNorm_$ERA", "lnN", &Process::era, map_list_of
      ("7TeV", 1.03)
      ("8TeV", 1.03));

  cb.cp().AddSyst<tuple<string, int, string>>(&cb,
    "CMS_htt_extrap_ztt_$BIN_$ERA", "lnN",
    [](Process* p){ return make_tuple(p->era(), p->bin_id(), p->process()); },
    {
      {make_tuple("7TeV", 1, "ZTT"),      1.05},
      {make_tuple("7TeV", 2, "ZTT"),      1.05},
      {make_tuple("7TeV", 3, "ZTT"),      1.05},
      {make_tuple("7TeV", 4, "ZTT"),      1.05},
      {make_tuple("7TeV", 5, "ZTT"),      1.05},
      {make_tuple("7TeV", 6, "ZTT"),      1.10},
      {make_tuple("8TeV", 1, "ZTT"),      1.05},
      {make_tuple("8TeV", 2, "ZTT"),      1.05},
      {make_tuple("8TeV", 3, "ZTT"),      1.05},
      {make_tuple("8TeV", 4, "ZTT"),      1.05},
      {make_tuple("8TeV", 5, "ZTT"),      1.05},
      {make_tuple("8TeV", 6, "ZTT"),      1.10},
      {make_tuple("8TeV", 7, "ZTT"),      1.13}
    });

  cb.cp().AddSyst<tuple<string, int, string>>(&cb,
    "CMS_htt_ttbarNorm_$ERA", "lnN",
    [](Process* p){ return make_tuple(p->era(), p->bin_id(), p->process()); },
    {
      {make_tuple("7TeV", 1, "TT"),      1.08},
      {make_tuple("7TeV", 2, "TT"),      1.08},
      {make_tuple("7TeV", 3, "TT"),      1.08},
      {make_tuple("7TeV", 4, "TT"),      1.08},
      {make_tuple("7TeV", 5, "TT"),      1.08},
      {make_tuple("7TeV", 6, "TT"),      1.08},
      {make_tuple("8TeV", 1, "TT"),      1.10},
      {make_tuple("8TeV", 2, "TT"),      1.10},
      {make_tuple("8TeV", 3, "TT"),      1.10},
      {make_tuple("8TeV", 4, "TT"),      1.10},
      {make_tuple("8TeV", 5, "TT"),      1.10},
      {make_tuple("8TeV", 6, "TT"),      1.08},
      {make_tuple("8TeV", 7, "TT"),      1.08}
    });

  cb.cp().AddSyst<tuple<string, int, string>>(&cb,
    "CMS_htt_ttbarNorm_$BIN_$ERA", "lnN",
    [](Process* p){ return make_tuple(p->era(), p->bin_id(), p->process()); },
    {
      {make_tuple("7TeV", 6, "TT"),      1.20},
      {make_tuple("8TeV", 6, "TT"),      1.33},
      {make_tuple("8TeV", 7, "TT"),      1.33}
    });

  cb.cp().AddSyst<tuple<string, int, string>>(&cb,
    "CMS_htt_WNorm_$BIN_$ERA", "lnN",
    [](Process* p){ return make_tuple(p->era(), p->bin_id(), p->process()); },
    {
      {make_tuple("7TeV", 1, "W"),      1.20},
      {make_tuple("7TeV", 2, "W"),      1.20},
      {make_tuple("7TeV", 3, "W"),      1.10},
      {make_tuple("7TeV", 4, "W"),      1.10},
      {make_tuple("7TeV", 5, "W"),      1.12},
      {make_tuple("7TeV", 6, "W"),      1.20},
      {make_tuple("8TeV", 1, "W"),      1.20},
      {make_tuple("8TeV", 2, "W"),      1.20},
      {make_tuple("8TeV", 3, "W"),      1.15},
      {make_tuple("8TeV", 4, "W"),      1.11},
      {make_tuple("8TeV", 5, "W"),      1.16},
      {make_tuple("8TeV", 6, "W"),      1.21},
      {make_tuple("8TeV", 7, "W"),      1.51}
    });

  cb.cp().process(true, {"VV"}).AddSyst<string>(&cb,
      "CMS_htt_DiBosonNorm_$ERA", "lnN", &Process::era, map_list_of
      ("7TeV", 1.15)
      ("8TeV", 1.15));

  cb.cp().AddSyst<tuple<string, int, string>>(&cb,
    "CMS_htt_DiBosonNorm_$BIN_$ERA", "lnN",
    [](Process* p){ return make_tuple(p->era(), p->bin_id(), p->process()); },
    {
      {make_tuple("7TeV", 6, "VV"),      1.37},
      {make_tuple("8TeV", 6, "VV"),      1.50},
      {make_tuple("8TeV", 7, "VV"),      1.45}
    });

  cb.cp().AddSyst<tuple<string, int, string>>(&cb,
    "CMS_htt_QCDSyst_$BIN_$ERA", "lnN",
    [](Process* p){ return make_tuple(p->era(), p->bin_id(), p->process()); },
    {
      {make_tuple("7TeV", 1, "QCD"),      1.10},
      {make_tuple("7TeV", 2, "QCD"),      1.10},
      {make_tuple("7TeV", 3, "QCD"),      1.10},
      {make_tuple("7TeV", 4, "QCD"),      1.10},
      {make_tuple("7TeV", 5, "QCD"),      1.10},
      {make_tuple("7TeV", 6, "QCD"),      1.22},
      {make_tuple("8TeV", 1, "QCD"),      1.06},
      {make_tuple("8TeV", 2, "QCD"),      1.06},
      {make_tuple("8TeV", 3, "QCD"),      1.10},
      {make_tuple("8TeV", 4, "QCD"),      1.10},
      {make_tuple("8TeV", 5, "QCD"),      1.10},
      {make_tuple("8TeV", 6, "QCD"),      1.30},
      {make_tuple("8TeV", 7, "QCD"),      1.70}
    });

  cb.cp().process(true, {"QCD"}).bin_id(true, {3}).AddSyst<string>(&cb,
      "CMS_htt_QCDShape_mutau_1jet_medium_$ERA", "shape", &Process::era, map_list_of
      ("7TeV", 1.00)("8TeV", 1.00));
  cb.cp().process(true, {"QCD"}).bin_id(true, {4}).AddSyst<string>(&cb,
      "CMS_htt_QCDShape_mutau_1jet_high_lowhiggs_$ERA", "shape", &Process::era, map_list_of
      ("7TeV", 1.00)("8TeV", 1.00));
  cb.cp().process(true, {"QCD"}).bin_id(true, {6}).AddSyst<string>(&cb,
      "CMS_htt_QCDShape_mutau_vbf_$ERA", "shape", &Process::era, map_list_of
      ("7TeV", 1.00));
  cb.cp().process(true, {"QCD"}).bin_id(true, {6}).AddSyst<string>(&cb,
      "CMS_htt_QCDShape_mutau_vbf_loose_$ERA", "shape", &Process::era, map_list_of
      ("8TeV", 1.00));

  cb.cp().AddSyst<tuple<string, int, string>>(&cb,
    "CMS_htt_ZJetFakeTau_$BIN_$ERA", "lnN",
    [](Process* p){ return make_tuple(p->era(), p->bin_id(), p->process()); },
    {
      {make_tuple("7TeV", 1, "ZJ"),      1.20},
      {make_tuple("7TeV", 2, "ZJ"),      1.20},
      {make_tuple("7TeV", 3, "ZJ"),      1.20},
      {make_tuple("7TeV", 4, "ZJ"),      1.20},
      {make_tuple("7TeV", 5, "ZJ"),      1.20},
      {make_tuple("7TeV", 6, "ZJ"),      1.30},
      {make_tuple("8TeV", 1, "ZJ"),      1.20},
      {make_tuple("8TeV", 2, "ZJ"),      1.20},
      {make_tuple("8TeV", 3, "ZJ"),      1.20},
      {make_tuple("8TeV", 4, "ZJ"),      1.20},
      {make_tuple("8TeV", 5, "ZJ"),      1.20},
      {make_tuple("8TeV", 6, "ZJ"),      1.30},
      {make_tuple("8TeV", 7, "ZJ"),      1.80}
    });

  cb.cp().AddSyst<tuple<string, int, string>>(&cb,
    "CMS_htt_ZLeptonFakeTau_$CHANNEL_$ERA", "lnN",
    [](Process* p){ return make_tuple(p->era(), p->bin_id(), p->process()); },
    {
      {make_tuple("7TeV", 1, "ZL"),      1.30},
      {make_tuple("7TeV", 2, "ZL"),      1.30},
      {make_tuple("7TeV", 3, "ZL"),      1.30},
      {make_tuple("7TeV", 4, "ZL"),      1.30},
      {make_tuple("7TeV", 5, "ZL"),      1.30},
      {make_tuple("7TeV", 6, "ZL"),      1.30},
      {make_tuple("8TeV", 1, "ZL"),      1.30},
      {make_tuple("8TeV", 2, "ZL"),      1.30},
      {make_tuple("8TeV", 3, "ZL"),      1.30},
      {make_tuple("8TeV", 4, "ZL"),      1.30},
      {make_tuple("8TeV", 5, "ZL"),      1.30},
      {make_tuple("8TeV", 6, "ZL"),      1.30},
      {make_tuple("8TeV", 7, "ZL"),      1.30}
    });

  cb.cp().AddSyst<tuple<string, int, string>>(&cb,
    "CMS_htt_ZLeptonFakeTau_$BIN_$ERA", "lnN",
    [](Process* p){ return make_tuple(p->era(), p->bin_id(), p->process()); },
    {
      {make_tuple("7TeV", 6, "ZL"),      2.00},
      {make_tuple("8TeV", 5, "ZL"),      1.26},
      {make_tuple("8TeV", 6, "ZL"),      1.70}
    });

  cb.cp().process(true, {"ZL"}).AddSyst<string>(&cb,
      "CMS_htt_ZLScale_mutau_$ERA", "shape", &Process::era, map_list_of
      ("7TeV", 1.00)("8TeV", 1.00));

  std::cout << " done\n";

  std::cout << "Extracting histograms from input root files...";
  cb.cp().era(true, {"7TeV"}).backgrounds().ExtractShapes(
      "htt_mt.input_7TeV.root", "$CHANNEL/$PROCESS",
      "$CHANNEL/$PROCESS_$SYSTEMATIC");
  cb.cp().era(true, {"8TeV"}).backgrounds().ExtractShapes(
      "htt_mt.input_8TeV.root", "$CHANNEL/$PROCESS",
      "$CHANNEL/$PROCESS_$SYSTEMATIC");
  cb.cp().era(true, {"7TeV"}).signals().ExtractShapes(
      "htt_mt.input_7TeV.root", "$CHANNEL/$PROCESS$MASS",
      "$CHANNEL/$PROCESS$MASS_$SYSTEMATIC");
  cb.cp().era(true, {"8TeV"}).signals().ExtractShapes(
      "htt_mt.input_8TeV.root", "$CHANNEL/$PROCESS$MASS",
      "$CHANNEL/$PROCESS$MASS_$SYSTEMATIC");
  std::cout << " done\n";

  std::cout << "Setting standardised bin names...";
  cb.ForEachObs(ch::SetStandardBinName<ch::Observation>);
  cb.ForEachProc(ch::SetStandardBinName<ch::Process>);
  cb.ForEachNus(ch::SetStandardBinName<ch::Nuisance>);
  std::cout << " done\n";

  std::cout << "Generating bbb uncertainties...";
  cb.cp().process(true, {"ZTT", "W", "QCD"}).AddBinByBin(0.1, true, &cb);
  std::cout << " done\n";

  // cb.PrintAll();

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

  // cb.cp().nus_type(true, {"shape"}).PrintAll();


  /* Still to do:
     [x] adding systematics
     [ ] adding workspaces, data and pdfs
     [ ] scaling signal processes
     [ ] merging bin errors
     [x] adding bbb uncertainties
     [ ] interpolated signal points (with fb templates)
  */

}


#include "CombineTools/interface/HttSystematics.h"
#include <string>
#include "CombineTools/interface/Systematics.h"
#include "CombineTools/interface/Process.h"
#include "CombineTools/interface/HelperFunctions.h"

namespace ch {

void AddDefaultSystematics(CombineHarvester *cb) {
  using ch::syst::SystMap;
  using ch::syst::era;
  using ch::syst::bin_id;
  using ch::syst::process;
  using ch::JoinStr;

  auto signal = Set2Vec(cb->cp().signals().GenerateSetFromProcs<std::string>(
      std::mem_fn(&Process::process)));

  cb->cp().signals()
      .AddSyst(cb, "lumi_$ERA", "lnN", SystMap<era>::init
      ({"7TeV"}, 1.022)
      ({"8TeV"}, 1.026));

  cb->cp().process({"ggH"})
      .AddSyst(cb, "pdf_gg", "lnN", SystMap<>::init
      (1.097));

  cb->cp().process({"qqH", "WH", "ZH"})
      .AddSyst(cb, "pdf_qqbar", "lnN", SystMap<process>::init
      ({"qqH"},       1.036)
      ({"WH", "ZH"},  1.040));

  cb->cp().process({"ggH"})
      .AddSyst(cb, "QCDscale_ggH", "lnN", SystMap<bin_id>::init
      ({1, 2}, 1.08));

  cb->cp().process({"ggH"})
      .AddSyst(cb, "QCDscale_ggH1in", "lnN", SystMap<bin_id>::init
      ({3}, 1.105)
      ({4}, 1.095)
      ({5}, 1.195));

  cb->cp().process({"ggH"})
      .AddSyst(cb, "QCDscale_ggH2in", "lnN", SystMap<bin_id>::init
      ({6}, 1.228)
      ({7}, 1.307));

  cb->cp().process({"qqH"})
      .AddSyst(cb, "QCDscale_qqH", "lnN", SystMap<bin_id>::init
      ({1}, 1.022)
      ({2}, 1.023)
      ({3}, 1.015)
      ({4}, 1.013)
      ({5}, 1.015)
      ({6}, 1.018)
      ({7}, 1.031));

  cb->cp().process({"WH", "ZH"})
      .AddSyst(cb, "QCDscale_VH", "lnN", SystMap<bin_id>::init
      ({1, 2},            1.010)
      ({3, 4, 5, 6, 7},   1.040));

  cb->cp().process({"qqH", "WH", "ZH"})
      .AddSyst(cb, "UEPS", "lnN", SystMap<bin_id>::init
      ({1}, 1.050)
      ({2}, 1.060)
      ({3}, 1.007)
      ({4}, 0.996)
      ({5}, 1.006)
      ({6}, 0.988)
      ({7}, 0.986));
  cb->cp().process({"ggH"})
      .AddSyst(cb, "UEPS", "lnN", SystMap<bin_id>::init
      ({1}, 1.013)
      ({2}, 1.028)
      ({3}, 0.946)
      ({4}, 0.954)
      ({5}, 0.983)
      ({6}, 0.893)
      ({7}, 0.881));

  cb->cp().process(JoinStr({signal, {"ZTT", "ZL", "ZJ", "TT", "VV"}}))
      .AddSyst(cb, "CMS_eff_m", "lnN", SystMap<>::init(1.02));

  cb->cp().process(JoinStr({signal, {"ZTT", "TT", "VV"}}))
      .AddSyst(cb, "CMS_eff_t_$CHANNEL_$ERA", "lnN", SystMap<>::init(1.08));

  cb->cp().process(JoinStr({signal, {"ZTT", "TT", "VV"}})).AddSyst(
      cb, "CMS_eff_t_$CHANNEL_medium_$ERA", "lnN",
      SystMap<bin_id>::init({1, 3}, 1.03)({6, 7}, 1.01));

  cb->cp().process(JoinStr({signal, {"ZTT", "TT", "VV"}}))
      .AddSyst(cb, "CMS_eff_t_$CHANNEL_high_$ERA", "lnN", SystMap<bin_id>::init
        ({2, 4, 5},  1.030)
        ({6},        1.012)
        ({7},        1.015));

  cb->cp().process(JoinStr({/*signal,*/ {"ZTT"}}))
      .AddSyst(cb, "CMS_scale_t_mutau_$ERA", "shape", SystMap<>::init(1.00));

  cb->cp()
      .AddSyst(cb,
        "CMS_scale_j_$ERA", "lnN", SystMap<era, bin_id, process>::init
        ({"7TeV"}, {1, 2},  {"ggH"},        0.98)
        ({"7TeV"}, {1, 2},  {"qqH"},        0.86)
        ({"7TeV"}, {1, 2},  {"WH", "ZH"},   0.97)
        ({"7TeV"}, {1, 2},  {"TT"},         0.99)
        ({"7TeV"}, {1, 2},  {"VV"},         0.98)
        ({"7TeV"}, {3},     {"ggH"},        1.05)
        ({"7TeV"}, {3},     {"qqH"},        1.01)
        ({"7TeV"}, {3},     {"WH", "ZH"},   1.05)
        ({"7TeV"}, {3},     {"VV"},         1.02)
        ({"7TeV"}, {4, 5},  {"ggH"},        1.03)
        ({"7TeV"}, {4, 5},  {"qqH"},        1.02)
        ({"7TeV"}, {4, 5},  {"WH", "ZH"},   1.03)
        ({"7TeV"}, {4, 5},  {"TT"},         1.01)
        ({"7TeV"}, {4, 5},  {"VV"},         1.04)
        ({"7TeV"}, {6},     {"ggH"},        1.20)
        ({"7TeV"}, {6},     {"qqH"},        1.05)
        ({"7TeV"}, {6},     {"WH", "ZH"},   1.20)
        ({"7TeV"}, {6},     {"TT"},         1.10)
        ({"7TeV"}, {6},     {"VV"},         1.15)
        ({"8TeV"}, {1},     {"ggH"},        0.98)
        ({"8TeV"}, {1},     {"qqH"},        0.92)
        ({"8TeV"}, {1},     {"WH", "ZH"},   0.96)
        ({"8TeV"}, {1},     {"ZL"},         0.99)
        ({"8TeV"}, {1},     {"ZJ"},         0.98)
        ({"8TeV"}, {1},     {"TT"},         0.95)
        ({"8TeV"}, {1},     {"VV"},         0.98)
        ({"8TeV"}, {2},     {"ggH"},        0.98)
        ({"8TeV"}, {2},     {"qqH"},        0.92)
        ({"8TeV"}, {2},     {"WH", "ZH"},   0.88)
        ({"8TeV"}, {2},     {"ZL"},         0.99)
        ({"8TeV"}, {2},     {"ZJ"},         0.97)
        ({"8TeV"}, {2},     {"TT"},         0.84)
        ({"8TeV"}, {2},     {"VV"},         0.97)
        ({"8TeV"}, {3},     {"ggH"},        1.04)
        ({"8TeV"}, {3},     {"qqH"},        0.99)
        ({"8TeV"}, {3},     {"WH", "ZH"},   1.01)
        ({"8TeV"}, {3},     {"ZL", "ZJ"},   1.02)
        ({"8TeV"}, {3},     {"VV"},         1.03)
        ({"8TeV"}, {4},     {"ggH"},        1.04)
        ({"8TeV"}, {4},     {"WH", "ZH"},   1.03)
        ({"8TeV"}, {4},     {"ZL", "ZJ"},   1.02)
        ({"8TeV"}, {4},     {"VV"},         1.02)
        ({"8TeV"}, {5},     {"ggH"},        1.02)
        ({"8TeV"}, {5},     {"WH", "ZH"},   1.01)
        ({"8TeV"}, {5},     {"TT"},         0.97)
        ({"8TeV"}, {5},     {"VV"},         1.03)
        ({"8TeV"}, {6},     {"ggH"},        1.10)
        ({"8TeV"}, {6},     {"qqH"},        1.04)
        ({"8TeV"}, {6},     {"WH", "ZH"},   1.15)
        ({"8TeV"}, {6},     {"ZL"},         1.05)
        ({"8TeV"}, {6},     {"ZJ"},         1.10)
        ({"8TeV"}, {6},     {"TT"},         1.05)
        ({"8TeV"}, {6},     {"VV"},         1.08)
        ({"8TeV"}, {7},     {"ggH"},        1.06)
        ({"8TeV"}, {7},     {"qqH"},        1.03)
        ({"8TeV"}, {7},     {"WH", "ZH"},   1.15)
        ({"8TeV"}, {7},     {"ZL"},         1.05)
        ({"8TeV"}, {7},     {"TT"},         1.05)
        ({"8TeV"}, {7},     {"VV"},         1.10));

  cb->cp()
      .AddSyst(cb, "CMS_htt_scale_met_$ERA", "lnN",
        SystMap<era, bin_id, process>::init
        ({"7TeV"}, {1, 2, 3, 4, 5, 6},  {signal},       1.05)
        ({"7TeV"}, {1, 2, 3, 4, 5, 6},  {"TT", "W"},    1.07)
        ({"7TeV"}, {1, 2, 3, 4, 5, 6},  {"ZL", "ZJ"},   1.05)
        ({"8TeV"}, {1},                 {"TT"},         1.05)
        ({"8TeV"}, {1},                 {"W"},          1.01)
        ({"8TeV"}, {2},                 {signal},       1.01)
        ({"8TeV"}, {2},                 {"TT"},         1.04)
        ({"8TeV"}, {2},                 {"W"},          1.01)
        ({"8TeV"}, {2},                 {"ZL"},         1.02)
        ({"8TeV"}, {3},                 {signal},       0.99)
        ({"8TeV"}, {3},                 {"TT"},         1.02)
        ({"8TeV"}, {3},                 {"W"},          1.01)
        ({"8TeV"}, {3},                 {"ZL"},         1.02)
        ({"8TeV"}, {4},                 {signal},       0.99)
        ({"8TeV"}, {4},                 {"TT"},         1.02)
        ({"8TeV"}, {4},                 {"W"},          1.01)
        ({"8TeV"}, {4},                 {"ZL"},         1.03)
        ({"8TeV"}, {5},                 {signal},       0.99)
        ({"8TeV"}, {5},                 {"TT"},         1.01)
        ({"8TeV"}, {5},                 {"W"},          1.03)
        ({"8TeV"}, {5},                 {"ZL"},         1.02)
        ({"8TeV"}, {5},                 {"ZJ"},         0.98)
        ({"8TeV"}, {6},                 {signal},       0.99)
        ({"8TeV"}, {6},                 {"TT", "W"},    1.04)
        ({"8TeV"}, {6},                 {"ZL", "ZJ"},   1.03)
        ({"8TeV"}, {7},                 {signal},       0.99)
        ({"8TeV"}, {7},                 {"TT", "W"},    1.03)
        ({"8TeV"}, {7},                 {"ZL", "ZJ"},   1.03));

  cb->cp()
      .AddSyst(cb, "CMS_eff_b_$ERA", "lnN", SystMap<era, bin_id, process>::init
        ({"7TeV"}, {1, 2, 3, 4, 5, 6},  {"TT"},      0.90)
        ({"7TeV"}, {3, 4, 5},           {"VV"},      0.98)
        ({"8TeV"}, {1},                 {"TT"},      0.98)
        ({"8TeV"}, {2, 3, 4},           {"TT"},      0.96)
        ({"8TeV"}, {5, 6, 7},           {"TT"},      0.94));

  cb->cp().process({"ZTT", "ZL", "ZJ"})
      .AddSyst(cb, "CMS_htt_zttNorm_$ERA", "lnN", SystMap<>::init(1.03));

  cb->cp().process({"ZTT"})
      .AddSyst(cb, "CMS_htt_extrap_ztt_$BIN_$ERA", "lnN", SystMap<bin_id>::init
        ({1, 2, 3, 4, 5},   1.05)
        ({6},               1.10)
        ({7},               1.13));

  cb->cp().process({"TT"})
      .AddSyst(cb, "CMS_htt_ttbarNorm_$ERA", "lnN", SystMap<era, bin_id>::init
        ({"7TeV"}, {1, 2, 3, 4, 5, 6},    1.08)
        ({"8TeV"}, {1, 2, 3, 4, 5},       1.10)
        ({"8TeV"}, {6, 7},                1.08));

  cb->cp().process({"TT"})
      .AddSyst(cb,
      "CMS_htt_ttbarNorm_$BIN_$ERA", "lnN", SystMap<era, bin_id>::init
      ({"7TeV"}, {6},     1.20)
      ({"8TeV"}, {6, 7},  1.33));

  cb->cp().process({"W"})
      .AddSyst(cb, "CMS_htt_WNorm_$BIN_$ERA", "lnN", SystMap<era, bin_id>::init
        ({"7TeV"}, {1, 2},    1.20)
        ({"7TeV"}, {3, 4},    1.10)
        ({"7TeV"}, {5},       1.12)
        ({"7TeV"}, {6},       1.20)
        ({"8TeV"}, {1, 2},    1.20)
        ({"8TeV"}, {3},       1.15)
        ({"8TeV"}, {4},       1.11)
        ({"8TeV"}, {5},       1.16)
        ({"8TeV"}, {6},       1.21)
        ({"8TeV"}, {7},       1.51));

  cb->cp().process({"VV"})
      .AddSyst(cb, "CMS_htt_DiBosonNorm_$ERA", "lnN", SystMap<>::init(1.15));

  cb->cp().process({"VV"})
      .AddSyst(cb,
      "CMS_htt_DiBosonNorm_$BIN_$ERA", "lnN", SystMap<era, bin_id>::init
        ({"7TeV"}, {6},      1.37)
        ({"8TeV"}, {6},      1.50)
        ({"8TeV"}, {7},      1.45));

  cb->cp().process({"QCD"})
      .AddSyst(cb,
      "CMS_htt_QCDSyst_$BIN_$ERA", "lnN", SystMap<era, bin_id>::init
        ({"7TeV"}, {1, 2, 3, 4, 5},   1.10)
        ({"7TeV"}, {6},               1.22)
        ({"8TeV"}, {1, 2},            1.06)
        ({"8TeV"}, {3, 4, 5},         1.10)
        ({"8TeV"}, {6},               1.30)
        ({"8TeV"}, {7},               1.70));

  cb->cp().process({"QCD"}).bin_id({3})
      .AddSyst(cb, "CMS_htt_QCDShape_mutau_1jet_medium_$ERA", "shape",
        SystMap<>::init(1.00));
  cb->cp().process({"QCD"}).bin_id({4})
      .AddSyst(cb, "CMS_htt_QCDShape_mutau_1jet_high_lowhiggs_$ERA", "shape",
        SystMap<>::init(1.00));
  cb->cp().process({"QCD"}).bin_id({6}).era({"7TeV"})
      .AddSyst(cb, "CMS_htt_QCDShape_mutau_vbf_$ERA", "shape",
        SystMap<>::init(1.00));
  cb->cp().process({"QCD"}).bin_id({6}).era({"8TeV"})
      .AddSyst(cb, "CMS_htt_QCDShape_mutau_vbf_loose_$ERA", "shape",
        SystMap<>::init(1.00));

  cb->cp().process({"ZJ"})
      .AddSyst(cb, "CMS_htt_ZJetFakeTau_$BIN_$ERA", "lnN", SystMap<bin_id>::init
        ({1, 2, 3, 4, 5},   1.20)
        ({6},               1.30)
        ({7},               1.80));

  cb->cp().process({"ZL"})
      .AddSyst(cb, "CMS_htt_ZLeptonFakeTau_$CHANNEL_$ERA", "lnN",
        SystMap<>::init(1.30));

  cb->cp().process({"ZL"})
      .AddSyst(cb, "CMS_htt_ZLeptonFakeTau_$BIN_$ERA", "lnN",
        SystMap<era, bin_id>::init
        ({"7TeV"}, {6},       2.00)
        ({"8TeV"}, {5},       1.26)
        ({"8TeV"}, {6},       1.70));

  cb->cp().process({"ZL"})
      .AddSyst(cb,
      "CMS_htt_ZLScale_mutau_$ERA", "shape", SystMap<>::init(1.00));
}
}

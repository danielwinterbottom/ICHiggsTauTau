#include <string>
#include "CombineTools/interface/CombineHarvester.h"
#include "CombineTools/interface/Observation.h"
#include "CombineTools/interface/Process.h"
#include "TH1F.h"
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
#include "RooFitResult.h"
#include "boost/format.hpp"

int main() {
  ch::CombineHarvester h_7;
  ch::CombineHarvester h_8;

  // boost::filesystem::path datacard_folder("htt/125");
  // boost::filesystem::directory_iterator dir_it(datacard_folder);
  // boost::regex datacard_rgx_7TeV(".*_7TeV.txt");
  // boost::regex datacard_rgx_8TeV(".*_8TeV.txt");
  // for (; dir_it != boost::filesystem::directory_iterator(); ++dir_it) {
  //   std::string path = dir_it->path().filename().string();
  //   if (boost::regex_match(path, datacard_rgx_7TeV)) {
  //     std::cout << "Parsing datacard: " << dir_it->path().string() << std::endl;
  //     // h_7.ParseDatacard(dir_it->path().string());
  //   }
  // }


/*
h_7.SmartParse("cmb/125/{ANA}_{CHN}_{CHNID}_{ERA}.txt");

*/

  h_7.ParseDatacard("htt/125/htt_mt_1_7TeV.txt", "htt", "7TeV", "mt", 1, "125");
  h_7.ParseDatacard("htt/125/htt_mt_2_7TeV.txt", "htt", "7TeV", "mt", 2, "125");
  h_7.ParseDatacard("htt/125/htt_mt_3_7TeV.txt", "htt", "7TeV", "mt", 3, "125");
  h_7.ParseDatacard("htt/125/htt_mt_4_7TeV.txt", "htt", "7TeV", "mt", 4, "125");
  h_7.ParseDatacard("cmb/125/htt_mt_5_7TeV.txt", "htt", "7TeV", "mt", 5, "125");
   // h_7.ParseDatacard("htt/125/htt_mt_6_7TeV.txt", "htt", "7TeV", "mt", 6, "125");
  // h_8.ParseDatacard("htt/125/htt_mt_1_8TeV.txt", "htt", "8TeV", "mt", 1, "125");
  // h_8.ParseDatacard("htt/125/htt_mt_2_8TeV.txt", "htt", "8TeV", "mt", 2, "125");
  // h_8.ParseDatacard("htt/125/htt_mt_3_8TeV.txt", "htt", "8TeV", "mt", 3, "125");
  // h_8.ParseDatacard("htt/125/htt_mt_4_8TeV.txt", "htt", "8TeV", "mt", 4, "125");
  // h_8.ParseDatacard("htt/125/htt_mt_5_8TeV.txt", "htt", "8TeV", "mt", 5, "125");
  // h_8.ParseDatacard("htt/125/htt_mt_6_8TeV.txt", "htt", "8TeV", "mt", 6, "125");
  // h_8.ParseDatacard("htt/125/htt_mt_7_8TeV.txt", "htt", "8TeV", "mt", 7, "125");

  // h_7.ParseDatacard("htt/125/htt_et_1_7TeV.txt", "htt", "7TeV", "et", 1, "125");
  // h_7.ParseDatacard("htt/125/htt_et_2_7TeV.txt", "htt", "7TeV", "et", 2, "125");
  // h_7.ParseDatacard("htt/125/htt_et_3_7TeV.txt", "htt", "7TeV", "et", 3, "125");
  // h_7.ParseDatacard("htt/125/htt_et_5_7TeV.txt", "htt", "7TeV", "et", 5, "125");
  // h_7.ParseDatacard("htt/125/htt_et_6_7TeV.txt", "htt", "7TeV", "et", 6, "125");
  // h_8.ParseDatacard("htt/125/htt_et_1_8TeV.txt", "htt", "8TeV", "et", 1, "125");
  // h_8.ParseDatacard("htt/125/htt_et_2_8TeV.txt", "htt", "8TeV", "et", 2, "125");
  // h_8.ParseDatacard("htt/125/htt_et_3_8TeV.txt", "htt", "8TeV", "et", 3, "125");
  // h_8.ParseDatacard("htt/125/htt_et_5_8TeV.txt", "htt", "8TeV", "et", 5, "125");
  // h_8.ParseDatacard("htt/125/htt_et_6_8TeV.txt", "htt", "8TeV", "et", 6, "125");
  // h_8.ParseDatacard("htt/125/htt_et_7_8TeV.txt", "htt", "8TeV", "et", 7, "125");

  // h_7.ParseDatacard("htt/125/htt_em_0_7TeV.txt", "htt", "7TeV", "em", 0, "125");
  // h_7.ParseDatacard("htt/125/htt_em_1_7TeV.txt", "htt", "7TeV", "em", 1, "125");
  // h_7.ParseDatacard("htt/125/htt_em_2_7TeV.txt", "htt", "7TeV", "em", 2, "125");
  // h_7.ParseDatacard("htt/125/htt_em_3_7TeV.txt", "htt", "7TeV", "em", 3, "125");
  // h_7.ParseDatacard("htt/125/htt_em_4_7TeV.txt", "htt", "7TeV", "em", 4, "125");
  // h_8.ParseDatacard("htt/125/htt_em_0_8TeV.txt", "htt", "8TeV", "em", 0, "125");
  // h_8.ParseDatacard("htt/125/htt_em_1_8TeV.txt", "htt", "8TeV", "em", 1, "125");
  // h_8.ParseDatacard("htt/125/htt_em_2_8TeV.txt", "htt", "8TeV", "em", 2, "125");
  // h_8.ParseDatacard("htt/125/htt_em_3_8TeV.txt", "htt", "8TeV", "em", 3, "125");
  // h_8.ParseDatacard("htt/125/htt_em_4_8TeV.txt", "htt", "8TeV", "em", 4, "125");
  // h_8.ParseDatacard("htt/125/htt_em_5_8TeV.txt", "htt", "8TeV", "em", 5, "125");

  // h_7.ParseDatacard("htt/125/htt_ee_0_7TeV.txt", "htt", "7TeV", "ee", 0, "125");
  // h_7.ParseDatacard("htt/125/htt_ee_1_7TeV.txt", "htt", "7TeV", "ee", 1, "125");
  // h_7.ParseDatacard("htt/125/htt_ee_2_7TeV.txt", "htt", "7TeV", "ee", 2, "125");
  // h_7.ParseDatacard("htt/125/htt_ee_3_7TeV.txt", "htt", "7TeV", "ee", 3, "125");
  // h_7.ParseDatacard("htt/125/htt_ee_4_7TeV.txt", "htt", "7TeV", "ee", 4, "125");
  // h_8.ParseDatacard("htt/125/htt_ee_0_8TeV.txt", "htt", "8TeV", "ee", 0, "125");
  // h_8.ParseDatacard("htt/125/htt_ee_1_8TeV.txt", "htt", "8TeV", "ee", 1, "125");
  // h_8.ParseDatacard("htt/125/htt_ee_2_8TeV.txt", "htt", "8TeV", "ee", 2, "125");
  // h_8.ParseDatacard("htt/125/htt_ee_3_8TeV.txt", "htt", "8TeV", "ee", 3, "125");
  // h_8.ParseDatacard("htt/125/htt_ee_4_8TeV.txt", "htt", "8TeV", "ee", 4, "125");

  // h_7.ParseDatacard("htt/125/htt_mm_0_7TeV.txt", "htt", "7TeV", "mm", 0, "125");
  // h_7.ParseDatacard("htt/125/htt_mm_1_7TeV.txt", "htt", "7TeV", "mm", 1, "125");
  // h_7.ParseDatacard("htt/125/htt_mm_2_7TeV.txt", "htt", "7TeV", "mm", 2, "125");
  // h_7.ParseDatacard("htt/125/htt_mm_3_7TeV.txt", "htt", "7TeV", "mm", 3, "125");
  // h_7.ParseDatacard("htt/125/htt_mm_4_7TeV.txt", "htt", "7TeV", "mm", 4, "125");
  // h_8.ParseDatacard("htt/125/htt_mm_0_8TeV.txt", "htt", "8TeV", "mm", 0, "125");
  // h_8.ParseDatacard("htt/125/htt_mm_1_8TeV.txt", "htt", "8TeV", "mm", 1, "125");
  // h_8.ParseDatacard("htt/125/htt_mm_2_8TeV.txt", "htt", "8TeV", "mm", 2, "125");
  // h_8.ParseDatacard("htt/125/htt_mm_3_8TeV.txt", "htt", "8TeV", "mm", 3, "125");
  // h_8.ParseDatacard("htt/125/htt_mm_4_8TeV.txt", "htt", "8TeV", "mm", 4, "125");

  // h_8.ParseDatacard("htt/125/htt_tt_0_8TeV.txt", "htt", "8TeV", "tt", 0, "125");
  // h_8.ParseDatacard("htt/125/htt_tt_1_8TeV.txt", "htt", "8TeV", "tt", 1, "125");
  // h_8.ParseDatacard("htt/125/htt_tt_2_8TeV.txt", "htt", "8TeV", "tt", 2, "125");

//  h_7.ParseDatacard("htt/125/vhtt_0_7TeV.txt", "vhtt", "7TeV", "vhtt", 0, "125");
//  h_7.ParseDatacard("htt/125/vhtt_1_7TeV.txt", "vhtt", "7TeV", "vhtt", 1, "125");
//  h_7.ParseDatacard("htt/125/vhtt_3_7TeV.txt", "vhtt", "7TeV", "vhtt", 3, "125");
//  h_7.ParseDatacard("htt/125/vhtt_4_7TeV.txt", "vhtt", "7TeV", "vhtt", 4, "125");
//  h_7.ParseDatacard("htt/125/vhtt_5_7TeV.txt", "vhtt", "7TeV", "vhtt", 5, "125");
//  h_7.ParseDatacard("htt/125/vhtt_6_7TeV.txt", "vhtt", "7TeV", "vhtt", 6, "125");
//  h_7.ParseDatacard("htt/125/vhtt_7_7TeV.txt", "vhtt", "7TeV", "vhtt", 7, "125");
//  h_7.ParseDatacard("htt/125/vhtt_8_7TeV.txt", "vhtt", "7TeV", "vhtt", 8, "125");
  // h_8.ParseDatacard("htt/125/vhtt_0_8TeV.txt", "vhtt", "8TeV", "vhtt", 0, "125");
  // h_8.ParseDatacard("htt/125/vhtt_1_8TeV.txt", "vhtt", "8TeV", "vhtt", 1, "125");
  // h_8.ParseDatacard("htt/125/vhtt_3_8TeV.txt", "vhtt", "8TeV", "vhtt", 3, "125");
  // h_8.ParseDatacard("htt/125/vhtt_4_8TeV.txt", "vhtt", "8TeV", "vhtt", 4, "125");
  // h_8.ParseDatacard("htt/125/vhtt_5_8TeV.txt", "vhtt", "8TeV", "vhtt", 5, "125");
  // h_8.ParseDatacard("htt/125/vhtt_6_8TeV.txt", "vhtt", "8TeV", "vhtt", 6, "125");
  // h_8.ParseDatacard("htt/125/vhtt_7_8TeV.txt", "vhtt", "8TeV", "vhtt", 7, "125");
  // h_8.ParseDatacard("htt/125/vhtt_8_8TeV.txt", "vhtt", "8TeV", "vhtt", 8, "125");

  TFile fit("cmb/125/out/mlfit.root");
  RooFitResult *s_plus_b_fit = dynamic_cast<RooFitResult *>(gDirectory->Get("fit_s"));

  auto postfit_params = ch::ExtractFitParameters(*s_plus_b_fit);
  h_7.UpdateParameters(postfit_params);
  h_7.PrintAll();


  auto procs = h_7.GenerateSetFromProcs<std::string>(std::mem_fn(&ch::Process::process));

  // TFile f("output.root","RECREATE");

  // TH1F obs = h_7.GetObservedShape();
  // obs.SetName("data_obs");
  // obs.Write();

  // for (auto proc : procs) {
  //   std::cout << proc
  //     << "\t" << h_7.shallow_copy().process(true, {proc}).GetRate()
  //     << "\t" << h_7.shallow_copy().process(true, {proc}).GetUncertainty()
  //     << "\t" << h_7.shallow_copy().process(true, {proc}).GetUncertainty(s_plus_b_fit, 500)
  //     << std::endl;
  //   TH1F p_hist = h_7.shallow_copy().process(true, {proc}).GetShape();
  //   p_hist.SetName(proc.c_str());
  //   p_hist.Write();
  // }

  // TH1F bkg = h_7.shallow_copy().backgrounds().GetShapeWithUncertainty(s_plus_b_fit, 2000);
  // bkg.SetName("Bkg");
  // bkg.Write();
  // f.Close();


  auto systematics = h_7.GenerateSetFromNus<std::string>(std::mem_fn(&ch::Nuisance::name));

  for (auto const& s : systematics) {
    auto procs = h_7.shallow_copy()
      .nus_name(true, {s})
      .GenerateSetFromNus<std::string>(std::mem_fn(&ch::Nuisance::process));
    auto rates = h_7.shallow_copy()
      .nus_name(true, {s})
      .GenerateSetFromNus<double>(std::mem_fn(&ch::Nuisance::value_u));
    std::set<double> cleaned_rates;
    for (auto r : rates) {
      if (r < 1.0) {
        cleaned_rates.insert(100. * (1.0 - r));
      } else {
        cleaned_rates.insert(100. * (r - 1.0));
      }
    }
    std::cout << s << " has procs ";
    for (auto const& p : procs) std::cout << p << " ";
    std::cout << " in range " << (boost::format("%.0f") % *(cleaned_rates.begin()))
      << "% - " << (boost::format("%.0f") % *(--cleaned_rates.end())) << "%" << std::endl;
  }
  /*
  SMHiggsXSTool xs_tool;


  // Scaling cross section
  auto masses = h.get_set_from_procs<string>(&Process::mass);
  auto signals = h.shallow_copy().signals().get_set_from_procs<string>(&Process::name);

  for (auto m : masses) {
    for (auto s : signals) {
      h.shallow_copy().mass(true, m).process(true, {s})
        .scale(xs_tool.get_xsec({s},m)*xs_tool.get_br({"tt"},m));
    }
  }

  // mergeing bbb
  auto bins = h.get_set_from_procs<string>(&Process::bin);
  for (auto b : bins) {
    CombineHarvester tmp = h.shallow_copy().process("true",{"W","QCD","TT"});
    // with direct acess of to elements

  }




  */
  // h_7.PrintAll();

  // ch::CombineHarvester tmp = h_7;

  // h_7.WriteDatacard("test_card.txt", "test_file.root");

  /*
    vector<string> categories = {
      "category1",
      "category2",
      "category3"
    }

  ExtractObservations(file, categories, "data_obs");
  ExtractBackgroundProcesses(file, categories, {"ZTT", "W", "QCD"});
  ExtractSignalProccesses(file, categories, {"ggH"}, {"120","125","130"});


  AddSystematic("lumi_7TeV","lnN", 1.03,
    processes = {"ZTT", "signal")},
    era       = {"7TeV"}
  )

  h.shallow_copy().process(true, {"ZTT,"signal"}).era(true, {"8TeV"})

  AddSystematic("$CHANNEL, $ERA, $ANA")


  */

  // std::cout << "---- 7 TeV ----" << std::endl;
  // h_7.Validate();
  // std::cout << "---- 8 TeV ----" << std::endl;
  // h_8.Validate();
  // double rate = h.GetRate();
  // double err = h.GetUncertainty();


  // // for (unsigned i = 0; i < 1000; ++i) double err = h.GetRate();
  // std::cout << "Rate: " << rate << std::endl;
  // std::cout << "Err:  " << err << std::endl;

  // TH1F shape = h.shallow_copy().process(true, {"ZTT"}).GetShape();
  // TFile f("output.root","RECREATE");
  // f.cd();
  // shape.Write();
  // f.Close();
  // h.PrintAll();
}

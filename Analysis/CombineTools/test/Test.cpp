#include <string>
#include "CombineTools/interface/CombineHarvester.h"
#include "CombineTools/interface/Observation.h"
#include "CombineTools/interface/Process.h"
#include "TH1F.h"

int main() {
  // ch::Observation obs;
  // TH1 const* test = obs.shape();
  // ch::CombineHarvester h;
  // h.ParseDatacard("paper_final/125/htt_mt_1_7TeV.txt", "125", "7TeV", "mt", 1);
  // h.PrintAll();
  ch::CombineHarvester h;
  // h.ParseDatacard("cmb/125/htt_mt_1_7TeV.txt", "htt", "7TeV", "mt", 1, "125");
  // h.ParseDatacard("cmb/125/htt_mt_2_7TeV.txt", "htt", "7TeV", "mt", 2, "125");
  // h.ParseDatacard("cmb/125/htt_mt_3_7TeV.txt", "htt", "7TeV", "mt", 3, "125");
  // h.ParseDatacard("cmb/125/htt_mt_4_7TeV.txt", "htt", "7TeV", "mt", 4, "125");
  // h.ParseDatacard("cmb/125/htt_mt_5_7TeV.txt", "htt", "7TeV", "mt", 5, "125");
  // h.ParseDatacard("cmb/125/htt_mt_6_7TeV.txt", "htt", "7TeV", "mt", 6, "125");
  h.ParseDatacard("cmb/125/htt_mt_1_8TeV.txt", "htt", "8TeV", "mt", 1, "125");
  // h.ParseDatacard("cmb/125/htt_mt_2_8TeV.txt", "htt", "8TeV", "mt", 2, "125");
  // h.ParseDatacard("cmb/125/htt_mt_3_8TeV.txt", "htt", "8TeV", "mt", 3, "125");
  // h.ParseDatacard("cmb/125/htt_mt_4_8TeV.txt", "htt", "8TeV", "mt", 4, "125");
  // h.ParseDatacard("cmb/125/htt_mt_5_8TeV.txt", "htt", "8TeV", "mt", 5, "125");
  // h.ParseDatacard("cmb/125/htt_mt_6_8TeV.txt", "htt", "8TeV", "mt", 6, "125");
  // h.ParseDatacard("cmb/125/htt_mt_7_8TeV.txt", "htt", "8TeV", "mt", 7, "125");

  // h.ParseDatacard("cmb/125/htt_et_1_7TeV.txt", "htt", "7TeV", "et", 1, "125");
  // h.ParseDatacard("cmb/125/htt_et_2_7TeV.txt", "htt", "7TeV", "et", 2, "125");
  // h.ParseDatacard("cmb/125/htt_et_3_7TeV.txt", "htt", "7TeV", "et", 3, "125");
  // h.ParseDatacard("cmb/125/htt_et_5_7TeV.txt", "htt", "7TeV", "et", 5, "125");
  // h.ParseDatacard("cmb/125/htt_et_6_7TeV.txt", "htt", "7TeV", "et", 6, "125");
  // h.ParseDatacard("cmb/125/htt_et_1_8TeV.txt", "htt", "8TeV", "et", 1, "125");
  // h.ParseDatacard("cmb/125/htt_et_2_8TeV.txt", "htt", "8TeV", "et", 2, "125");
  // h.ParseDatacard("cmb/125/htt_et_3_8TeV.txt", "htt", "8TeV", "et", 3, "125");
  // h.ParseDatacard("cmb/125/htt_et_5_8TeV.txt", "htt", "8TeV", "et", 5, "125");
  // h.ParseDatacard("cmb/125/htt_et_6_8TeV.txt", "htt", "8TeV", "et", 6, "125");
  // h.ParseDatacard("cmb/125/htt_et_7_8TeV.txt", "htt", "8TeV", "et", 7, "125");

  // h.ParseDatacard("cmb/125/htt_em_0_7TeV.txt", "htt", "7TeV", "em", 0, "125");
  // h.ParseDatacard("cmb/125/htt_em_1_7TeV.txt", "htt", "7TeV", "em", 1, "125");
  // h.ParseDatacard("cmb/125/htt_em_2_7TeV.txt", "htt", "7TeV", "em", 2, "125");
  // h.ParseDatacard("cmb/125/htt_em_3_7TeV.txt", "htt", "7TeV", "em", 3, "125");
  // h.ParseDatacard("cmb/125/htt_em_4_7TeV.txt", "htt", "7TeV", "em", 4, "125");
  // h.ParseDatacard("cmb/125/htt_em_0_8TeV.txt", "htt", "8TeV", "em", 0, "125");
  // h.ParseDatacard("cmb/125/htt_em_1_8TeV.txt", "htt", "8TeV", "em", 1, "125");
  // h.ParseDatacard("cmb/125/htt_em_2_8TeV.txt", "htt", "8TeV", "em", 2, "125");
  // h.ParseDatacard("cmb/125/htt_em_3_8TeV.txt", "htt", "8TeV", "em", 3, "125");
  // h.ParseDatacard("cmb/125/htt_em_4_8TeV.txt", "htt", "8TeV", "em", 4, "125");
  // h.ParseDatacard("cmb/125/htt_em_5_8TeV.txt", "htt", "8TeV", "em", 5, "125");

  // h.ParseDatacard("cmb/125/htt_ee_0_7TeV.txt", "htt", "7TeV", "ee", 0, "125");
  // h.ParseDatacard("cmb/125/htt_ee_1_7TeV.txt", "htt", "7TeV", "ee", 1, "125");
  // h.ParseDatacard("cmb/125/htt_ee_2_7TeV.txt", "htt", "7TeV", "ee", 2, "125");
  // h.ParseDatacard("cmb/125/htt_ee_3_7TeV.txt", "htt", "7TeV", "ee", 3, "125");
  // h.ParseDatacard("cmb/125/htt_ee_4_7TeV.txt", "htt", "7TeV", "ee", 4, "125");
  // h.ParseDatacard("cmb/125/htt_ee_0_8TeV.txt", "htt", "8TeV", "ee", 0, "125");
  // h.ParseDatacard("cmb/125/htt_ee_1_8TeV.txt", "htt", "8TeV", "ee", 1, "125");
  // h.ParseDatacard("cmb/125/htt_ee_2_8TeV.txt", "htt", "8TeV", "ee", 2, "125");
  // h.ParseDatacard("cmb/125/htt_ee_3_8TeV.txt", "htt", "8TeV", "ee", 3, "125");
  // h.ParseDatacard("cmb/125/htt_ee_4_8TeV.txt", "htt", "8TeV", "ee", 4, "125");

  // h.ParseDatacard("cmb/125/htt_mm_0_7TeV.txt", "htt", "7TeV", "mm", 0, "125");
  // h.ParseDatacard("cmb/125/htt_mm_1_7TeV.txt", "htt", "7TeV", "mm", 1, "125");
  // h.ParseDatacard("cmb/125/htt_mm_2_7TeV.txt", "htt", "7TeV", "mm", 2, "125");
  // h.ParseDatacard("cmb/125/htt_mm_3_7TeV.txt", "htt", "7TeV", "mm", 3, "125");
  // h.ParseDatacard("cmb/125/htt_mm_4_7TeV.txt", "htt", "7TeV", "mm", 4, "125");
  // h.ParseDatacard("cmb/125/htt_mm_0_8TeV.txt", "htt", "8TeV", "mm", 0, "125");
  // h.ParseDatacard("cmb/125/htt_mm_1_8TeV.txt", "htt", "8TeV", "mm", 1, "125");
  // h.ParseDatacard("cmb/125/htt_mm_2_8TeV.txt", "htt", "8TeV", "mm", 2, "125");
  // h.ParseDatacard("cmb/125/htt_mm_3_8TeV.txt", "htt", "8TeV", "mm", 3, "125");
  // h.ParseDatacard("cmb/125/htt_mm_4_8TeV.txt", "htt", "8TeV", "mm", 4, "125");

  // h.ParseDatacard("cmb/125/htt_tt_0_8TeV.txt", "htt", "8TeV", "tt", 0, "125");
  // h.ParseDatacard("cmb/125/htt_tt_1_8TeV.txt", "htt", "8TeV", "tt", 1, "125");
  // h.ParseDatacard("cmb/125/htt_tt_2_8TeV.txt", "htt", "8TeV", "tt", 2, "125");

  // h.ParseDatacard("cmb/125/vhtt_0_7TeV.txt", "vhtt", "7TeV", "vhtt", 0, "125");
  // h.ParseDatacard("cmb/125/vhtt_1_7TeV.txt", "vhtt", "7TeV", "vhtt", 1, "125");
  // h.ParseDatacard("cmb/125/vhtt_3_7TeV.txt", "vhtt", "7TeV", "vhtt", 3, "125");
  // h.ParseDatacard("cmb/125/vhtt_4_7TeV.txt", "vhtt", "7TeV", "vhtt", 4, "125");
  // h.ParseDatacard("cmb/125/vhtt_5_7TeV.txt", "vhtt", "7TeV", "vhtt", 5, "125");
  // h.ParseDatacard("cmb/125/vhtt_6_7TeV.txt", "vhtt", "7TeV", "vhtt", 6, "125");
  // h.ParseDatacard("cmb/125/vhtt_7_7TeV.txt", "vhtt", "7TeV", "vhtt", 7, "125");
  // h.ParseDatacard("cmb/125/vhtt_8_7TeV.txt", "vhtt", "7TeV", "vhtt", 8, "125");
  // h.ParseDatacard("cmb/125/vhtt_0_8TeV.txt", "vhtt", "8TeV", "vhtt", 0, "125");
  // h.ParseDatacard("cmb/125/vhtt_1_8TeV.txt", "vhtt", "8TeV", "vhtt", 1, "125");
  // h.ParseDatacard("cmb/125/vhtt_3_8TeV.txt", "vhtt", "8TeV", "vhtt", 3, "125");
  // h.ParseDatacard("cmb/125/vhtt_4_8TeV.txt", "vhtt", "8TeV", "vhtt", 4, "125");
  // h.ParseDatacard("cmb/125/vhtt_5_8TeV.txt", "vhtt", "8TeV", "vhtt", 5, "125");
  // h.ParseDatacard("cmb/125/vhtt_6_8TeV.txt", "vhtt", "8TeV", "vhtt", 6, "125");
  // h.ParseDatacard("cmb/125/vhtt_7_8TeV.txt", "vhtt", "8TeV", "vhtt", 7, "125");
  // h.ParseDatacard("cmb/125/vhtt_8_8TeV.txt", "vhtt", "8TeV", "vhtt", 8, "125");
  double rate = h.GetRate(); 
  double err = h.GetUncertainty(); 


  // for (unsigned i = 0; i < 1000; ++i) double err = h.GetRate();
  std::cout << "Rate: " << rate << std::endl;
  std::cout << "Err:  " << err << std::endl;

  TH1F shape = h.shallow_copy().process(true, {"ZTT"}).GetShape();
  TFile f("output.root","RECREATE");
  f.cd();
  shape.Write();
  f.Close();
  h.PrintAll();
}

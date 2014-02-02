#include <string>
#include "CombineTools/interface/CombineHarvester.h"
#include "CombineTools/interface/Observation.h"
#include "CombineTools/interface/Process.h"
#include "TH1.h"

int main() {
  // ch::Observation obs;
  // TH1 const* test = obs.shape();
  // ch::CombineHarvester h;
  // h.ParseDatacard("paper_final/125/htt_mt_1_7TeV.txt", "125", "7TeV", "mt", 1);
  // h.PrintAll();

  ch::CombineHarvester h;
  // h.ParseDatacard("cmb/125/htt_mt_1_8TeV.txt", "htt", "8TeV", "mt", 1, "125");
  // h.ParseDatacard("cmb/125/htt_mt_2_8TeV.txt", "htt", "8TeV", "mt", 2, "125");
  // h.ParseDatacard("cmb/125/htt_mt_3_8TeV.txt", "htt", "8TeV", "mt", 3, "125");
  // h.ParseDatacard("cmb/125/htt_mt_4_8TeV.txt", "htt", "8TeV", "mt", 4, "125");
  // h.ParseDatacard("cmb/125/htt_mt_5_8TeV.txt", "htt", "8TeV", "mt", 5, "125");
  // h.ParseDatacard("cmb/125/htt_mt_6_8TeV.txt", "htt", "8TeV", "mt", 6, "125");
  // h.ParseDatacard("cmb/125/htt_mt_7_8TeV.txt", "htt", "8TeV", "mt", 7, "125");
  h.ParseDatacard("cmb/125/vhtt_0_8TeV.txt", "vhtt", "8TeV", "vhtt", 0, "125");
  h.ParseDatacard("cmb/125/vhtt_1_8TeV.txt", "vhtt", "8TeV", "vhtt", 1, "125");
  h.ParseDatacard("cmb/125/vhtt_3_8TeV.txt", "vhtt", "8TeV", "vhtt", 3, "125");
  h.ParseDatacard("cmb/125/vhtt_4_8TeV.txt", "vhtt", "8TeV", "vhtt", 4, "125");
  h.ParseDatacard("cmb/125/vhtt_5_8TeV.txt", "vhtt", "8TeV", "vhtt", 5, "125");
  h.ParseDatacard("cmb/125/vhtt_6_8TeV.txt", "vhtt", "8TeV", "vhtt", 6, "125");
  h.ParseDatacard("cmb/125/vhtt_7_8TeV.txt", "vhtt", "8TeV", "vhtt", 7, "125");
  h.ParseDatacard("cmb/125/vhtt_8_8TeV.txt", "vhtt", "8TeV", "vhtt", 8, "125");
  h.PrintAll();
}

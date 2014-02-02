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
  h.ParseDatacard("cmb/125/htt_mt_1_8TeV.txt", "htt", "8TeV", "mt", 1, "125");
  ch::CombineHarvester s;
  s = h;
}

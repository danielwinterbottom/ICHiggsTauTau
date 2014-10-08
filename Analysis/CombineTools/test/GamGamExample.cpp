#include <string>
#include <TFile.h>
#include "CombineTools/interface/CombineHarvester.h"

using namespace std;

int main() {
  ch::CombineHarvester cb;
  cb.SetVerbosity(3);
  cb.ParseDatacard("output/CMSDAS/hgg_8TeV_MVA_cat0145.txt", "hgg", "8TeV",
                   "hgg", 0, "*");

  TFile fout("hgg_output.root", "RECREATE");

  cb.WriteDatacard("hgg_output.txt", fout);

  fout.Close();

  cb.PrintAll();
}

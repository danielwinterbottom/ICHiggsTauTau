#include <vector>
#include <iostream>
#include <fstream>
#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TLatex.h"
#include "math.h"

int main(int argc, char *argv[]){
  //std::cout<<"processing file "<<argv[1]<<std::endl;
  TFile *f1 = new TFile(argv[1]);
  TH1F *isordersame;
  f1->GetObject("JES/JESisordersame",isordersame);
  double numentries = isordersame->GetEntries();
  double numdifferent = isordersame->GetBinContent(25);
  double numswapped = isordersame->GetBinContent(19);
  double percdiff=(numdifferent/numentries)*100;
  double percswap=(numswapped/numentries)*100;
  std::cout<<" "<<percdiff<<" %"<<std::endl;
  //std::cout<<"There are "<<numentries<<" events and "<<numdifferent<<" have different jets. That's "<<percdiff<<"%"<<std::endl;
  //std::cout<<"There are "<<numentries<<" events and "<<numswapped<<" have swapped jets. That's "<<percswap<<"%"<<std::endl;

  return 0;
}

#include <iostream>
#include <string>
#include "TFile.h"
#include "TTree.h"

#include<string>

int main(int argc, char* argv[]) {


  if (argc != 2){
    std::cerr << "Need 1 arg: <input>" << std::endl;
    return 1;
  }
  std::string input = argv[1];

  TFile f_in(input.c_str());
  TTree *t_in = dynamic_cast<TTree*>(f_in.Get("svfit"));
  if (!t_in) return 1;
  int entries_in = t_in->GetEntries();
  
  std::string output = input;
  if (output.find("input.root") != input.npos) {
    std::size_t pos = output.find("input.root");
    output.replace(pos, std::string("input.root").length(), "output.root");
  } else {
    return 1;
  }

  TFile f_out(output.c_str());
  TTree *t_out = dynamic_cast<TTree*>(f_out.Get("svfit"));
  if (!t_out) return 1;
  int entries_out = t_out->GetEntries();

  if (entries_in == entries_out) {
    return 0;
  } else {
    return 1;
  }

}



#include <iostream>
#include <vector>
#include <string>
#include "boost/program_options.hpp"
#include "boost/lexical_cast.hpp"
#include "TFile.h"
#include "TFileMerger.h"
#include "Utilities/interface/FnRootTools.h"


namespace po = boost::program_options;

using std::string;
using std::vector;

int main(int argc, char* argv[]) {
  // Input options
  string filelist;
  string prefix;
  string output;
  bool is_local;
  double size_limit; // in MB

  po::variables_map vm;
  po::options_description config("Configuration");
  config.add_options()
      ("filelist",    po::value<string>   (&filelist)->    required())
      ("prefix",      po::value<string>   (&prefix)->      default_value(""))
      ("output",      po::value<string>   (&output)->      required())
      ("is_local",    po::value<bool>     (&is_local)->   required())
      ("size_limit",  po::value<double>   (&size_limit)->  required());
  po::store(po::command_line_parser(argc, argv).options(config).run(), vm);
  po::notify(vm);

  vector<string> files = ic::ParseFileLines(filelist);
  if (files.size() == 0) {
    std::cout << "Error, filelist is missing or empty" << std::endl;
    exit(1);
  }
  for (unsigned i = 0; i < files.size(); ++i) files[i] = prefix + files[i];

  double current_size = 0.0;
  std::vector<TFile *> current_files;
  unsigned counter = 1;
  for (unsigned i = 0; i < files.size(); ++i) {
    current_files.push_back(TFile::Open(files[i].c_str()));
    TFile * current_file = current_files.back();
    double size = static_cast<double>(current_file->GetSize());
    std::cout << i << " " << size << "\n";
    // if (((current_size + size) > size_limit) || (i+1) == files.size()) {
    //   TFileMerger merger(is_local);
    //   merger.OutputFile((output + "_" +
    //                      boost::lexical_cast<string>(counter)+".root").c_str());
    //   for (unsigned j = 0; j < current_files.size(); ++j) {
    //     merger.AddFile(current_files[j]);
    //     merger.Merge();
    //   }
    //   ++counter;
    //   for (unsigned j = 0; j < current_files.size(); ++j) {
    //     current_files[i]->Close();
    //   }
    //   current_files.clear();
    //   current_size = 0.0;
    // }
    current_file->Close();
  }
}

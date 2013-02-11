#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include <boost/lexical_cast.hpp>
#include <fstream>


namespace ic {

  std::vector<std::string> ParseFileLines(std::string const& file_name) {
    // Build a vector of input files
    std::vector<std::string> files;
    std::ifstream file;
    file.open(file_name.c_str());
    if (!file.is_open()) {
      std::cerr << "Warning: File " << file_name << " cannot be opened." << std::endl;
      return files;
    }
    std::string line = "";
    while(std::getline(file, line)) {//while loop through lines
      files.push_back(line);
    }
    file.close();
    return files;
  }

  // Vertical morphing between template, based on implementation here:
  // http://root.cern.ch/root/html532/src/RooStats__HistFactory__FlexibleInterpVar.cxx.html#193
  void VerticalMorph(TH1F *central, TH1F const*up, TH1F const*down, double shift) {

    int nbins = central->GetNbinsX();
    if (up->GetNbinsX() != nbins || down->GetNbinsX() != nbins) {
      std::cerr << "Error in VerticalMorph: up and down histograms do not have the same binning" << std::endl;
    }
    for (int i = 1; i <= nbins; ++i) {
      double high = up->GetBinContent(i);
      double low = down->GetBinContent(i);
      double nominal = central->GetBinContent(i);
      double total = nominal;
      double a = 0.5*(high+low)-nominal;
      double b = 0.5*(high-low);
      double c = 0;
      if(shift > 1.0) {
        total += (2.0*a+b)*(shift-1.0)+high-nominal;
      } else if (shift < -1.0) {
        total += -1.0*(2.0*a-b)*(shift+1.0)+low-nominal;
      } else {
        total += a*pow(shift,2) + b*shift+c;
      }
      if (total <= 0) total = 1E-9;
      central->SetBinContent(i, total);
      std::cout << "Bin " << i <<": Nominal " << nominal << "\tDown " << low << "\tUp " << high << "\tResult " << total << std::endl;
    }
  }


} //namespace

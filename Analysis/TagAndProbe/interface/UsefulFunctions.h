#ifndef ICHiggsTauTau_Analysis_TagAndProbe_UsefulFunctions_h
#define IcHiggsTauTau_Analysis_TagAndProbe_UsefulFunctions_h
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include <cmath>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include "Math/VectorUtil.h"
#include "UserCode/ICHiggsTauTau//interface/Objects.hh"


namespace ic {

  template <class T, class U> void erase_if(T & t, U pred) {
    t.erase(std::remove_if(t.begin(), t.end(), pred), t.end());
  }

  
  double DR(Candidate const cand1, Candidate const cand2); 
  bool IsFilterMatched(Candidate const cand, std::vector<TriggerObject> const& objs, std::string const& filter, double const& max_dr);
  bool ElectronHTTId(Electron const& elec, unsigned const& mode);
  bool MuonHTTId(Muon const& muon); 
  int testfunction(bool test);
  std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems); 
  double PropError(double y1, double err_y1, double y2, double err_y2);

  template<class T>
  double PF04IsolationVal(T const cand, double const& dbeta) {
    double iso =  cand.dr04_pfiso_charged_all() 
                  + std::max(cand.dr04_pfiso_neutral() + cand.dr04_pfiso_gamma() - dbeta * cand.dr04_pfiso_pu(), 0.0);
    iso = iso / cand.pt();
    return iso;

  }

}
#endif

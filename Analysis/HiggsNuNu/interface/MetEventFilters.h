#ifndef ICHiggsTauTau_Module_MetEventFilters_h
#define ICHiggsTauTau_Module_MetEventFilters_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HinvPrint.h"

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <set>
#include "boost/functional/hash.hpp"

namespace ic {

  std::istream & operator>>(std::istream & is, RunLumiEvent & evt){
    std::string lLine;
    is>>lLine;
    std::vector<std::string> lVec;
    boost::split(lVec, lLine, boost::is_any_of(":"));
    if (lLine.size()< 6) return is;
    if (lVec.size() != 3) {
      std::cout << "---- Error reading laser filter file! String is: " << lLine << ", expect run:lumiBlock:event. Quitting..." << std::endl;
      throw;
    }
    else {
      std::istringstream(lVec[0])>>evt.run; 
      std::istringstream(lVec[1])>>evt.lumi;
      std::istringstream(lVec[2])>>evt.evt;
    } 
    return is; 
  }

  typedef std::set <RunLumiEvent> evtsArray;

  std::istream & operator>>(std::istream & is, evtsArray & evtSet){

    RunLumiEvent temp; 
    while(1){
      is>>temp; 
      if(is.eof()){
	break; 
      }
      evtSet.insert(temp);
    }
    return is; 
  }
 
  class MetEventFilters : public ModuleBase {
  private:
    std::vector<std::string> input_vec_;
    bool doFilters_;
    
    evtsArray badEvts_;

    bool extractEvents(std::string inputfile);

    unsigned countRejected_;

  public:
    MetEventFilters(std::string const& name, 
		    std::vector<std::string> input_vec, 
		    bool doFilters);
    virtual ~MetEventFilters();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
  };

}//namespace


#endif

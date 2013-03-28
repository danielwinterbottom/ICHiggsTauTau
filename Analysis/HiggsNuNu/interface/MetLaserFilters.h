#ifndef ICHiggsTauTau_Module_MetLaserFilters_h
#define ICHiggsTauTau_Module_MetLaserFilters_h

#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/interface/Candidate.hh"

#include <string>
#include <boost/algorithm/string.hpp>
#include <set>
#include "boost/functional/hash.hpp"

namespace ic {

  struct BadEvent {
    int run;
    int lumi;
    unsigned event;

    std::size_t RunLumiEvtHash() const {
      std::size_t id = 0;
      boost::hash_combine(id, run);
      boost::hash_combine(id, lumi);
      boost::hash_combine(id, event);
      return id;
    }


  };
  
  std::istream & operator>>(std::istream & is, BadEvent & evt){
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
      std::istringstream(lVec[2])>>evt.event;
    } 
    return is; 
  }

  typedef std::set <std::size_t> evtsHashArray;

  std::istream & operator>>(std::istream & is, evtsHashArray & evtSet){

    BadEvent temp; 
    while(1){
      is>>temp; 
      if(is.eof()){
	break; 
      }
      evtSet.insert(temp.RunLumiEvtHash()); 
    }
    return is; 
  }
 
  class MetLaserFilters : public ModuleBase {
  private:
    std::string hcal_input_name_;
    std::string ecal_input_name_;
    bool doFilters_;

    evtsHashArray badEvts_;

    bool extractEvents(std::string inputfile);

  public:
    MetLaserFilters(std::string const& name, 
		    std::string hcal_input_name, 
		    std::string ecal_input_name, 
		    bool doFilters);
    virtual ~MetLaserFilters();

    virtual int PreAnalysis();
    virtual int Execute(TreeEvent *event);
    virtual int PostAnalysis();
    virtual void PrintInfo();
    
  };

}//namespace


#endif

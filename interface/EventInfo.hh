#ifndef ICHiggsTauTau_EventInfo_hh
#define ICHiggsTauTau_EventInfo_hh
#include <string>
#include <map>
#include <iostream>
#include "UserCode/ICHiggsTauTau/interface/city.h"


namespace ic {

  class EventInfo {

  private:
    typedef std::map<std::string, double> SDMap;
    typedef std::map<std::string, bool> SBMap;
    typedef std::map<std::size_t, float> TBMap;

  public:
    EventInfo();
    virtual ~EventInfo();

    inline bool is_data() const { return is_data_; }
    inline void set_is_data(bool const& is_data) { is_data_ = is_data; }

    inline unsigned event() const { return event_; }
    inline void set_event(unsigned const& event) { event_ = event; }

    inline int run() const { return run_; }
    inline void set_run(int const& run) { run_ = run; }

    inline int lumi_block() const { return lumi_block_; }
    inline void set_lumi_block(int const& lumi_block) { lumi_block_ = lumi_block; }

    inline int bunch_crossing() const { return bunch_crossing_; }
    inline void set_bunch_crossing(int const& bunch_crossing) { bunch_crossing_ = bunch_crossing; }

    inline double jet_rho() const { return jet_rho_; }
    inline void set_jet_rho(double const& jet_rho) { jet_rho_ = jet_rho; }

    inline double lepton_rho() const { return lepton_rho_; }
    inline void set_lepton_rho(double const& lepton_rho) { lepton_rho_ = lepton_rho; }

    inline unsigned good_vertices() const { return good_vertices_; }
    inline void set_good_vertices(unsigned const& good_vertices) { good_vertices_ = good_vertices; }

    inline double weight(std::string label) const {
      SDMap::const_iterator it = weights_.find(label);
      if (it != weights_.end()) {
        return it->second;
      } else {
        std::cerr << "Weight \"" << label << "\" not found!" << std::endl;
        return 1.0;
      }
    }

    inline bool weight_defined(std::string label) const {
      SDMap::const_iterator it = weights_.find(label);
      return it != weights_.end();
    }

    inline void set_weight(std::string const& label, double const& weight, bool const& enabled) {
      if (weight != weight) {
	std::cerr << " -- weight " << label << " has NAN value, setting to 1..." << std::endl;
	//weight = 1.;
	weights_[label] = 1.;
      }
      else weights_[label] = weight;
      weight_status_[label] = enabled;
    }

    inline void set_weight(std::string label, double const& weight) {
      bool enabled = true;
      if (label.size() > 0) {
        if (label.at(0) == '!') {
          label.erase(0,1);
          enabled = false;
        }
      }
      if (weight != weight) {
	std::cerr << " -- weight " << label << " has NAN value, setting to 1..." << std::endl;
	weights_[label] = 1.0;
      }
      else weights_[label] = weight;
      weight_status_[label] = enabled;
    }


    // Product of all weights, ignoring those starting with the character '!'
    inline double total_weight() const {
      SDMap::const_iterator it;
      double weight = 1.0;
      for (it = weights_.begin(); it != weights_.end(); ++it) {
        SBMap::const_iterator st_it = weight_status_.find(it->first);
        if (st_it != weight_status_.end()) {
          if (!st_it->second) continue;
        }
        weight = it->second * weight;
      }
      return weight;
    }

    inline bool weight_is_enabled(std::string label) {
      if (weight_defined(label)) {
        return weight_status_[label];
      } else {
        return false;
      }
    }

    inline void enable_weight(std::string label) {
      if (weight_defined(label)) { 
        weight_status_[label] = true;
      } 
    }

    inline void disable_weight(std::string label) {
      if (weight_defined(label)) { 
        weight_status_[label] = false;
      } 
    }

    inline TBMap const& filters() const { return filters_; }
    inline void set_filters(TBMap const& filters) { filters_ = filters; }

    inline void set_filter_result(std::string const& label, bool const& result) {
      filters_[CityHash64(label)] = result;
    }
    inline bool filter_result(std::string const& label) {
      TBMap::const_iterator it = filters_.find(CityHash64(label));
      if (it != filters_.end()) {
        return it->second;
      } else {
        std::cerr << "Filter \"" << label << "\" not found!" << std::endl;
        return true;
      }    
    }
    inline bool total_filter_result() const {
      TBMap::const_iterator it;
      bool result = true;
      for (it = filters_.begin(); it != filters_.end(); ++it) {
        result = it->second && result;
      }
      return result;
    }




virtual void Print() const;


private:
      bool is_data_;
      unsigned event_;
      int run_;
      int lumi_block_;
      int bunch_crossing_;
      double jet_rho_;
      double lepton_rho_;
      SDMap weights_;
      SBMap weight_status_;
      unsigned good_vertices_;
      TBMap filters_;

};

}
#endif

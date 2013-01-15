#ifndef ICHiggsTauTau_EventInfo_hh
#define ICHiggsTauTau_EventInfo_hh
#include <string>
#include <map>
#include <iostream>

namespace ic {

  class EventInfo {

  private:
    typedef std::map<std::string, double> SDMap;

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

    inline void set_weight(std::string const& label, double const& weight) {
      weights_[label] = weight;
    }

    inline double total_weight() const {
      SDMap::const_iterator it;
      double weight = 1.0;
      for (it = weights_.begin(); it != weights_.end(); ++it) {
        weight = it->second * weight;
      }
      return weight;
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
      unsigned good_vertices_;

};

}
#endif

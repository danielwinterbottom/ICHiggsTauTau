#ifndef ICHiggsTauTau_EventInfo_hh
#define ICHiggsTauTau_EventInfo_hh

#ifdef __CINT__
typedef char __signed;
typedef char int8_t;
#endif

#include <string>
#include <map>
#include <iostream>
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "Rtypes.h"

namespace ic {

/**
 * @brief Stores core event information such as run, lumi and event number, as
 * well as event weights and filter results.
 */
class EventInfo {
 private:
  typedef std::map<std::string, double> SDMap;
  typedef std::map<std::string, bool> SBMap;
  typedef std::map<std::size_t, float> TBMap;

 public:
  EventInfo();
  virtual ~EventInfo();
  virtual void Print() const;

  /// @name Properties
  /**@{*/
  /// If event is real data returns `true`, otherwise `false`
  inline bool is_data() const { return is_data_; }

  /// Event number
  inline unsigned long long event() const { return event_; }

  /// Run number
  inline int run() const { return run_; }

  /// Lumisection number
  inline int lumi_block() const { return lumi_block_; }

  /// Bunch crossing number
  inline int bunch_crossing() const { return bunch_crossing_; }

  /// Energy density used for the jet energy corrections in this event
  inline double jet_rho() const { return jet_rho_; }

  /// Energy density used for calculating lepton isolation in this event
  inline double lepton_rho() const { return lepton_rho_; }

  /// Generator level HT, used for combining HT binned samples with inclusive samples
  inline double gen_ht() const { return gen_ht_; }


  /// Generator level M_ll
  inline double gen_mll() const { return gen_mll_; }

  /// Number of outgoing partons at generator level, used for combining n-jet binned samples with inclusive samples
  inline unsigned n_outgoing_partons() const { return n_outgoing_partons_; }

  /// Number of reconstructed vertices passing some baseline quality
  /// requirements
  inline unsigned good_vertices() const { return good_vertices_; }
  /**@}*/

  /// @name Setters
  /**@{*/
  /// @copybrief is_data()
  inline void set_is_data(bool const& is_data) { is_data_ = is_data; }

  /// @copybrief event()
  inline void set_event(unsigned long long const& event) { event_ = event; }

  /// @copybrief run()
  inline void set_run(int const& run) { run_ = run; }

  /// @copybrief lumi_block()
  inline void set_lumi_block(int const& lumi_block) {
    lumi_block_ = lumi_block;
  }

  /// @copybrief bunch_crossing()
  inline void set_bunch_crossing(int const& bunch_crossing) {
    bunch_crossing_ = bunch_crossing;
  }

  /// @copybrief jet_rho()
  inline void set_jet_rho(double const& jet_rho) { jet_rho_ = jet_rho; }

  /// @copybrief lepton_rho()
  inline void set_lepton_rho(double const& lepton_rho) {
    lepton_rho_ = lepton_rho;
  }

  /// @copybrief gen_ht()
  inline void set_gen_ht(double const& gen_ht) { gen_ht_ = gen_ht; }


  /// @copybrief gen_mll()
  inline void set_gen_mll(double const& gen_mll) { gen_mll_ = gen_mll; }

  /// @copybrief n_outgoing_partons()
  inline void set_n_outgoing_partons(unsigned const& n_outgoing_partons) { n_outgoing_partons_ = n_outgoing_partons; }

  /// @copybrief good_vertices()
  inline void set_good_vertices(unsigned const& good_vertices) {
    good_vertices_ = good_vertices;
  }
  /**@}*/

  /// @name Event weights
  /**@{*/
  /**
   * @brief Get the the value of a specific weight
   * @return The value of the weight if defined, 1.0 otherwise
   */
  inline double weight(std::string label) const {
    SDMap::const_iterator it = weights_.find(label);
    if (it != weights_.end()) {
      return it->second;
    } else {
      std::cerr << "Weight \"" << label << "\" not found!" << std::endl;
      return 1.0;
    }
  }

  /**
   * @brief Check if a specific weight is defined
   * @return `true` if defined, `false` otherwise
   */
  inline bool weight_defined(std::string label) const {
    SDMap::const_iterator it = weights_.find(label);
    return it != weights_.end();
  }

  /**
   * @brief Add a new weight, overriding any existing value with the same label
   * @param label The weight label
   * @param weight The weight value
   * @param enabled Whether the weight should be enabled when calculating the total_weight()
   */
  inline void set_weight(std::string const& label, double const& weight,
                         bool const& enabled) {
    if (weight != weight) {
      std::cerr << " -- weight " << label << " has NAN value, setting to 1..."
                << std::endl;
      weights_[label] = 1.;
    } else {
      weights_[label] = weight;
    }
    weight_status_[label] = enabled;
  }

  /**
   * @brief Add a new weight. If label begins with the character '!' this weight
   *will be disabled when calculating the total_weight(), and the '!' will be
   *stripped from the stored label
   * @param label The weight label
   * @param weight The weight value
   */
  inline void set_weight(std::string label, double const& weight) {
    bool enabled = true;
    if (label.size() > 0) {
      if (label.at(0) == '!') {
        label.erase(0, 1);
        enabled = false;
      }
    }
    if (weight != weight) {
      std::cerr << " -- weight " << label << " has NAN value, setting to 1..."
                << std::endl;
      weights_[label] = 1.0;
    } else {
      weights_[label] = weight;
    }
    weight_status_[label] = enabled;
  }

  /// Calculate the product of all stored and enabled weights
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

  /// Return `true` if the weight with `label` is enabled, `false` otherwise
  inline bool weight_is_enabled(std::string label) {
    if (weight_defined(label)) {
      return weight_status_[label];
    } else {
      return false;
    }
  }

  /// Enable the weight with `label` in the total_weight() calculation
  inline void enable_weight(std::string label) {
    if (weight_defined(label)) {
      weight_status_[label] = true;
    }
  }

  /// Disable the weight with `label` in the total_weight() calculation
  inline void disable_weight(std::string label) {
    if (weight_defined(label)) {
      weight_status_[label] = false;
    }
  }
  /**@}*/

  /// @name Event filters
  /**@{*/
  /// Get the map containing all filter results. The map key is the hash of
  /// the original string chosen to identify each filter
  inline std::map<std::size_t, float> const& filters() const {
    return filters_;
  }

  /// Set the complete filter result map to a new value
  inline void set_filters(std::map<std::size_t, float> const& filters) {
    filters_ = filters;
  }

  /// Set the filter result for `label` to `result`, overwriting any existing
  /// filter result with this label
  inline void set_filter_result(std::string const& label, bool const& result) {
    filters_[CityHash64(label)] = result;
  }

  /// Get the filter result for `label`, return `true` if not defined
  inline bool filter_result(std::string const& label) {
    TBMap::const_iterator it = filters_.find(CityHash64(label));
    if (it != filters_.end()) {
      return it->second;
    } else {
      std::cerr << "Filter \"" << label << "\" not found!" << std::endl;
      return true;
    }
  }

  /// Returns `true` if all filter results are `true`, `false` otherwise
  inline bool total_filter_result() const {
    TBMap::const_iterator it;
    bool result = true;
    for (it = filters_.begin(); it != filters_.end(); ++it) {
      result = it->second && result;
    }
    return result;
  }
  /**@}*/

 private:
  bool is_data_;
  unsigned long long event_;
  int run_;
  int lumi_block_;
  int bunch_crossing_;
  double jet_rho_;
  double lepton_rho_;
  double gen_ht_;
  unsigned n_outgoing_partons_;
  double gen_mll_;
  SDMap weights_;
  SBMap weight_status_;
  unsigned good_vertices_;
  TBMap filters_;

 #ifndef SKIP_CINT_DICT
 public:
  ClassDef(EventInfo, 7);
 #endif
};
}
/** \example plugins/ICEventInfoProducer.cc */
#endif

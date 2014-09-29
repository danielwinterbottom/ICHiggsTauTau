#ifndef RooHttSMPdf_h
#define RooHttSMPdf_h
#include <map>
#include "RooHistPdf.h"
#include "RooDataHist.h"
#include "RooRealProxy.h"
#include "RooArgSet.h"
#include "RooAbsReal.h"
#include "TH1F.h"
#include "Rtypes.h"

// #include <memory>
// #include <string>
// #include "TH1.h"
// #include "CombineTools/interface/MakeUnique.h"

class RooHttSMPdf : public RooHistPdf {
 private:
  mutable std::map<double, TH1F> hmap_;
  typedef std::map<double, TH1F>::const_iterator MapIter;
  RooRealProxy mh_;
  mutable double current_mh_;
  bool can_morph_;
  std::vector<double> binning_;


  /*
  Things we need to store look like this:
  TAxis final_binning
  TAxis morph_binning

  at each masspoint {

    array: nominal
    v_array: +shapes
    v_array: -shapes

    array:

  }


  */

 public:
  RooHttSMPdf();
  RooHttSMPdf(const char* name, const char* title, const RooArgSet& vars,
              const RooDataHist& dhist, RooAbsReal& mh);
  RooHttSMPdf(const RooHttSMPdf& other, const char* name = 0);
  virtual ~RooHttSMPdf() {}
  virtual TObject* clone(const char* newname) const {
    return new RooHttSMPdf(*this, newname);
  }

  void AddPoint(double point, TH1F hist);
  void SetBinning(std::vector<double> const& binning);

 protected:
  /*
  Some thoughts:
   - need to implement a clone method
   - some method for workspace import, and test this
   - getting class into combine code
   - creating norm object
   - rebinning
   - tidy up constructor - do we want to do ownership?
  */

  TH1F morph(TH1F const& hist1, TH1F const& hist2, double par1, double par2,
             double parinterp) const;

  void UpdateDataHist(TH1F const& hist) const;

  virtual Double_t evaluate() const;

 public:
  ClassDef(RooHttSMPdf, 1);
};

#endif

#ifndef ICHiggsTauTau_HiggsNuNu_LTPlotElement_h                                                                       
#define ICHiggsTauTau_HiggsNuNu_LTPlotElement_h
#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/HiggsNuNuAnalysisTools.h"
#include <string>
#include <vector>
#include <utility>
#include "TH1F.h"
#include "TObject.h"

namespace ic{
  class LTPlotElement{
  public:
    LTPlotElement();
    ~LTPlotElement();
    void ApplyStyle();
  protected:
    CLASS_MEMBER(LTPlotElement,int,color)
    CLASS_MEMBER(LTPlotElement,int,marker_color)
    CLASS_MEMBER(LTPlotElement,int,line_color)
    CLASS_MEMBER(LTPlotElement,int,fill_color)
    CLASS_MEMBER(LTPlotElement,int,fill_style)
    CLASS_MEMBER(LTPlotElement,int,line_style)
    CLASS_MEMBER(LTPlotElement,int,marker_style)
    CLASS_MEMBER(LTPlotElement,bool,draw_fill)
    CLASS_MEMBER(LTPlotElement,bool,draw_fill_in_legend)
    CLASS_MEMBER(LTPlotElement,bool,draw_marker)    
    CLASS_MEMBER(LTPlotElement,bool,draw_line)    
    CLASS_MEMBER(LTPlotElement,bool,draw_stat_error_y)    
    CLASS_MEMBER(LTPlotElement,int,line_width)    
    CLASS_MEMBER(LTPlotElement,double,marker_size)    
    CLASS_MEMBER(LTPlotElement,std::string,legname)
    CLASS_MEMBER(LTPlotElement,std::string,unit)
    CLASS_MEMBER(LTPlotElement,double,scale)
    CLASS_MEMBER(LTPlotElement,bool,in_stack)//NOTE DATA CANNOT BE STACKED AT THE MOMENT
    CLASS_MEMBER(LTPlotElement,bool,is_data)
    CLASS_MEMBER(LTPlotElement,bool,is_trigeff)
    CLASS_MEMBER(LTPlotElement,bool,is_inrationum)
    CLASS_MEMBER(LTPlotElement,bool,is_inratioden)
    CLASS_MEMBER(LTPlotElement,int,has_dderrors)
    CLASS_MEMBER(LTPlotElement,std::string,drawopts)
    CLASS_MEMBER(LTPlotElement,std::string,legopts)
    CLASS_MEMBER(LTPlotElement,std::string,sample)
    CLASS_MEMBER(LTPlotElement,std::vector<std::string>,blindvar)
    std::vector<std::pair<double,double> > blindrange_;
    LTPlotElement set_blindrange(std::vector<std::pair<double,double> > const& blindrange) {blindrange_ = blindrange; return *this; }
    std::vector<std::pair<double,double> > blindrange() {return blindrange_; }
    CLASS_MEMBER(LTPlotElement,TH1F*,hist_ptr)
    /* TH1F* hist_ptr(){return hist_ptr_;} */
    /* LTPlotElement set_hist_ptr(TH1F* hist_ptr){hist_ptr_=(TH1F*)hist_ptr->Clone();return *this;} */
    /* TH1F* hist_ptr_; */
  };

}

#endif

#ifndef ICHiggsTauTau_HiggsTauTau_EffectiveEvents_h
#define ICHiggsTauTau_HiggsTauTau_EffectiveEvents_h
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TreeEvent.h"
#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/ModuleBase.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HTTPlots.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"

namespace ic {

class EffectiveEvents : public ModuleBase {


 private:

  CLASS_MEMBER(EffectiveEvents, fwlite::TFileService*, fs)
  CLASS_MEMBER(EffectiveEvents, bool, do_qcd_scale_wts)
  CLASS_MEMBER(EffectiveEvents, bool, do_pdf_wts)
  TTree *outtree_;
  int mcsign_;
  double gen_ht_;
  
  double scale1_;
  double scale2_;
  double scale3_;
  double scale4_;
  double scale5_;
  double scale6_;
  double scale7_;
  double scale8_;
  double scale9_;
  
  double wt_pdf_1_;
  double wt_pdf_2_;
  double wt_pdf_3_;
  double wt_pdf_4_;
  double wt_pdf_5_;
  double wt_pdf_6_;
  double wt_pdf_7_;
  double wt_pdf_8_;
  double wt_pdf_9_;
  double wt_pdf_10_;
  double wt_pdf_11_;
  double wt_pdf_12_;
  double wt_pdf_13_;
  double wt_pdf_14_;
  double wt_pdf_15_;
  double wt_pdf_16_;
  double wt_pdf_17_;
  double wt_pdf_18_;
  double wt_pdf_19_;
  double wt_pdf_20_;
  double wt_pdf_21_;
  double wt_pdf_22_;
  double wt_pdf_23_;
  double wt_pdf_24_;
  double wt_pdf_25_;
  double wt_pdf_26_;
  double wt_pdf_27_;
  double wt_pdf_28_;
  double wt_pdf_29_;
  double wt_pdf_30_;
  double wt_pdf_31_;
  double wt_pdf_32_;
  double wt_pdf_33_;
  double wt_pdf_34_;
  double wt_pdf_35_;
  double wt_pdf_36_;
  double wt_pdf_37_;
  double wt_pdf_38_;
  double wt_pdf_39_;
  double wt_pdf_40_;
  double wt_pdf_41_;
  double wt_pdf_42_;
  double wt_pdf_43_;
  double wt_pdf_44_;
  double wt_pdf_45_;
  double wt_pdf_46_;
  double wt_pdf_47_;
  double wt_pdf_48_;
  double wt_pdf_49_;
  double wt_pdf_50_;
  double wt_pdf_51_;
  double wt_pdf_52_;
  double wt_pdf_53_;
  double wt_pdf_54_;
  double wt_pdf_55_;
  double wt_pdf_56_;
  double wt_pdf_57_;
  double wt_pdf_58_;
  double wt_pdf_59_;
  double wt_pdf_60_;
  double wt_pdf_61_;
  double wt_pdf_62_;
  double wt_pdf_63_;
  double wt_pdf_64_;
  double wt_pdf_65_;
  double wt_pdf_66_;
  double wt_pdf_67_;
  double wt_pdf_68_;
  double wt_pdf_69_;
  double wt_pdf_70_;
  double wt_pdf_71_;
  double wt_pdf_72_;
  double wt_pdf_73_;
  double wt_pdf_74_;
  double wt_pdf_75_;
  double wt_pdf_76_;
  double wt_pdf_77_;
  double wt_pdf_78_;
  double wt_pdf_79_;
  double wt_pdf_80_;
  double wt_pdf_81_;
  double wt_pdf_82_;
  double wt_pdf_83_;
  double wt_pdf_84_;
  double wt_pdf_85_;
  double wt_pdf_86_;
  double wt_pdf_87_;
  double wt_pdf_88_;
  double wt_pdf_89_;
  double wt_pdf_90_;
  double wt_pdf_91_;
  double wt_pdf_92_;
  double wt_pdf_93_;
  double wt_pdf_94_;
  double wt_pdf_95_;
  double wt_pdf_96_;
  double wt_pdf_97_;
  double wt_pdf_98_;
  double wt_pdf_99_;
  double wt_pdf_100_;
  
  double wt_alphasdown_;
  double wt_alphasup_;

 public:
  EffectiveEvents(std::string const& name);
  virtual ~EffectiveEvents();

  virtual int PreAnalysis();
  virtual int Execute(TreeEvent *event);
  virtual int PostAnalysis();
  virtual void PrintInfo();

};

}
#endif

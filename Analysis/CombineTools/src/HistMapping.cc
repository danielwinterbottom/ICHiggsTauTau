#include "CombineTools/interface/HistMapping.h"
#include <string>

namespace ch {

  bool HistMapping::IsHist() const {
    if (pattern.find(':') == pattern.npos) {
      return true;
    } else {
      return false;
    }
  }

  bool HistMapping::IsPdf() const {
    if (pattern.find(':') != pattern.npos) {
      return true;
    } else {
      return false;
    }
  }

  bool HistMapping::IsData() const {
    if (pattern.find(':') != pattern.npos) {
      return true;
    } else {
      return false;
    }
  }
}

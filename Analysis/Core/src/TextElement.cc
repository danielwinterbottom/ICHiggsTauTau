#include "UserCode/ICHiggsTauTau/Analysis/Core/interface/TextElement.h"

namespace ic {
  
  TextElement::TextElement(std::string const& text, double const& size, double const& x_pos, double const& y_pos) {
    text_ = text;
    size_ = size;
    x_pos_ = x_pos;
    y_pos_ = y_pos;
  }

}

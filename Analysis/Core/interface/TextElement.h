#ifndef AGilbert_Analysis_TextElement_h
#define AGilbert_Analysis_TextElement_h

#include <string>

namespace ic {

  class TextElement {
    private:
      std::string text_;
      double size_;
      double x_pos_;
      double y_pos_;

    public:
      TextElement(std::string const& text, double const& size, double const& x_pos, double const& y_pos);

      //Getters
      std::string text() const { return text_; }
      double size() const { return size_; }
      double x_pos() const { return x_pos_; }
      double y_pos() const { return y_pos_; }
    
      //Setters
      void set_text(std::string text) { text_ = text; }
      void set_size(double size) { size_ = size; }
      void set_x_pos(double x_pos) { x_pos_ = x_pos; }
      void set_y_pos(double y_pos) { y_pos_ = y_pos; }

  };
}
#endif

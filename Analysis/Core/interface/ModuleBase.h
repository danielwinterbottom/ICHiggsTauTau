#ifndef ICHiggsTauTau_Core_ModuleBase_h
#define ICHiggsTauTau_Core_ModuleBase_h

#include <string>
#include <iostream>                     // for operator<<, cout, ostream, etc
#include <vector>                       // for vector
#include <map>
#include "boost/bind.hpp"               // for bind
#include "boost/function.hpp"
#include "boost/format.hpp"
#include "boost/any.hpp"
namespace ic { class TreeEvent; }

#define CLASS_MEMBER(classn,type,name)                                                \
    private:                                                                          \
      type name##_;                                                                   \
    public:                                                                           \
      virtual classn & set_##name(type const& name) {name##_ = name; return *this; }  \
    private:

namespace ic {

class ModuleBase {
 private:
  std::string module_name_;
  unsigned events_processed_;
  std::map<std::string, boost::any> products_; 

 protected:
  void PrintHeader(std::string const& classname);
  template <class T>
  void PrintArg(std::string const& name, T const& arg) {
    std::cout << boost::format("%-15s : %-60s\n") % name % arg;
  }

 public:
  ModuleBase(std::string const& name);
  virtual ~ModuleBase();

  inline void IncreaseProcessedCount() { ++events_processed_; }
  inline unsigned EventsProcessed() { return events_processed_; }
  inline std::string ModuleName() { return module_name_; }

  inline virtual int PreAnalysis() { return 0; }
  virtual int Execute(ic::TreeEvent*) = 0;
  inline virtual int PostAnalysis() { return 0; }
  inline virtual void PrintInfo() { return; }

  bool ProductExists(std::string const& name);
  std::map<std::string, boost::any> GetProducts();

  template <class T>
  T& GetProduct(std::string const& name) {
    if (ProductExists(name)) {
      return boost::any_cast<T&>(products_[name]);
    } else {
      throw std::runtime_error(
          "[ic::ModuleBase::Get] No product with name " + name + " exists");
    }
  }

  template <class T>
  void AddToProducts(std::string name, T const& product) {
    if (!ProductExists(name)) {
      products_[name] = product;
    } else {
      throw std::runtime_error(
          "[ic::ModuleBase::Add] Product with name " + name + " already exists");
    }
  }

};
}

#endif

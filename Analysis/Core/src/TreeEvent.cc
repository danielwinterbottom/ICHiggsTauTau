#include "Core/interface/TreeEvent.h"

class TTree;

namespace ic {

TreeEvent::TreeEvent() { event_ = 0; }

TreeEvent::~TreeEvent() { ; }

void TreeEvent::SetEvent(unsigned event) {
  event_ = event;
  Clear();
  for (unsigned i = 0; i < auto_add_funcs_.size(); ++i) {
    auto_add_funcs_[i](event);
  }
}

void TreeEvent::SetTree(TTree* tree) {
  tree_ = tree;
  handlers_.clear();
  cached_funcs_.clear();
  auto_add_funcs_.clear();
  if (tree) {
    TObjArray const* branches = tree_->GetListOfBranches();
    for (int i = 0; i < branches->GetSize(); ++i) {
      TBranch const* b = dynamic_cast<TBranch const*>(branches->At(i));
      branch_names_.insert(std::string(b->GetName()));
    }
  }
}

void TreeEvent::List() { Event::List(); }

bool TreeEvent::ExistsInTree(std::string const& branch_name) {
  return static_cast<bool>(branch_names_.count(branch_name));
}
bool TreeEvent::ExistsInEvent(std::string const& name) {
  return Event::Exists(name);
}

std::string TreeEvent::FormMissingMessage(std::string const& name,
                                          std::string const& branch_name) {
  std::string msg = (boost::format(
                         "No product with name %s in the ic::Event, "
                         "and no branch with name %s in the TTree\n") %
                     name % branch_name).str();
  return msg;
}
}

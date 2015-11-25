#include "Core/interface/TreeEvent.h"

class TTree;

namespace ic {

TreeEvent::TreeEvent() : Event(), tree_(nullptr), event_(0) {}

TreeEvent::~TreeEvent() { DeleteAndClearHandlers(); }

void TreeEvent::SetEvent(int64_t event) {
  event_ = event;
  Clear();
  for (unsigned i = 0; i < auto_add_funcs_.size(); ++i) {
    auto_add_funcs_[i](event);
  }
  for (auto bh : handlers_) bh.second->SetNoOverwrite(false);
}

void TreeEvent::SetTree(TTree* tree) {
  tree_ = tree;
  DeleteAndClearHandlers();
  cached_funcs_.clear();
  auto_add_funcs_.clear();
  if (tree) {
    TObjArray const* branches = tree_->GetListOfBranches();
    for (int i = 0; i < branches->GetSize(); ++i) {
      TBranch const* b = dynamic_cast<TBranch const*>(branches->At(i));
      if (b) {
        branch_names_.insert(std::string(b->GetName()));
      } else {
        std::cout << "[ic::TreeEvent::SetTree] Encountered some non-TBranch "
                      "object in the list of branches\n";
      }
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
  std::string msg =
      (boost::format(
           "[ic::TreeEvent] No product with name %s in the ic::Event, "
           "and no branch with name %s in the TTree\n") %
       name % branch_name).str();
  return msg;
}

void TreeEvent::DeleteAndClearHandlers() {
  for (auto & bh : handlers_) delete bh.second;
  handlers_.clear();
}
}

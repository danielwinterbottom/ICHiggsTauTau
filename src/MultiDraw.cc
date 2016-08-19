#include "../interface/MultiDraw.hh"
#include <iostream>
#include "TH1D.h"
#include "TStopwatch.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include <map>

void MultiDraw(TTree *inTree, TObjArray *Formulae, TObjArray *Weights,
               TObjArray *Hists, UInt_t ListLen) {
  unsigned long i = 0;
  unsigned long NumEvents = inTree->GetEntries();

  std::vector<TTreeFormula *> v_vars(ListLen, nullptr);
  std::vector<TTreeFormula *> v_weights(ListLen, nullptr);
  std::vector<TH1D *> v_hists(ListLen);

  std::vector<double> r_vars(ListLen, 0.);
  std::vector<double> r_weights(ListLen, 0.);
  std::vector<unsigned> i_vars(ListLen, 0);
  std::vector<unsigned> i_weights(ListLen, 0);
  std::map<std::string, unsigned> map_vars;
  std::map<std::string, unsigned> map_weights;

  bool optimize = true;
  for (unsigned idx = 0; idx < ListLen; ++idx) {
    if (optimize) {
      auto const& itv = map_vars.find(Formulae->At(idx)->GetTitle());
      if (itv == map_vars.end()) {
        map_vars[Formulae->At(idx)->GetTitle()] = idx;
        v_vars[idx] = static_cast<TTreeFormula *>(Formulae->At(idx));
        i_vars[idx] = idx;
      } else {
        i_vars[idx] = itv->second;
      }

      auto const& itw = map_weights.find(Weights->At(idx)->GetTitle());
      if (itw == map_weights.end()) {
        map_weights[Weights->At(idx)->GetTitle()] = idx;
        v_weights[idx] = static_cast<TTreeFormula *>(Weights->At(idx));
        i_weights[idx] = idx;
      } else {
        i_weights[idx] = itw->second;
      }
    } else {
      v_vars[idx] = static_cast<TTreeFormula *>(Formulae->At(idx));
      i_vars[idx] = idx;
      v_weights[idx] = static_cast<TTreeFormula *>(Weights->At(idx));
      i_weights[idx] = idx;
    }

    v_hists[idx] = static_cast<TH1D *>(Hists->At(idx));
  }

  double Value = 0.;
  double Weight = 0.;
  double commonWeight = 1.;
  double treeWeight = inTree->GetWeight();
  Int_t TreeNumber = -1;

  TStopwatch s;
  for (i = 0; i < NumEvents; i++) {
    // Display progress every 20000 events
    if (i % 20000 == 0) {
      std::cout.precision(2);

      double nTodo = NumEvents - i, perSecond = 20000 / s.RealTime();
      Int_t seconds = (Int_t)(nTodo / perSecond),
            minutes = (Int_t)(seconds / 60.);
      seconds -= (Int_t)(minutes * 60.);

      std::cout << "Done " << (double(i) / (double(NumEvents)) * 100.0f)
                << "% ";
      if (minutes) std::cout << minutes << " minutes ";
      std::cout << seconds << " seconds remain.                            \r";

      std::cout.flush();
      s.Start();
    }

    if (TreeNumber != inTree->GetTreeNumber()) {
      treeWeight = inTree->GetWeight();
      TreeNumber = inTree->GetTreeNumber();
    }

    inTree->LoadTree(inTree->GetEntryNumber(i));

    commonWeight *= treeWeight;

    for (unsigned j = 0; j < ListLen; j++) {
      if (v_vars[j]) {
        r_vars[j] = v_vars[j]->EvalInstance();
      }
      if (v_weights[j]) {
        r_weights[j] = v_weights[j]->EvalInstance();
      }
      Value = r_vars[i_vars[j]];
      Weight = r_weights[i_weights[j]] * commonWeight;
      if (Weight) {
        v_hists[j]->Fill(Value, Weight);
      }
    }
  }
}

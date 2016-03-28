// MultiDraw.cxx:
// Draws many histograms in one loop over a tree. 
// A little bit like a TTree::Draw which can make many histograms

// Based on implementation here:
// https://github.com/pwaller/minty/blob/master/minty/junk/MultiDraw.cxx
#include "Rtypes.h"

class TTree;
class TTreeFormula;
class TObjArray;

void MultiDraw(TTree *inTree, TTreeFormula *commonWeightFormula,
               TObjArray *Formulae, TObjArray *Weights, TObjArray *Hists,
               UInt_t ListLen);
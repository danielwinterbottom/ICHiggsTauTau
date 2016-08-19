"""Efficiently draw multiple histograms with one loop over all events in a TTree

This script injects a MultiDraw method into TTree when it is imported.
"""

import ROOT
import sys
import fileinput
import random
import string
import os
# import re
ROOT.gSystem.Load('libUserCodeICHiggsTauTau')


def randomword(length):
   return ''.join(random.choice(string.lowercase) for i in range(length))


def MakeTObjArray(theList, takeOwnership=True):
    """Turn a python iterable into a ROOT TObjArray"""

    result = ROOT.TObjArray()
    if takeOwnership:
        result.SetOwner()

    # Make PyROOT give up ownership of the things that are being placed in the
    # TObjArary. They get deleted because of result.SetOwner()
    for item in theList:
        if takeOwnership:
            ROOT.SetOwnership(item, False)
        result.Add(item)

    return result


def MultiDraw(self, Formulae, Compiled=False):
    """Draws many histograms in one loop over a tree.

        Instead of:
        MyTree.Draw( "nlcts >> a(100, -1, 1)", "weightA" )
        MyTree.Draw( "nlcts >> b(100, -1, 1)", "weightB" )

        Do:
        MyTree.MultiDraw( ( "nlcts >> a(100, -1, 1)", "weightA" ),
                          ( "nlcts >> b(100, -1, 1)", "weightB" ) )

        This is significantly faster when there are many histograms to be drawn.
        The first parameter, CommonWeight, decides a weight given to all
        histograms.

        An arbitrary number of additional histograms may be specified. They can
        either be specified with just a string containing the formula to be
        drawn, the histogram name and bin configuration.

        Alternatively it can be a tuple, with  said string, and an additional
        string specifying the weight to be applied to that histogram only.
    """

    results, formulae, weights, formulaeStr, weightsStr = [], [], [], [], []

    # lastFormula, lastWeight = None, None

    for i, origFormula in enumerate(Formulae):
        print "Have an origFormula", origFormula

        # Expand out origFormula and weight, otherwise just use weight of 1.
        if type(origFormula) == tuple:
            origFormula, weight = origFormula
        else:
            origFormula, weight = origFormula, "1"

        # Our way is simpler, require each variable to end in (...) or [...] to give the binning
        # and always create a new hist
        pos_open = origFormula.rfind('(')
        pos_close = origFormula.rfind(')')
        if pos_open is -1 or pos_close is -1 or pos_open > pos_close:
            raise RuntimeError('You bus')
        str_binning = [x.strip() for x in origFormula[pos_open+1:pos_close].split(',')]
        binning = []
        if len(str_binning) == 3:
            binning = [int(str_binning[0]), float(str_binning[1]), float(str_binning[2])]
        formula = origFormula[:pos_open].strip()
        ROOT.TH1.AddDirectory(False)
        hist = ROOT.TH1D(origFormula+':'+weight, origFormula, *binning)

        results.append(hist)

        # The following two 'if' clauses check that the next formula is different
        # to the previous one. If it is not, we add an ordinary TObject.
        # Then, the dynamic cast in MultiDraw.cxx fails, giving 'NULL', and
        # The previous value is used. This saves the recomputing of identical
        # values

        # if formula != lastFormula:
        f = ROOT.TTreeFormula("formula%i" % i, formula, self)
        f.SetTitle(formula)
        if not f.GetTree():
            raise RuntimeError("TTreeFormula didn't compile: " + formula)
        f.SetQuickLoad(True)
        formulae.append(f)
        formulaeStr.append(formula)
        # else:
        #     formulae.append(ROOT.TObject())

        # if weight != lastWeight:
        f = ROOT.TTreeFormula("weight%i" % i, weight, self)
        f.SetTitle(weight)
        if not f.GetTree():
            raise RuntimeError("TTreeFormula didn't compile: " + formula)
        f.SetQuickLoad(True)
        weights.append(f)
        weightsStr.append(weight)
        # else:
        #     weights.append(ROOT.TObject())

        # lastFormula, lastWeight = formula, weight

    if Compiled:
        fname = "%sSelector%s" % (self.GetName(), randomword(7))
        self.MakeSelector(fname, "=legacy")
        for line in fileinput.input('%s.h' % fname, inplace=1):
            print line,
            if line.startswith('#include <TSelector.h>'):
                print '#include <TH1F.h>'
                print '#include <TObjArray.h>'
            if line.startswith('   virtual void    Terminate();'):
                print '\n   TObjArray *hists;'

        for line in fileinput.input('%s.C' % fname, inplace=1):
            if line.startswith('   return kTRUE;'):
                print '   %s::GetEntry(entry);' % fname
                print '   double weight_value = 0.;'
                for i, f in enumerate(formulaeStr):
                    print '   weight_value = %s;' % (weightsStr[i])
                    print '   if (weight_value) static_cast<TH1D*>(hists->UncheckedAt(%i))->Fill(%s, weight_value);' % (i, f)
                    # print '  std::cout << "%i %s:" << %s << "\\n";' % (i, weightsStr[i], weightsStr[i])
                print '   if (entry % 50000 == 0) {'
                print '      std::cout << "Done " << (double(entry) / (double(fChain->GetEntries())) * 100.0) << "%    \\r";'
                print '      std::cout.flush();'
                print '   }'

            print line,
            if line.startswith('#include <TStyle.h>'):
                print '#include<iostream>'
                print 'using std::abs;'
        ROOT.gROOT.LoadMacro('%s.C++' % fname)
        selector = getattr(ROOT, fname)()
        objarr = MakeTObjArray(results, takeOwnership=False)
        selector.hists = objarr
        self.Process(selector)
        ROOT.gSystem.Unload('%s_C.so' % fname)
        os.system('rm %s*' % fname)
        return results


    from ROOT import MultiDraw as _MultiDraw
    from time import time
    start = time()

    # Ensure that formulae are told when tree changes
    fManager = ROOT.TTreeFormulaManager()
    for formula in formulae + weights:
        if type(formula) == ROOT.TTreeFormula:
            fManager.Add(formula)

    fManager.Sync()
    self.SetNotify(fManager)

    # Draw everything!
    _MultiDraw(self,
               MakeTObjArray(formulae),
               MakeTObjArray(weights),
               MakeTObjArray(results, takeOwnership=False),
               len(Formulae))

    print "Took %.2fs" % (time() - start), " " * 20

    return results

ROOT.TTree.MultiDraw = MultiDraw

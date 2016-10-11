"""Efficiently draw multiple histograms with one loop over all events in a TTree

This script injects a MultiDraw method into TTree when it is imported.
"""

import ROOT
import sys
import fileinput
import random
import string
import os
import re
from array import array
ROOT.gSystem.Load('libUserCodeICHiggsTauTau')

def split_vals(vals):
    """Converts a string '1:3|1,4,5' into a list [1, 2, 3, 4, 5]"""
    res = set()
    first = vals.split(',')
    for f in first:
        second = re.split('[:|]', f)
        # print second
        if len(second) == 1:
            res.add(float(second[0]))
        if len(second) == 3:
            x1 = float(second[0])
            while x1 < float(second[1]) + 1E-5:
                res.add(x1)
                x1 += float(second[2])
    return sorted([x for x in res])


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


def GetBinningArgs(arg, is_variable):
    if is_variable:
        binning = split_vals(arg)
        return [len(binning) - 1, array('d', binning)]
    else:
        str_binning = [x.strip() for x in arg.split(',')]
        binning = []
        if len(str_binning) == 3:
            return [int(str_binning[0]), float(str_binning[1]), float(str_binning[2])]
        else:
            return None

def MultiDraw(self, Formulae, Compiled=False):
    results, formulae, weights, formulaeStr, weightsStr = [], [], [], [], []

    # lastFormula, lastWeight = None, None

    for i, origFormula in enumerate(Formulae):

        # Expand out origFormula and weight, otherwise just use weight of 1.
        if type(origFormula) == tuple:
            origFormula, weight = origFormula
        else:
            origFormula, weight = origFormula, "1"

        # Our way is simpler, require each variable to end in (...) or [...] to give the binning
        # and always create a new hist

        split_var = origFormula.split(';')
        origFormula = split_var[0]
        print "Formula: ", origFormula, weight

        var_binned_x = False
        var_binned_y = False
        is_2d = False
        if origFormula[-1] == ')':
            pos_open = origFormula.rfind('(')
            pos_close = origFormula.rfind(')')
        if origFormula[-1] == ']':
            var_binned_x = True
            pos_open = origFormula.rfind('[')
            pos_close = origFormula.rfind(']')
        if pos_open is -1 or pos_close is -1 or pos_open > pos_close:
            raise RuntimeError('You bus')
        bin_args_x = GetBinningArgs(origFormula[pos_open+1:pos_close], var_binned_x)
        formula = origFormula[:pos_open].strip()

        # Check if this is a 2D histogram with syntax
        # [var_y],[var_x],[binning_y],[binning_x]
        if formula[-1] == ',':
            is_2d = True
            if formula[-2] == ')':
                pos_open_y = formula.rfind('(')
                pos_close_y = formula.rfind(')')
            if origFormula[-2] == ']':
                var_binned_y = True
                pos_open_y = origFormula.rfind('[')
                pos_close_y = origFormula.rfind(']')
            if pos_open_y is -1 or pos_close_y is -1 or pos_open_y > pos_close_y:
                raise RuntimeError('You bus')
            bin_args_y = GetBinningArgs(formula[pos_open_y + 1:pos_close_y], var_binned_y)
            formula = formula[:pos_open_y].split(',')
            # print formula
        else:
            formula = [formula]

        ROOT.TH1.AddDirectory(False)
        if not is_2d:
            hist = ROOT.TH1D(origFormula+':'+weight, origFormula, *bin_args_x)
        else:
            hist = ROOT.TH2F(origFormula+':'+weight, origFormula, *(bin_args_x + bin_args_y))

        if len(split_var) > 1:
            hist.GetXaxis().SetTitle(split_var[1])
        if len(split_var) > 2:
            hist.GetXaxis().SetTitle(split_var[2])
            hist.GetYaxis().SetTitle(split_var[1])

        if is_2d:
            results.append(ROOT.TObject())
        results.append(hist)

        # The following two 'if' clauses check that the next formula is different
        # to the previous one. If it is not, we add an ordinary TObject.
        # Then, the dynamic cast in MultiDraw.cxx fails, giving 'NULL', and
        # The previous value is used. This saves the recomputing of identical
        # values

        for form in formula:
            f = ROOT.TTreeFormula("formula%i" % i, form, self)
            f.SetTitle(form)
            if not f.GetTree():
                raise RuntimeError("TTreeFormula didn't compile: " + form)
            f.SetQuickLoad(True)
            formulae.append(f)
            formulaeStr.append(form)

            f = ROOT.TTreeFormula("weight%i" % i, weight, self)
            f.SetTitle(weight)
            if not f.GetTree():
                raise RuntimeError("TTreeFormula didn't compile: " + weight)
            f.SetQuickLoad(True)
            weights.append(f)
            weightsStr.append(weight)


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
               len(formulae))

    print "Took %.2fs" % (time() - start), " " * 20
    return results

ROOT.TTree.MultiDraw = MultiDraw

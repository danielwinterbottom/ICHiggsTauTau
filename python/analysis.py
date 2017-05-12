
from UserCode.ICHiggsTauTau.uncertainties import ufloat
from collections import defaultdict, OrderedDict
import ROOT
import glob
import os
import json
import pprint
import sys
import UserCode.ICHiggsTauTau.MultiDraw as MultiDraw
import numpy as np

ROOT.TH1.AddDirectory(0)


class Sel(object):
    def __init__(self, **kwargs):
        self.terms = kwargs

    def __call__(self, **kwargs):
        res = self.terms.copy()
        res.update(kwargs)
        return ' * '.join(['(' + x + ')' for x in res.values()])

    def copy(self, **kwargs):
        res = Sel(**self.terms)
        res.update(**kwargs)
        return res

    def __getattr__(self, name):
        return self.terms[name]

    def update(self, **kwargs):
        self.terms.update(kwargs)


def WriteToTFile(obj, file, path):
    file.cd()
    as_vec = path.split('/')
    if len(as_vec) >= 1:
        for i in xrange(0, len(as_vec)-1):
            if not ROOT.gDirectory.GetDirectory(as_vec[i]):
                ROOT.gDirectory.mkdir(as_vec[i])
            ROOT.gDirectory.cd(as_vec[i])
    if not ROOT.gDirectory.FindKey(as_vec[-1]):
        obj.SetName(as_vec[-1])
        ROOT.gDirectory.WriteTObject(obj, as_vec[-1])
    ROOT.gDirectory.cd('/')


class Shape(object):
    def __init__(self, hist, rate=None):
        self.hist = hist  # will also set the rate to the integral of the hist
        if rate is not None:
            self.rate = rate

    def _Int(self):
        if self._hist.GetDimension() == 1:
            return self._hist.Integral(0, self._hist.GetNbinsX() + 1)
        elif self._hist.GetDimension() == 2:
            return self._hist.Integral(0, self._hist.GetNbinsX() + 1, 0, self._hist.GetNbinsY() + 1)

    def _IntErr(self):
        err = ROOT.Double()
        if self._hist.GetDimension() == 1:
            val = self._hist.IntegralAndError(0, self._hist.GetNbinsX() + 1, err)
        elif self._hist.GetDimension() == 2:
            val = self._hist.IntegralAndError(0, self._hist.GetNbinsX() + 1, 0, self._hist.GetNbinsY() + 1, err)
        return ufloat(val, err)

    @property
    def hist(self):
        # print 'Getting hist for %s' % self.__repr__()
        return self._hist

    @hist.setter
    def hist(self, hist):
        # print 'Setting hist for %s ' % self.__repr__()
        self._hist = hist.Clone()
        self._rate = self._IntErr()

    @property
    def rate(self):
        # print 'Getting rate for %s ' % self.__repr__()
        return self._rate

    @rate.setter
    def rate(self, rate):
        # print 'Setting rate to {:.3f}'.format(rate)
        self._rate = rate
        integral = self._Int()
        if integral == 0.:
            print 'Error, histogram integral is zero'
            return
        self._hist.Scale(rate.n / integral)

    def copy(self):
        return Shape(self.hist.Clone(), self.rate)

    def __iadd__(self, other):
        if isinstance(other, Shape):
            self.hist.Add(other.hist)
            self.rate += other.rate
        else:
            self.rate += other
        return self

    def __isub__(self, other):
        if isinstance(other, Shape):
            self.hist.Add(other.hist, -1)
            self.rate -= other.rate
        else:
            self.rate -= other
        return self

    def __imul__(self, other):
        self.rate *= other
        return self

    def __add__(self, other):
        cpy = self.copy()
        return cpy.__iadd__(other)

    __radd__ = __add__

    def __sub__(self, other):
        cpy = self.copy()
        return cpy.__isub__(other)

    def __mul__(self, other):
        cpy = self.copy()
        return cpy.__imul__(other)

    __rmul__ = __mul__

    def Print(self):
        # self.hist.Print()
        print 'rate={:.3f}, entries={:g}, sum={:g}'.format(self.rate, self.hist.GetEntries(), self.hist.GetSumOfWeights())


# def LumiScale(info, data, sample):
#     if 'events' not in info[sample] or 'xs' not in info[sample]:
#         return 1.0
#     return info[data]['lumi'] / (info[sample]['events'] / info[sample]['xs'])


# def GetTreeLists(node, result=None):
#     if result is None:
#         result = defaultdict(list)
#     if node.__class__.from_external_tree:
#         result[node.sample].append((node.variable, node.selection, node))
#     else:
#         for subnode in node.subnodes:
#             GetTreeLists(subnode, result)
#     return result


class TTreeEvaluator:
    def __init__(self, tree_name, filename):
        self.tree_name = tree_name
        self.filename = filename
        self.tree = None  # initially none -> we'll open it later
        self.file = None

    def PrepareTree(self):
        self.file = ROOT.TFile(self.filename)
        self.tree = self.file.Get(self.tree_name)

    def Draw(self, draw_list, compiled=False):
        self.PrepareTree()
        return MultiDraw.MultiDraw(self.tree, draw_list, Compiled=compiled)


class TChainEvaluator(TTreeEvaluator):
    def __init__(self, tree_name, filelist):
        self.tree_name = tree_name
        self.filelist = filelist
        self.tree = ROOT.TChain(self.tree_name)
        for f in self.filelist:
            # If I understand correctly ROOT shouldn't be keeping
            # these files open all the time, which is what we want
            self.tree.AddFile(f)

    def PrepareTree(self):
        pass


class BaseNode:
    def __init__(self, name):
        self.name = name

    def GetNameStr(self):
        return '%s::%s' % (self.__class__.__name__, self.name)

    def GetInfoStr(self):
        return ''

    def SubNodes(self):
        return []

    def PrintTree(self, indent=0):
        print '%s%s%s' % (' ' * indent, self.GetNameStr(), self.GetInfoStr())
        for node in self.SubNodes():
            node.PrintTree(indent=(indent + 2))

    def AddRequests(self, manifest):
        pass

    def Objects(self):
        return dict()

    def OutputPrefix(self, node=None):
        return self.name

    def Output(self, file, prefix=''):
        objects = self.Objects()
        for key, val in objects.iteritems():
            WriteToTFile(val, file, '%s/%s' % (prefix, key))
        for node in self.SubNodes():
            node.Output(file, '%s/%s' % (prefix, self.OutputPrefix(node)))

    def Run(self):
        for node in self.SubNodes():
            node.Run()
        self.RunSelf()

    def RunSelf(self):
        pass


class BasicNode(BaseNode):
    def __init__(self, name, sample, variable, selection, factors=list()):
        BaseNode.__init__(self, name)
        self.sample = sample
        self.variable = variable
        self.selection = selection
        self.factors = factors
        self.shape = None

    def GetInfoStr(self):
        if self.shape is not None:
            self.shape.Print()
        factors_str = ''
        if len(self.factors) > 0:
            factor_vals = [('%.4g' % x) for x in self.factors]
            factors_str = ', factors=' + '*'.join(factor_vals)
        return '[%s, variable=%s, selection=%s%s]' % (self.sample, self.variable.split(';')[0], self.selection, factors_str)

    # This means processing to the point where we can hand our shape over
    def RunSelf(self):
        for factor in self.factors:
            self.shape *= factor

    def AddRequests(self, manifest):
        manifest.append((self.sample, self.variable, self.selection, self, 'shape'))

    def Objects(self):
        return {self.name: self.shape.hist}

class ListNode(BaseNode):
    def __init__(self, name):
        BaseNode.__init__(self, name)
        self.nodes = OrderedDict()
        self.add_output_prefix = True

    def __getitem__(self, key):
        return self.nodes[key]

    def AddNode(self, node):
        self.nodes[node.name] = node

    def SubNodes(self):
        return self.nodes.values()

    def AddRequests(self, manifest):
        for node in self.SubNodes():
            node.AddRequests(manifest)

    def OutputPrefix(self, node=None):
        if self.add_output_prefix:
            return self.name
        else:
            return ''

class SummedNode(ListNode):
    def __init__(self, name):
        ListNode.__init__(self, name)
        self.shape = None

    def RunSelf(self):
        self.shape = sum([node.shape for node in self.nodes.values()])

    def Objects(self):
        return {self.name: self.shape.hist}

    def OutputPrefix(self, node=None):
        if self.add_output_prefix:
            return self.name + '.subnodes'
        else:
            return ''


class SubtractNode(BaseNode):
    def __init__(self, name, initial, subtract):
        BaseNode.__init__(self, name)
        self.shape = None
        self.initial_node = initial
        self.subtract_node = subtract

    def RunSelf(self):
        self.shape = self.initial_node.shape - self.subtract_node.shape

    def Objects(self):
        return {self.name: self.shape.hist}

    def OutputPrefix(self, node=None):
        return self.name + '.subnodes'

    def SubNodes(self):
        return [self.initial_node, self.subtract_node]

    def AddRequests(self, manifest):
        for node in self.SubNodes():
            node.AddRequests(manifest)

class HttQCDNode(BaseNode):
    def __init__(self, name, data, subtract, factor):
        BaseNode.__init__(self, name)
        self.shape = None
        self.data_node = data
        self.subtract_node = subtract
        self.factor = factor

    def RunSelf(self):
        self.shape = self.factor * (self.data_node.shape - self.subtract_node.shape)

    def Objects(self):
        return {self.name: self.shape.hist}

    def OutputPrefix(self, node=None):
        return self.name + '.subnodes'

    def SubNodes(self):
        return [self.data_node, self.subtract_node]

    def AddRequests(self, manifest):
        for node in self.SubNodes():
            node.AddRequests(manifest)

class HttWQCDCombinedNode(BaseNode):
    def __init__(self, name, name_w, name_qcd,
                 data_lmt_ss, data_hmt_os, data_hmt_ss,
                 sub_lmt_ss, sub_hmt_os, sub_hmt_ss,
                 w_lmt_os, w_lmt_ss, w_hmt_os, w_hmt_ss,
                 qcd_os_ss, strategy='simultaneous'):
        BaseNode.__init__(self, name)
        self.name_w = name_w
        self.name_qcd = name_qcd
        self.qcd_lmt_ss = None
        self.qcd_lmt_os = None
        self.qcd_hmt_ss = None
        self.qcd_hmt_os = None
        self.w_lmt_ss = None
        self.w_lmt_os = None
        self.w_hmt_ss = None
        self.w_hmt_os = None
        self.data_lmt_ss = data_lmt_ss
        self.data_hmt_os = data_hmt_os
        self.data_hmt_ss = data_hmt_ss
        self.sub_lmt_ss = sub_lmt_ss
        self.sub_hmt_os = sub_hmt_os
        self.sub_hmt_ss = sub_hmt_ss
        self.w_mc_lmt_os = w_lmt_os
        self.w_mc_lmt_ss = w_lmt_ss
        self.w_mc_hmt_os = w_hmt_os
        self.w_mc_hmt_ss = w_hmt_ss
        self.qcd_os_ss = qcd_os_ss
        # Strategies:
        #   simultaneous: the new method of solving for W/QCD in high mT
        #           run1: Assumes no QCD in the high MT regions
        #             mc: simply takes the W from MC, no QCD in the high mT region
        self.strategy = strategy


    def RunSelf(self):

        # First need the number of data-subtracted high mt events in os and ss
        n_hmt_os = (self.data_hmt_os.shape - self.sub_hmt_os.shape).rate
        n_hmt_ss = (self.data_hmt_ss.shape - self.sub_hmt_ss.shape).rate

        # Next part is strategy dependent
        if self.strategy == 'simultaneous':
            # Solve the simultaneous eqns:
            # n_hmt_os = w_os_ss * n_ss_w + qcd_os_ss * n_ss_qcd
            # n_hmt_ss = n_ss_w + n_ss_qcd

            # We take the W OS/SS ratio from the MC in the high mT region
            w_os_ss = self.w_mc_hmt_os.shape.rate / self.w_mc_hmt_ss.shape.rate
            # The QCD OS/SS ratio is supplied by the user
            qcd_os_ss = self.qcd_os_ss

            n = np.array([n_hmt_os.n, n_hmt_ss.n])
            vals = np.array([[w_os_ss.n, qcd_os_ss], [1, 1]])
            x = np.linalg.solve(vals, n)
            n_ss_w = x[0]
            n_ss_qcd = x[1]

            # From this get a global W scale factor from MC->data
            w_data_mc_ss = n_ss_w / self.w_mc_hmt_ss.shape.rate.n
            w_data_mc_os = w_data_mc_ss

            print 'n_hmt_os:  %s' % n_hmt_os
            print 'n_hmt_ss:  %s' % n_hmt_ss
            print 'w_os_ss:   %s' % w_os_ss
            print 'n_ss_w:    %s' % n_ss_w
            print 'n_ss_qcd:  %s' % n_ss_qcd
            print 'n_os_w:    %f' % (n_ss_w * w_os_ss.n)
            print 'n_os_qcd:  %f' % (n_ss_qcd * qcd_os_ss)
            print 'w_data_mc: %f' % w_data_mc_ss
        elif self.strategy == 'run1':
            # Assume data-bkg in each high mt region is just W
            w_data_mc_os = n_os.n / self.w_mc_hmt_os.shape.rate.n
            w_data_mc_ss = n_ss.n / self.w_mc_hmt_ss.shape.rate.n

            print 'n_hmt_os:     %s' % n_hmt_os
            print 'n_hmt_ss:     %s' % n_hmt_ss
            print 'w_data_mc_os: %f' % w_data_mc_os
            print 'w_data_mc_ss: %f' % w_data_mc_ss
        elif self.strategy == 'mc':
            # MC normalisation as-is
            w_data_mc_os = 1.0
            w_data_mc_ss = 1.0
        else:
            raise RuntimeError('Strategy not recognised!')

        self.w_lmt_os = self.w_mc_lmt_os.shape * w_data_mc_os
        self.w_lmt_ss = self.w_mc_lmt_ss.shape * w_data_mc_ss
        self.w_hmt_os = self.w_mc_hmt_os.shape * w_data_mc_os
        self.w_hmt_ss = self.w_mc_hmt_ss.shape * w_data_mc_ss

        # Low mt SS QCD always defined as data - bkkgs - W
        self.qcd_lmt_ss = self.data_lmt_ss.shape - self.sub_lmt_ss.shape - self.w_lmt_ss
        # Low mt OS QCD always defined as SS QCD * qcd_os_ss
        self.qcd_lmt_os = self.qcd_os_ss * self.qcd_lmt_ss

        print 'data_lmt_ss:   %s' % self.data_lmt_ss.shape.rate
        print 'sub_lmt_ss:    %s' % self.sub_lmt_ss.shape.rate
        print 'w_lmt_ss:      %s' % self.w_lmt_ss.rate

        if self.strategy == 'simultaneous':
            self.qcd_hmt_ss = self.data_hmt_ss.shape - self.sub_hmt_ss.shape - self.w_hmt_ss
            self.qcd_hmt_os = qcd_os_ss * self.qcd_hmt_ss
        elif self.strategy in ['run1', 'mc']:
            # No QCD in the high mT regions
            self.qcd_hmt_ss = self.data_hmt_ss.shape * 0.0
            self.qcd_hmt_os = self.data_hmt_os.shape * 0.0

    def Objects(self):
        return {
            self.name_w: self.w_lmt_os.hist,
            self.name_qcd: self.qcd_lmt_os.hist,
            self.OutputPrefix(self.data_lmt_ss) + '/' + self.name_w: self.w_lmt_ss.hist,
            self.OutputPrefix(self.data_lmt_ss) + '/' +  self.name_qcd: self.qcd_lmt_ss.hist,
            self.OutputPrefix(self.data_hmt_os) + '/' + self.name_w: self.w_hmt_os.hist,
            self.OutputPrefix(self.data_hmt_os) + '/' +  self.name_qcd: self.qcd_hmt_os.hist,
            self.OutputPrefix(self.data_hmt_ss) + '/' + self.name_w: self.w_hmt_ss.hist,
            self.OutputPrefix(self.data_hmt_ss) + '/' +  self.name_qcd: self.qcd_hmt_ss.hist
            }

    def OutputPrefix(self, node):
        if node in [self.data_lmt_ss, self.sub_lmt_ss]:
            return self.name + '.lmt_ss'
        if node in [self.data_hmt_ss, self.sub_hmt_ss]:
            return self.name + '.hmt_ss'
        if node in [self.data_hmt_os, self.sub_hmt_os]:
            return self.name + '.hmt_os'
        return self.name + '.subnodes'

    def SubNodes(self):
        return [self.data_lmt_ss, self.data_hmt_os, self.data_hmt_ss,
                self.sub_lmt_ss, self.sub_hmt_os, self.sub_hmt_ss,
                self.w_mc_lmt_os, self.w_mc_lmt_ss, self.w_mc_hmt_os, self.w_mc_hmt_ss]

    def AddRequests(self, manifest):
        for node in self.SubNodes():
            node.AddRequests(manifest)

class Analysis(object):
    def __init__(self):
        self.trees = {}
        self.nodes = ListNode('analysis')
        self.info = {}
        self.remaps = {}
        self.compiled = False

    def Run(self):
        manifest = []
        self.nodes.AddRequests(manifest)
        # print manifest
        drawdict = defaultdict(list)
        outdict = defaultdict(list)
        for entry in manifest:
            drawdict[entry[0]].append(entry[1:3])
            outdict[entry[0]].append(entry[3:5])
        # print drawdict
        # print outdict
        for sample in drawdict:
            # print sample
            res = self.trees[sample].Draw(drawdict[sample], compiled=self.compiled)
            res = [x for x in res if isinstance(x, ROOT.TH1)]
            for i, hist in enumerate(res):
                # if not isinstance(hist, ROOT.TH1):
                #     continue
                setattr(outdict[sample][i][0], outdict[sample][i][1], Shape(hist))
        self.nodes.Run()

    def AddSamples(self, dir, tree, fallback=None):
        files = glob.glob(dir)
        if fallback is not None:
          files += glob.glob(fallback)
        seen_names = set()
        for f in files:
            testf = ROOT.TFile(f)
            if testf.Get(tree) != None:
                name = os.path.splitext(os.path.basename(f))[0]
                if name in seen_names:
                    print '>> Skipping %s because we already loaded it' % f
                    continue
                seen_names.add(name)
                newname = name
                if name in self.remaps:
                    newname = self.remaps[name]
                self.trees[newname] = TTreeEvaluator(tree, f)
            testf.Close()

    def AddInfo(self, file, scaleTo=None):
        with open(file) as jsonfile:
            info = json.load(jsonfile)
        for sa in info:
            name = sa
            if sa in self.remaps:
                name = self.remaps[sa]
            if name in self.trees:
                self.info[name] = info[sa]
        if scaleTo is not None and scaleTo in self.info:
            lumi = self.info[scaleTo]['lumi']
            for name, data in self.info.iteritems():
                if 'xs' in data and 'evt' in data:
                    #print name, data
                    data['sf'] = lumi / (float(data['evt']) / float(data['xs']))
                else:
                    data['sf'] = 1.0
        #pprint.pprint(self.info)

    def BasicFactory(self, name, sample=None, var='', sel='', factors=[], scaleToLumi=True):
        myfactors = []
        if sample is None:
            sample = name
        if scaleToLumi:
            myfactors = factors[:]
            myfactors.append(self.info[sample]['sf'])
        return BasicNode(name, sample, var, sel, factors=myfactors)

    def SummedFactory(self, name, samples, var='', sel='', factors=[], scaleToLumi=True):
        res = SummedNode(name)
        for sa in samples:
            res.AddNode(self.BasicFactory(sa, sa, var, sel, factors, scaleToLumi))
        return res



from UserCode.ICHiggsTauTau.uncertainties import ufloat
from collections import defaultdict, OrderedDict
import ROOT
import UserCode.ICHiggsTauTau.MultiDraw as MultiDraw

ROOT.TH1.AddDirectory(0)


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
        return self._hist.Integral(0, self._hist.GetNbinsX() + 1)

    def _IntErr(self):
            err = ROOT.Double()
            val = self._hist.IntegralAndError(0, self._hist.GetNbinsX() + 1, err)
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

    def __sub__(self, other):
        cpy = self.copy()
        return cpy.__isub__(other)

    def __mul__(self, other):
        cpy = self.copy()
        return cpy.__imul__(other)

    def Print(self):
        self.hist.Print()
        print 'Rate: {:.3f}'.format(self.rate)


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

    def Output(self, file, prefix=''):
        objects = self.Objects()
        for key, val in objects.iteritems():
            WriteToTFile(val, file, '%s/%s' % (prefix, key))
        for node in self.SubNodes():
            node.Output(file, '%s/%s' % (prefix, self.name))


class BasicNode(BaseNode):
    def __init__(self, name, sample, variable, selection):
        BaseNode.__init__(self, name)
        self.sample = sample
        self.variable = variable
        self.selection = selection
        self.factors = []
        self.shape = None

    def GetInfoStr(self):
        if self.shape is not None:
            self.shape.Print()
        return '[%s, variable=%s, selection=%s]' % (self.sample, self.variable, self.selection)

    # This means processing to the point where we can hand our shape over
    def Run(self):
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

    def __getitem__(self, key):
        return self.nodes[key]

    def AddNode(self, node):
        self.nodes[node.name] = node

    def SubNodes(self):
        return self.nodes.values()

    def AddRequests(self, manifest):
        for node in self.SubNodes():
            node.AddRequests(manifest)


class Analysis(object):
    def __init__(self):
        self.trees = {}
        self.nodes = ListNode('analysis')
        self.info = {}

    def Run(self):
        manifest = []
        self.nodes.AddRequests(manifest)
        print manifest
        drawdict = defaultdict(list)
        outdict = defaultdict(list)
        for entry in manifest:
            drawdict[entry[0]].append(entry[1:3])
            outdict[entry[0]].append(entry[3:5])
        print drawdict
        print outdict
        for sample in drawdict:
            print sample
            res = self.trees[sample].Draw(drawdict[sample])
            for i, hist in enumerate(res):
                setattr(outdict[sample][i][0], outdict[sample][i][1], Shape(hist))

# import os
# import json
# import stat
# import copy
# import glob
# from math import ceil
# from functools import partial
# from multiprocessing import Pool
from collections import defaultdict
import ROOT
import UserCode.ICHiggsTauTau.MultiDraw as MultiDraw

"""
BasicSelection: just a string
NamedSelections: dict which we take the AND of

The list of nodes should probably be a function so that
modules can generate it dynamically if needed
"""


def LumiScale(info, data, sample):
    if 'events' not in info[sample] or 'xs' not in info[sample]:
        return 1.0
    return info[data]['lumi'] / (info[sample]['events'] / info[sample]['xs'])


def GetTreeLists(node, result=None):
    if result is None:
        result = defaultdict(list)
    if node.__class__.from_external_tree:
        result[node.sample].append((node.variable, node.selection, node))
    else:
        for subnode in node.subnodes:
            GetTreeLists(subnode, result)
    return result


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
    from_external_tree = False

    def __init__(self, name):
        self.name = name
        self.subnodes = []
        self.objects = {}

    def GetNameStr(self):
        return '%s::%s' % (self.__class__.__name__, self.name)

    def GetInfoStr(self):
        return ''

    def PrintTree(self, indent=0):
        print '%s%s%s' % (' '*indent, self.GetNameStr(), self.GetInfoStr())
        for node in self.subnodes:
            node.PrintTree(indent=(indent+2))

    def GetObjects(self):
        for node in self.subnodes:
            self.objects.update(node.GetObjects())
        return self.objects


class BasicNode(BaseNode):
    from_external_tree = True

    def __init__(self, name, sample, variable, selection):
        BaseNode.__init__(self, name)
        self.sample = sample
        self.variable = variable
        self.selection = selection
        self.shape = None
        self.rate = None

    def GetInfoStr(self):
        return '[%s, variable=%s, selection=%s]' % (self.sample, self.variable, self.selection)

    def GetObjects(self):
        self.shape.SetName(self.name)
        self.objects[self.name] = self.shape
        return self.objects


class ScaledNode(BaseNode):
    def __init__(self, name, **kwargs):
        BaseNode.__init__(self, name)
        self.scales = list(kwargs.values())

    def GetObjects(self):
        self.shape = self.subnodes[0].shape.Clone()
        self.shape.SetName(self.name)
        sf = 1.0
        for x in self.scales:
            sf *= x
        self.shape.Scale(sf)
        self.objects[self.name] = self.shape
        return self.objects

    def GetInfoStr(self):
        return '[%s]' % ','.join([str(x) for x in self.scales])



class RootNode(BaseNode):
    def __init__(self, name):
        BaseNode.__init__(self, name)
    #     self.objects = {}

    # def GetObjects(self):
    #     for node in self.subnodes:
    #         self.objects.extend(node.GetObjects())


"""

ana.insertNode('ZTT', ScaledNode(LumiScaler(ana['ZTT'].sample))
ana.addNode('VV', SummedNode(ana.selectByAttribute('diboson')))
ana.cloneNode('VV', 'VVJ', AppendToSelection('gen_match_2 == 4'))


ana.AddChainedNodes(name='ZTT', BasicNode(sample='DYJetsToLL'), ScaledNode(LumiScalar('DYJetsToLL')))


Modify(ana, 'variable', 'pt_1(100,0,100)') --> is callable? no -> asign

Modify(ana['VVJ'], 'selection', AddSelection('gen_match_2==4')) --> is callable, call on the attribute




Step 1) Walk the entire tree starting from the root node
Step 2) at each node, ask the node if it needs to get histograms from the evaluators
Step 3) Node should give us the requests, we should keep track of what to return to each node
Step 4) Then need to effectively walk the tree backwards, propagating the info from node to node until the root (collection) node is good

Node negociation

parent -> child: GetObject(): can you give me your object?
child -> parent: No, return None

"""
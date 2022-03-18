import ROOT
import uproot
import numpy as np
import pandas as pd
import re
import ROOT
import warnings
import re
import pickle
import os
import xgboost as xgb
import matplotlib.pyplot as plt
import fnmatch
import itertools
import math
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
from collections import Counter
from numpy.lib.recfunctions import append_fields
from pandas import DataFrame, RangeIndex
from root_numpy import root2array, list_trees
from root_numpy import list_branches
from root_numpy.extern.six import string_types
from math import ceil

def setup_dataframe(loc,root_files,channel,year,vars,selection):
  # Set up variables to get from root file trees
  get_variables = []

  # Get variables needed for training
  mod_vars = []
  for i in vars:
    if not (("*" in i) or ("/" in i) or ("&&" in i) or ("||" in i) or ("!" in i) or ("==" in i) or (">" in i) or ("<" in i)):
      get_variables.append(i)
    else:
      i_strip = i.replace(" ","")
      mod_vars.append(i_strip)
      for j in re.split(r"[\b\W\b]+",i_strip):
        if j not in get_variables and not j.isdigit(): get_variables.append(j)

  # Get variables needed for selection
  for i in re.split(r"[\W]+",selection):
    if not (unicode(i, 'utf-8').isnumeric() or i == "fabs" or i == ""):
      if i not in get_variables:
        get_variables.append(i)
  print ">> Getting variables from ROOT files: "
  print get_variables

  # Add all root file trees for variables determined above to dataframes
  for i in root_files:
    tree = uproot.open(loc+'/'+i+'_'+channel+'_'+year+'.root')["ntuple"]
    if i == root_files[0]:
      df = tree.pandas.df(get_variables)
    else:
      df = pd.concat([df,tree.pandas.df(get_variables)],ignore_index=True, sort=False)

  # Cut dataframe with selection
  print ">> Selected events matching the condition: "
  print selection
  df = cut_dataframe(df,selection)

  # Calculate modified variables
  print ">> Calculating modified variables and adding them to the dataframe"
  for i in mod_vars:
    df = modify_variable(df,i)

  # Removing variables not used in training
  print ">> Removing variables not needed for training"
  for i in get_variables:
    if i not in vars:
      var_name  = i.replace("*","_times_").replace("/","_over_").replace("==","_").replace(">","_gt_").replace("<","_lt_")
      df = df.drop([var_name],axis=1)

  return df


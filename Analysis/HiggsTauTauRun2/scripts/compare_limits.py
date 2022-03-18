#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT as R
from array import array
import argparse
import json
import math

parser = argparse.ArgumentParser()
parser.add_argument('--output',help= 'Output folder', default='')
parser.add_argument('--channel',help= 'Channel for limits', default='all')
parser.add_argument('--year', help= 'Name of input year', default='all')
parser.add_argument('--ratio', help= 'Add ratio', default=False)
args = parser.parse_args()

output = args.output
channel = args.channel
year = args.year
ratio = args.ratio

if year == 'all':
  title_right = '138 fb^{-1} (13 TeV)'
elif year == '2016':
  title_right = '2016: 35.9 fb^{-1} (13 TeV)'
elif year == '2017':
  title_right = '2017: 41.5 fb^{-1} (13 TeV)'
elif year == '2018':
  title_right = '2018: 59.7 fb^{-1} (13 TeV)'

if channel == 'tt':
  title_left = '#tau_{h}#tau_{h}'
elif channel == 'mt':
  title_left = '#mu#tau_{h}'
elif channel == 'et':
  title_left = 'e#tau_{h}'
elif channel == 'fake':
  title_left = '#tau_{h}#tau_{h},#mu#tau_{h},e#tau_{h}'
elif channel == 'all':
  title_left = '#tau_{h}#tau_{h},#mu#tau_{h},e#tau_{h},e#mu'
elif channel == 'lt':
  title_left = '#mu#tau_{h},e#tau_{h}'

############## BITS TO EDIT ###################

draw_main_file = True

#title_right = "" 

### ATLAS PLOT ### 

main_file = '/vols/cms/gu18/Offline/output/MSSM/ATLAS_limits/mssm_*H_combined_cmb.json'

ratio_files = [
               '/vols/cms/gu18/Offline/output/MSSM/ATLAS_limits/mssm_*H_combined_cmb_2.json',
               "/vols/cms/dw515/MSSMLowMass/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/model_independent_limits/Feb28_all_all_bsm-model-indep/combined/cmb/mssm_*H_lowandhigh_combined_cmb.json",
               "mssm_*H_lowandhigh_combined_cmb.json",
               ]

plotting_dict = {
    main_file     :{'legend':{'obs':{'Label':'ATLAS Observed', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'obs':  { 'LineWidth' : 2, 'LineColor' : 4, 'LineStyle':1,'MarkerStyle':33, 'MarkerColor':4}}},
    ratio_files[0]:{'legend':{'exp0':{'Label':'ATLAS Expected', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'exp0':  { 'LineWidth' : 2, 'LineColor' : 4, 'LineStyle':2,'MarkerStyle':27, 'MarkerColor':4}}},
    ratio_files[1]:{'legend':{'obs':{'Label':'CMS Observed', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'obs':  { 'LineWidth' : 2, 'LineColor' :2, 'LineStyle':1,'MarkerStyle':33, 'MarkerColor':2}}},
    ratio_files[2]:{'legend':{'exp0':{'Label':'CMS Expected', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'exp0':  { 'LineWidth' : 2, 'LineColor' :2, 'LineStyle':2,'MarkerStyle':27, 'MarkerColor':2}}},
                }


### CHANNELS PLOT ###

#main_file = '/vols/cms/gu18/Offline/output/MSSM/HIG_17_020_limits/mssm_021117_Hbkg_*H_cmb.json'

#ratio_files = [
#               '/vols/cms/gu18/CH/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/model_independent_limits/KIT_mt_all_mssm_classic/combined/cmb/mssm_*H_combined_cmb.json',
#               '/vols/cms/gu18/CH/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/model_independent_limits/KIT_et_all_mssm_classic/combined/cmb/mssm_*H_combined_cmb.json',
#               '/vols/cms/gu18/CH/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/model_independent_limits/KIT_fix_v3_em_all_mssm_classic/combined/cmb/mssm_*H_combined_cmb.json',
#               '/vols/cms/gu18/CH/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/model_independent_limits/KIT_tt_all_mssm_classic/combined/cmb/mssm_*H_combined_cmb.json',
#               '/vols/cms/gu18/CH/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/model_independent_limits/KIT_fix_v3_all_all_mssm_classic/combined/cmb/mssm_*H_combined_cmb.json',
#              ]

#plotting_dict = {
#    main_file     :{'legend':{'obs':{'Label':'HIG-17-020', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'obs':  { 'LineWidth' : 2, 'LineColor' : 1, 'LineStyle':1,'MarkerStyle':33, 'MarkerColor':1}}},
#    ratio_files[0]:{'legend':{'obs':{'Label':'#mu#tau_{h}', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'obs':  { 'LineWidth' : 2, 'LineColor' :2, 'LineStyle':2,'MarkerStyle':34, 'MarkerColor':2}}},
#    ratio_files[1]:{'legend':{'obs':{'Label':'e#tau_{h}', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'obs':  { 'LineWidth' : 2, 'LineColor' : 4, 'LineStyle':2,'MarkerStyle':23, 'MarkerColor':4}}},
#    ratio_files[2]:{'legend':{'obs':{'Label':'e#mu', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'obs':  { 'LineWidth' : 2, 'LineColor' : 95, 'LineStyle':2,'MarkerStyle':22, 'MarkerColor':95}}},
#    ratio_files[3]:{'legend':{'obs':{'Label':'#tau_{h}#tau_{h}', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}}, 'line' : {'obs':  { 'LineWidth' : 2, 'LineColor' :209, 'LineStyle':2, 'MarkerStyle':21, 'MarkerColor':209}}},
#    ratio_files[4]:{'legend':{'obs':{'Label':'Combined', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}}, 'line' : {'obs':  { 'LineWidth' : 2, 'LineColor' : 1, 'LineStyle':2,'MarkerStyle':20, 'MarkerColor':1}}},
#                }

### YEARS PLOT ###

#main_file = '/vols/cms/gu18/Offline/output/MSSM/HIG_17_020_limits/mssm_021117_Hbkg_*H_cmb.json'
#
#ratio_files = [
#               '/vols/cms/gu18/Offline/output/MSSM/HIG_17_020_limits/mssm_021117_Hbkg_*H_cmb_2.json',
#               '/vols/cms/gu18/CH/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/model_independent_limits/KIT_fix_v3_all_2016_mssm_classic/combined/cmb/mssm_*H_combined_cmb.json',
#               '/vols/cms/gu18/CH/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/model_independent_limits/KIT_fix_v3_all_2017_mssm_classic/combined/cmb/mssm_*H_combined_cmb.json',
#               '/vols/cms/gu18/CH/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/model_independent_limits/KIT_fix_v3_all_2018_mssm_classic/combined/cmb/mssm_*H_combined_cmb.json',
#               '/vols/cms/gu18/CH/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/model_independent_limits/KIT_fix_v3_all_all_mssm_classic/combined/cmb/mssm_*H_combined_cmb.json',
#              ]
#
#plotting_dict = {
#    main_file     :{'legend':{'obs':{'Label':'HIG-17-020 (35.9 fb^{-1})', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'obs':  { 'LineWidth' : 2, 'LineColor' : 8, 'LineStyle':1,'MarkerStyle':33, 'MarkerColor':8}}},
#    ratio_files[0]:{'legend':{'exp0':{'Label':'HIG-17-020 (35.9 fb^{-1})', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'exp0':  { 'LineWidth' : 2, 'LineColor' : 8, 'LineStyle':2,'MarkerStyle':27, 'MarkerColor':8}}},
#    ratio_files[1]:{'legend':{'obs':{'Label':'2016 (35.9 fb^{-1})', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'obs':  { 'LineWidth' : 2, 'LineColor' :2, 'LineStyle':2,'MarkerStyle':34, 'MarkerColor':2}}},
#    ratio_files[2]:{'legend':{'obs':{'Label':'2017 (41.5 fb^{-1})', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'obs':  { 'LineWidth' : 2, 'LineColor' : 4, 'LineStyle':2,'MarkerStyle':23, 'MarkerColor':4}}},
#    ratio_files[3]:{'legend':{'obs':{'Label':'2018 (59.7 fb^{-1})', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'obs':  { 'LineWidth' : 2, 'LineColor' : 95, 'LineStyle':2,'MarkerStyle':22, 'MarkerColor':95}}},
#    ratio_files[4]:{'legend':{'obs':{'Label':'Combined (137 fb^{-1})', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}}, 'line' : {'obs':  { 'LineWidth' : 2, 'LineColor' :1, 'LineStyle':2, 'MarkerStyle':21, 'MarkerColor':1}}},
#                }

#main_file = '/vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/vlq_combined_vlq_betaRd33_minus1_minus1_scenario.json'
#
#ratio_files = [
#               '/vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/vlq_combined_vlq_betaRd33_minus1_minus1_scenario_no_interference.json',
#              ]
#
#plotting_dict = {
#    main_file     :{'legend':{'exp0':{'Label':'Matched + Interference', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'exp0':  { 'LineWidth' : 2, 'LineColor' : 4, 'LineStyle':2,'MarkerStyle':33, 'MarkerColor':4}}},
#    ratio_files[0]:{'legend':{'exp0':{'Label':'Matched', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'exp0':  { 'LineWidth' : 2, 'LineColor' : 8, 'LineStyle':2,'MarkerStyle':27, 'MarkerColor':8}}},
#                }

#main_file = '/vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/analysis/1412_tt_2018_interference_betaRd33_0/datacards_vector_leptoquark/combined/cmb/vlq_betaRd33_0_cmb_TeV.json'

#ratio_files = [
#               '/vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/analysis/bdt_1612_v2_betaRd33_0/datacards_vector_leptoquark/combined/cmb/vlq_betaRd33_0_cmb_TeV.json',
#               #'/vols/cms/gu18/CH_unblinding/CMSSW_10_2_25/src/CombineHarvester/MSSMvsSMRun2Legacy/analysis/1412_tt_2018_no_interference_betaRd33_0/datacards_vector_leptoquark/combined/cmb/vlq_betaRd33_0_cmb_TeV.json'
#              ]

#plotting_dict = {
#    main_file     :{'legend':{'exp0':{'Label':'m_{T}^{tot}', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'exp0':  { 'LineWidth' : 2, 'LineColor' : 4, 'LineStyle':2,'MarkerStyle':33, 'MarkerColor':4}}},
#    ratio_files[0]:{'legend':{'exp0':{'Label':'BDT Score', 'LegendStyle' : 'LP', 'DrawStyle' : 'LPSAME'}},    'line' : {'exp0':  { 'LineWidth' : 2, 'LineColor' : 8, 'LineStyle':2,'MarkerStyle':27, 'MarkerColor':8}}},
#                }


def DrawAxisHists(pads, axis_hists, def_pad=None):
  for i, pad in enumerate(pads):
    pad.cd()
    axis_hists[i].Draw('AXIS')
    axis_hists[i].Draw('AXIGSAME')
  if def_pad is not None:
    def_pad.cd()

def DoRatioJson(num,denom,output_name):
  with open(denom) as denom_file:
    denom_dict = json.load(denom_file)
  with open(num) as num_file:
    num_dict = json.load(num_file)
  
  ratio_dict = {}
  for i_key,i_val in denom_dict.items():
    for j_key,j_val in num_dict.items():
      if i_key == j_key:
        ratio_dict[i_key] = {'exp0':j_val['exp0']/i_val['exp0']}

  with open(output_name,'w') as fp:
    json.dump(ratio_dict,fp)

def ModTDRStyle(width=600, height=600, t=0.06, b=0.12, l=0.16, r=0.04):
    """Modified version of the tdrStyle

    Args:
        width (int): Canvas width in pixels
        height (int): Canvas height in pixels
        t (float): Pad top margin [0-1]
        b (float): Pad bottom margin [0-1]
        l (float): Pad left margin [0-1]
        r (float): Pad right margin [0-1]
    """
    #SetTDRStyle()

    # Set the default canvas width and height in pixels
    R.gStyle.SetCanvasDefW(width)
    R.gStyle.SetCanvasDefH(height)

    # Set the default margins. These are given as fractions of the pad height
    # for `Top` and `Bottom` and the pad width for `Left` and `Right`. But we
    # want to specify all of these as fractions of the shortest length.
    def_w = float(R.gStyle.GetCanvasDefW())
    def_h = float(R.gStyle.GetCanvasDefH())

    scale_h = (def_w / def_h) if (def_h > def_w) else 1.
    scale_w = (def_h / def_w) if (def_w > def_h) else 1.

    def_min = def_h if (def_h < def_w) else def_w

    R.gStyle.SetPadTopMargin(t * scale_h)
    # default 0.05
    R.gStyle.SetPadBottomMargin(b * scale_h)
    # default 0.13
    R.gStyle.SetPadLeftMargin(l * scale_w)
    # default 0.16
    R.gStyle.SetPadRightMargin(r * scale_w)
    # default 0.02
    # But note the new CMS style sets these:
    # 0.08, 0.12, 0.12, 0.04

    # Set number of axis tick divisions
    R.gStyle.SetNdivisions(506, 'XYZ')  # default 510

    # Some marker properties not set in the default tdr style
    R.gStyle.SetMarkerColor(R.kBlack)
    R.gStyle.SetMarkerSize(1.0)

    R.gStyle.SetLabelOffset(0.007, 'YZ')
    # This is an adhoc adjustment to scale the x-axis label
    # offset when we stretch plot vertically
    # Will also need to increase if first x-axis label has more than one digit
    R.gStyle.SetLabelOffset(0.005 * (3. - 2. / scale_h), 'X')

    # In this next part we do a slightly involved calculation to set the axis
    # title offsets, depending on the values of the TPad dimensions and
    # margins. This is to try and ensure that regardless of how these pad
    # values are set, the axis titles will be located towards the edges of the
    # canvas and not get pushed off the edge - which can often happen if a
    # fixed value is used.
    title_size = 0.05
    title_px = title_size * def_min
    label_size = 0.04
    R.gStyle.SetTitleSize(title_size, 'XYZ')
    R.gStyle.SetLabelSize(label_size, 'XYZ')

    R.gStyle.SetTitleXOffset(0.5 * scale_h *
                             (1.2 * (def_h * b * scale_h - 0.6 * title_px)) /
                             title_px)
    R.gStyle.SetTitleYOffset(0.5 * scale_w *
                             (1.2 * (def_w * l * scale_w - 0.6 * title_px)) /
                             title_px)

    # Only draw ticks where we have an axis
    R.gStyle.SetPadTickX(0)
    R.gStyle.SetPadTickY(0)
    R.gStyle.SetTickLength(0.02, 'XYZ')

    R.gStyle.SetLegendBorderSize(0)
    R.gStyle.SetLegendFont(42)
    R.gStyle.SetLegendFillColor(0)
    R.gStyle.SetFillColor(0)

    R.gROOT.ForceStyle()


def CreateAxisHist(src, at_limits=True):
    backup = R.gPad
    tmp = R.TCanvas()
    tmp.cd()
    src.Draw('AP')
    result = src.GetHistogram().Clone('tmp')
    if (at_limits):
        min = 0.
        max = 0.
        x = R.Double(0.)
        y = R.Double(0.)
        src.GetPoint(0, x, y)
        min = float(x)
        max = float(x)
        for i in range(1, src.GetN()):
            src.GetPoint(i, x, y)
            if x < min:
                min = float(x)
            if x > max:
                max = float(x)
        result.GetXaxis().SetLimits(min, max)
    R.gPad = backup
    return result


def CreateAxisHists(n, src, at_limits):
    res = []
    h = CreateAxisHist(src, at_limits)
    for i in xrange(n):
        res.append(h.Clone('tmp%i'%i))
    return res

def DrawCMSLogo(pad, cmsText, extraText, iPosX, relPosX, relPosY, relExtraDY, extraText2='', cmsTextSize=0.8):
    """Blah
    
    Args:
        pad (TYPE): Description
        cmsText (TYPE): Description
        extraText (TYPE): Description
        iPosX (TYPE): Description
        relPosX (TYPE): Description
        relPosY (TYPE): Description
        relExtraDY (TYPE): Description
        extraText2 (str): Description
        cmsTextSize (float): Description
    
    Returns:
        TYPE: Description
    """
    pad.cd()
    cmsTextFont = 62  # default is helvetic-bold

    writeExtraText = len(extraText) > 0
    writeExtraText2 = len(extraText2) > 0
    extraTextFont = 52

    # text sizes and text offsets with respect to the top frame
    # in unit of the top margin size
    lumiTextOffset = 0.2
    # cmsTextSize = 0.8
    # float cmsTextOffset    = 0.1;  // only used in outOfFrame version

    # ratio of 'CMS' and extra text size
    extraOverCmsTextSize = 0.76

    outOfFrame = False
    if iPosX / 10 == 0:
        outOfFrame = True

    alignY_ = 3
    alignX_ = 2
    if (iPosX / 10 == 0):
        alignX_ = 1
    if (iPosX == 0):
        alignX_ = 1
    if (iPosX == 0):
        alignY_ = 1
    if (iPosX / 10 == 1):
        alignX_ = 1
    if (iPosX / 10 == 2):
        alignX_ = 2
    if (iPosX / 10 == 3):
        alignX_ = 3
    # if (iPosX == 0): relPosX = 0.14
    align_ = 10 * alignX_ + alignY_

    l = pad.GetLeftMargin()
    t = pad.GetTopMargin()
    r = pad.GetRightMargin()
    b = pad.GetBottomMargin()

    latex = R.TLatex()
    latex.SetNDC()
    latex.SetTextAngle(0)
    latex.SetTextColor(R.kBlack)

    extraTextSize = extraOverCmsTextSize * cmsTextSize
    pad_ratio = (float(pad.GetWh()) * pad.GetAbsHNDC()) / \
        (float(pad.GetWw()) * pad.GetAbsWNDC())
    if (pad_ratio < 1.):
        pad_ratio = 1.
    if outOfFrame:
        latex.SetTextFont(cmsTextFont)
        latex.SetTextAlign(11)
        latex.SetTextSize(cmsTextSize * t * pad_ratio)
        latex.DrawLatex(l, 1 - t + lumiTextOffset * t, cmsText)

    posX_ = 0
    if iPosX % 10 <= 1:
        posX_ = l + relPosX * (1 - l - r)
    elif (iPosX % 10 == 2):
        posX_ = l + 0.5 * (1 - l - r)
    elif (iPosX % 10 == 3):
        posX_ = 1 - r - relPosX * (1 - l - r)

    posY_ = 1 - t - relPosY * (1 - t - b)
    if not outOfFrame:
        latex.SetTextFont(cmsTextFont)
        latex.SetTextSize(cmsTextSize * t * pad_ratio)
        latex.SetTextAlign(align_)
        latex.DrawLatex(posX_, posY_, cmsText)
        if writeExtraText:
            latex.SetTextFont(extraTextFont)
            latex.SetTextAlign(align_)
            latex.SetTextSize(extraTextSize * t * pad_ratio)
            latex.DrawLatex(
                posX_, posY_ - relExtraDY * cmsTextSize * t, extraText)
            if writeExtraText2:
                latex.DrawLatex(
                    posX_, posY_ - 1.8 * relExtraDY * cmsTextSize * t, extraText2)
    elif writeExtraText:
        if iPosX == 0:
            posX_ = l + relPosX * (1 - l - r)
            posY_ = 1 - t + lumiTextOffset * t
        latex.SetTextFont(extraTextFont)
        latex.SetTextSize(extraTextSize * t * pad_ratio)
        latex.SetTextAlign(align_)
        latex.DrawLatex(posX_, posY_, extraText)


def LimitBandTGraphFromJSON(js, central, lo, hi):
    xvals = []
    yvals = []
    yvals_lo = []
    yvals_hi = []
    for key in js:
        xvals.append(float(key))
        yvals.append(js[key][central])
        yvals_lo.append(js[key][central] - js[key][lo])
        yvals_hi.append(js[key][hi] - js[key][central])
    graph = R.TGraphAsymmErrors(len(xvals), array('d', xvals), array('d', yvals), array(
        'd', [0]), array('d', [0]), array('d', yvals_lo), array('d', yvals_hi))
    graph.Sort()
    return graph

def LimitTGraphFromJSON(js, label):
    xvals = []
    yvals = []
    for key in js:
        xvals.append(float(key))
        yvals.append(js[key][label])
    graph = R.TGraph(len(xvals), array('d', xvals), array('d', yvals))
    graph.Sort()
    return graph


def LimitTGraphFromJSONFile(jsfile, label):
    with open(jsfile) as jsonfile:
        js = json.load(jsonfile)
    return LimitTGraphFromJSON(js, label)

def StandardLimitsFromJSONFile(json_file, draw=['obs', 'exp0', 'exp1', 'exp2']):
    graphs = {}
    data = {}
    with open(json_file) as jsonfile:
        data = json.load(jsonfile)
    if 'obs' in draw:
        graphs['obs'] = LimitTGraphFromJSON(data, 'obs')
    if 'exp0' in draw or 'exp' in draw:
        graphs['exp0'] = LimitTGraphFromJSON(data, 'exp0')
    #if 'exp1' in draw or 'exp' in draw:
    #    graphs['exp1'] = LimitBandTGraphFromJSON(data, 'exp0', 'exp-1', 'exp+1')
    #if 'exp2' in draw or 'exp' in draw:
    #    graphs['exp2'] = LimitBandTGraphFromJSON(data, 'exp0', 'exp-2', 'exp+2')
    return graphs

def DrawTitle(pad, text, align, textOffset=0.2,textSize=0.6):
    pad_backup = R.gPad
    pad.cd()
    t = pad.GetTopMargin()
    l = pad.GetLeftMargin()
    r = pad.GetRightMargin()

    pad_ratio = (float(pad.GetWh()) * pad.GetAbsHNDC()) / \
        (float(pad.GetWw()) * pad.GetAbsWNDC())
    if pad_ratio < 1.:
        pad_ratio = 1.

    latex = R.TLatex()
    latex.SetNDC()
    latex.SetTextAngle(0)
    latex.SetTextColor(R.kBlack)
    latex.SetTextFont(42)
    latex.SetTextSize(textSize * t * pad_ratio)

    y_off = 1 - t + textOffset * t
    if align == 1:
        latex.SetTextAlign(11)
    if align == 1:
        latex.DrawLatex(l, y_off, text)
    if align == 2:
        latex.SetTextAlign(21)
    if align == 2:
        latex.DrawLatex(l + (1 - l - r) * 0.5, y_off, text)
    if align == 3:
        latex.SetTextAlign(31)
    if align == 3:
        latex.DrawLatex(1 - r, y_off, text)
    pad_backup.cd()

def GetAxisHist(pad):
    pad_obs = pad.GetListOfPrimitives()
    if pad_obs is None:
        return None
    obj = None
    for obj in pad_obs:
        if obj.InheritsFrom(R.TH1.Class()):
            return obj
        if obj.InheritsFrom(R.TMultiGraph.Class()):
            return obj.GetHistogram()
        if obj.InheritsFrom(R.TGraph.Class()):
            return obj.GetHistogram()
        if obj.InheritsFrom(R.THStack.Class()):
            return obj.GetHistogram()
    return None

def FixBothRanges(pad, fix_y_lo, frac_lo, fix_y_hi, frac_hi):
    """Adjusts y-axis range such that a lower and a higher value are located a
    fixed fraction of the frame height away from a new minimum and maximum
    respectively.

    This function is useful in conjunction with GetPadYMax which returns the
    maximum or minimum y value of all histograms and graphs drawn on the pad.

    In the example below, the minimum and maximum values found via this function
    are used as the `fix_y_lo` and `fix_y_hi` arguments, and the spacing fractions
    as 0.15 and 0.30 respectively.

    @code
    FixBothRanges(pad, GetPadYMin(pad), 0.15, GetPadYMax(pad), 0.30)
    @endcode

    ![](figures/FixBothRanges.png)

    Args:
        pad (TPad): A TPad on which histograms and graphs have already been drawn
        fix_y_lo (float): The y value which will end up a fraction `frac_lo` above
                          the new axis minimum.
        frac_lo (float): A fraction of the y-axis height
        fix_y_hi (float): The y value which will end up a fraction `frac_hi` below
                         from the new axis maximum.
        frac_hi (float): A fraction of the y-axis height
    """
    hobj = GetAxisHist(pad)
    ymin = fix_y_lo
    ymax = fix_y_hi
    if R.gPad.GetLogy():
        if ymin == 0.:
            print 'Cannot adjust log-scale y-axis range if the minimum is zero!'
            return
        ymin = math.log10(ymin)
        ymax = math.log10(ymax)
    fl = frac_lo
    fh = frac_hi

    ymaxn = (
        (1. / (1. - (fh*fl/((1.-fl)*(1.-fh))))) *
        (1. / (1. - fh)) *
        (ymax - fh*ymin)
        )
    yminn = (ymin - fl*ymaxn) / (1. - fl)
    if R.gPad.GetLogy():
        yminn = math.pow(10, yminn)
        ymaxn = math.pow(10, ymaxn)
    hobj.SetMinimum(yminn)
    hobj.SetMaximum(ymaxn)

def Set(obj, **kwargs):
    for key, value in kwargs.iteritems():
        if value is None:
            getattr(obj, 'Set' + key)()
        elif isinstance(value, (list, tuple)):
            getattr(obj, 'Set' + key)(*value)
        else:
            getattr(obj, 'Set' + key)(value)

def StyleLimitBand(graph_dict, overwrite_style_dict=None):
    style_dict = {
            'obs' : { 'LineWidth' : 2},
            'exp0' : { 'LineWidth' : 2, 'LineColor' : R.kRed},
            #'exp1' : { 'FillColor' : R.kGreen},
            #'exp2' : { 'FillColor' : R.kYellow}
            }
    if overwrite_style_dict is not None:
        for key in overwrite_style_dict:
            if key in style_dict:
                style_dict[key].update(overwrite_style_dict[key])
            else:
                style_dict[key] = overwrite_style_dict[key]
    for key in graph_dict:
        Set(graph_dict[key],**style_dict[key])

def DrawLimitBand(pad, graph_dict, draw=['exp2', 'exp1', 'exp0', 'obs'], draw_legend=None,
                  legend=None, legend_overwrite=None):
    legend_dict = {
        'obs' : { 'Label' : 'Observed', 'LegendStyle' : 'LP', 'DrawStyle' : 'PSAME'},
        'exp0' : { 'Label' : 'Expected', 'LegendStyle' : 'LP', 'DrawStyle' : 'PSAME'},
        #'exp1' : { 'Label' : '#pm1#sigma Expected', 'LegendStyle' : 'F', 'DrawStyle' : '3SAME'},
        #'exp2' : { 'Label' : '#pm2#sigma Expected', 'LegendStyle' : 'F', 'DrawStyle' : '3SAME'}
    }
    print legend_overwrite
    if legend_overwrite is not None:
        for key in legend_overwrite:
            if key in legend_dict:
                legend_dict[key].update(legend_overwrite[key])
            else:
                legend_dict[key] = legend_overwrite[key]

    print legend_dict
    pad.cd()
    for key in draw:
        if key in graph_dict:
            print legend_dict[key]['DrawStyle']
            graph_dict[key].Draw(legend_dict[key]['DrawStyle'])
    if legend is not None:
        if draw_legend is None:
            draw_legend = reversed(draw)
        for key in draw_legend:
            if key in graph_dict:
                legend.AddEntry(graph_dict[key],legend_dict[key]['Label'],legend_dict[key]['LegendStyle'])

def TwoPadSplit(split_point, gap_low, gap_high):
    upper = R.TPad('upper', 'upper', 0., 0., 1., 1.)
    upper.SetBottomMargin(split_point + gap_high)
    upper.SetFillStyle(4000)
    upper.Draw()
    lower = R.TPad('lower', 'lower', 0., 0., 1., 1.)
    lower.SetTopMargin(1 - split_point + gap_low)
    lower.SetFillStyle(4000)
    lower.Draw()
    upper.cd()
    result = [upper, lower]
    return result

def PositionedLegend(width, height, pos, offset, horizontaloffset=None, verticaloffset=None):
    o = offset
    ho = horizontaloffset
    vo = verticaloffset
    if not ho:
      ho = o
    if not vo:
      vo = 0 
    w = width
    h = height
    l = R.gPad.GetLeftMargin()
    t = R.gPad.GetTopMargin() + vo
    b = R.gPad.GetBottomMargin()
    r = R.gPad.GetRightMargin()
    if pos == 1:
        return R.TLegend(l + ho, 1 - t - o - h, l + ho + w, 1 - t - o, '', 'NBNDC')
    if pos == 2:
        c = l + 0.5 * (1 - l - r)
        return R.TLegend(c - 0.5 * w, 1 - t - o - h, c + 0.5 * w, 1 - t - o, '', 'NBNDC')
    if pos == 3:
        return R.TLegend(1 - r - ho - w, 1 - t - o - h, 1 - r - ho, 1 - t - o, '', 'NBNDC')
    if pos == 4:
        return R.TLegend(l + ho, b + o, l + ho + w, b + o + h, '', 'NBNDC')
    if pos == 5:
        c = l + 0.5 * (1 - l - r)
        return R.TLegend(c - 0.5 * w, b + o, c + 0.5 * w, b + o + h, '', 'NBNDC')
    if pos == 6:
        return R.TLegend(1 - r - ho - w, b + o, 1 - r - ho, b + o + h, '', 'NBNDC')


def ChooseAxisLabels(axis,labels,size=0.04,logx=False):
  axis.GetXaxis().SetLabelSize(0)
  axis_min = axis.GetXaxis().GetXmin()
  axis_max = axis.GetXaxis().GetXmax()
  b = R.gPad.GetBottomMargin()
  l = R.gPad.GetLeftMargin()
  r = 1 -R.gPad.GetRightMargin()
  for num in labels:
    latex = R.TLatex()
    latex.SetTextAngle(0)
    latex.SetTextColor(R.kBlack)
    latex.SetTextFont(42)
    latex.SetTextSize(size)
    latex.SetTextAlign(21)
    if not logx:
      x_shift = ((num - axis_min)*(r - l))/(axis_max - axis_min)
    else:
      x_shift = (math.log(num,10) - math.log(axis_min,10))*(r - l)/(math.log(axis_max,10) - math.log(axis_min,10))
    latex.DrawLatex(l+x_shift, b-0.04, str(num))

        
## Boilerplate
ModTDRStyle()
R.gStyle.SetNdivisions(510, 'XYZ') # probably looks better


for bb_gg in ['bb','gg']:
#for bb_gg in [1]:

  output_plot = '{}/limit_comparison_{}'.format(output,bb_gg)
  process = "{}#phi".format(bb_gg)

  ratio_files_bb_gg = []
  for i in ratio_files: ratio_files_bb_gg.append(i.replace("*",bb_gg))
  main_file_bb_gg = main_file.replace("*",bb_gg)
  print ratio_files_bb_gg
  for i in range(0,len(ratio_files)):
    ratio_files_bb_gg.append(ratio_files[i].replace("*",bb_gg))
    plotting_dict[ratio_files_bb_gg[i]] = plotting_dict[ratio_files[i]]


  plotting_dict[main_file_bb_gg] = plotting_dict[main_file]

  ##### Set Up Plotting Template #####

  canv = R.TCanvas(output_plot, output_plot)

  if ratio:
    pads = TwoPadSplit(0.30, 0.01, 0.01)
  else:
    pads = []
    pads.append(R.TPad('upper', 'upper', 0., 0., 1., 1.))
    pads[0].SetFillStyle(4000)
    pads[0].Draw()

  print plotting_dict
  print main_file_bb_gg
  main_file_root = StandardLimitsFromJSONFile(main_file_bb_gg, plotting_dict[main_file_bb_gg]["line"].keys()[0])
  other_file_root_dict = {}
  for i in ratio_files_bb_gg: 
    other_file_root_dict[i] = StandardLimitsFromJSONFile(i, plotting_dict[i]["line"].keys()[0])
  axis = CreateAxisHists(len(pads), other_file_root_dict[ratio_files_bb_gg[-1]].values()[0], True)
  DrawAxisHists(pads, axis, pads[0])

  #DrawTitle(pads[0], "Observed:", 3, textOffset=0.1,textSize=0.6)
  if draw_main_file:
    #latex = R.TLatex()
    #latex.SetNDC()
    #latex.SetTextAngle(0)
    #latex.SetTextColor(R.kBlack)
    #latex.SetTextFont(42)
    #latex.SetTextSize(0.03)
    #latex.DrawLatex(0.62, 0.9, "Observed:")
    legend = PositionedLegend(0.34, 0.2, 3, 0.015,verticaloffset=0.035)
    #legend = PositionedLegend(0.34, 0.2, 3, 0.015,verticaloffset=0.55)
    #legend = PositionedLegend(0.34, 0.2, 3, 0.015, verticaloffset=0.35)

    legend.SetTextSize(0.03)
    #latex2 = R.TLatex()
    #latex2.SetNDC()
    #latex2.SetTextAngle(0)
    #latex2.SetTextColor(R.kBlack)
    #latex2.SetTextFont(42)
    #latex2.SetTextSize(0.03)
    #latex2.DrawLatex(0.62, 0.81, "Expected:")
    #legend2 = PositionedLegend(0.34, 0.2, 3, 0.015,verticaloffset=0.125)
    #legend2 = PositionedLegend(0.34, 0.1, 3, 0.015,verticaloffset=0.125)
    #legend2.SetTextSize(0.03)
    DrawCMSLogo(canv, 'CMS', 'Internal', 11, 0.04, 0.03, 1.0, '', 1)
  else:
    latex = R.TLatex()
    latex.SetNDC()
    latex.SetTextAngle(0)
    latex.SetTextColor(R.kBlack)
    latex.SetTextFont(42)
    latex.SetTextSize(0.03)
    latex.DrawLatex(0.66, 0.9, "Expected:")
    legend2 = PositionedLegend(0.3, 0.2, 3, 0.015,verticaloffset=0.035)
    legend2.SetTextSize(0.03)
    DrawCMSLogo(canv, 'CMS', 'Preliminary', 11, 0.04, 0.03, 1.0, '', 1)



  Set(pads[0], Logx=True, Logy=True)
  if ratio:  Set(pads[1], Logx=False, Logy=False)

  axis[0].GetYaxis().SetTitle('95% CL limit on #sigma#font[42]{('+process+')}#upoint#font[42]{BR}#font[42]{(#phi#rightarrow#tau#tau)} [pb]')
  #axis[0].GetYaxis().SetTitle('g_{U}')
  if ratio:  
    axis[1].GetXaxis().SetNoExponent()
    axis[1].GetYaxis().SetLabelSize(0.025)
    axis[1].GetXaxis().SetTitle('m_{#phi} (GeV)')
    axis[1].GetXaxis().SetMoreLogLabels()
    axis[0].GetXaxis().SetLabelSize(0)
    axis[0].GetXaxis().SetLabelOffset(axis[0].GetXaxis().GetLabelOffset()*2)
  else:
    axis[0].GetXaxis().SetNoExponent()
    axis[0].GetYaxis().SetLabelSize(0.025)
    axis[0].GetXaxis().SetMoreLogLabels()
    axis[0].GetXaxis().SetTitle('m_{#phi} (GeV)')
    #ChooseAxisLabels(axis[0],[100,200,300,600,1000,2000],size=0.04,logx=True)
    #axis[0].GetXaxis().SetTitle('M_{U} (TeV)')



  axis[0].GetYaxis().SetLabelSize(0.04)
  DrawTitle(pads[0], title_right, 3)
  #DrawTitle(pads[0], title_left, 1)

  ##### Draw Limits #####
  FixBothRanges(pads[0], 0.0001, 0, 500, 0)
  StyleLimitBand(main_file_root,overwrite_style_dict=plotting_dict[main_file]['line'])
  if draw_main_file:
    DrawLimitBand(pads[0], main_file_root,legend=legend,legend_overwrite=plotting_dict[main_file]['legend'])
  for i_key in ratio_files:
    i_key = i_key.replace("*",bb_gg)
  #for i_key,i_val in other_file_root_dict.items():
    i_val = other_file_root_dict[i_key]
    StyleLimitBand(i_val,overwrite_style_dict=plotting_dict[i_key]['line'])
    if main_file in i_key:
      if draw_main_file:
        DrawLimitBand(pads[0], i_val,legend=legend,legend_overwrite=plotting_dict[i_key]['legend'])
    else:
      #DrawLimitBand(pads[0], i_val,legend=legend2,legend_overwrite=plotting_dict[i_key]['legend'])
      DrawLimitBand(pads[0], i_val,legend=legend,legend_overwrite=plotting_dict[i_key]['legend'])

  ##### Draw Limit Ratio #####
  if ratio:
    #FixBothRanges(pads[1], 0.05, 0.1, 10, 0.1)
    FixBothRanges(pads[1], 0.9, 0.1, 1.1, 0.1)

    pads[1].cd()
    line = R.TLine(0.5,1,5,1)
    line.SetLineWidth(plotting_dict[main_file]["line"]["exp0"]["LineWidth"])
    line.SetLineColor(plotting_dict[main_file]["line"]["exp0"]["LineColor"])
    line.SetLineStyle(plotting_dict[main_file]["line"]["exp0"]["LineStyle"])
    #line.SetMarkerStyle(plotting_dict[main_file]["line"]["exp0"]["MarkerStyle"])
    #line.SetMarkerColor(plotting_dict[main_file]["line"]["exp0"]["MarkerColor"])
    line.Draw()

    for i in ratio_files_bb_gg:
      DoRatioJson(i,main_file_bb_gg,i.replace('.json','_ratio.json'))

    ratio_file_root_dict = {}
    for i in ratio_files_bb_gg: ratio_file_root_dict[i] = StandardLimitsFromJSONFile(i.replace('.json','_ratio.json'), 'exp0')

    for i_key,i_val in ratio_file_root_dict.items():
      StyleLimitBand(i_val,overwrite_style_dict=plotting_dict[i_key]['line'])
      DrawLimitBand(pads[1], i_val, legend_overwrite=plotting_dict[i_key]['legend'])

  if draw_main_file:
    pads[0].cd()
    legend.Draw()
  #legend2.Draw()
  canv.Print('.pdf')


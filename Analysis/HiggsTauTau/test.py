import ROOT as R
from functools import partial

def SetTDRStyle():
  # For the canvas:
  R.gStyle.SetCanvasBorderMode(0)
  R.gStyle.SetCanvasColor(R.kWhite)
  R.gStyle.SetCanvasDefH(600)  # Height of canvas
  R.gStyle.SetCanvasDefW(600)  # Width of canvas
  R.gStyle.SetCanvasDefX(0)    # POsition on screen
  R.gStyle.SetCanvasDefY(0)

  # For the Pad:
  R.gStyle.SetPadBorderMode(0)
  # R.gStyle.SetPadBorderSize(Width_t size = 1)
  R.gStyle.SetPadColor(R.kWhite)
  R.gStyle.SetPadGridX(False)
  R.gStyle.SetPadGridY(False)
  R.gStyle.SetGridColor(0)
  R.gStyle.SetGridStyle(3)
  R.gStyle.SetGridWidth(1)

  # For the frame:
  R.gStyle.SetFrameBorderMode(0)
  R.gStyle.SetFrameBorderSize(1)
  R.gStyle.SetFrameFillColor(0)
  R.gStyle.SetFrameFillStyle(0)
  R.gStyle.SetFrameLineColor(1)
  R.gStyle.SetFrameLineStyle(1)
  R.gStyle.SetFrameLineWidth(1)

  # For the histo:
  # R.gStyle.SetHistFillColor(1)
  # R.gStyle.SetHistFillStyle(0)
  R.gStyle.SetHistLineColor(1)
  R.gStyle.SetHistLineStyle(0)
  R.gStyle.SetHistLineWidth(1)
  # R.gStyle.SetLegoInnerR(Float_t rad = 0.5)
  # R.gStyle.SetNumberContours(Int_t number = 20)

  R.gStyle.SetEndErrorSize(2)
  # R.gStyle.SetErrorMarker(20)
  # R.gStyle.SetErrorX(0.)

  R.gStyle.SetMarkerStyle(20)

  # For the fit/function:
  R.gStyle.SetOptFit(1)
  R.gStyle.SetFitFormat("5.4g")
  R.gStyle.SetFuncColor(2)
  R.gStyle.SetFuncStyle(1)
  R.gStyle.SetFuncWidth(1)

  # For the date:
  R.gStyle.SetOptDate(0)
  # R.gStyle.SetDateX(Float_t x = 0.01)
  # R.gStyle.SetDateY(Float_t y = 0.01)

  # For the statistics box:
  R.gStyle.SetOptFile(0)
  R.gStyle.SetOptStat(0);  # To display the mean and RMS:   SetOptStat("mr")
  R.gStyle.SetStatColor(R.kWhite)
  R.gStyle.SetStatFont(42)
  R.gStyle.SetStatFontSize(0.025)
  R.gStyle.SetStatTextColor(1)
  R.gStyle.SetStatFormat("6.4g")
  R.gStyle.SetStatBorderSize(1)
  R.gStyle.SetStatH(0.1)
  R.gStyle.SetStatW(0.15)
  # R.gStyle.SetStatStyle(Style_t style = 1001)
  # R.gStyle.SetStatX(Float_t x = 0)
  # R.gStyle.SetStatY(Float_t y = 0)

  # Margins:
  R.gStyle.SetPadTopMargin(0.05)
  R.gStyle.SetPadBottomMargin(0.13)
  R.gStyle.SetPadLeftMargin(0.16)
  R.gStyle.SetPadRightMargin(0.02)

  # For the Global title:
  R.gStyle.SetOptTitle(0)
  R.gStyle.SetTitleFont(42)
  R.gStyle.SetTitleColor(1)
  R.gStyle.SetTitleTextColor(1)
  R.gStyle.SetTitleFillColor(10)
  R.gStyle.SetTitleFontSize(0.05)
  # R.gStyle.SetTitleH(0); # Set the height of the title box
  # R.gStyle.SetTitleW(0); # Set the width of the title box
  # R.gStyle.SetTitleX(0); # Set the position of the title box
  # R.gStyle.SetTitleY(0.985); # Set the position of the title box
  # R.gStyle.SetTitleStyle(Style_t style = 1001)
  # R.gStyle.SetTitleBorderSize(2)

  # For the axis titles:
  R.gStyle.SetTitleColor(1, "XYZ")
  R.gStyle.SetTitleFont(42, "XYZ")
  R.gStyle.SetTitleSize(0.06, "XYZ")
  # Another way to set the size?
  # R.gStyle.SetTitleXSize(Float_t size = 0.02)
  # R.gStyle.SetTitleYSize(Float_t size = 0.02)
  R.gStyle.SetTitleXOffset(0.9)
  R.gStyle.SetTitleYOffset(1.25)
  # R.gStyle.SetTitleOffset(1.1, "Y"); # Another way to set the Offset

  # For the axis labels:

  R.gStyle.SetLabelColor(1, "XYZ")
  R.gStyle.SetLabelFont(42, "XYZ")
  R.gStyle.SetLabelOffset(0.007, "XYZ")
  R.gStyle.SetLabelSize(0.05, "XYZ")

  # For the axis:

  R.gStyle.SetAxisColor(1, "XYZ")
  R.gStyle.SetStripDecimals(True)
  R.gStyle.SetTickLength(0.03, "XYZ")
  R.gStyle.SetNdivisions(510, "XYZ")
  R.gStyle.SetPadTickX(1)
  R.gStyle.SetPadTickY(1)

  # Change for log plots:
  R.gStyle.SetOptLogx(0)
  R.gStyle.SetOptLogy(0)
  R.gStyle.SetOptLogz(0)

  # Postscript options:
  R.gStyle.SetPaperSize(20., 20.)
  # R.gStyle.SetLineScalePS(Float_t scale = 3)
  # R.gStyle.SetLineStyleString(Int_t i, const char* text)
  # R.gStyle.SetHeaderPS(const char* header)
  # R.gStyle.SetTitlePS(const char* pstitle)

  # R.gStyle.SetBarOffset(Float_t baroff = 0.5)
  # R.gStyle.SetBarWidth(Float_t barwidth = 0.5)
  # R.gStyle.SetPaintTextFormat(const char* format = "g")
  # R.gStyle.SetPalette(Int_t ncolors = 0, Int_t* colors = 0)
  # R.gStyle.SetTimeOffset(Double_t toffset)
  # R.gStyle.SetHistMinimumZero(kTRUE)

  R.gStyle.SetHatchesLineWidth(5)
  R.gStyle.SetHatchesSpacing(0.05)

def ModTDRStyle(width=600, height=600, t=0.06, b=0.12, l=0.16, r=0.04):
  SetTDRStyle()

  # Set the default canvas width and height in pixels
  R.gStyle.SetCanvasDefW(width)
  R.gStyle.SetCanvasDefH(height)

  # Set the default margins.
  # These are given as fractions of the pad height for `Top` and `Bottom` and
  # the pad width for `Left` and `Right`. But we want to specify all of these
  # as fractions of the shortest length.
  def_w = float(R.gStyle.GetCanvasDefW())
  def_h = float(R.gStyle.GetCanvasDefH())

  scale_h = (def_w / def_h) if (def_h > def_w) else 1.
  scale_w = (def_h / def_w) if (def_w > def_h) else 1.

  def_min = def_h if (def_h < def_w) else def_w

  R.gStyle.SetPadTopMargin(t * scale_h);      # default 0.05
  R.gStyle.SetPadBottomMargin(b * scale_h);   # default 0.13
  R.gStyle.SetPadLeftMargin(l * scale_w);     # default 0.16
  R.gStyle.SetPadRightMargin(r * scale_w);    # default 0.02
  # But note the new CMS style sets these:
  # 0.08, 0.12, 0.12, 0.04

  # Set number of axis tick divisions
  R.gStyle.SetNdivisions(506, "XYZ") # default 510

  # Some marker properties not set in the default tdr style
  R.gStyle.SetMarkerColor(R.kBlack)
  R.gStyle.SetMarkerSize(1.0)


  R.gStyle.SetLabelOffset(0.007, "YZ")
  # This is an adhoc adjustment to scale the x-axis label
  # offset when we strect plot vertically
  # Will also need to increase if first x-axis label has more than one digit
  R.gStyle.SetLabelOffset(0.005 * (3. - 2. / scale_h), "X")

  # In this next part we do a slightly involved calculation to set the axis
  # title offsets, depending on the values of the TPad dimensions and margins.
  # This is to try and ensure that regardless of how these pad values are set,
  # the axis titles will be located towards the edges of the canvas and not get
  # pushed off the edge - which can often happen if a fixed value is used.
  title_size = 0.05
  title_px = title_size * def_min
  label_size = 0.04
  R.gStyle.SetTitleSize(title_size, "XYZ")
  R.gStyle.SetLabelSize(label_size, "XYZ")

  R.gStyle.SetTitleXOffset(0.5 * scale_h *
                          (1.2 * (def_h * b * scale_h - 0.6 * title_px)) /
                          title_px)
  R.gStyle.SetTitleYOffset(0.5 * scale_w *
                          (1.2 * (def_w * l * scale_w - 0.6 * title_px)) /
                          title_px)

  # Only draw ticks where we have an axis
  R.gStyle.SetPadTickX(0)
  R.gStyle.SetPadTickY(0)
  R.gStyle.SetTickLength(0.02, "XYZ")

  R.gStyle.SetLegendBorderSize(0)
  R.gStyle.SetLegendFont(42)
  R.gStyle.SetLegendFillColor(0)
  R.gStyle.SetFillColor(0)

  R.gROOT.ForceStyle()


def DrawCMSLogo(pad, cmsText, extraText, iPosX, relPosX, relPosY, relExtraDY):
  pad.cd()
  cmsTextFont = 61 # default is helvetic-bold

  writeExtraText = len(extraText) > 0
  extraTextFont = 52

  # text sizes and text offsets with respect to the top frame
  # in unit of the top margin size
  lumiTextOffset = 0.2
  cmsTextSize = 0.8
  # float cmsTextOffset    = 0.1;  // only used in outOfFrame version

  # ratio of "CMS" and extra text size
  extraOverCmsTextSize = 0.76

  outOfFrame = False
  if iPosX / 10 == 0:
    outOfFrame = True

  alignY_ = 3
  alignX_ = 2
  if (iPosX / 10 == 0): alignX_ = 1
  if (iPosX == 0): alignX_ = 1
  if (iPosX == 0): alignY_ = 1
  if (iPosX / 10 == 1): alignX_ = 1
  if (iPosX / 10 == 2): alignX_ = 2
  if (iPosX / 10 == 3): alignX_ = 3
  if (iPosX == 0): relPosX = 0.14
  align_ = 10 * alignX_ + alignY_

  l = pad.GetLeftMargin()
  t = pad.GetTopMargin()
  r = pad.GetRightMargin()
  b = pad.GetBottomMargin()

  latex = R.TLatex()
  latex.SetNDC()
  latex.SetTextAngle(0)
  latex.SetTextColor(R.kBlack)

  extraTextSize = extraOverCmsTextSize * cmsTextSize;
  pad_ratio = (float(pad.GetWh()) * pad.GetAbsHNDC()) / (float(pad.GetWw()) * pad.GetAbsWNDC())
  if (pad_ratio < 1.): pad_ratio = 1.


  if outOfFrame:
    latex.SetTextFont(cmsTextFont)
    latex.SetTextAlign(11)
    latex.SetTextSize(cmsTextSize * t * pad_ratio)
    latex.DrawLatex(l, 1 - t + lumiTextOffset * t, cmsText)


  posX_ = 0;
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
      latex.DrawLatex(posX_, posY_ - relExtraDY * cmsTextSize * t, extraText)
  elif writeExtraText:
    if iPosX == 0:
      posX_ = l + relPosX * (1 - l - r)
      posY_ = 1 - t + lumiTextOffset * t
    latex.SetTextFont(extraTextFont)
    latex.SetTextSize(extraTextSize * t * pad_ratio)
    latex.SetTextAlign(align_)
    latex.DrawLatex(posX_, posY_, extraText)


def PositionedLegend(width, height, pos, offset):
  o = offset
  w = width
  h = height
  l = R.gPad.GetLeftMargin()
  t = R.gPad.GetTopMargin()
  b = R.gPad.GetBottomMargin()
  r = R.gPad.GetRightMargin()
  if pos == 1:
    return R.TLegend(l + o, 1 - t - o - h, l + o + w, 1 - t - o, "", "NBNDC")
  if pos == 2:
    c = l + 0.5 * (1 - l - r)
    return R.TLegend(c - 0.5 * w, 1 - t - o - h, c + 0.5 * w, 1 - t - o, "", "NBNDC")
  if pos == 3 :
    return R.TLegend(1 - r - o - w, 1 - t - o - h, 1 - r - o, 1 - t - o, "", "NBNDC")
  if pos == 4:
    return R.TLegend(l + o, b + o, l + o + w, b + o + h, "", "NBNDC")
  if pos == 5:
    c = l + 0.5 * (1 - l - r)
    return R.TLegend(c - 0.5 * w, b + o, c + 0.5 * w, b + o + h, "", "NBNDC")
  if pos == 6:
    return R.TLegend(1 - r - o - w, b + o, 1 - r - o, b + o + h, "","NBNDC");


def Get(file, obj):
  R.TH1.AddDirectory(False)
  f_in = R.TFile(file)
  res = R.gDirectory.Get(obj)
  f_in.Close()
  return res

def RocCurveFrom1DHists(h_x, h_y, cut_is_greater_than):
  backup = R.TH1.AddDirectoryStatus()
  R.TH1.AddDirectory(False)
  x_den = h_x.Clone()
  x_num = h_x.Clone()
  x_err = R.Double(0.)
  x_int = h_x.IntegralAndError(0, h_x.GetNbinsX() + 1, x_err)
  for i in range(1, h_x.GetNbinsX() + 1):
    x_part_err = R.Double(0.)
    x_part_int = h_x.IntegralAndError(i, h_x.GetNbinsX() + 1, x_part_err) if cut_is_greater_than else h_x.IntegralAndError(0, i, x_part_err)
    x_den.SetBinContent(i, x_int)
    x_den.SetBinError(i, x_err)
    x_num.SetBinContent(i, x_part_int)
    x_num.SetBinError(i, x_part_err)
  y_den = h_y.Clone()
  y_num = h_y.Clone()
  y_err = R.Double(0.)
  y_int = h_y.IntegralAndError(0, h_y.GetNbinsX() + 1, y_err)
  for i in range(1, h_y.GetNbinsX() + 1):
    y_part_err = R.Double(0.)
    y_part_int = h_y.IntegralAndError(i, h_y.GetNbinsX() + 1, y_part_err) if cut_is_greater_than else h_y.IntegralAndError(0, i, y_part_err)
    y_den.SetBinContent(i, y_int)
    y_den.SetBinError(i, y_err)
    y_num.SetBinContent(i, y_part_int)
    y_num.SetBinError(i, y_part_err)
  # x_den.Print("all")
  # x_num.Print("all")
  # y_den.Print("all")
  # y_num.Print("all")
  x_gr = R.TGraphAsymmErrors(x_num, x_den)
  y_gr = R.TGraphAsymmErrors(y_num, y_den)

  res = y_gr.Clone()
  for i in range (0, res.GetN()):
    res.GetX()[i] = x_gr.GetY()[i]
    res.GetEXlow()[i] = x_gr.GetEYlow()[i]
    res.GetEXhigh()[i] = x_gr.GetEYhigh()[i]
  res.Sort()
  R.TH1.AddDirectory(backup)
  return res

def CreateAxisHist(src, at_limits):
  backup = R.gPad
  tmp = R.TCanvas()
  tmp.cd()
  src.Draw("AP")
  result = src.GetHistogram().Clone()
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
      if x < min: min = float(x)
      if x > max: max = float(x)
    result.GetXaxis().SetLimits(min, max)
  R.gPad = backup
  return result

class MultiGraphPlot:
  def __init__(self, **args):
    self.styler = args.get('styler', ModTDRStyle)
    self.draw = args.get('draw')
    self.outputFileName = args.get('outputFileName','test.pdf')
    self.legend = args.get('legend', None)
    self.pad = args.get('pad', None)
    self.logy = args.get('logy', None)

  def Create(self):
    self.styler()
    if not self.pad:
      self.canv = R.TCanvas('canv', 'canv')
      self.pad = R.TPad("pad", "pad", 0., 0., 1., 1.)
    self.pad.SetGridx(True)
    self.pad.SetGridy(True)
    self.pad.Draw()
    self.pad.cd()
    self.axis = CreateAxisHist(self.draw[0].src, True)
    self.pad.cd()
    self.axis.GetXaxis().SetLimits(0., 1.)
    self.axis.GetYaxis().SetRangeUser(1E-4, 0.015)
    self.axis.GetXaxis().SetTitle("#tau_{h} Efficiency")
    self.axis.GetYaxis().SetTitle("Jet#rightarrow#tau_{h} Fake Rate")
    self.axis.Draw()
    for d in self.draw:
      ele = d.get()
      ele.Draw(d.drawOpts + 'SAME')
    if self.legend:
      self.leg = self.legend()
      for d in self.draw:
        self.leg.AddEntry(d.src, d.src.GetTitle() if not d.legend else d.legend, 'L')
      self.leg.Draw()
    if self.logy is not None: self.pad.SetLogy(self.logy)
    DrawCMSLogo(self.pad, "CMS", "Simulation", 0, 0.045, 0.035, 1.2)
    self.canv.SaveAs(self.outputFileName)

def StyleGraph(obj, **args):
  obj.SetLineColor(args.get('lineColor', 1))
  obj.SetFillColor(args.get('fillColor', 0))
  obj.SetMarkerColor(args.get('markerColor', 1))
  obj.SetMarkerStyle(args.get('markerStyle', 20))
  obj.SetLineWidth(2)

class Graph:
  def __init__(self, **args):
    self.src = args.get('src')
    self.style = args.get('style')
    self.drawOpts = args.get('drawOpts', 'CP')
    self.legend = args.get('legend', None)

  def get(self):
    self.style(self.src)
    return self.src

f_real = 'output/toytau_test.root'
f_fake = 'output/toytau_qcd.root'
d_rec = 'ToyTauStudy_taus'
d_toy = 'ToyTauStudy_toyTaus'
dm = 'all_dm'


ModTDRStyle()


plot = MultiGraphPlot(
  outputFileName = 'neutral_hadron_iso_roc_curves.pdf',
  draw = [
    Graph(
      src = RocCurveFrom1DHists(Get(f_real, d_rec+'/'+dm+'/cmb_iso'), Get(f_fake, d_rec+'/'+dm+'_fake/cmb_iso'), False),
      style = partial(StyleGraph, lineColor=2), legend = 'Reco #tau_{h} Combined Iso'
    ),
    Graph(
      src = RocCurveFrom1DHists(Get(f_real, d_rec+'/'+dm+'/cmb_iso_gvn_nh_iso_10p0_scaled'), Get(f_fake, d_rec+'/'+dm+'_fake/cmb_iso_gvn_nh_iso_10p0_scaled'), False),
      style = partial(StyleGraph, lineColor=12), legend = ' + I_{nh} < max(10, 0.1*p_{T}) GeV'
    ),
    Graph(
      src = RocCurveFrom1DHists(Get(f_real, d_rec+'/'+dm+'/cmb_iso_gvn_nh_iso_5p0'), Get(f_fake, d_rec+'/'+dm+'_fake/cmb_iso_gvn_nh_iso_5p0'), False),
      style = partial(StyleGraph, lineColor=48), legend = ' + I_{nh} < 5 GeV'
    ),
    Graph(
      src = RocCurveFrom1DHists(Get(f_real, d_rec+'/'+dm+'/cmb_iso_gvn_nh_iso_2p0'), Get(f_fake, d_rec+'/'+dm+'_fake/cmb_iso_gvn_nh_iso_2p0'), False),
      style = partial(StyleGraph, lineColor=46), legend = ' + I_{nh} < 2 GeV'
    ),
    Graph(
      src = RocCurveFrom1DHists(Get(f_real, d_toy+'/'+dm+'/cmb_iso'), Get(f_fake, d_toy+'/'+dm+'_fake/cmb_iso'), False),
      style = partial(StyleGraph, lineColor=4), legend = 'Gen #tau_{h} Combined Iso'
    ),
    Graph(
      src = RocCurveFrom1DHists(Get(f_real, d_toy+'/'+dm+'/cmb_iso_gvn_nh_iso_10p0_scaled'), Get(f_fake, d_toy+'/'+dm+'_fake/cmb_iso_gvn_nh_iso_10p0_scaled'), False),
      style = partial(StyleGraph, lineColor=12), legend = ' + I_{nh} < max(10, 0.1*p_{T}) GeV'
    ),
    Graph(
      src = RocCurveFrom1DHists(Get(f_real, d_toy+'/'+dm+'/cmb_iso_gvn_nh_iso_5p0'), Get(f_fake, d_toy+'/'+dm+'_fake/cmb_iso_gvn_nh_iso_5p0'), False),
      style = partial(StyleGraph, lineColor=39), legend = ' + I_{nh} < 5 GeV'
    ),
    Graph(
      src = RocCurveFrom1DHists(Get(f_real, d_toy+'/'+dm+'/cmb_iso_gvn_nh_iso_2p0'), Get(f_fake, d_toy+'/'+dm+'_fake/cmb_iso_gvn_nh_iso_2p0'), False),
      style = partial(StyleGraph, lineColor=38), legend = ' + I_{nh} < 2 GeV'
    ),
  ],
  legend = partial(PositionedLegend, 0.35, 0.25, 1, 0.02),
  logy = False
)
for d in plot.draw: d.drawOpts = 'LX'
plot.Create()

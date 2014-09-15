#include <string>
// #include "boost/filesystem.hpp"
// #include "boost/regex.hpp"
// #include "boost/format.hpp"
#include "boost/bind.hpp"
// #include "boost/assign/list_of.hpp"
#include "CombineTools/interface/CombineHarvester.h"
#include "CombineTools/interface/Observation.h"
#include "CombineTools/interface/Process.h"
#include "CombineTools/interface/HelperFunctions.h"
#include "CombineTools/interface/MorphFunctions.h"
#include "CombineTools/interface/HttSystematics.h"
#include "CombineTools/interface/RooHttSMPdf.h"
// #include "CombineTools/interface/RooHttYield.h"s
// #include "TH1F.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooGenericPdf.h"

using boost::bind;
using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using std::map;
using std::set;

int main() {
  TH1::AddDirectory(false);
  ch::CombineHarvester cb;

  vector<pair<int, string>> mt_cats = {
      make_pair(8, "muTau_nobtag"),
      make_pair(9, "muTau_btag")};

  vector<string> masses = {"90",  "100", "120", "130", "140", "160", "180",
                           "200", "250", "300", "350", "400", "450", "500",
                           "600", "700", "800", "900", "1000"};

  std::cout << "Adding observations...";
  cb.AddObservations({"*"}, {"htt"}, {"7TeV", "8TeV"}, {"mt"}, mt_cats);
  std::cout << " done\n";

  std::cout << "Adding background processes...";
  cb.AddProcesses({"*"}, {"htt"}, {"7TeV", "8TeV"}, {"mt"},
                  {"ZTT", "W", "QCD", "ZL", "ZJ", "TT", "VV"}, mt_cats, false);
  std::cout << " done\n";

  std::cout << "Adding signal processes...";
  cb.AddProcesses(masses, {"htt"}, {"7TeV", "8TeV"}, {"mt"}, {"ggH"}, mt_cats,
                  true);
  std::cout << " done\n";

  std::cout << "Adding systematic uncertainties...";
  ch::AddMSSMSystematics(&cb);
  std::cout << " done\n";

  std::cout << "Extracting histograms from input root files...";
  cb.cp().era({"7TeV"}).backgrounds().ExtractShapes(
      "data/demo/htt_mt.inputs-mssm-7TeV-0.root", "$CHANNEL/$PROCESS_fine_binning",
      "$CHANNEL/$PROCESS_$SYSTEMATIC");
  cb.cp().era({"8TeV"}).backgrounds().ExtractShapes(
      "data/demo/htt_mt.inputs-mssm-8TeV-0.root", "$CHANNEL/$PROCESS_fine_binning",
      "$CHANNEL/$PROCESS_$SYSTEMATIC");
  cb.cp().era({"7TeV"}).signals().ExtractShapes(
      "data/demo/htt_mt.inputs-mssm-7TeV-0.root", "$CHANNEL/$PROCESS$MASS_fine_binning",
      "$CHANNEL/$PROCESS$MASS_$SYSTEMATIC");
  cb.cp().era({"8TeV"}).signals().ExtractShapes(
      "data/demo/htt_mt.inputs-mssm-8TeV-0.root", "$CHANNEL/$PROCESS$MASS_fine_binning",
      "$CHANNEL/$PROCESS$MASS_$SYSTEMATIC");
  std::cout << " done\n";

  std::cout << "Scaling signal process rates for acceptance...\n";
  map<string, TGraph> xs;
  for (std::string const& e : {"7TeV", "8TeV"}) {
    for (std::string const& p : {"ggH", "bbH"}) {
      ch::ParseTable(&xs, "data/xsecs_brs/mssm_" + p + "_" + e + "_accept.txt",
                     {p + "_" + e});
    }
  }

  for (std::string const& e : {"7TeV", "8TeV"}) {
    for (std::string const& p : {"ggH", "bbH"}) {
      std::cout << "Scaling for process " << p << " and era " << e << "\n";

      cb.cp().process({p}).era({e}).ForEachProc(
          bind(ch::ScaleProcessRate, _1, &xs, p+"_"+e, ""));
    }
  }

  std::cout << "Setting standardised bin names...";
  cb.ForEachObs(ch::SetStandardBinName<ch::Observation>);
  cb.ForEachProc(ch::SetStandardBinName<ch::Process>);
  cb.ForEachNus(ch::SetStandardBinName<ch::Nuisance>);
  std::cout << " done\n";

  cb.era({"8TeV"}).bin_id({8});

  ch::CombineHarvester cb_hm = cb;

  cb.VariableRebin({0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130,
                    140, 150, 160, 170, 180, 190, 200});

  // Drop shape uncerts on signal in the hm for now,
  // the fb versions aren't in the datacards
  cb_hm.FilterNus([&](ch::Nuisance const* p) {
    return p->type() == "shape";
  });

  bool do_parametric = true;

  if (do_parametric) {

  std::vector<double> bins_hm;
  double x = 200.;
  while (x < 1501.) {
    bins_hm.push_back(x);
    x += 5.;
  }
  cb_hm.VariableRebin(bins_hm);
  cb_hm.PrintAll();

  RooWorkspace ws("htt", "htt");
  set<string> bins =
      cb_hm.GenerateSetFromObs<string>(mem_fn(&ch::Observation::bin));
  for (auto const& b : bins) {
    ch::CombineHarvester tmp = std::move(cb_hm.cp().bin({b}).backgrounds());
    TH1F tot_bkg = tmp.GetShape();

    TH1F sig = cb_hm.cp().bin({b}).mass({"900"}).signals().GetShape();

    tot_bkg.Scale(tmp.GetObservedRate()/tot_bkg.Integral());
    tot_bkg.Add(&sig, 0.0);
    for (int i = 1; i <= tot_bkg.GetNbinsX(); ++i) {
      tot_bkg.SetBinContent(i, std::floor(tot_bkg.GetBinContent(i) + 0.5));
    }
    double final_rate = tot_bkg.Integral();
    tot_bkg.Scale(1.0/tot_bkg.Integral());
    // tmp.ForEachObs([&](ch::Observation * obs) {
    //   obs->set_rate(final_rate);
    //   obs->set_shape(ic::make_unique<TH1F>(tot_bkg));
    // });
    tmp.process({"ZTT"});
    tmp.ForEachProc([&](ch::Process *proc) {
      proc->set_process("bkg");
      proc->set_rate(final_rate);
      proc->set_shape(nullptr);
    });

    cb_hm.process({"ggH", "bkg"});

    RooRealVar mtt("CMS_th1x", "CMS_th1x", 0,
                   static_cast<float>(tot_bkg.GetNbinsX()));
    RooRealVar lA((b + "_a").c_str(), "", 50, 0.01, 1000);
    RooRealVar lB((b + "_b").c_str(), "", 50, -10500,
                  10500);
    RooRealVar lC((b + "_c").c_str(), "", 50, 0,
                  10500);
    // std::string fn = "exp(-(m-"+iStr+")/(a+0.001*b*(m-"+iStr+")))";
    std::string fn = "exp(-CMS_th1x/("+b+"_a+"+b+"_b*0.001*CMS_th1x))";
    std::cout << "fn = " << fn << std::endl;
    RooGenericPdf bkg_pdf((b + "_bkgpdf").c_str(), fn.c_str(),
                          RooArgList(mtt, lA, lB));
    RooRealVar bkg_norm((b + "_bkgpdf_norm").c_str(), "", 1., 0., 10000.);
    ws.import(bkg_pdf);
    ws.import(bkg_norm);
  }

  cb_hm.AddWorkspace(&ws);
  cb_hm.cp().backgrounds().ExtractPdfs("htt", "$CHANNEL_bkgpdf");
  cb_hm.PrintAll();
  } else {
    cb_hm.VariableRebin(
        {200, 225, 250, 275, 300, 325, 350, 400, 500, 700, 1000, 1500});
    std::cout << "Merging bin errors...\n";
    cb_hm.cp().process({"W", "QCD", "ZTT", "ZL", "ZJ", "TT", "VV"})
        .MergeBinErrors(0.05, 0.4);
    cb_hm.cp().process({"W", "QCD", "ZTT", "ZL", "ZJ", "TT", "VV"})
        .AddBinByBin(0.05, true, &cb_hm);
    std::cout << "...done\n";
  }

  // // cb.bin({"htt_mt_5_8TeV", "htt_mt_6_8TeV"});
  // // cb.nus_type({"shape"}, false);
  // RooWorkspace ws("htt", "htt");

  // bool do_morphing = true;
  // if (do_morphing) {
  //   set<string> bins =
  //       cb.GenerateSetFromObs<string>(mem_fn(&ch::Observation::bin));
  //   ch::CombineHarvester cb_sig = std::move(cb.cp().signals());
  //   RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
  //   ch::BuildRooMorphing(ws, cb_sig, "MH", true);
  //   cb.FilterNus([&](ch::Nuisance const* p) {
  //     return p->type() == "shape" && p->signal();
  //   });
  //   cb.mass({"125", "*"});
  //   cb.cp().signals().ForEachProc([&](ch::Process* p) {
  //     p->set_shape(nullptr);
  //     p->set_rate(1.0);
  //   });

  //   cb.AddWorkspace(&ws);
  //   cb.cp().signals().ExtractPdfs("htt", "$CHANNEL_$PROCESS_mpdf");
  // }
  // cb.cp().nus_type({"shape"}).PrintAll();


  // cb.AddWorkspace(&ws);
  // ws.Print();
  // cb.cp().signals().ExtractPdfs("htt", "$CHANNEL_$PROCESS_pdf");

  // cb.cp().mass({"125", "*"}).PrintAll();
  // std::cout << "Merging bin errors...\n";
  // cb.cp().bin_id({0, 1, 2, 3, 4}).process({"W", "QCD"})
  //     .MergeBinErrors(0.1, 0.4);
  // cb.cp().bin_id({5}).era({"7TeV"}).process({"W"})
  //     .MergeBinErrors(0.1, 0.4);
  // cb.cp().bin_id({5, 6}).era({"8TeV"}).process({"W"})
  //     .MergeBinErrors(0.1, 0.4);
  // cb.cp().bin_id({7}).era({"8TeV"}).process({"W", "ZTT"})
  //     .MergeBinErrors(0.1, 0.4);
  // std::cout << "...done\n";

  // std::cout << "Generating bbb uncertainties...\n";
  // cb.cp().bin_id({0, 1, 2, 3, 4}).process({"W", "QCD"})
  //     .AddBinByBin(0.1, true, &cb);
  // cb.cp().bin_id({5}).era({"7TeV"}).process({"W"})
  //     .AddBinByBin(0.1, true, &cb);
  // cb.cp().bin_id({5, 6}).era({"8TeV"}).process({"W"})
  //     .AddBinByBin(0.1, true, &cb);
  // cb.cp().bin_id({7}).era({"8TeV"}).process({"W", "ZTT"})
  //     .AddBinByBin(0.1, true, &cb);
  // std::cout << "...done\n";

  TFile output("htt_mt.mssm.root", "RECREATE");
  cb_hm.cp().mass({"900", "*"}).WriteDatacard("htt_mssm_hm.txt", output);
  // for (auto b : bins) {
  //   for (auto m : masses /*{"125"}*/) {
  //     std::cout << "Writing datacard for bin: " << b << " and mass: " << m
  //               << "\n";
  //     cb.cp().bin({b}).mass({m, "*"}).WriteDatacard(
  //         b + "_" + m + ".txt", output);
  //   }
  // }

}

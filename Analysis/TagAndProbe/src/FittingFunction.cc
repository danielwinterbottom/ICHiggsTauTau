#include <iostream>
#include <fstream>
#include <TSystem.h>
#include "TFile.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLatex.h"

#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooVoigtian.h"
#include "RooCBShape.h"
#include "RooBifurGauss.h"
#include "RooAddPdf.h"
#include "RooExponential.h"
#include "RooSimultaneous.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooFFTConvPdf.h"
#include "RooBreitWigner.h"
#include "boost/format.hpp"
#include "RooMsgService.h"

#include "UserCode/ICHiggsTauTau/Analysis/TagAndProbe/interface/FittingFunction.h"

using namespace RooFit ;

    int fit(std::string filename, std::string type, bool elec, bool isdata){

        RooRealVar* rooMass_ = new RooRealVar("Mass","Mass of lepton pair",60.0, 120.0,
            "GeV/c^{2}");

        RooRealVar Mass = *rooMass_;
        RooCategory sample("sample","");
        sample.defineType("Pass", 1);
        sample.defineType("Fail", 2); 

        std::string TP,TF;

        if(type =="id" || type=="idbins")
        {
            TP="id_h_TP_";
            TF="id_h_TF_";
        }
        if(type =="iso" || type=="isobins")
        {
            TP="iso_h_TP_";
            TF="iso_h_TF_";
        }
        if(type =="idiso" || type == "idisobins")
        {
            TP="idiso_h_TP_";
            TF="idiso_h_TF_";
        }
        if(type =="trg")
        {
            TP="h_TP_";
            TF="h_TF_";
        }

        TFile* f;

        if(elec && (type=="id" || type=="iso" || type=="idiso"|| type=="idbinned" || type=="isobinned" || type=="idisobinned") && isdata)
        {
            f=new TFile("eeTandPIdIsodata.root","r");
        }
        if(elec && (type=="id" || type=="iso" || type=="idiso"|| type=="idbinned" || type=="isobinned" || type=="idisobinned") && !isdata)
        {
            f=new TFile("eeTandPIdIsoMC.root","r");
        }
        if(elec && (type=="trg") && isdata)
        {
            f=new TFile("eeTandPtrgdata.root","r");
        }
        if(elec && (type=="trg") && !isdata)
        {
            f=new TFile("eeTandPtrgMC.root","r");
        }
        if(!elec && (type=="id" || type=="iso" || type=="idiso"|| type=="idbinned" || type=="isobinned" || type=="idisobinned") && isdata)
        {
            f=new TFile("mumuTandPIdIsodata.root","r");
        }
        if(!elec && (type=="id" || type=="iso" || type=="idiso"|| type=="idbinned" || type=="isobinned" || type=="idisobinned") && !isdata)
        {
            f=new TFile("mumuTandPIdIsoMC.root","r");
        }
        if(!elec && (type=="trg") && isdata)
        {
            f=new TFile("mumuTandPtrgdata.root","r");
        }
        if(!elec && (type=="trg") && !isdata)
        {
            f=new TFile("mumuTandPtrgMC.root","r");
        }


        TFile* f2 = new TFile("FitHistos.root", "RECREATE");
        f2->cd();

        ofstream myfile, myfile2;
        if(elec && isdata)
        {
            myfile2.open("electron_eff_data.txt", ios::out | ios::app);
        }
        if(!elec && isdata)
        {
            myfile2.open("muon_eff_data.txt", ios::out | ios::app);
        }
        if(elec && !isdata)
        {
            myfile2.open("electron_eff_MC.txt", ios::out | ios::app);
        }
        if(!elec && !isdata)
        {
            myfile2.open("muon_eff_MC.txt", ios::out | ios::app);
        }
        //Read in the pass and fail histograms, output of eeTagandProbe.C.
        TH1F *hist_pass = (TH1F*)f->Get((TP+filename).c_str());
        TH1F *hist_fail = (TH1F*)f->Get((TF+filename).c_str());

        
        //Specify the two sets of data in the form of DataHists.
        RooDataHist* data_pass = new RooDataHist("data_pass","data_pass",
        RooArgList(Mass), hist_pass);
        RooDataHist* data_fail = new RooDataHist("data_fail","data_fail",
        RooArgList(Mass), hist_fail);
        //Create a combined dataset of the two datasets so that they can be fit
        //simultanously at the end.
        RooDataHist* data_comb = new RooDataHist("fitData","fitData",
        RooArgList(Mass),RooFit::Index(sample),RooFit::Import("Pass",*data_pass),
        RooFit::Import("Fail",*data_fail) );


        //Set up the fit parameters for background and signal
        RooRealVar* MeanFail_= new RooRealVar("MeanFail","MeanFail",
        91.1, 88.8, 93.2);
        //90.0, 86.2, 95.2);

        RooRealVar* WidthFail_= new RooRealVar("WidthFail","WidthFail", 2, 1, 10);

        RooRealVar* ResolutionFail_= new RooRealVar("ResolutionFail",
        "ResolutionFail", 2.0, 0.1, 5.);

        RooRealVar* MeanPass_= new RooRealVar("MeanPass","MeanPass",
        91.1, 88.8, 93.2);
        //90.0, 86.2, 95.2);
        RooRealVar* WidthPass_= new RooRealVar("WidthPass","WidthPass", 2.5, 1, 10);
        RooRealVar* ResolutionPass_= new RooRealVar("ResolutionPass",
        "ResolutionPass", 1.5, 0.1, 5.);


        RooRealVar* WidthLPass_  = new RooRealVar("WidthLPass","WidthLPass",
        3., 1., 10.);
        RooRealVar* WidthRPass_  = new RooRealVar("WidthRPass","WidthRPass",
        3., 1., 10.);
//        RooRealVar* BifurGaussFracPass_ = new RooRealVar("BifurGaussFracPass",
//        "",0.2, 0.0, 1.0);


//        RooRealVar* WidthLFail_  = new RooRealVar("WidthLFail","WidthLFail",
//        3., 1., 10.);
//        RooRealVar* WidthRFail_  = new RooRealVar("WidthRFail","WidthRFail",
//        3., 1., 10.);
//        RooRealVar* BifurGaussFracFail_ = new RooRealVar("BifurGaussFracFail",
//        "", 0.2, 0.0, 1.0);


        //Generate PDFs based on these parameters for fitting. There are many 
        //possibilities and combinations that could be used-see Andrew's 
        //original code for some ideas.

        //voigtian=breit wigner convoluted with gaussian for detector resolution.

        RooAbsPdf* voigtianPassPdf= new RooVoigtian("voigtianPassPdf", "", Mass,
        *MeanPass_, *WidthPass_, *ResolutionPass_);
        RooAbsPdf* voigtianFailPdf= new RooVoigtian("voigtianFailPdf", "", Mass,
        *MeanPass_, *WidthFail_, *ResolutionFail_);


/*
        //bifur=bifurcated gaussian

        RooAbsPdf* bifurGaussPassPdf_ = new RooBifurGauss("bifurGaussPassPdf", 
        "", Mass, *MeanPass_, *WidthLPass_, *WidthRPass_);
        RooAbsPdf* bifurGaussFailPdf_ = new RooBifurGauss("bifurGaussFailPdf",
        "", Mass, *MeanFail_, *WidthLFail_, *WidthRFail_);

*/

        //--------------------------------------------------------------------------
        // Variables for the fraction when using two summed PDFs
        //--------------------------------------------------------------------------
  //       RooRealVar* FracPass_ = new RooRealVar("FracPass","",0.75, 0.5, 1.0);
  //       RooRealVar* FracFail_ = new RooRealVar("FracFail","",0.75, 0.5, 1.0);

        RooAbsPdf* signalShapePassPdf= voigtianPassPdf;
        RooAbsPdf* signalShapeFailPdf= voigtianFailPdf;
/*
        RooAbsPdf* signalShapePassPdf= new RooAddPdf("signalShapePassPdf", "", 
        *voigtianPassPdf, *bifurGaussPassPdf_, *FracPass_);
        RooAbsPdf* signalShapeFailPdf= new RooAddPdf("signalShapeFailPdf", "", 
        *voigtianFailPdf, *bifurGaussFailPdf_, *FracFail_);
*/

        // Background PDFs are exponentials (so why all the stuff above?) 
        RooRealVar* bkgShapePass = new RooRealVar("bkgShapePass","bkgShapePass",
        -0.0001,-10.,0.);
        RooRealVar* bkgShapeFail = new RooRealVar("bkgShapeFail","bkgShapeFail",
        -0.0001,-10.,0.);

        RooExponential* bkgShapePassPdf = new RooExponential("bkgShapePassPdf",
        "bkgShapePassPdf",Mass, *bkgShapePass);
        RooExponential* bkgShapeFailPdf = new RooExponential("bkgShapeFailPdf",
        "bkgShapeFailPdf",Mass, *bkgShapeFail);

        RooRealVar* nSigPass = new RooRealVar("nSigPass", "nSigPass",
        1000,0.0,5000000.0);
        RooRealVar* nSigFail = new RooRealVar("nSigFail", "nSigFail",
        1000,0.0,5000000.0);
        RooRealVar* nBkgPass = new RooRealVar("nBkgPass","nBkgPass",
        1000.0, 0.0, 10000000.0);
        RooRealVar* nBkgFail = new RooRealVar("nBkgFail","nBkgFail",
        1000.0, 0.0, 10000000.0);
        RooFormulaVar* eff = new RooFormulaVar("eff","nSigPass/(nSigPass+nSigFail)",RooArgList(*nSigPass,*nSigFail));

        RooArgList componentsPass(*signalShapePassPdf,*bkgShapePassPdf);
        RooArgList yieldsPass(*nSigPass, *nBkgPass);
        RooArgList componentsFail(*signalShapeFailPdf,*bkgShapeFailPdf);
        RooArgList yieldsFail(*nSigFail, *nBkgFail);

        //Add the signal and background components together weighted by the numbers
        //of signal and background.
        RooAddPdf pdfPass("pdfPass","extended sum pdf", componentsPass, yieldsPass);
        RooAddPdf pdfFail("pdfFail","extended sum pdf", componentsFail, yieldsFail);

        // The total simultaneous fit ...
        RooSimultaneous totalPdf("totalPdf","totalPdf", sample);
        totalPdf.addPdf(pdfPass,"Pass");
        totalPdf.Print();
        totalPdf.addPdf(pdfFail,"Fail");
        totalPdf.Print();

        RooFitResult *fitResult = totalPdf.fitTo(*data_comb, RooFit::Save(true), 
        RooFit::Extended(true), RooFit::PrintLevel(-1),SumW2Error(kFALSE));
        if (fitResult->covQual()!=3) cerr << "WARNING -- inaccurate errors" << endl;
        fitResult->Print("v");

        std::cout << "nSigPass: " << nSigPass->getVal() << std::endl;
        std::cout << "nBkgPass: " << nBkgPass->getVal() << std::endl;
        std::cout << "Total: " << nSigPass->getVal()+ nBkgPass->getVal()
        << std::endl;
        std::cout << "nSigFail: " << nSigFail->getVal() << std::endl;
        std::cout << "nBkgFail: " << nBkgFail->getVal() << std::endl;
        std::cout << "Total: " << nSigFail->getVal()+ nBkgFail->getVal()
        << std::endl;
        std::cout << "Eff: " << eff->getVal()<< "+/-" << 
        eff->getPropagatedError(*fitResult) << std::endl;
        RooAbsData::ErrorType errorType = RooAbsData::Poisson;

        //TCanvas* c1 = new TCanvas("c1","c1",600,600);
        RooPlot* frame1 = Mass.frame();
        frame1->SetMinimum(0);
        data_pass->plotOn(frame1,RooFit::DataError(errorType),SumW2Error(kTRUE));
        pdfPass.plotOn(frame1,RooFit::ProjWData(*data_pass), 
        RooFit::Components(*bkgShapePassPdf),RooFit::LineColor(kRed), SumW2Error(kTRUE));
        pdfPass.plotOn(frame1,RooFit::ProjWData(*data_pass), SumW2Error(kTRUE));
        TCanvas* c1 = new TCanvas("c1","c1",600,600);
        frame1->Draw("e0");

        std::cout << frame1->chiSquare() << std::endl;
        std::cout << TMath::Prob(frame1->chiSquare(), 6) << std::endl;

        // Add some text with the results
        TLatex *title_latex = new TLatex();
        title_latex->SetNDC();
        title_latex->SetTextSize(0.03);
        boost::format f_nSigPass("SigPass: %.1f #pm %.1f");
        f_nSigPass % nSigPass->getVal() % nSigPass->getError();
        boost::format f_nBkgPass("BkgPass: %.1f #pm %.1f");
        f_nBkgPass % nBkgPass->getVal() % nBkgPass->getError();
        boost::format f_eff("Eff: %.3f #pm %.3f");
        f_eff % eff->getVal() %eff->getPropagatedError(*fitResult);
        title_latex->DrawLatex(0.62,0.89,f_nSigPass.str().c_str());
        title_latex->DrawLatex(0.62,0.85,f_nBkgPass.str().c_str());
        title_latex->DrawLatex(0.62,0.81,f_eff.str().c_str());

        c1->Update();
    //    c1->SaveAs("ee_TP_fit.pdf");

        TCanvas* c2 = new TCanvas("c2","c2",600,600);
        RooPlot* frame2 = Mass.frame();
        frame2->SetMinimum(0);
        data_fail->plotOn(frame2,RooFit::DataError(errorType));
        pdfFail.plotOn(frame2,RooFit::ProjWData(*data_fail), 
        RooFit::Components(*bkgShapeFailPdf),RooFit::LineColor(kRed));
        pdfFail.plotOn(frame2,RooFit::ProjWData(*data_fail));
        frame2->Draw("e0");
        std::cout << frame2->chiSquare() << std::endl;
        std::cout << TMath::Prob(frame2->chiSquare(), 6) << std::endl;


        boost::format f_nSigFail("SigFail: %.1f #pm %.1f");
        f_nSigFail % nSigFail->getVal() % nSigFail->getError();
        boost::format f_nBkgFail("BkgFail: %.1f #pm %.1f");
        f_nBkgFail % nBkgFail->getVal() % nBkgFail->getError();
        title_latex->DrawLatex(0.62,0.89,f_nSigFail.str().c_str());
        title_latex->DrawLatex(0.62,0.85,f_nBkgFail.str().c_str());
        title_latex->DrawLatex(0.62,0.81,f_eff.str().c_str());

        c2->Update();
    //    c2->SaveAs("ee_TF_fit.pdf");
        double results[2];
        results[0]=eff->getVal();
        results[1]=eff->getPropagatedError(*fitResult);
        if(TMath::Prob(frame1->chiSquare(),6)>0.005 && TMath::Prob(frame2->chiSquare(),6)>0.005)
        { 
      //      myfile << type << " " << filename << " " << results[0] << " " << results[1] << std::endl;
            myfile2 << type << " " << filename << " " << results[0] << " " << results[1] << std::endl;
        }
        else
        { 
            std::cout << "============POOR FIT: " << filename << " ============"<< std::endl;
        //    myfile << "============POOR FIT: " << filename << " ============"<< std::endl;
            myfile2 << "============POOR FIT========: " << type << " " << filename << " " << results[0] << " " << results[1] << std::endl;
        }
        myfile2.close();


        f2->Write();
        f2->Close();
        delete f2;

        return 0;
    }


    int fitone(std::string filename, std::string type, bool elec, bool isdata, double low, double high, bool isbifurc, bool samemean){

        RooRealVar* rooMass_ = new RooRealVar("Mass","Mass of lepton pair",60.0, 120.0,
            "GeV/c^{2}");

        RooRealVar Mass = *rooMass_;
        RooCategory sample("sample","");
        sample.defineType("Pass", 1);
        sample.defineType("Fail", 2); 

        std::string TP,TF;

        if(type =="id" || type=="idbins")
        {
            TP="id_h_TP_";
            TF="id_h_TF_";
        }
        if(type =="iso" || type=="isobins")
        {
            TP="iso_h_TP_";
            TF="iso_h_TF_";
        }
        if(type =="idiso" || type=="idisobins")
        {
            TP="idiso_h_TP_";
            TF="idiso_h_TF_";
        }
        if(type =="trg")
        {
            TP="h_TP_";
            TF="h_TF_";
        }

        TFile* f;

        if(elec && (type=="id" || type=="iso" || type=="idiso" || type=="idbinned" || type=="isobinned" || type=="idisobinned") && isdata)
        {
            f=new TFile("eeTandPIdIsodata.root","r");
        }
        if(elec && (type=="id" || type=="iso" || type=="idiso"|| type=="idbinned" || type=="isobinned" || type=="idisobinned") && !isdata)
        {
            f=new TFile("eeTandPIdIsoMC.root","r");
        }
        if(elec && (type=="trg") && isdata)
        {
            f=new TFile("eeTandPtrgdata.root","r");
        }
        if(elec && (type=="trg") && !isdata)
        {
            f=new TFile("eeTandPtrgMC.root","r");
        }
        if(!elec && (type=="id" || type=="iso" || type=="idiso"|| type=="idbinned" || type=="isobinned" || type=="idisobinned") && isdata)
        {
            f=new TFile("mumuTandPIdIsodata.root","r");
        }
        if(!elec && (type=="id" || type=="iso" || type=="idiso"|| type=="idbinned" || type=="isobinned" || type=="idisobinned") && !isdata)
        {
            f=new TFile("mumuTandPIdIsoMC.root","r");
        }
        if(!elec && (type=="trg") && isdata)
        {
            f=new TFile("mumuTandPtrgdata.root","r");
        }
        if(!elec && (type=="trg") && !isdata)
        {
            f=new TFile("mumuTandPtrgMC.root","r");
        }

        TFile* f2 = new TFile("FitHistos.root", "RECREATE");
        f2->cd();

        ofstream  myfile2;

        if(elec && isdata)
        {
            myfile2.open("electron_eff_data_temp.txt", ios::out | ios::app);
        }
        if(!elec && isdata)
        {
            myfile2.open("muon_eff_data_temp.txt", ios::out | ios::app);
        }
        if(elec && !isdata)
        {
            myfile2.open("electron_eff_MC_temp.txt", ios::out | ios::app);
        }
        if(!elec && !isdata)
        {
            myfile2.open("muon_eff_MC_temp.txt", ios::out | ios::app);
        }
        //Read in the pass and fail histograms, output of eeTagandProbe.C.
        TH1 *hist_pass = (TH1*)f->Get((TP+filename).c_str());
        TH1 *hist_fail = (TH1*)f->Get((TF+filename).c_str());
        
        //hist_pass=hist_pass->Rebin();
        //hist_fail=hist_fail->Rebin();
        
        //Specify the two sets of data in the form of DataHists.
        RooDataHist* data_pass = new RooDataHist("data_pass","data_pass",
        RooArgList(Mass), hist_pass);
        RooDataHist* data_fail = new RooDataHist("data_fail","data_fail",
        RooArgList(Mass), hist_fail);
        //Create a combined dataset of the two datasets so that they can be fit
        //simultanously at the end.
        RooDataHist* data_comb = new RooDataHist("fitData","fitData",
        RooArgList(Mass),RooFit::Index(sample),RooFit::Import("Pass",*data_pass),
        RooFit::Import("Fail",*data_fail) );


        //Set up the fit parameters for background and signal
        RooRealVar* MeanFail_= new RooRealVar("MeanFail","MeanFail",
        91.1, low, high);
        //90.0, 86.2, 95.2);
        RooRealVar* WidthFail_= new RooRealVar("WidthFail","WidthFail", 2, 1, 10);
        RooRealVar* ResolutionFail_= new RooRealVar("ResolutionFail",
        "ResolutionFail", 2, 0., 5.);

        RooRealVar* MeanPass_= new RooRealVar("MeanPass","MeanPass",
        91.1, low, high);
        //90.0, 86.2, 95.2);
        RooRealVar* WidthPass_= new RooRealVar("WidthPass","WidthPass", 2.5, 1, 10);
        RooRealVar* ResolutionPass_= new RooRealVar("ResolutionPass",
        "ResolutionPass", 1.5, 0., 5.);


        RooRealVar* WidthLPass_  = new RooRealVar("WidthLPass","WidthLPass",
        3., 1., 10.);
        RooRealVar* WidthRPass_  = new RooRealVar("WidthRPass","WidthRPass",
        3., 1., 10.);


        RooRealVar* WidthLFail_  = new RooRealVar("WidthLFail","WidthLFail",
        3., 1., 10.);
        RooRealVar* WidthRFail_  = new RooRealVar("WidthRFail","WidthRFail",
        3., 1., 10.);

        RooAbsPdf* voigtianPassPdf;
        RooAbsPdf* voigtianFailPdf;
        RooAbsPdf* bifurGaussPassPdf_;
        RooAbsPdf* bifurGaussFailPdf_;

        if(!samemean)
        {
            voigtianPassPdf= new RooVoigtian("voigtianPassPdf", "", Mass,
            *MeanPass_, *WidthPass_, *ResolutionPass_);
            voigtianFailPdf= new RooVoigtian("voigtianFailPdf", "", Mass,
            *MeanFail_, *WidthFail_, *ResolutionFail_);

            bifurGaussPassPdf_ = new RooBifurGauss("bifurGaussPassPdf", 
            "", Mass, *MeanPass_, *WidthLPass_, *WidthRPass_);
            bifurGaussFailPdf_ = new RooBifurGauss("bifurGaussFailPdf",
            "", Mass, *MeanFail_, *WidthLFail_, *WidthRFail_);
        }
        else
        {
            voigtianPassPdf= new RooVoigtian("voigtianPassPdf", "", Mass,
            *MeanPass_, *WidthPass_, *ResolutionPass_);
            voigtianFailPdf= new RooVoigtian("voigtianFailPdf", "", Mass,
            *MeanPass_, *WidthFail_, *ResolutionFail_);


            bifurGaussPassPdf_ = new RooBifurGauss("bifurGaussPassPdf", 
            "", Mass, *MeanPass_, *WidthLPass_, *WidthRPass_);
            bifurGaussFailPdf_ = new RooBifurGauss("bifurGaussFailPdf",
            "", Mass, *MeanPass_, *WidthLFail_, *WidthRFail_);
        }


        RooRealVar* FracPass_ = new RooRealVar("FracPass","",0.75, 0.5, 1.0);
        RooRealVar* FracFail_ = new RooRealVar("FracFail","",0.75, 0.5, 1.0);
        RooAbsPdf* signalShapePassPdf;
        RooAbsPdf* signalShapeFailPdf;
        if(!isbifurc)
        {
            signalShapePassPdf= voigtianPassPdf;
            signalShapeFailPdf= voigtianFailPdf;
        }
        else
        {
            signalShapePassPdf= new RooAddPdf("signalShapePassPdf", "", 
            *voigtianPassPdf, *bifurGaussPassPdf_, *FracPass_);
            signalShapeFailPdf= new RooAddPdf("signalShapeFailPdf", "", 
            *voigtianFailPdf, *bifurGaussFailPdf_, *FracFail_);
        }

 
        RooRealVar* bkgShapePass = new RooRealVar("bkgShapePass","bkgShapePass",
        -0.0001,-10.,0.);
        RooRealVar* bkgShapeFail = new RooRealVar("bkgShapeFail","bkgShapeFail",
        -0.0001,-10.,0.);

        RooExponential* bkgShapePassPdf = new RooExponential("bkgShapePassPdf",
        "bkgShapePassPdf",Mass, *bkgShapePass);
        RooExponential* bkgShapeFailPdf = new RooExponential("bkgShapeFailPdf",
        "bkgShapeFailPdf",Mass, *bkgShapeFail);

        RooRealVar* nSigPass = new RooRealVar("nSigPass", "nSigPass",
        1000,0.0,5000000.0);
        RooRealVar* nSigFail = new RooRealVar("nSigFail", "nSigFail",
        1000,0.0,5000000.0);
        RooRealVar* nBkgPass = new RooRealVar("nBkgPass","nBkgPass",
        1000.0, 0.0, 10000000.0);
        RooRealVar* nBkgFail = new RooRealVar("nBkgFail","nBkgFail",
        1000.0, 0.0, 10000000.0);
        RooFormulaVar* eff = new RooFormulaVar("eff","nSigPass/(nSigPass+nSigFail)",RooArgList(*nSigPass,*nSigFail));

        RooArgList componentsPass(*signalShapePassPdf,*bkgShapePassPdf);
        RooArgList yieldsPass(*nSigPass, *nBkgPass);
        RooArgList componentsFail(*signalShapeFailPdf,*bkgShapeFailPdf);
        RooArgList yieldsFail(*nSigFail, *nBkgFail);

        //Add the signal and background components together weighted by the numbers
        //of signal and background.
        RooAddPdf pdfPass("pdfPass","extended sum pdf", componentsPass, yieldsPass);
        RooAddPdf pdfFail("pdfFail","extended sum pdf", componentsFail, yieldsFail);

        // The total simultaneous fit ...
        RooSimultaneous totalPdf("totalPdf","totalPdf", sample);
        totalPdf.addPdf(pdfPass,"Pass");
        totalPdf.Print();
        totalPdf.addPdf(pdfFail,"Fail");
        totalPdf.Print();

        RooFitResult *fitResult;
        fitResult = totalPdf.fitTo(*data_comb, RooFit::Save(true), 
        RooFit::Extended(true), RooFit::PrintLevel(-1),SumW2Error(kFALSE));
        if (fitResult->covQual()!=3) cerr << "WARNING -- inaccurate errors" << endl;
        fitResult->Print("v");

        std::cout << "nSigPass: " << nSigPass->getVal() << std::endl;
        std::cout << "nBkgPass: " << nBkgPass->getVal() << std::endl;
        std::cout << "Total: " << nSigPass->getVal()+ nBkgPass->getVal()
        << std::endl;
        std::cout << "nSigFail: " << nSigFail->getVal() << std::endl;
        std::cout << "nBkgFail: " << nBkgFail->getVal() << std::endl;
        std::cout << "Total: " << nSigFail->getVal()+ nBkgFail->getVal()
        << std::endl;
        std::cout << "Eff: " << eff->getVal()<< "+/-" << 
        eff->getPropagatedError(*fitResult) << std::endl;
        RooAbsData::ErrorType errorType = RooAbsData::Poisson;


        RooPlot* frame1 = Mass.frame();
        frame1->SetMinimum(0);
        data_pass->plotOn(frame1,RooFit::DataError(errorType),SumW2Error(kTRUE));
        pdfPass.plotOn(frame1,RooFit::ProjWData(*data_pass), 
        RooFit::Components(*bkgShapePassPdf),RooFit::LineColor(kRed), SumW2Error(kTRUE));
        pdfPass.plotOn(frame1,RooFit::ProjWData(*data_pass), SumW2Error(kTRUE));
        TCanvas* c1 = new TCanvas("c1","c1",600,600);
        frame1->Draw("e0");

        std::cout << frame1->chiSquare() << std::endl;
        std::cout << frame1->chiSquare(6) << std::endl;
        std::cout << TMath::Prob(frame1->chiSquare(), 6) << std::endl;

        // Add some text with the results
        TLatex *title_latex = new TLatex();
        title_latex->SetNDC();
        title_latex->SetTextSize(0.03);
        boost::format f_nSigPass("SigPass: %.1f #pm %.1f");
        f_nSigPass % nSigPass->getVal() % nSigPass->getError();
        boost::format f_nBkgPass("BkgPass: %.1f #pm %.1f");
        f_nBkgPass % nBkgPass->getVal() % nBkgPass->getError();
        boost::format f_eff("Eff: %.3f #pm %.3f");
        f_eff % eff->getVal() %eff->getPropagatedError(*fitResult);
        title_latex->DrawLatex(0.62,0.89,f_nSigPass.str().c_str());
        title_latex->DrawLatex(0.62,0.85,f_nBkgPass.str().c_str());
        title_latex->DrawLatex(0.62,0.81,f_eff.str().c_str());

        c1->Update();
    //    c1->SaveAs("ee_TP_fit.pdf");

        TCanvas* c2 = new TCanvas("c2","c2",600,600);
        RooPlot* frame2 = Mass.frame();
        frame2->SetMinimum(0);
        data_fail->plotOn(frame2,RooFit::DataError(errorType));
        pdfFail.plotOn(frame2,RooFit::ProjWData(*data_fail), 
        RooFit::Components(*bkgShapeFailPdf),RooFit::LineColor(kRed));
        pdfFail.plotOn(frame2,RooFit::ProjWData(*data_fail));
        frame2->Draw("e0");
        std::cout << frame2->chiSquare() << std::endl;
        std::cout << frame2->chiSquare(6) << std::endl;
        std::cout << TMath::Prob(frame2->chiSquare(), 6) << std::endl;

        boost::format f_nSigFail("SigFail: %.1f #pm %.1f");
        f_nSigFail % nSigFail->getVal() % nSigFail->getError();
        boost::format f_nBkgFail("BkgFail: %.1f #pm %.1f");
        f_nBkgFail % nBkgFail->getVal() % nBkgFail->getError();
        title_latex->DrawLatex(0.62,0.89,f_nSigFail.str().c_str());
        title_latex->DrawLatex(0.62,0.85,f_nBkgFail.str().c_str());
        title_latex->DrawLatex(0.62,0.81,f_eff.str().c_str());

        c2->Update();
    //    c2->SaveAs("ee_TF_fit.pdf");
        double results[2];
        results[0]=eff->getVal();
        results[1]=eff->getPropagatedError(*fitResult);
        
        myfile2 << type << " " << filename << " " << results[0] << " " << results[1] << std::endl;      
        myfile2.close();

        f2->Write();
        f2->Close();
        delete f2;

        return 0;
    }





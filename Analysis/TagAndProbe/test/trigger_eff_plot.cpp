#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <TH1.h>
#include <TLatex.h>
#include <TGraphErrors.h>
#include <TFile.h>
#include <TCanvas.h>
#include "boost/lexical_cast.hpp"

using namespace std;

int main(int argc, char* argv[]){

    std::cout << "Arguments passed to program: " << argc << std::endl;
    for (int i = 0; i < argc; ++i){
        std::cout << i << "\t" << argv[i] << std::endl;
    }
    if (argc != 6){
        std::cerr << "Need <iselec> <isdata> <plus/minus/abs> <xmin> <xmax>" << std::endl;
        exit(1);
    }

    bool elec, isdata;
    elec=boost::lexical_cast<bool>(argv[1]);
    isdata=boost::lexical_cast<bool>(argv[2]);
    std::string etatype=argv[3];
    int xmin=boost::lexical_cast<int>(argv[4]);
    int xmax=boost::lexical_cast<int>(argv[5]);

    TFile *f1 = new TFile("TriggerBinnedHistos.root", "RECREATE");
    f1->cd();


    ifstream bins, efficiencies, vtxbins;

    if(elec)
    {
        bins.open("electron_triggerptbins.txt");
        vtxbins.open("electron_vtxbinsfine.txt");
        if(isdata)
        {
            efficiencies.open("electron_eff_data.txt");
        }
        else
        {
            efficiencies.open("electron_eff_MC.txt");
        }

    }
    else
    {
        bins.open("muon_triggerptbins.txt");
        vtxbins.open("muon_vtxbinsfine.txt");
        if(isdata)
        {
            efficiencies.open("muon_eff_data.txt");
        }
        else
        {
            efficiencies.open("muon_eff_MC.txt");
        }
    }

    vector<std::string> binning;
    vector<int> binbounds;

    while(!bins.eof())
    {
        std::string b;
        getline(bins,b);
        binning.push_back(b);
    }
    bins.close();
    
    for(unsigned i=0;i<(binning.size())-1;i++)
    {
        binbounds.push_back(boost::lexical_cast<int>(binning[i]));
    }

    //Data
    const Int_t n1=binbounds.size();
    //set bins with 'error bars' for width of bin
    Double_t x1[binbounds.size()];
    Double_t err_x1[binbounds.size()];
    //for(unsigned i=0; i<(binbounds.size())-1; i++)
    for(unsigned i=0; i<12; i++)
    {
        x1[i]=binbounds[i]+((binbounds[i+1]-binbounds[i])/2.0);
        err_x1[i]=(binbounds[i+1]-binbounds[i])/2.0;
    }
    //Efficiencies corresponding to above bins
    //barrel

    vector<std::string> vtxbinning;
    vector<int> vtxbinbounds;

    while(!vtxbins.eof())
    {
        std::string b;
        getline(vtxbins,b); 
        vtxbinning.push_back(b);
       
    }
    vtxbins.close();
    
    for(unsigned i=0;i<(vtxbinning.size()-1);i++)
    {
        vtxbinbounds.push_back(boost::lexical_cast<int>(vtxbinning[i]));
        
    }


    const Int_t n3=3;
    Double_t x3[vtxbinbounds.size()];
    Double_t err_x3[vtxbinbounds.size()];
    
    for(unsigned i=0; i<(vtxbinbounds.size()-1); i++)
    {
        x3[i]=vtxbinbounds[i]+((vtxbinbounds[i+1]-vtxbinbounds[i])/2.0);   
        err_x3[i]=(vtxbinbounds[i+1]-vtxbinbounds[i])/2.0;
    }

    vector<std::string> ideffs, isoeffs, idisoeffs;
    vector<std::string> effsB, effsE, effsvtx;
    while(!efficiencies.eof())
    {
        std::string b;
        getline(efficiencies,b);
       
        if(b.find("trg")!=std::string::npos)
        {
         //   std::cout << b << std::endl; 
            effsB.push_back(b);
        } 
        if(b.find("vtx")!=std::string::npos)
        {
         //   std::cout << b << std::endl; 
            effsvtx.push_back(b);
        } 
    }
    efficiencies.close();

    Double_t yB[effsB.size()];
    Double_t err_yB[effsB.size()];
    Double_t yE[effsB.size()];
    Double_t err_yE[effsB.size()];
    Double_t yEb[effsB.size()];
    Double_t err_yEb[effsB.size()];
    Double_t y3[effsvtx.size()];
    Double_t err_y3[effsvtx.size()];
   
    std::string bar, end1, end2;
    if(etatype=="abs")
    {
        bar="B";
        end1="E";
        end2="Eb";
    }
    if(etatype=="plus")
    {
        bar="Bplus";
        end1="Eplus";
        end2="Ebplus";
    }
    if(etatype=="minus")
    {
        bar="Bminus";
        end1="Eminus";
        end2="Ebminus";
    }


    for(unsigned i=0; i<effsB.size(); i++)
    {    
        stringstream strings(effsB[i]); 
        std::string part;
        std::vector<std::string> parts;
        while(getline(strings, part, ' '))
        {
            parts.push_back(part);
        }
        if(parts[1].find(bar)!=std::string::npos
            && (!(etatype=="abs") || (!(parts[1].find("Bplus")!=std::string::npos) && !(parts[1].find("Bminus")!=std::string::npos))))
        {
            for(int j=2; j<12; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    yB[j]=boost::lexical_cast<double>(parts[2]);
                    err_yB[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
            if(parts[1].find(boost::lexical_cast<std::string>(1))!=std::string::npos
                && !(parts[1].find(boost::lexical_cast<std::string>(10))!=std::string::npos)
                && !(parts[1].find(boost::lexical_cast<std::string>(11))!=std::string::npos)
                && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
            {
                yB[0]=boost::lexical_cast<double>(parts[2]);
                err_yB[0]=boost::lexical_cast<double>(parts[3]);
            }
            if(parts[1].find(boost::lexical_cast<std::string>(2))!=std::string::npos
                && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
            {
                yB[1]=boost::lexical_cast<double>(parts[2]);
                err_yB[1]=boost::lexical_cast<double>(parts[3]);
            }
            
        }
        if(parts[1].find(end1)!=std::string::npos && !(parts[1].find(end2)!=std::string::npos)
            && (!(etatype=="abs") || (!(parts[1].find("Eplus")!=std::string::npos) && !(parts[1].find("Eminus")!=std::string::npos)
            && !(parts[1].find("Ebminus")!=std::string::npos) && !(parts[1].find("Ebplus")!=std::string::npos))))
        {
            for(int j=2; j<12; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    yE[j]=boost::lexical_cast<double>(parts[2]);
                    err_yE[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
            if(parts[1].find(boost::lexical_cast<std::string>(1))!=std::string::npos
                && !(parts[1].find(boost::lexical_cast<std::string>(10))!=std::string::npos)
                && !(parts[1].find(boost::lexical_cast<std::string>(11))!=std::string::npos)
                && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
            {
                yE[0]=boost::lexical_cast<double>(parts[2]);
                err_yE[0]=boost::lexical_cast<double>(parts[3]); 
            }
            if(parts[1].find(boost::lexical_cast<std::string>(2))!=std::string::npos
                && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
            {
                yE[1]=boost::lexical_cast<double>(parts[2]);
                err_yE[1]=boost::lexical_cast<double>(parts[3]);
            }
            
        }
        if(parts[1].find(end2)!=std::string::npos
            && (!(etatype=="abs") || (!(parts[1].find("Ebplus")!=std::string::npos) && !(parts[1].find("Ebminus")!=std::string::npos))))
        {
            for(int j=2; j<12; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    yEb[j]=boost::lexical_cast<double>(parts[2]);
                    err_yEb[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
            if(parts[1].find(boost::lexical_cast<std::string>(1))!=std::string::npos
                && !(parts[1].find(boost::lexical_cast<std::string>(10))!=std::string::npos)
                && !(parts[1].find(boost::lexical_cast<std::string>(11))!=std::string::npos)
                && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
            {
                yEb[0]=boost::lexical_cast<double>(parts[2]);
                err_yEb[0]=boost::lexical_cast<double>(parts[3]); 
            }
            if(parts[1].find(boost::lexical_cast<std::string>(2))!=std::string::npos
                && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
            {
                yEb[1]=boost::lexical_cast<double>(parts[2]);
                err_yEb[1]=boost::lexical_cast<double>(parts[3]);
            }
            
        }
    }
    
    for(unsigned i=0; i<effsvtx.size(); i++)
    { 
        stringstream strings(effsvtx[i]); 
        std::string part;
        std::vector<std::string> parts;
        while(getline(strings, part, ' '))
        {
            parts.push_back(part);
        }
        if(parts[1].find("vtx")!=std::string::npos) 
        {
            for(int j=0; j<3; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    y3[j]=boost::lexical_cast<double>(parts[2]);
                    err_y3[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
        }
    }
   
    
    
    if(elec) std::cout << "Efficiencies for Barrel: " <<std::endl;
    if(!elec && etatype=="abs") std::cout << "Efficiencies for |eta| < 0.8: " <<std::endl;
    if(!elec && etatype=="plus") std::cout << "Efficiencies for 0 < eta < 0.8: " <<std::endl;
    if(!elec && etatype=="minus") std::cout << "Efficiencies for -0.8 < eta < 0: " <<std::endl;
    for(int i=0; i<12; i++)
    {
        std::cout << i << " " << yB[i] << " " << err_yB[i] << std::endl;
    }
    if(elec) std::cout << "Efficiencies for Endcaps: " <<std::endl;
    if(!elec && etatype=="abs") std::cout << "Efficiencies for 0.8 < |eta| < 1.2: " <<std::endl;
    if(!elec && etatype=="plus") std::cout << "Efficiencies for 0.8 < eta < 1.2: " <<std::endl;
    if(!elec && etatype=="minus") std::cout << "Efficiencies for -1.2 < eta < -0.8: " <<std::endl;
    for(int i=0; i<12; i++)
    {
        std::cout << i << " " << yE[i] << " " << err_yE[i] << std::endl;
    }
    if(!elec && etatype=="abs") std::cout << "Efficiencies for |eta| > 1.2: " <<std::endl;
    if(!elec && etatype=="plus") std::cout << "Efficiencies for eta > 1.2: " <<std::endl;
    if(!elec && etatype=="minus") std::cout << "Efficiencies for eta < -1.2: " <<std::endl;
    if(!elec)
    {
        for(int i=0; i<12; i++)
        {
            std::cout << i << " " << yEb[i] << " " << err_yEb[i] << std::endl;
        }
    }
    std::cout << "Efficiencies in vtx bins (if needed): " << std::endl;
    for(int i=0; i<3; i++)
    {
        std::cout << i << " " << y3[i] << " " << err_y3[i] << std::endl;
    }
    
    
    TCanvas *c1 = new TCanvas("c1", "error plot", 200, 10, 700, 500);
    
    TH1F *h1=c1->DrawFrame(xmin, -0.05, xmax, 1);
    h1->SetYTitle("Efficiency");
    if(elec)
    {
        h1->SetXTitle("Electron p_{T}");
    }
    else
    { 
        h1->SetXTitle("Muon p_{T}");
    }
    h1->GetXaxis()->SetTitleColor(1);
    
    TGraphErrors *gr1 = new TGraphErrors(n1,x1,yB,err_x1,err_yB);
    if(elec)
    {
        gr1->SetMarkerColor(kRed);
    }
    else gr1->SetMarkerColor(kBlue);
    TAxis *gr1_x= gr1->GetXaxis();
    gr1_x->SetLimits(xmin, xmax);
    gr1->SetMarkerStyle(20);
    gr1->Draw("P");


    TCanvas *c2 = new TCanvas("c2", "error plot", 200, 10, 700, 500);
    TH1F *h2=c2->DrawFrame(xmin, -0.05, xmax, 1);
    h2->SetYTitle("Efficiency");
    if(elec)
    {
        h2->SetXTitle("Electron p_{T}");
    }
    else h2->SetXTitle("Muon p_{T}");
    h2->GetXaxis()->SetTitleColor(1);

    TGraphErrors *gr2 = new TGraphErrors(n1,x1,yE,err_x1,err_yE);
    if(elec)
    {
        gr2->SetMarkerColor(kRed);
    }
    else gr2->SetMarkerColor(kBlue);
    TAxis *gr2_x= gr2->GetXaxis();
    gr2_x->SetLimits(xmin, xmax);
    gr2->SetMarkerStyle(20);
    gr2->Draw("P");

    TCanvas *c3 = new TCanvas("c3", "error plot", 200, 10, 700, 500);
    TH1F *h3=c3->DrawFrame(xmin, -0.05, xmax, 1);
    h3->SetYTitle("Efficiency");
    if(elec)
    {
        h3->SetXTitle("Electron p_{T}");
    }
    else h3->SetXTitle("Muon p_{T}");
    h3->GetXaxis()->SetTitleColor(1);
    TGraphErrors *gr3 = new TGraphErrors(n1,x1,yEb,err_x1,err_yEb);
    if(elec)
    {
        gr3->SetMarkerColor(kRed);
    }
    else gr3->SetMarkerColor(kBlue);
    TAxis *gr3_x= gr3->GetXaxis();
    gr3_x->SetLimits(xmin, xmax);
    gr3->SetMarkerStyle(20);
    gr3->Draw("P");

    TCanvas *c4 = new TCanvas("c4", "error plot", 200, 10, 700, 500);
    TH1F *h4=c4->DrawFrame(0, -0.05, 100, 1);
    h4->SetYTitle("Efficiency");
    if(elec)
    {
        h4->SetXTitle("Nvtx");
    }
    else h4->SetXTitle("Nvtx");
    h4->GetXaxis()->SetTitleColor(1);
    TGraphErrors *gr4 = new TGraphErrors(n3,x3,y3,err_x3,err_y3);
    if(elec)
    {
        gr4->SetMarkerColor(kRed);
    }
    else gr4->SetMarkerColor(kBlue);
    gr4->SetMarkerStyle(20);
    //gr2->GetYaxis->SetTitle("Efficiency");
    gr4->Draw("P");

    gr1->Write("Graph1");
    gr2->Write("Graph2");
    gr3->Write("Graph3");
    gr4->Write("Graph4");


    h1->Write("Hist1");
    h2->Write("Hist2");
    h3->Write("Hist3");

    c1->Write();
    c2->Write();
    c3->Write();
    c4->Write();

    //f1->Write();*/
    f1->Close();
    delete f1;

    return 0;
    }





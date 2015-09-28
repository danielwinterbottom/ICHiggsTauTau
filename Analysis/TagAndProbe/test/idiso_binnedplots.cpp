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
    if (argc != 4){
        std::cerr << "Need <iselec> <isdata> <id/iso/idiso>" << std::endl;
        exit(1);
    }

    bool elec, isdata;
    elec=boost::lexical_cast<bool>(argv[1]);
    isdata=boost::lexical_cast<bool>(argv[2]);
    std::string type=boost::lexical_cast<std::string>(argv[3]);
    
    TFile *f1=NULL;
    if(elec && isdata)
    {
        if(type=="idiso")
        {
            f1= new TFile("ElectronIdIsoBinnedHistosData.root", "RECREATE");
        }
        if(type=="id")
        {
            f1= new TFile("ElectronIdBinnedHistosData.root", "RECREATE");
        }
        if(type=="iso")
        {
            f1= new TFile("ElectronIsoBinnedHistosData.root", "RECREATE");
        }
    }
    if(elec && !isdata)
    {
        if(type=="idiso")
        {
            f1= new TFile("ElectronIdIsoBinnedHistosMC.root", "RECREATE");
        }
        if(type=="id")
        {
            f1= new TFile("ElectronIdBinnedHistosMC.root", "RECREATE");
        }
        if(type=="iso")
        {
            f1= new TFile("ElectronIsoBinnedHistosMC.root", "RECREATE");
        }
    }
    if(!elec && isdata)
    {
        if(type=="idiso")
        {
            f1= new TFile("MuonIdIsoBinnedHistosData.root", "RECREATE");
        }
        if(type=="id")
        {
            f1= new TFile("MuonIdBinnedHistosData.root", "RECREATE");
        }
        if(type=="iso")
        {
            f1= new TFile("MuonIsoBinnedHistosData.root", "RECREATE");
        }
    }
    if(!elec && !isdata)
    {
        if(type=="idiso")
        {
            f1= new TFile("MuonIdIsoBinnedHistosMC.root", "RECREATE");
        }
        if(type=="id")
        {
            f1= new TFile("MuonIdBinnedHistosMC.root", "RECREATE");
        }
        if(type=="iso")
        {
            f1= new TFile("MuonIsoBinnedHistosMC.root", "RECREATE");
        }
    }
    f1->cd();
    TCanvas *c1 = new TCanvas("c1", "error plot", 200, 10, 700, 500);

    TH1F *h1=c1->DrawFrame(10, -0.05, 150, 1);
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

    ifstream ptbins, etabins, efficiencies, vtxbins;

    if(elec)
    {
        ptbins.open("electron_ptbinsfine.txt");
        etabins.open("electron_etabinsfine.txt");
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
        ptbins.open("muon_ptbinsfine.txt");
        etabins.open("muon_etabinsfine.txt");
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

    vector<std::string> ptbinning;
    vector<int> ptbinbounds;

    while(!ptbins.eof())
    {
        std::string b;
        getline(ptbins,b);
        ptbinning.push_back(b);
       
    }
    ptbins.close();
    
    for(unsigned i=0;i<(ptbinning.size()-1);i++)
    {
        ptbinbounds.push_back(boost::lexical_cast<int>(ptbinning[i]));
    }

    const Int_t n1=ptbinbounds.size()-1;
    Double_t x1[ptbinbounds.size()];
    Double_t err_x1[ptbinbounds.size()];
    for(unsigned i=0; i<(ptbinbounds.size()-1); i++)
    {
        x1[i]=ptbinbounds[i]+((ptbinbounds[i+1]-ptbinbounds[i])/2.0);   
        err_x1[i]=(ptbinbounds[i+1]-ptbinbounds[i])/2.0;
    }
    
    vector<std::string> etabinning;
    vector<double> etabinbounds;
        
    while(!etabins.eof())
    {
        std::string b;
        getline(etabins,b);
        etabinning.push_back(b);
    }
    etabins.close();
    
    for(unsigned i=0;i<(etabinning.size())-1;i++)
    { 
        etabinbounds.push_back(boost::lexical_cast<double>(etabinning[i]));   
    }
     
    const Int_t n2=etabinbounds.size()-1;
   
    Double_t x2[etabinbounds.size()];
    Double_t err_x2[etabinbounds.size()];
    for(unsigned i=0; i<(etabinbounds.size())-1; i++)
    {
        x2[i]=etabinbounds[i]+(fabs((etabinbounds[i+1]-etabinbounds[i])/2.0));   
        err_x2[i]=fabs((etabinbounds[i+1]-etabinbounds[i]))/2.0;

    }

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

    const Int_t n3=vtxbinbounds.size()-1;
    Double_t x3[vtxbinbounds.size()];
    Double_t err_x3[vtxbinbounds.size()];
    for(unsigned i=0; i<(vtxbinbounds.size()-1); i++)
    {
        x3[i]=vtxbinbounds[i]+((vtxbinbounds[i+1]-vtxbinbounds[i])/2.0);   
        err_x3[i]=(vtxbinbounds[i+1]-vtxbinbounds[i])/2.0;
    }

    vector<std::string> ideffs, isoeffs, idisoeffs;
    while(!efficiencies.eof())
    {
        std::string b;
        getline(efficiencies,b);
       
        if(b.find("idbins")!=std::string::npos)
        { 
            ideffs.push_back(b);
        } 
        if(b.find("isobins")!=std::string::npos)
        { 
            isoeffs.push_back(b);
        } 
        if(b.find("idisobins")!=std::string::npos)
        { 
            idisoeffs.push_back(b);
        } 
    }
    efficiencies.close();

    Double_t y1[ptbinbounds.size()];
    Double_t err_y1[ptbinbounds.size()];
    Double_t y2[etabinbounds.size()];
    Double_t err_y2[etabinbounds.size()];
    Double_t y3[vtxbinbounds.size()];
    Double_t err_y3[vtxbinbounds.size()];
  
    if(type=="idiso")
    {
        for(unsigned i=0; i<idisoeffs.size(); i++)
        {    
            stringstream strings(idisoeffs[i]); 
            std::string part;
            std::vector<std::string> parts;
            while(getline(strings, part, ' '))
            {
                parts.push_back(part);
            }
            if(parts[1].find("pt")!=std::string::npos)
            {
                for(int j=2; j<6; j++)
                {
                    if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                    {
                        y1[j]=boost::lexical_cast<double>(parts[2]);
                        err_y1[j]=boost::lexical_cast<double>(parts[3]);
                    }
                }
                if(parts[1].find(boost::lexical_cast<std::string>(1))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(10))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(11))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y1[0]=boost::lexical_cast<double>(parts[2]);
                    err_y1[0]=boost::lexical_cast<double>(parts[3]);
                }
                if(parts[1].find(boost::lexical_cast<std::string>(2))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y1[1]=boost::lexical_cast<double>(parts[2]);
                    err_y1[1]=boost::lexical_cast<double>(parts[3]);
                }
            } 
            if(parts[1].find("eta")!=std::string::npos)
            {
                for(int j=2; j<10; j++)
                { 
                    if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                    {
                        y2[j]=boost::lexical_cast<double>(parts[2]);
                        err_y2[j]=boost::lexical_cast<double>(parts[3]);
                    }
                }
                if(parts[1].find(boost::lexical_cast<std::string>(1))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(10))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(11))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y2[0]=boost::lexical_cast<double>(parts[2]);
                    err_y2[0]=boost::lexical_cast<double>(parts[3]); 
                }
                if(parts[1].find(boost::lexical_cast<std::string>(2))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y2[1]=boost::lexical_cast<double>(parts[2]);
                    err_y2[1]=boost::lexical_cast<double>(parts[3]);
                }
                
            }
            if(parts[1].find("vtx")!=std::string::npos)
            {
                for(int j=2; j<10; j++)
                { 
                    if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                    {
                        y3[j]=boost::lexical_cast<double>(parts[2]);
                        err_y3[j]=boost::lexical_cast<double>(parts[3]);
                    }
                }
                if(parts[1].find(boost::lexical_cast<std::string>(1))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(10))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(11))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y3[0]=boost::lexical_cast<double>(parts[2]);
                    err_y3[0]=boost::lexical_cast<double>(parts[3]); 
                }
                if(parts[1].find(boost::lexical_cast<std::string>(2))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y3[1]=boost::lexical_cast<double>(parts[2]);
                    err_y3[1]=boost::lexical_cast<double>(parts[3]);
                }
                
            }
        }
    }
    if(type=="id")
    {
        for(unsigned i=0; i<ideffs.size(); i++)
        {    
            stringstream strings(ideffs[i]); 
            std::string part;
            std::vector<std::string> parts;
            while(getline(strings, part, ' '))
            {
                parts.push_back(part);
            }
            if(parts[1].find("pt")!=std::string::npos)
            {
                for(int j=2; j<6; j++)
                {
                    if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                    {
                        y1[j]=boost::lexical_cast<double>(parts[2]);
                        err_y1[j]=boost::lexical_cast<double>(parts[3]);
                    }
                }
                if(parts[1].find(boost::lexical_cast<std::string>(1))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(10))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(11))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y1[0]=boost::lexical_cast<double>(parts[2]);
                    err_y1[0]=boost::lexical_cast<double>(parts[3]);
                }
                if(parts[1].find(boost::lexical_cast<std::string>(2))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y1[1]=boost::lexical_cast<double>(parts[2]);
                    err_y1[1]=boost::lexical_cast<double>(parts[3]);
                }
            } 
            if(parts[1].find("eta")!=std::string::npos)
            {
                for(int j=2; j<10; j++)
                { 
                    if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                    {
                        y2[j]=boost::lexical_cast<double>(parts[2]);
                        err_y2[j]=boost::lexical_cast<double>(parts[3]);
                    }
                }
                if(parts[1].find(boost::lexical_cast<std::string>(1))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(10))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(11))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y2[0]=boost::lexical_cast<double>(parts[2]);
                    err_y2[0]=boost::lexical_cast<double>(parts[3]); 
                }
                if(parts[1].find(boost::lexical_cast<std::string>(2))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y2[1]=boost::lexical_cast<double>(parts[2]);
                    err_y2[1]=boost::lexical_cast<double>(parts[3]);
                }
                
            }
            if(parts[1].find("vtx")!=std::string::npos)
            {
                for(int j=2; j<10; j++)
                { 
                    if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                    {
                        y3[j]=boost::lexical_cast<double>(parts[2]);
                        err_y3[j]=boost::lexical_cast<double>(parts[3]);
                    }
                }
                if(parts[1].find(boost::lexical_cast<std::string>(1))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(10))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(11))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y3[0]=boost::lexical_cast<double>(parts[2]);
                    err_y3[0]=boost::lexical_cast<double>(parts[3]); 
                }
                if(parts[1].find(boost::lexical_cast<std::string>(2))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y3[1]=boost::lexical_cast<double>(parts[2]);
                    err_y3[1]=boost::lexical_cast<double>(parts[3]);
                }
                
            }
        }
    }
    if(type=="iso")
    {
        for(unsigned i=0; i<isoeffs.size(); i++)
        {    
            stringstream strings(isoeffs[i]); 
            std::string part;
            std::vector<std::string> parts;
            while(getline(strings, part, ' '))
            {
                parts.push_back(part);
            }
            if(parts[1].find("pt")!=std::string::npos)
            {
                for(int j=2; j<6; j++)
                {
                    if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                    {
                        y1[j]=boost::lexical_cast<double>(parts[2]);
                        err_y1[j]=boost::lexical_cast<double>(parts[3]);
                    }
                }
                if(parts[1].find(boost::lexical_cast<std::string>(1))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(10))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(11))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y1[0]=boost::lexical_cast<double>(parts[2]);
                    err_y1[0]=boost::lexical_cast<double>(parts[3]);
                }
                if(parts[1].find(boost::lexical_cast<std::string>(2))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y1[1]=boost::lexical_cast<double>(parts[2]);
                    err_y1[1]=boost::lexical_cast<double>(parts[3]);
                }
            } 
            if(parts[1].find("eta")!=std::string::npos)
            {
                for(int j=2; j<10; j++)
                { 
                    if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                    {
                        y2[j]=boost::lexical_cast<double>(parts[2]);
                        err_y2[j]=boost::lexical_cast<double>(parts[3]);
                    }
                }
                if(parts[1].find(boost::lexical_cast<std::string>(1))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(10))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(11))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y2[0]=boost::lexical_cast<double>(parts[2]);
                    err_y2[0]=boost::lexical_cast<double>(parts[3]); 
                }
                if(parts[1].find(boost::lexical_cast<std::string>(2))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y2[1]=boost::lexical_cast<double>(parts[2]);
                    err_y2[1]=boost::lexical_cast<double>(parts[3]);
                }
                
            }
            if(parts[1].find("vtx")!=std::string::npos)
            {
                for(int j=2; j<10; j++)
                { 
                    if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                    {
                        y3[j]=boost::lexical_cast<double>(parts[2]);
                        err_y3[j]=boost::lexical_cast<double>(parts[3]);
                    }
                }
                if(parts[1].find(boost::lexical_cast<std::string>(1))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(10))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(11))!=std::string::npos)
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y3[0]=boost::lexical_cast<double>(parts[2]);
                    err_y3[0]=boost::lexical_cast<double>(parts[3]); 
                }
                if(parts[1].find(boost::lexical_cast<std::string>(2))!=std::string::npos
                    && !(parts[1].find(boost::lexical_cast<std::string>(12))!=std::string::npos))
                {
                    y3[1]=boost::lexical_cast<double>(parts[2]);
                    err_y3[1]=boost::lexical_cast<double>(parts[3]);
                }
                
            }
        }
    }  


    TGraphErrors *gr1 = new TGraphErrors(n1,x1,y1,err_x1,err_y1);

    gr1->SetMarkerStyle(20);
    //gr1->SetMarkerSize(1);
    gr1->SetMarkerColor(kRed);
   
    gr1->Draw("P");

    TCanvas *c2 = new TCanvas("c2", "error plot", 200, 10, 700, 500);
    TH1F *h2=c2->DrawFrame(-2.3, -0.05, 2.3, 1);
    h2->SetYTitle("Efficiency");
    if(elec)
    {
        h2->SetXTitle("Electron eta");
    }
    else
    {
        h2->SetXTitle("Muon eta");
    }

    h2->GetXaxis()->SetTitleColor(1);
    TGraphErrors *gr2 = new TGraphErrors(n2, x2,y2,err_x2,err_y2);
    gr2->SetMarkerColor(kRed);
    gr2->SetMarkerStyle(20);
    gr2->Draw("P");

    TCanvas *c3 = new TCanvas("c3", "error plot", 200, 10, 700, 500);
    TH1F *h3=c3->DrawFrame(0, -0.05, 30, 1);
    h3->SetYTitle("Efficiency");
    if(elec)
    {
        h3->SetXTitle("Nvtx");
    }
    else
    {
        h3->SetXTitle("Nvtx");
    }

    h3->GetXaxis()->SetTitleColor(1);
    TGraphErrors *gr3 = new TGraphErrors(n3, x3,y3,err_x3,err_y3);
    gr3->SetMarkerColor(kRed);
    gr3->SetMarkerStyle(20);
    gr3->Draw("P");
    
    gr1->Write("Graph1");
    gr2->Write("Graph2");
    gr3->Write("Graph3");

    c1->Write();
    c2->Write();
    c3->Write();

    //f1->Write();
    f1->Close();
    delete f1;

    return 0;
}





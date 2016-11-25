#include <iostream>
#include <fstream>
#include <TSystem.h>
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TSystem.h"
#include "boost/lexical_cast.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/TagAndProbe/interface/FittingFunction.h"

int main(int argc, char* argv[]){
    //using namespace ic;

    bool elec;
    //elec=argv[2];
    bool isdata;
    bool bifurc, samemean;
    //isdata=argv[3];
    double low, high;

    // Give argument info to user
    std::cout << "Arguments passed to program: " << argc << std::endl;
    for (int i = 0; i < argc; ++i){
    std::cout << i << "\t" << argv[i] << std::endl;
    }
    if (argc != 9){
    std::cerr << "Need args: <id/iso/idiso/trg> <bin> <iselec> <isdata> <low> <high> <bifurc> <samemean>" << std::endl;
    exit(1);
    }

    std::string type=argv[1];
    std::string bin=argv[2];

    elec=boost::lexical_cast<bool>(argv[3]);
    isdata=boost::lexical_cast<bool>(argv[4]);
    low=boost::lexical_cast<double>(argv[5]);
    high=boost::lexical_cast<double>(argv[6]);
    bifurc=boost::lexical_cast<bool>(argv[7]);
    samemean=boost::lexical_cast<bool>(argv[8]);


    if(type=="id")
    {
        fitone(bin,"id", elec, isdata, low, high , bifurc , samemean);
    }
    if(type=="idbins")
    {
        fitone(bin,"idbins", elec, isdata, low, high , bifurc , samemean);
    }
    if(type=="iso")
    {
        fitone(bin, "iso", elec, isdata, low, high , bifurc , samemean);
    }
    if(type=="isobins")
    {
        fitone(bin, "isobins", elec, isdata, low, high , bifurc , samemean);
    }
    if(type=="idiso")
    {    
        fitone(bin, "idiso", elec, isdata, low, high , bifurc , samemean);
    }
    if(type=="idisobins")
    {    
        fitone(bin, "idisobins", elec, isdata, low, high , bifurc , samemean);
    }
    if(type=="trgB" || type=="trgE" || type=="trgbins" || type=="trgBplus" || type=="trgEplus" || type=="trgBminus" || type=="trgBplus")
    { 
        fitone(bin,"trg", elec, isdata, low, high , bifurc , samemean); 
    }

    return 0;
}

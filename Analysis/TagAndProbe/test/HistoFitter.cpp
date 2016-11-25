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

std::string type=argv[1];
bool elec;
//elec=argv[2];
bool isdata;
//isdata=argv[3];

  // Give argument info to user
  std::cout << "Arguments passed to program: " << argc << std::endl;
  for (int i = 0; i < argc; ++i){
    std::cout << i << "\t" << argv[i] << std::endl;
  }
  if (argc != 4){
    std::cerr << "Need 2 args: <id/iso/idiso/trg> <iselec> <isdata>" << std::endl;
    exit(1);
  }

    elec=boost::lexical_cast<bool>(argv[2]);
    isdata=boost::lexical_cast<bool>(argv[3]);

//std::cout << "elec:  " << elec << " argv[2]: " << argv[2]<<std::endl;
//std::cout << "isdata:  " << isdata << " argv[3]: " << *argv[3] << std::endl;
//std::cout << "pre-function " << elec << " " << isdata << std::endl;

    if(type=="id")
    {
        std::string s1="1B";
        std::string s2="1E";
        std::string s3="1Eb";
        std::string s4="2B";
        std::string s5="2E";
        std::string s6="2Eb";
        std::string s7="3B";
        std::string s8="3E";
        std::string s9="3Eb";
        if(elec)
        {
            fit(s1,"id", elec, isdata);
            fit(s2,"id", elec, isdata);
            fit(s4,"id", elec, isdata);
            fit(s5,"id", elec, isdata);
        }
        else
        {
            fit(s1,"id", elec, isdata);
            fit(s2,"id", elec, isdata);
            fit(s3,"id", elec, isdata);
            fit(s4,"id", elec, isdata);
            fit(s5,"id", elec, isdata);
            fit(s6,"id", elec, isdata);
            fit(s7,"id", elec, isdata);
            fit(s8,"id", elec, isdata);
            fit(s9,"id", elec, isdata);
        }
    }
    if(type=="iso")
    {
        std::string s1="1B";
        std::string s2="1E";
        std::string s3="1Eb";
        std::string s4="2B";
        std::string s5="2E";
        std::string s6="2Eb";
        std::string s7="3B";
        std::string s8="3E";
        std::string s9="3Eb";
        if(elec)
        {
            fit(s1,"iso", elec, isdata);
            fit(s2,"iso", elec, isdata);
            fit(s4,"iso", elec, isdata);
            fit(s5,"iso", elec, isdata);
        }
        else
        {
            fit(s1,"iso", elec, isdata);
            fit(s2,"iso", elec, isdata);
            fit(s3,"iso", elec, isdata);
            fit(s4,"iso", elec, isdata);
            fit(s5,"iso", elec, isdata);
            fit(s6,"iso", elec, isdata);
            fit(s7,"iso", elec, isdata);
            fit(s8,"iso", elec, isdata);
            fit(s9,"iso", elec, isdata);
        }
    }
    if(type=="idiso")
    {
        std::string s1="1B";
        std::string s2="1E";
        std::string s3="1Eb";
        std::string s4="2B";
        std::string s5="2E";
        std::string s6="2Eb";
        std::string s7="3B";
        std::string s8="3E";
        std::string s9="3Eb";
        if(elec)
        {
            fit(s1,"idiso", elec, isdata);
            fit(s2,"idiso", elec, isdata);
            fit(s4,"idiso", elec, isdata);
            fit(s5,"idiso", elec, isdata);
        }
        else
        {
            fit(s1,"idiso", elec, isdata);
            fit(s2,"idiso", elec, isdata);
            fit(s3,"idiso", elec, isdata);
            fit(s4,"idiso", elec, isdata);
            fit(s5,"idiso", elec, isdata);
            fit(s6,"idiso", elec, isdata);
            fit(s7,"idiso", elec, isdata);
            fit(s8,"idiso", elec, isdata);
            fit(s9,"idiso", elec, isdata);
        }
        
    }
    if(type=="trgB")
    {
        std::string s1="1B";
        std::string s2="2B";
        std::string s3="3B";
        std::string s4="4B";
        std::string s5="5B";
        std::string s6="6B";
        std::string s7="7B";
        std::string s8="8B";
        std::string s9="9B";
        std::string s10="10B";
        std::string s11="11B";
        std::string s12="12B";
        std::string s25="1vtx";
        std::string s26="2vtx";
        std::string s27="3vtx";
        fit(s1, "trg", elec, isdata);
        fit(s2, "trg", elec, isdata);
        fit(s3, "trg", elec, isdata);
        fit(s4, "trg", elec, isdata);
        fit(s5, "trg", elec, isdata);
        fit(s6, "trg", elec, isdata);
        fit(s7, "trg", elec, isdata);
        fit(s8, "trg", elec, isdata);
        fit(s9, "trg", elec, isdata);
        fit(s10, "trg", elec, isdata);
        fit(s11, "trg", elec, isdata);
        fit(s12, "trg", elec, isdata);/*
        fit(s25, "trg", elec, isdata);
        fit(s26, "trg", elec, isdata);
        fit(s27, "trg", elec, isdata);*/
    }
    if(type=="trgE")
    {
        std::string s1="1E";
        std::string s2="2E";
        std::string s3="3E";
        std::string s4="4E";
        std::string s5="5E";
        std::string s6="6E";
        std::string s7="7E";
        std::string s8="8E";
        std::string s9="9E";
        std::string s10="10E";
        std::string s11="11E";
        std::string s12="12E";
        std::string s13="1Eb";
        std::string s14="2Eb";
        std::string s15="3Eb";
        std::string s16="4Eb";
        std::string s17="5Eb";
        std::string s18="6Eb";
        std::string s19="7Eb";
        std::string s20="8Eb";
        std::string s21="9Eb";
        std::string s22="10Eb";
        std::string s23="11Eb";
        std::string s24="12Eb";
        fit(s1, "trg", elec, isdata);
        fit(s2, "trg", elec, isdata);
        fit(s3, "trg", elec, isdata);
        fit(s4, "trg", elec, isdata);
        fit(s5, "trg", elec, isdata);
        fit(s6, "trg", elec, isdata);
        fit(s7, "trg", elec, isdata);
        fit(s8, "trg", elec, isdata);
        fit(s9, "trg", elec, isdata);
        fit(s10, "trg", elec, isdata);
        fit(s11, "trg", elec, isdata);
        fit(s12, "trg", elec, isdata);
        if(!elec)
        {
            fit(s13, "trg", elec, isdata);
            fit(s14, "trg", elec, isdata);
            fit(s15, "trg", elec, isdata);
            fit(s16, "trg", elec, isdata);
            fit(s17, "trg", elec, isdata);
            fit(s18, "trg", elec, isdata);
            fit(s19, "trg", elec, isdata);
            fit(s20, "trg", elec, isdata);
            fit(s21, "trg", elec, isdata);
            fit(s22, "trg", elec, isdata);
            fit(s23, "trg", elec, isdata);
            fit(s24, "trg", elec, isdata);
        }
    }
    if(type=="trgBplus")
    {
        std::string s1="1Bplus";
        std::string s2="2Bplus";
        std::string s3="3Bplus";
        std::string s4="4Bplus";
        std::string s5="5Bplus";
        std::string s6="6Bplus";
        std::string s7="7Bplus";
        std::string s8="8Bplus";
        std::string s9="9Bplus";
        std::string s10="10Bplus";
        std::string s11="11Bplus";
        std::string s12="12Bplus";
        fit(s1, "trg", elec, isdata);
        fit(s2, "trg", elec, isdata);
        fit(s3, "trg", elec, isdata);
        fit(s4, "trg", elec, isdata);
        fit(s5, "trg", elec, isdata);
        fit(s6, "trg", elec, isdata);
        fit(s7, "trg", elec, isdata);
        fit(s8, "trg", elec, isdata);
        fit(s9, "trg", elec, isdata);
        fit(s10, "trg", elec, isdata);
        fit(s11, "trg", elec, isdata);
        fit(s12, "trg", elec, isdata);
    }
    if(type=="trgEplus")
    {
        std::string s1="1Eplus";
        std::string s2="2Eplus";
        std::string s3="3Eplus";
        std::string s4="4Eplus";
        std::string s5="5Eplus";
        std::string s6="6Eplus";
        std::string s7="7Eplus";
        std::string s8="8Eplus";
        std::string s9="9Eplus";
        std::string s10="10Eplus";
        std::string s11="11Eplus";
        std::string s12="12Eplus";
        std::string s13="1Ebplus";
        std::string s14="2Ebplus";
        std::string s15="3Ebplus";
        std::string s16="4Ebplus";
        std::string s17="5Ebplus";
        std::string s18="6Ebplus";
        std::string s19="7Ebplus";
        std::string s20="8Ebplus";
        std::string s21="9Ebplus";
        std::string s22="10Ebplus";
        std::string s23="11Ebplus";
        std::string s24="12Ebplus";
        fit(s1, "trg", elec, isdata);
        fit(s2, "trg", elec, isdata);
        fit(s3, "trg", elec, isdata);
        fit(s4, "trg", elec, isdata);
        fit(s5, "trg", elec, isdata);
        fit(s6, "trg", elec, isdata);
        fit(s7, "trg", elec, isdata);
        fit(s8, "trg", elec, isdata);
        fit(s9, "trg", elec, isdata);
        fit(s10, "trg", elec, isdata);
        fit(s11, "trg", elec, isdata);
        fit(s12, "trg", elec, isdata);
        if(!elec)
        {
            fit(s13, "trg", elec, isdata);
            fit(s14, "trg", elec, isdata);
            fit(s15, "trg", elec, isdata);
            fit(s16, "trg", elec, isdata);
            fit(s17, "trg", elec, isdata);
            fit(s18, "trg", elec, isdata);
            fit(s19, "trg", elec, isdata);
            fit(s20, "trg", elec, isdata);
            fit(s21, "trg", elec, isdata);
            fit(s22, "trg", elec, isdata);
            fit(s23, "trg", elec, isdata);
            fit(s24, "trg", elec, isdata);
        }
    }
    if(type=="trgBminus")
    {
        std::string s1="1Bminus";
        std::string s2="2Bminus";
        std::string s3="3Bminus";
        std::string s4="4Bminus";
        std::string s5="5Bminus";
        std::string s6="6Bminus";
        std::string s7="7Bminus";
        std::string s8="8Bminus";
        std::string s9="9Bminus";
        std::string s10="10Bminus";
        std::string s11="11Bminus";
        std::string s12="12Bminus";
        fit(s1, "trg", elec, isdata);
        fit(s2, "trg", elec, isdata);
        fit(s3, "trg", elec, isdata);
        fit(s4, "trg", elec, isdata);
        fit(s5, "trg", elec, isdata);
        fit(s6, "trg", elec, isdata);
        fit(s7, "trg", elec, isdata);
        fit(s8, "trg", elec, isdata);
        fit(s9, "trg", elec, isdata);
        fit(s10, "trg", elec, isdata);
        fit(s11, "trg", elec, isdata);
        fit(s12, "trg", elec, isdata);
    }
    if(type=="trgEminus")
    {
        std::string s1="1Eminus";
        std::string s2="2Eminus";
        std::string s3="3Eminus";
        std::string s4="4Eminus";
        std::string s5="5Eminus";
        std::string s6="6Eminus";
        std::string s7="7Eminus";
        std::string s8="8Eminus";
        std::string s9="9Eminus";
        std::string s10="10Eminus";
        std::string s11="11Eminus";
        std::string s12="12Eminus";
        std::string s13="1Ebminus";
        std::string s14="2Ebminus";
        std::string s15="3Ebminus";
        std::string s16="4Ebminus";
        std::string s17="5Ebminus";
        std::string s18="6Ebminus";
        std::string s19="7Ebminus";
        std::string s20="8Ebminus";
        std::string s21="9Ebminus";
        std::string s22="10Ebminus";
        std::string s23="11Ebminus";
        std::string s24="12Ebminus";
        fit(s1, "trg", elec, isdata);
        fit(s2, "trg", elec, isdata);
        fit(s3, "trg", elec, isdata);
        fit(s4, "trg", elec, isdata);
        fit(s5, "trg", elec, isdata);
        fit(s6, "trg", elec, isdata);
        fit(s7, "trg", elec, isdata);
        fit(s8, "trg", elec, isdata);
        fit(s9, "trg", elec, isdata);
        fit(s10, "trg", elec, isdata);
        fit(s11, "trg", elec, isdata);
        fit(s12, "trg", elec, isdata);
        if(!elec)
        {
            fit(s13, "trg", elec, isdata);
            fit(s14, "trg", elec, isdata);
            fit(s15, "trg", elec, isdata);
            fit(s16, "trg", elec, isdata);
            fit(s17, "trg", elec, isdata);
            fit(s18, "trg", elec, isdata);
            fit(s19, "trg", elec, isdata);
            fit(s20, "trg", elec, isdata);
            fit(s21, "trg", elec, isdata);
            fit(s22, "trg", elec, isdata);
            fit(s23, "trg", elec, isdata);
            fit(s24, "trg", elec, isdata);
        }
    }
    if(type=="trgbins")
    {
        std::string s1="1eta";
        std::string s2="2eta";
        std::string s3="3eta";
        std::string s4="4eta";
        std::string s5="5eta";
        std::string s6="6eta";
        std::string s7="7eta";
        std::string s8="8eta";
        std::string s9="9eta";
        std::string s10="10eta";
        fit(s1, "trg", elec, isdata);
        fit(s2, "trg", elec, isdata);
        fit(s3, "trg", elec, isdata);
        fit(s4, "trg", elec, isdata);
        fit(s5, "trg", elec, isdata);
        fit(s6, "trg", elec, isdata);
        fit(s7, "trg", elec, isdata);
        fit(s8, "trg", elec, isdata);
        fit(s9, "trg", elec, isdata);
        fit(s10, "trg", elec, isdata);
    }
    if(type=="idbins")
    {
        std::string s1="1pt";
        std::string s2="2pt";
        std::string s3="3pt";
        std::string s4="4pt";
        std::string s5="5pt";
        std::string s6="6pt";
        std::string s7="1eta";
        std::string s8="2eta";
        std::string s9="3eta";
        std::string s10="4eta";
        std::string s11="5eta";
        std::string s12="6eta";
        std::string s13="7eta";
        std::string s14="8eta";
        std::string s15="9eta";
        std::string s16="10eta";
        std::string s17="1vtx";
        std::string s18="2vtx";
        std::string s19="3vtx";
        std::string s20="4vtx";
        std::string s21="5vtx";
        std::string s22="6vtx";
        std::string s23="7vtx";
        std::string s24="8vtx";
        std::string s25="9vtx";
        std::string s26="10vtx";
        fit(s1, "idbins", elec, isdata);
        fit(s2, "idbins", elec, isdata);
        fit(s3, "idbins", elec, isdata);
        fit(s4, "idbins", elec, isdata);
        fit(s5, "idbins", elec, isdata);
        fit(s6, "idbins", elec, isdata);
        fit(s7, "idbins", elec, isdata);
        fit(s8, "idbins", elec, isdata);
        fit(s9, "idbins", elec, isdata);
        fit(s10, "idbins", elec, isdata);
        fit(s11, "idbins", elec, isdata);
        fit(s12, "idbins", elec, isdata);
        fit(s13, "idbins", elec, isdata);
        fit(s14, "idbins", elec, isdata);
        fit(s15, "idbins", elec, isdata);
        fit(s16, "idbins", elec, isdata);
        fit(s17, "idbins", elec, isdata);
        fit(s18, "idbins", elec, isdata);
        fit(s19, "idbins", elec, isdata);
        fit(s20, "idbins", elec, isdata);
        fit(s21, "idbins", elec, isdata);
        fit(s22, "idbins", elec, isdata);
        fit(s23, "idbins", elec, isdata);
        fit(s24, "idbins", elec, isdata);
        fit(s25, "idbins", elec, isdata);
        fit(s26, "idbins", elec, isdata);
    }
    if(type=="isobins")
    {
        std::string s1="1pt";
        std::string s2="2pt";
        std::string s3="3pt";
        std::string s4="4pt";
        std::string s5="5pt";
        std::string s6="6pt";
        std::string s7="1eta";
        std::string s8="2eta";
        std::string s9="3eta";
        std::string s10="4eta";
        std::string s11="5eta";
        std::string s12="6eta";
        std::string s13="7eta";
        std::string s14="8eta";
        std::string s15="9eta";
        std::string s16="10eta";
        std::string s17="1vtx";
        std::string s18="2vtx";
        std::string s19="3vtx";
        std::string s20="4vtx";
        std::string s21="5vtx";
        std::string s22="6vtx";
        std::string s23="7vtx";
        std::string s24="8vtx";
        std::string s25="9vtx";
        std::string s26="10vtx";
        fit(s1, "isobins", elec, isdata);
        fit(s2, "isobins", elec, isdata);
        fit(s3, "isobins", elec, isdata);
        fit(s4, "isobins", elec, isdata);
        fit(s5, "isobins", elec, isdata);
        fit(s6, "isobins", elec, isdata);
        fit(s7, "isobins", elec, isdata);
        fit(s8, "isobins", elec, isdata);
        fit(s9, "isobins", elec, isdata);
        fit(s10, "isobins", elec, isdata);
        fit(s11, "isobins", elec, isdata);
        fit(s12, "isobins", elec, isdata);
        fit(s13, "isobins", elec, isdata);
        fit(s14, "isobins", elec, isdata);
        fit(s15, "isobins", elec, isdata);
        fit(s16, "isobins", elec, isdata);
        fit(s17, "isobins", elec, isdata);
        fit(s18, "isobins", elec, isdata);
        fit(s19, "isobins", elec, isdata);
        fit(s20, "isobins", elec, isdata);
        fit(s21, "isobins", elec, isdata);
        fit(s22, "isobins", elec, isdata);
        fit(s23, "isobins", elec, isdata);
        fit(s24, "isobins", elec, isdata);
        fit(s25, "isobins", elec, isdata);
        fit(s26, "isobins", elec, isdata);
    }
    if(type=="idisobins")
    {
        std::string s1="1pt";
        std::string s2="2pt";
        std::string s3="3pt";
        std::string s4="4pt";
        std::string s5="5pt";
        std::string s6="6pt";
        std::string s7="1eta";
        std::string s8="2eta";
        std::string s9="3eta";
        std::string s10="4eta";
        std::string s11="5eta";
        std::string s12="6eta";
        std::string s13="7eta";
        std::string s14="8eta";
        std::string s15="9eta";
        std::string s16="10eta";
        std::string s17="1vtx";
        std::string s18="2vtx";
        std::string s19="3vtx";
        std::string s20="4vtx";
        std::string s21="5vtx";
        std::string s22="6vtx";
        std::string s23="7vtx";
        std::string s24="8vtx";
        std::string s25="9vtx";
        std::string s26="10vtx";
        fit(s1, "idisobins", elec, isdata);
        fit(s2, "idisobins", elec, isdata);
        fit(s3, "idisobins", elec, isdata);
        fit(s4, "idisobins", elec, isdata);
        fit(s5, "idisobins", elec, isdata);
        fit(s6, "idisobins", elec, isdata);
        fit(s7, "idisobins", elec, isdata);
        fit(s8, "idisobins", elec, isdata);
        fit(s9, "idisobins", elec, isdata);
        fit(s10, "idisobins", elec, isdata);
        fit(s11, "idisobins", elec, isdata);
        fit(s12, "idisobins", elec, isdata);
        fit(s13, "idisobins", elec, isdata);
        fit(s14, "idisobins", elec, isdata);
        fit(s15, "idisobins", elec, isdata);
        fit(s16, "idisobins", elec, isdata);
        fit(s17, "idisobins", elec, isdata);
        fit(s18, "idisobins", elec, isdata);
        fit(s19, "idisobins", elec, isdata);
        fit(s20, "idisobins", elec, isdata);
        fit(s21, "idisobins", elec, isdata);
        fit(s22, "idisobins", elec, isdata);
        fit(s23, "idisobins", elec, isdata);
        fit(s24, "idisobins", elec, isdata);
        fit(s25, "idisobins", elec, isdata);
        fit(s26, "idisobins", elec, isdata);
    }

    return 0;
}

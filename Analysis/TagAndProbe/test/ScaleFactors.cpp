#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "boost/lexical_cast.hpp"
#include "UserCode/ICHiggsTauTau/Analysis/TagAndProbe/interface/UsefulFunctions.h"
using namespace std;
using namespace ic;

int main(int argc, char* argv[])
{
    std::cout << "Arguments passed to program: " << argc << std::endl;
    for (int i = 0; i < argc; ++i){
        std::cout << i << "\t" << argv[i] << std::endl;
    }
    if (argc != 2){
        std::cerr << "Need <iselec>" << std::endl;
        exit(1);
    }
    bool elec=boost::lexical_cast<bool>(argv[1]);

    ifstream eff_data, eff_mc;
    if(elec)
    {
        eff_data.open("electron_eff_data.txt");
        eff_mc.open("electron_eff_MC.txt");
    }
    else
    {
        eff_data.open("muon_eff_data.txt");
        eff_mc.open("muon_eff_MC.txt");
    }

    vector<std::string> dataideffs, dataisoeffs, dataidisoeffs;
    while(!eff_data.eof())
    {
        std::string b;
        getline(eff_data,b);
       
        if(b.find("id")!=std::string::npos
            && !(b.find("idbins")!=std::string::npos)
            && !(b.find("idiso")!=std::string::npos ))

        { 
            dataideffs.push_back(b);
        } 
        if(b.find("iso")!=std::string::npos
            && !(b.find("isobins")!=std::string::npos)
            && !(b.find("idiso")!=std::string::npos ))
        { 
            dataisoeffs.push_back(b);
        } 
        if(b.find("idiso")!=std::string::npos
            && !(b.find("idisobins")!=std::string::npos))
        { 
            dataidisoeffs.push_back(b);
        } 
    }
    eff_data.close();

    vector<std::string> mcideffs, mcisoeffs, mcidisoeffs;
    while(!eff_mc.eof())
    {
        std::string b;
        getline(eff_mc,b);
       
        if(b.find("id")!=std::string::npos
            && !(b.find("idbins")!=std::string::npos)
            && !(b.find("idiso")!=std::string::npos ))
        { 
            mcideffs.push_back(b);
        } 
        if(b.find("iso")!=std::string::npos
            && !(b.find("isobins")!=std::string::npos)
            && !(b.find("idiso")!=std::string::npos ))
        { 
            mcisoeffs.push_back(b);
        } 
        if(b.find("idiso")!=std::string::npos
            && !(b.find("idisobins")!=std::string::npos))
        { 
            mcidisoeffs.push_back(b);
        } 
    }
    eff_mc.close();

    double y1_data[dataideffs.size()];
    double err_y1_data[dataideffs.size()];
    double y2_data[dataideffs.size()];
    double err_y2_data[dataideffs.size()];
    double y3_data[dataideffs.size()];
    double err_y3_data[dataideffs.size()];
    double y1_mc[mcideffs.size()];
    double err_y1_mc[mcideffs.size()];
    double y2_mc[mcideffs.size()];
    double err_y2_mc[mcideffs.size()];
    double y3_mc[mcideffs.size()];
    double err_y3_mc[mcideffs.size()];
    double y1iso_data[dataisoeffs.size()];
    double err_y1iso_data[dataisoeffs.size()];
    double y2iso_data[dataisoeffs.size()];
    double err_y2iso_data[dataisoeffs.size()];
    double y3iso_data[dataisoeffs.size()];
    double err_y3iso_data[dataisoeffs.size()];
    double y1iso_mc[mcisoeffs.size()];
    double err_y1iso_mc[mcisoeffs.size()];
    double y2iso_mc[mcisoeffs.size()];
    double err_y2iso_mc[mcisoeffs.size()];
    double y3iso_mc[mcisoeffs.size()];
    double err_y3iso_mc[mcisoeffs.size()];

    for(unsigned i=0; i<dataideffs.size(); i++)
    {    
        stringstream strings(dataideffs[i]); 
        std::string part;
        std::vector<std::string> parts;
        while(getline(strings, part, ' '))
        {
            parts.push_back(part);
        }
        if(parts[1].find("B")!=std::string::npos)
        {
            for(int j=0; j<3; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    y1_data[j]=boost::lexical_cast<double>(parts[2]);
                    err_y1_data[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
        } 
        if(parts[1].find("E")!=std::string::npos&& !(parts[1].find("Eb")!=std::string::npos))
        {
            for(int j=0; j<3; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    y2_data[j]=boost::lexical_cast<double>(parts[2]);
                    err_y2_data[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
        }
        if(parts[1].find("Eb")!=std::string::npos)
        {
            for(int j=0; j<3; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    y3_data[j]=boost::lexical_cast<double>(parts[2]);
                    err_y3_data[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
        }
    }
    for(unsigned i=0; i<mcideffs.size(); i++)
    {    
        stringstream strings(mcideffs[i]); 
        std::string part;
        std::vector<std::string> parts;
        while(getline(strings, part, ' '))
        {
            parts.push_back(part);
        }
        if(parts[1].find("B")!=std::string::npos)
        {
            for(int j=0; j<3; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    y1_mc[j]=boost::lexical_cast<double>(parts[2]); 
                    err_y1_mc[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
        } 
        if(parts[1].find("E")!=std::string::npos && !(parts[1].find("Eb")!=std::string::npos))
        {
            for(int j=0; j<3; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    y2_mc[j]=boost::lexical_cast<double>(parts[2]);
                    err_y2_mc[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
        }
        if(parts[1].find("Eb")!=std::string::npos)
        {
            for(int j=0; j<3; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    y3_mc[j]=boost::lexical_cast<double>(parts[2]);
                    err_y3_mc[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
        }
    }
    for(unsigned i=0; i<dataisoeffs.size(); i++)
    {    
        stringstream strings(dataisoeffs[i]); 
        std::string part;
        std::vector<std::string> parts;
        while(getline(strings, part, ' '))
        {
            parts.push_back(part);
        }
        if(parts[1].find("B")!=std::string::npos)
        {
            for(int j=0; j<3; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    y1iso_data[j]=boost::lexical_cast<double>(parts[2]);
                    err_y1iso_data[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
        } 
        if(parts[1].find("E")!=std::string::npos  && !(parts[1].find("Eb")!=std::string::npos))
        {
            for(int j=0; j<3; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    y2iso_data[j]=boost::lexical_cast<double>(parts[2]);
                    err_y2iso_data[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
        }
        if(parts[1].find("Eb")!=std::string::npos )
        {
            for(int j=0; j<3; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    y3iso_data[j]=boost::lexical_cast<double>(parts[2]);
                    err_y3iso_data[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
        }
    }
    for(unsigned i=0; i<mcisoeffs.size(); i++)
    {    
        stringstream strings(mcisoeffs[i]); 
        std::string part;
        std::vector<std::string> parts;
        while(getline(strings, part, ' '))
        {
            parts.push_back(part);
        }
        if(parts[1].find("B")!=std::string::npos)
        {
            for(int j=0; j<3; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    y1iso_mc[j]=boost::lexical_cast<double>(parts[2]); 
                    err_y1iso_mc[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
        } 
        if(parts[1].find("E")!=std::string::npos && !(parts[1].find("Eb")!=std::string::npos))
        {
            for(int j=0; j<3; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    y2iso_mc[j]=boost::lexical_cast<double>(parts[2]);
                    err_y2iso_mc[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
        }
        if(parts[1].find("Eb")!=std::string::npos)
        {
            for(int j=0; j<3; j++)
            {
                if(parts[1].find(boost::lexical_cast<std::string>(j+1))!=std::string::npos)
                {
                    y3iso_mc[j]=boost::lexical_cast<double>(parts[2]);
                    err_y3iso_mc[j]=boost::lexical_cast<double>(parts[3]);
                }
            }
        }
    }

    int i_final, i_initial;
    if(elec) i_final=2;
    else i_final=2;
    if(elec) i_initial=0;
    else i_initial=0;

    std::cout << "======================================================================" << std::endl;
    for(int i=i_initial; i<=i_final; i++)
    {   
        std::cout << y1_mc[i]<< " +/- " << err_y1_mc[i] << " " << y1_data[i] << " +/- " << err_y1_data[i] << " " <<(y1_data[i]/y1_mc[i]) <<
        " +/- " << PropError(y1_mc[i], err_y1_mc[i], y1_data[i], err_y1_data[i]) << std::endl; 
        std::cout << y2_mc[i]<< " +/- " << err_y2_mc[i] << " " << y2_data[i] << " +/- " << err_y2_data[i] << " " <<(y2_data[i]/y2_mc[i]) <<
        " +/- " << PropError(y2_mc[i], err_y2_mc[i], y2_data[i], err_y2_data[i]) << std::endl; 
        std::cout << y3_mc[i]<< " +/- " << err_y3_mc[i] << " " << y3_data[i] << " +/- " << err_y3_data[i] << " " <<(y3_data[i]/y3_mc[i]) <<
        " +/- " << PropError(y3_mc[i], err_y3_mc[i], y3_data[i], err_y3_data[i]) << std::endl; 
    
    }
    std::cout << "======================================================================" << std::endl;
    for(int i=i_initial; i<=i_final; i++)
    {
        std::cout << y1iso_mc[i]<< " +/- " << err_y1iso_mc[i] << " " << y1iso_data[i] << " +/- " << err_y1iso_data[i] 
            << " " <<(y1iso_data[i]/y1iso_mc[i]) << " +/- " << PropError(y1iso_mc[i], err_y1iso_mc[i], y1iso_data[i], err_y1iso_data[i])<< std::endl; 
        std::cout << y2iso_mc[i]<< " +/- " << err_y2iso_mc[i] << " " << y2iso_data[i] << " +/- " << err_y2iso_data[i] 
            << " " <<(y2iso_data[i]/y2iso_mc[i]) << " +/- " << PropError(y2iso_mc[i], err_y2iso_mc[i], y2iso_data[i], err_y2iso_data[i])<<std::endl;  
        std::cout << y3iso_mc[i]<< " +/- " << err_y3iso_mc[i] << " " << y3iso_data[i] << " +/- " << err_y3iso_data[i] 
            << " " <<(y3iso_data[i]/y3iso_mc[i]) << " +/- " << PropError(y3iso_mc[i], err_y3iso_mc[i], y3iso_data[i], err_y3iso_data[i])<<std::endl;  
    }
    std::cout << "======================================================================" << std::endl;

    return 0;
}

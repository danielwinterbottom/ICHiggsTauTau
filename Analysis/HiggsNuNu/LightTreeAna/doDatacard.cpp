#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include "TROOT.h"
#include "TDirectory.h"
#include "TError.h"
#include "TFile.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TString.h"
#include <boost/algorithm/string.hpp>
// #include "boost/filesystem.hpp"
#include <boost/program_options.hpp>

// namespace fs = boost::filesystem;
namespace po = boost::program_options;
std::vector<std::string> samples ({"qqH", "wel", "wmu", "wtau", "qcd", "zvv", "vv", "wg", "top"});
std::string var = "metnomuons";

//Config Struct - POD-Type that stores information read from the configuration file
double Integral(TH1F* hist);

struct systematic{
    std::string datacard_name;
    std::string syst_effect;
    int gmN_value;
    double effect_value;
    std::vector<bool> target_samples;
    std::vector<TFile*> files;
    std::vector<TH1F*> hists;
    systematic() = default;
    double get_stat_effect(){
        for (auto i = 0; i < samples.size(); i++){
            if (target_samples[i]==true){
                return Integral((TH1F*) (files[0]->GetDirectory(TString(samples[i])))->Get(TString(var)));
            }
        }
        return 0.;
    }
    systematic(std::string dcname, std::string fprefix, std::string effect_, std::string samples_) : datacard_name(dcname){
        gmN_value = 0;
        effect_value =0.;
        if (samples_ == "all"){
            for(auto i = samples.begin(); i != samples.end(); i++){
                target_samples.push_back(true);
            }
        }
        else {
            std::vector<std::string> strs;
            boost::split(strs, samples_, boost::is_any_of("\t "), boost::token_compress_on);
            for (auto i = samples.begin(); i != samples.end(); i++){
                if (std::find(strs.begin(), strs.end(), *i) != strs.end()) target_samples.push_back(true);
                else target_samples.push_back(false);
            }
        }
        files.push_back(TFile::Open("central.root"));
        if (fprefix != "central"){
            if (fprefix == "JER") {
                files.push_back(TFile::Open((fprefix+"WORSE.root").c_str()));
                files.push_back(TFile::Open((fprefix+"BETTER.root").c_str()));
            }
            else{
                files.push_back(TFile::Open((fprefix+"UP.root").c_str()));
                files.push_back(TFile::Open((fprefix+"DOWN.root").c_str()));
            }
        }
        std::vector<std::string> effects_data;
        boost::split(effects_data, effect_, boost::is_any_of("-"), boost::token_compress_on);
        if (effects_data.size() > 2) {
            gmN_value = stoi(effects_data[1]);
            effect_value = stod(effects_data[2]);
        }
        else if(effects_data[1]=="stat"){
            effect_value = get_stat_effect();
        }
        else if (effects_data.size() > 1) effect_value = stod(effects_data[1]);
        syst_effect = effects_data[0];
    }
} ;


void get_histograms(systematic& syst, std::string sample){
    TDirectory *dir;
    dir = syst.files[0]->GetDirectory(sample.c_str());
    syst.hists.push_back((TH1F*) dir->Get(TString(var)));
    if (syst.datacard_name != "rate"){
        if (syst.files.size() == 1){
            syst.hists.push_back((TH1F*) dir->Get(TString(var+syst.datacard_name+"Up")));        
            syst.hists.push_back((TH1F*) dir->Get(TString(var+syst.datacard_name+"Down")));        
        }
        else {
            dir = syst.files[1]->GetDirectory(sample.c_str());
            syst.hists.push_back((TH1F*) dir->Get(var.c_str()));
            dir = syst.files[2]->GetDirectory(sample.c_str());
            syst.hists.push_back((TH1F*) dir->Get(var.c_str()));        
        }
    }
}

void write_shapes(std::string sample, TFile* f_out, systematic& syst, bool is_central = false){ //Grab the hist and write it out
    TDirectory *dir;
    dir = f_out->GetDirectory(sample.c_str());
    dir->cd();
    if (is_central) {
        (syst.hists[0])->Write();
    }
    else{
        (syst.hists[1])->SetName(TString(var+"_"+syst.datacard_name+"Up"));
        (syst.hists[2])->SetName(TString(var+"_"+syst.datacard_name+"Down"));
        (syst.hists[1])->Write();
        (syst.hists[2])->Write();
    }
    return;
} 

double Integral (TH1F* hist){
    double integ = hist->Integral(0, hist->GetNbinsX()+1);
    return integ;
}

std::string get_yield(systematic& syst, std::string sample){
    double central, up, down;
    central = Integral(syst.hists[0]);
    if (syst.datacard_name == "rate"){
        return std::to_string(central);
    }
    up = Integral(syst.hists[1]);
    if (syst.datacard_name == "shape"){
        return std::to_string(up);
    }
    down = Integral(syst.hists[2]);
    return std::to_string(down/central) + "/" + std::to_string(up/central);
}

void process(fstream& output, TFile* outfile, systematic& syst){
    output << syst.datacard_name;
    if (syst.datacard_name != "rate") output << "\t" << syst.syst_effect;
    if (syst.gmN_value != 0) output << " " << syst.gmN_value;
    output << "\t";
    for (unsigned int i = 0; i < samples.size(); i++){
        get_histograms(syst, samples[i]);
        if (syst.syst_effect == "shape"){
            if (syst.datacard_name == "rate") write_shapes(samples[i], outfile, syst, true);
            else write_shapes(samples[i], outfile, syst);
        }
        if (syst.target_samples[i] == false){ 
            output << "-\t";
            continue;
        }
        if (syst.effect_value != 0){
            output << syst.effect_value << "\t";
            continue;
        }
        output << get_yield(syst, samples[i]) << "\t";
        syst.hists.erase(syst.hists.begin(), syst.hists.end());
    }
    output << std::endl;
    for(auto i = syst.files.begin(); i != syst.files.end(); i++){
        (*i)->Close();
    }
}

double get_observation(systematic& syst){
    return Integral((TH1F*) (syst.files[0]->GetDirectory(samples[0].c_str()))->Get(var.c_str()));
}

int main(int argc, char* argv[]){
    gErrorIgnoreLevel = 5000;
    gStyle->SetOptStat(false);
    std::string bin, syst_list_name, shape_output_name, dc_output_name, files_dir;
    //Parse Configs to get the files directory and syst list
    po::options_description opts("Allowed Options");
    opts.add_options()
        ("help", "produce help message")
        ("bin", po::value(&bin)->default_value("vbf"), "Datacard Bin")
        ("var", po::value(&var), "Discriminant Variable")
        ("input-dir", po::value(&files_dir)->default_value("./"), "Directory of Analysed Light Trees")
        ("shapes-output", po::value(&shape_output_name)->default_value("combine_shapes.root"), "Shapes file for combine")
        ("systematics-list", po::value(&syst_list_name)->default_value("systematics.txt"), "List of systematics")
        ("datacard-output", po::value(&dc_output_name)->default_value("datacard.txt"), "DataCard")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, opts), vm);
    po::notify(vm);
    if (vm.count("help")){
        std::cout << opts << std::endl;
        return 1;
    }
    // std::cout << "Reading Files from " << files_dir << std::endl;
    std::cout << "Writing Shapes to " << shape_output_name << std::endl;
    std::cout << "Reading systematics from " << files_dir << syst_list_name << std::endl;
    TFile *shapes_out = TFile::Open(shape_output_name.c_str(), "RECREATE");     //Create File to hold the shape plot
    for (auto i = samples.begin(); i != samples.end(); i++) shapes_out->mkdir((*i).c_str()); // Create a directory for each sample
    //Check for syst list
    //Open Syst List and Datacard output
    fstream systematic_list(syst_list_name.c_str(), std::ios::in);
    fstream datacard(dc_output_name.c_str(), std::ios::out);
    std::vector<systematic> systematics;
    std::string line;
    std::string seperator (50, '-');
    std::vector<std::string> strs;
    while(getline(systematic_list, line)){ //Parse systematic list
        if (line[0] == '#' || line=="\n") continue;
        boost::trim(line);
        boost::split(strs, line, boost::is_any_of("\t "), boost::token_compress_on);
        std::string sample_ = "";
        if (strs.size() > 4){
            for_each(strs.begin()+3, strs.end(), [&](std::string str_){ //Collapse the sample list down to one string
                sample_ = sample_ + str_ + "\t";
            });
        }
        else sample_ = strs[3];
        systematics.push_back(systematic(strs[0], strs[1], strs[2], sample_));
    }
    //Build the datacard
    datacard << "nmax \t1 \n";
    datacard << "jmax \t" << samples.size() -1 << "\nkmax \t" << systematics.size() -1 << "\n";
    datacard << seperator << std::endl;
    datacard << "shapes \t * \t * \t" << shape_output_name << "\t $PROCESS \t $PROCESS_$SYSTEMATIC" << std::endl;
    datacard << seperator << std::endl;
    datacard << "bin \t" << bin << " \nobservation \t" << get_observation(systematics[0]) << std::endl;
    datacard << seperator << std::endl;
    datacard << "bin \t";
    for (unsigned int i = 0; i < samples.size(); i++) {
        datacard << bin << "\t";
    }
    datacard << std::endl;
    datacard << "process \t";
    for (unsigned int i = 0; i < samples.size(); i++){ 
        datacard << i << "\t";
    }
    datacard << std::endl;
    datacard << "process \t";
    for (unsigned int i = 0; i < samples.size(); i++) {
        datacard << samples[i] << "\t";
    }
    datacard << std::endl;
    //Run over the sytematics
    for_each(systematics.begin(), systematics.end(), [&](systematic syst){
        std::cout << syst.datacard_name << std::endl;
        process(datacard, shapes_out, syst);
        if (syst.datacard_name == "rate") datacard << seperator << std::endl;
    });
    systematic_list.close();
    datacard.close();
    return 0;
}

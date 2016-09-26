//./bin/CombineCategories --inputDirectory mt_output/test3/ --OneJetCats 3 --MakePlots

// ROOT includes
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"

// C++ includes
#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include <string>
#include <cstdlib>
#include <math.h>
#include <cmath> 
#include <algorithm>
#include <sstream>
#include "boost/filesystem.hpp" 

struct ProgramOptions{
  ProgramOptions(){
    dirname        = "";
    zerojet_cat_num = 2;
    vbf_cat_num    = 2;
    onejet_cat_num = 3;
    RecreateOutput = false;
    MakePlots = false;
    CutLine = 1;
    channel = "mt";
    variable = "m_vis";
  }

  std::string dirname;
  unsigned vbf_cat_num;
  unsigned onejet_cat_num;
  unsigned zerojet_cat_num;
  bool RecreateOutput;
  bool MakePlots;
  unsigned CutLine;
  std::string channel;
  std::string variable;
  
};

struct Cut{
  std::string name;
  std::string value;
  unsigned type;
  //Cut types: 0: '==', 1: '<', 2: '>' 
};
struct Category{
  std::string name;
  std::vector<Cut> cuts;
  std::string exclude;
};

inline bool file_exists(const std::string& name){
  std::ifstream f(name.c_str());
  return f.good();
}

int find_nth(std::string fullstring, int pos, std::string sub, int nth){
  size_t found_pos = fullstring.find(sub, pos);
  if(0 == nth || std::string::npos == found_pos)  return found_pos;
  return find_nth(fullstring, found_pos+1, sub, nth-1);
}

struct greater {
  bool operator() (std::string a, std::string b) {
    int f = 0; int l = 0;
    l = find_nth(a, 0, " ", 0);
    double c = std::stod(a.substr(f, l - f));
    l = find_nth(b, 0, " ", 0);
    double d = std::stod(b.substr(f, l - f));
    return (c > d);
  }
};

void SortFile(std::string file_name, std::string sorted_name){
  std::cout << "Sorting file: " << file_name << std::endl;
  std::vector<std::string> file_lines;
  std::ifstream input(file_name.c_str());
  std::string line;
  while (std::getline(input,line)){
    file_lines.push_back(line);
  }
  input.close();
  std::sort(file_lines.begin(),file_lines.end(),greater());
  std::ofstream output(sorted_name.c_str());
  for(unsigned i=0; i<file_lines.size(); ++i){
    output << file_lines[i] << "\n";
  }
  output.close();
  std::cout << "Finished sorting. Name of sorted file: " << sorted_name << std::endl;
}

void postprocessOptimizeCategories(std::string dirname){
  std::string file_name = dirname+"/optimizeOutput.txt";
  bool file_check = file_exists(file_name);
  if(!file_check){
    std::cout << "Adding outputs in directory: " << dirname << std::endl;
    std::string system_command;
    system_command = "for f in "+ dirname + "/output_*; do cat \"$f\" >>" + dirname + "/optimizeOutput.txt ; done";
    //system_command = "cat " + dirname + "/output_* > " + dirname + "/optimizeOutput.txt";
    system(system_command.c_str());
    std::string sorted_name = dirname + "/optimizeOutputSorted.txt";
    SortFile(file_name, sorted_name);
    system_command="for f in "+ dirname + "/output_*; do rm \"$f\"; done"; 
    system(system_command.c_str());
    std::cout << "Finished adding outputs." << std::endl;
  }
}

void find_cuts(std::string line, std::string start_char, std::string end_char, unsigned start_char_num, unsigned end_char_num, std::vector<Cut> &cuts, unsigned n_colon, std::string &exclude){
  
  int f = 0; int l = 0;
  exclude = "";
  if(n_colon != 0){
    f = find_nth(line, 0, ": ", n_colon-1);
    l = line.length();
    line = line.substr(f + 2, l - f -2); 
  }
  if(n_colon == 0) f = find_nth(line, 0, " ", 5);
  else f = find_nth(line, 0, " ", 3);

  l = find_nth(line, 0, ":", 0);
  if(l<0) l = line.length();
  //std::cout << line << std::endl;
  std::string cut_string = line.substr(f + 1, l - f);
  //std::cout << "full string: " << cut_string << std::endl;
  f = find_nth(cut_string, 0, "!(", 0);
  if(f > -1){
    std::reverse(cut_string.begin(), cut_string.end());
    l = cut_string.length() - find_nth(cut_string, 0, ")", 0);
    std::reverse(cut_string.begin(), cut_string.end());
    exclude = cut_string.substr(f, l - f+1);
  }
  //std::cout << "exclude: " << exclude << std::endl;
  f = find_nth(cut_string, 0, start_char, start_char_num);
  l = find_nth(cut_string, 0, end_char, end_char_num);
  cut_string = cut_string.substr(f + start_char.length(), l - f - end_char.length());
  for(int i=cut_string.length()-1; i>=0; --i){
    if(isdigit(cut_string.at(i))){ break; }
    else { cut_string.erase(i, 1);}
  }
  
  unsigned count_space = 0;
  for(unsigned i=0; i<cut_string.length(); ++i){
    std::string temp = cut_string.substr(i, 1);
    if(temp == " ") count_space++;
  }
  
  for(unsigned i = 0; i<=count_space; ++i){
    std::string temp;  
    if(start_char == "" && i == 0){
      f = 0;
      l = find_nth(cut_string, 0, " ", i);
      temp = cut_string.substr(f, l - f);
    } else{
      f = find_nth(cut_string, 0, " ", i-1);
      l = find_nth(cut_string, 0, " ", i);
      temp = cut_string.substr(f + 1, l - f - 1);
    }
    if(temp == "&&" ) continue;
    //std::cout << temp << std::endl;
    Cut cut;
    f = 0;
    if(temp.find("==") != std::string::npos){  
      cut.type = 0;
      l = find_nth(temp, 0, "=", 0);
      cut.name = temp.substr(f, l - f);
      f = std::max(l,find_nth(temp, 0, "=", 1));
    }
    else if( temp.find("<") != std::string::npos){
      cut.type = 1;
      l = find_nth(temp, 0, "<", 0);
      cut.name = temp.substr(f, l - f);
      f = std::max(l,find_nth(temp, 0, "=", 0));
    }
    else if( temp.find(">") != std::string::npos){
      cut.type = 2;
      l = find_nth(temp, 0, ">", 0);
      cut.name = temp.substr(f, l - f);
      f = std::max(l,find_nth(temp, 0, "=", 0));
    }
    cut.value = std::stod(temp.substr(f+1, temp.length() - f));
    cut.value = temp.substr(f+1, temp.length() - f);
    cuts.push_back(cut);
  }
}

bool processArgs(int argc, char* argv[], ProgramOptions &opt){
  
  for (int i=1; i < argc; ++i) {
    std::string arg = argv[i];
    //if     (arg == "--isData") {opt.isData  = true;}
    if(arg == "--inputDirectory"){
      if(i+1<argc){i++; opt.dirname = argv[i];}

    }else if(arg == "--VBFCats"){
      if(i+1<argc){i++; opt.vbf_cat_num = atoi(argv[i]);}
    }
    else if(arg == "--OneJetCats"){
      if(i+1<argc){i++; opt.onejet_cat_num = atoi(argv[i]);}
    } else if(arg == "--ZeroJetCats"){
      if(i+1<argc){i++; opt.zerojet_cat_num = atoi(argv[i]);}
    } else if(arg == "--RecreateOutput") {opt.RecreateOutput  = true;}
    else if(arg == "--MakePlots") {opt.MakePlots  = true;}
    else if(arg == "--CutLine"){
      if(i+1<argc){i++; opt.CutLine = atoi(argv[i]);}
    } else if(arg == "--channel"){
      if(i+1<argc){i++; opt.channel = argv[i];}
    } else if(arg == "--MassVariable"){
      if(i+1<argc){i++; opt.variable = argv[i];}
    }
  }
  
  return 1;
}

int main(int argc, char* argv[]){
    
  ProgramOptions options;
  if(!processArgs(argc, argv, options)){return -1;}
  
  std::cout << "\n===== program options ====="                        << std::endl;
  std::cout << "inputDirectory         = " << options.dirname         << std::endl;
  std::cout << "VBFCats                = " << options.vbf_cat_num     << std::endl;
  std::cout << "OneJetCats             = " << options.onejet_cat_num  << std::endl;
  std::cout << "ZeroJetCats            = " << options.zerojet_cat_num << std::endl;
  std::cout << "RecreateOutput         = " << options.RecreateOutput  << std::endl;
  std::cout << "MakePlots              = " << options.MakePlots       << std::endl;
  std::cout << "CutLine                = " << options.CutLine         << std::endl;
  std::cout << "channel                = " << options.channel         << std::endl;
  std::cout << "MassVariable           = " << options.variable        << std::endl;
  std::cout << "===========================\n"                        << std::endl;
  
  std::string system_command;
  bool file_check;
  
  //////
  file_check = file_exists(options.dirname+"/combinedZeroJetSorted.txt");
  if(options.RecreateOutput || !file_check){
    if(options.zerojet_cat_num == 2){  
      std::cout << "Combining 0 jet categories.." << std::endl;
      
      postprocessOptimizeCategories(options.dirname + "/0Jet/2Cat/tight/");
      postprocessOptimizeCategories(options.dirname + "/0Jet/2Cat/loose/");
      
      std::string filename1 = options.dirname + "/0Jet/2Cat/loose/optimizeOutputSorted.txt";
      std::string filename2 = options.dirname + "/0Jet/2Cat/tight/optimizeOutputSorted.txt";
      
      std::string outfile_name = options.dirname + "/combined0Jet.txt";
      std::ofstream outfile(outfile_name.c_str());
      std::ifstream input1(filename1.c_str());
      std::string line1;
      while (std::getline(input1,line1)){
        std::string zerojet11 = line1.substr(line1.find("!(") + 2, line1.find(")") - line1.find("!(") - 2);
        int f = 0;
        int l = 0;
        l = find_nth(line1, 0, " ", 0);
        double ams1 = std::stod(line1.substr(f, l - f));
        f = find_nth(line1, 0, " ", 1);
        l = find_nth(line1, 0, " ", 2);
        std::string tauid1 = line1.substr(f + 1, l - f - 1);
        f = find_nth(line1, 0, " ", 3);
        l = find_nth(line1, 0, " ", 4);
        std::string mt1 = line1.substr(f + 1, l - f - 1);
        std::ifstream input2(filename2.c_str());
        std::string line2;
        while (std::getline(input2, line2)){
          int l; int f;
          f = find_nth(line2, 0, " ", 5);
          l = line2.find(")");  
          std::string zerojet2 = line2.substr(f + 1, l - f - 1);
          if(zerojet11 != zerojet2) continue;
          f = find_nth(line2, 0, " ", 1);
          l = find_nth(line2, 0, " ", 2);
          std::string tauid2 = line2.substr(f + 1, l - f - 1);
          if(tauid1 != tauid2) continue;
          f = find_nth(line2, 0, " ", 3);
          l = find_nth(line2, 0, " ", 4);
          std::string mt2 = line2.substr(f + 1, l - f - 1);
          if(mt1 != mt2) continue;
          f = 0;
          l = find_nth(line2, 0, " ", 0);
          double ams2 = std::stod(line2.substr(f, l - f));
          
          double ams_total = sqrt(ams1*ams1 + ams2*ams2);
          f = find_nth(line1, 0, " ", 1);
          l = line1.length();
          std::string output_string = line1.substr(f + 1, l - f - 1);
          std::string output_line = Form("%f && %s",ams_total,output_string.c_str());
          outfile << output_line << "\n";
        }
        input2.close();
      }
      
      input1.close();
      outfile.close();
      std::string sortedfile_name = options.dirname + "/combined0JetSorted.txt";
      SortFile(outfile_name, sortedfile_name);
      std::cout << "Finished combining 0Jet categories.." << std::endl;
    } else if(options.zerojet_cat_num == 1){
      if(!boost::filesystem::exists( "/0Jet/1Cat" )){
        system_command = "cp -R " + options.dirname + "/0Jet/2Cat/tight/ " + options.dirname + "/0Jet/1Cat";
        system(system_command.c_str());
      }
      postprocessOptimizeCategories(options.dirname + "/0Jet/1Cat");
      system_command = "cp " + options.dirname + "/0Jet/1Cat/optimizeOutput.txt " + options.dirname + "/combined0Jet.txt";
      system(system_command.c_str());
      system_command = "cp " + options.dirname + "/0Jet/1Cat/optimizeOutputSorted.txt " + options.dirname + "/combined0JetSorted.txt";
      system(system_command.c_str());
    }
  }
  
  
  ///////
  
  file_check = file_exists(options.dirname+"/combinedVBFSorted.txt");
  if(options.RecreateOutput || !file_check){
    if(options.vbf_cat_num == 2){  
      std::cout << "Combining VBF categories.." << std::endl;
      
      postprocessOptimizeCategories(options.dirname + "/2Jet/2Cat/tight/");
      postprocessOptimizeCategories(options.dirname + "/2Jet/2Cat/loose/");
      
      std::string filename1 = options.dirname + "/2Jet/2Cat/loose/optimizeOutputSorted.txt";
      std::string filename2 = options.dirname + "/2Jet/2Cat/tight/optimizeOutputSorted.txt";
      
      std::string outfile_name = options.dirname + "/combinedVBF.txt";
      std::ofstream outfile(outfile_name.c_str());
      std::ifstream input1(filename1.c_str());
      std::string line1;
      while (std::getline(input1,line1)){
        std::string vbf1 = line1.substr(line1.find("!(") + 2, line1.find(")") - line1.find("!(") - 2);
        int f = 0;
        int l = 0;
        l = find_nth(line1, 0, " ", 0);
        double ams1 = std::stod(line1.substr(f, l - f));
        f = find_nth(line1, 0, " ", 1);
        l = find_nth(line1, 0, " ", 2);
        std::string tauid1 = line1.substr(f + 1, l - f - 1);
        f = find_nth(line1, 0, " ", 3);
        l = find_nth(line1, 0, " ", 4);
        std::string mt1 = line1.substr(f + 1, l - f - 1);
        std::ifstream input2(filename2.c_str());
        std::string line2;
        while (std::getline(input2, line2)){
          int l; int f;
          f = find_nth(line2, 0, " ", 5);
          l = line2.find(")");  
          std::string vbf2 = line2.substr(f + 1, l - f - 1);
          if(vbf1 != vbf2) continue;
          f = find_nth(line2, 0, " ", 1);
          l = find_nth(line2, 0, " ", 2);
          std::string tauid2 = line2.substr(f + 1, l - f - 1);
          if(tauid1 != tauid2) continue;
          f = find_nth(line2, 0, " ", 3);
          l = find_nth(line2, 0, " ", 4);
          std::string mt2 = line2.substr(f + 1, l - f - 1);
          if(mt1 != mt2) continue;
          f = 0;
          l = find_nth(line2, 0, " ", 0);
          double ams2 = std::stod(line2.substr(f, l - f));
          
          double ams_total = sqrt(ams1*ams1 + ams2*ams2);
          f = find_nth(line1, 0, " ", 1);
          l = line1.length();
          std::string output_string = line1.substr(f + 1, l - f - 1);
          std::string output_line = Form("%f && %s",ams_total,output_string.c_str());
          outfile << output_line << "\n";
        }
        input2.close();
      }
      
      input1.close();
      outfile.close();
      std::string sortedfile_name = options.dirname + "/combinedVBFSorted.txt";
      SortFile(outfile_name, sortedfile_name);
      std::cout << "Finished combining VBF categories.." << std::endl;
    } else if(options.vbf_cat_num == 1){
      if(!boost::filesystem::exists( "/2Jet/1Cat" )){
        system_command = "cp -R " + options.dirname + "/2Jet/2Cat/tight/ " + options.dirname + "/2Jet/1Cat";
        system(system_command.c_str());
      }
      postprocessOptimizeCategories(options.dirname + "/2Jet/1Cat");
      system_command = "cp " + options.dirname + "/2Jet/1Cat/optimizeOutput.txt " + options.dirname + "/combinedVBF.txt";
      system(system_command.c_str());
      system_command = "cp " + options.dirname + "/2Jet/1Cat/optimizeOutputSorted.txt " + options.dirname + "/combinedVBFSorted.txt";
      system(system_command.c_str());
    }
  }
  
  file_check = file_exists(options.dirname+"/combined1JetSorted.txt");
  if(options.RecreateOutput || !file_check){
    if(options.onejet_cat_num == 3){ 
      std::cout << "Combining 1Jet categories.." << std::endl;
      postprocessOptimizeCategories(options.dirname + "/1Jet/2Cat/tight/");
      postprocessOptimizeCategories(options.dirname + "/1Jet/2Cat/loose/");
      file_check = file_exists(options.dirname + "/1Jet/3Cat/medium/optimizeOutput.txt");
      if (!file_check){
        system_command = "mkdir -p " + options.dirname + "/1Jet/3Cat";
        system(system_command.c_str());
        system_command = "cp -R " + options.dirname + "/1Jet/2Cat/loose " + options.dirname + "/1Jet/3Cat/.";
        system(system_command.c_str());
        system_command = "cp -R " + options.dirname + "/1Jet/2Cat/loose " + options.dirname + "/1Jet/3Cat/medium";
        system(system_command.c_str());
        system_command = "cp -R " + options.dirname + "/1Jet/2Cat/tight " + options.dirname + "/1Jet/3Cat/.";
        system(system_command.c_str());
      }
    
      std::string filename1 = options.dirname + "/1Jet/3Cat/loose/optimizeOutput.txt";
      std::string filename2 = options.dirname + "/1Jet/3Cat/medium/optimizeOutput.txt";
      std::string filename3 = options.dirname + "/1Jet/3Cat/tight/optimizeOutput.txt";
      
      std::string outfile_name = options.dirname + "/combined1Jet.txt";
      std::ofstream outfile(outfile_name.c_str());
      std::ifstream input1(filename1.c_str());
      std::string line1;
      while (std::getline(input1,line1)){
        int f; int l;  
        f = find_nth(line1, 0, "!(", 1);
        l = find_nth(line1, 0, ")", 1);
        std::string vbf1 = line1.substr(f + 2, l - f - 2);
        f = line1.find("!(");
        l = line1.find(")");
        std::string onejet1 = line1.substr(f + 2, l - f - 2);
        f = find_nth(line1, 0, " ", 1);
        l = find_nth(line1, 0, " ", 2);
        std::string tauid1 = line1.substr(f + 1, l - f - 1);
        f = find_nth(line1, 0, " ", 3);
        l = find_nth(line1, 0, " ", 4);
        std::string mt1 = line1.substr(f + 1, l - f - 1);
        f = 0;
        l = find_nth(line1, 0, " ", 0);
        double ams1 = std::stod(line1.substr(f, l - f));
        
        std::ifstream input2(filename2.c_str());
        std::string line2;

        while (std::getline(input2,line2)){
          int f; int l;  
          f = find_nth(line2, 0, "!(", 1);
          l = find_nth(line2, 0, ")", 1);
          std::string vbf2 = line2.substr(f + 2, l - f - 2);
          if(vbf1 != vbf2) continue;
          f = line2.find("!(");
          l = line2.find(")");
          std::string onejet2_tight = line2.substr(f + 2, l - f - 2);
          f = find_nth(line2, 0, " ", 5);
          l = find_nth(line2, 0, "!(", 0);
          std::string onejet2_loose = line2.substr(f + 1, l - f - 5);
          if(onejet1 != onejet2_loose) continue;
          f = find_nth(line2, 0, " ", 1);
          l = find_nth(line2, 0, " ", 2);
          std::string tauid2 = line2.substr(f + 1, l - f - 1);
          if(tauid1 != tauid2) continue;
          f = find_nth(line2, 0, " ", 3);
          l = find_nth(line2, 0, " ", 4);
          std::string mt2 = line2.substr(f + 1, l - f - 1);
          if(mt1 != mt2) continue;
          f = 0;
          l = find_nth(line2, 0, " ", 0);
          double ams2 = std::stod(line2.substr(f, l - f));
        
          std::ifstream input3(filename3.c_str());
          std::string line3;
          while (std::getline(input3,line3)){
            int f; int l;  
            f = line3.find("!(");
            l = line3.find(")");
            std::string vbf3 = line3.substr(f + 2, l - f - 2);
            if(vbf2 != vbf3) continue;
            f = find_nth(line3, 0, " ", 5);
            l = find_nth(line3, 0, "!(", 0);
            std::string onejet3 = line3.substr(f + 1, l - f - 5);
            if(onejet2_tight != onejet3) continue;
            f = find_nth(line3, 0, " ", 1);
            l = find_nth(line3, 0, " ", 2);
            std::string tauid3 = line3.substr(f + 1, l - f - 1);
            if(tauid2 != tauid3) continue;
            f = find_nth(line3, 0, " ", 3);
            l = find_nth(line3, 0, " ", 4);
            std::string mt3 = line3.substr(f + 1, l - f - 1);
            if(mt2 != mt3) continue;
            
            f = 0;
            l = find_nth(line3, 0, " ", 0);
            double ams3 = std::stod(line3.substr(f, l - f));
            double ams_total = sqrt(ams1*ams1 + ams2*ams2 + ams3*ams3);
            f = find_nth(line1, 0, " ", 1);
            l = line1.length();
            std::string output_string1 = line1.substr(f + 1, l - f - 1);
            f = find_nth(line2, 0, " ", 1);
            l = line2.length();
            std::string output_string2 = line2.substr(f + 1, l - f - 1);
            std::string output_line = Form("%f && %s : %s",ams_total,output_string1.c_str(),output_string2.c_str());
            outfile << output_line << "\n";
          }
          input3.close();
        }
        input2.close();
      }
      input1.close();
      outfile.close();
      std::string sortedfile_name = options.dirname + "/combined1JetSorted.txt";
      SortFile(outfile_name, sortedfile_name);
      std::cout << "Finished combining 1Jet categories.." << std::endl;
    } else if (options.onejet_cat_num == 2){
      std::cout << "Combining 1Jet categories.." << std::endl;
      postprocessOptimizeCategories(options.dirname + "/1Jet/2Cat/tight/");
      postprocessOptimizeCategories(options.dirname + "/1Jet/2Cat/loose/");
      std::string filename1 = options.dirname + "/1Jet/2Cat/loose/optimizeOutput.txt";
      std::string filename2 = options.dirname + "/1Jet/2Cat/tight/optimizeOutput.txt";
      
      std::string outfile_name = options.dirname + "/combined1Jet.txt";
      std::ofstream outfile(outfile_name.c_str());
      std::ifstream input1(filename1.c_str());
      std::string line1;
      while (std::getline(input1,line1)){
        int f; int l;  
        f = find_nth(line1, 0, "!(", 1);
        l = find_nth(line1, 0, ")", 1);
        std::string vbf1 = line1.substr(f + 2, l - f - 2);
        f = line1.find("!(");
        l = line1.find(")");
        std::string onejet1 = line1.substr(f + 2, l - f - 2);
        f = find_nth(line1, 0, " ", 1);
        l = find_nth(line1, 0, " ", 2);
        std::string tauid1 = line1.substr(f + 1, l - f - 1);
        f = find_nth(line1, 0, " ", 3);
        l = find_nth(line1, 0, " ", 4);
        std::string mt1 = line1.substr(f + 1, l - f - 1);
        f = 0;
        l = find_nth(line1, 0, " ", 0);
        double ams1 = std::stod(line1.substr(f, l - f));
        
        std::ifstream input2(filename2.c_str());
        std::string line2;
        while (std::getline(input2,line2)){
          int f; int l;  
          f = line2.find("!(");
          l = line2.find(")");
          std::string vbf2 = line2.substr(f + 2, l - f - 2);
          if(vbf1 != vbf2) continue;
          f = find_nth(line2, 0, " ", 5);
          l = find_nth(line2, 0, "!(", 0);
          std::string onejet2 = line2.substr(f + 1, l - f - 5);
          if(onejet1 != onejet2) continue;
          f = find_nth(line2, 0, " ", 1);
          l = find_nth(line2, 0, " ", 2);
          std::string tauid2 = line2.substr(f + 1, l - f - 1);
          if(tauid1 != tauid2) continue;
          f = find_nth(line2, 0, " ", 3);
          l = find_nth(line2, 0, " ", 4);
          std::string mt2 = line2.substr(f + 1, l - f - 1);
          if(mt1 != mt2) continue;
          
          f = 0;
          l = find_nth(line2, 0, " ", 0);
          double ams2 = std::stod(line2.substr(f, l - f));
          double ams_total = sqrt(ams1*ams1 + ams2*ams2);
          f = find_nth(line1, 0, " ", 1);
          l = line1.length();
          std::string output_string = line1.substr(f + 1, l - f - 1);
          std::string output_line = Form("%f && %s",ams_total,output_string.c_str());
          outfile << output_line << "\n";
        }
        input2.close();
      }
      input1.close();
      outfile.close();
      std::string sortedfile_name = options.dirname + "/combined1JetSorted.txt";
      SortFile(outfile_name, sortedfile_name);
      std::cout << "Finished combining 1Jet categories.." << std::endl;
    } else if (options.onejet_cat_num == 1){
      if(!boost::filesystem::exists( "/1Jet/1Cat" )){
        system_command = "cp -R " + options.dirname + "/1Jet/2Cat/tight/ " + options.dirname + "/1Jet/1Cat";
        system(system_command.c_str());
      }
      file_check = file_exists(options.dirname + "/1Jet/1Cat/optimizeOutput.txt");
      if (!file_check){
        postprocessOptimizeCategories(options.dirname + "/1Jet/1Cat");
      }    
      system_command = "cp " + options.dirname + "/1Jet/1Cat/optimizeOutput.txt " + options.dirname + "/combined1Jet.txt";
      system(system_command.c_str());
      system_command = "cp " + options.dirname + "/1Jet/1Cat/optimizeOutputSorted.txt " + options.dirname + "/combined1JetSorted.txt";
      system(system_command.c_str());
    }
  }
  
  file_check = file_exists(options.dirname+"/combined1JetPlusVBFSorted.txt");
  if(options.RecreateOutput || !file_check){
    std::cout << "Combining 1Jet and VBF categories.." << std::endl;
    std::string filename1 = options.dirname + "/combinedVBFSorted.txt";
    std::string filename2 = options.dirname + "/combined1JetSorted.txt";
    
    std::string outfile_name = options.dirname + "/combined1JetPlusVBF.txt";
    std::ofstream outfile(outfile_name.c_str());
    std::ifstream input1(filename1.c_str());
    std::string line1;
    while (std::getline(input1,line1)){
      int l; int f;
      std::string vbf1;
      if(options.vbf_cat_num == 2){
        l = line1.find("!(");
        f = find_nth(line1, 0, " ", 5);
        vbf1 = line1.substr(f + 1, l - f - 5);
      }
      else{
          l = line1.length();
          f = find_nth(line1, 0, " ", 5);
          vbf1 = line1.substr(f + 1, l - f - 1); //check that this should be l - f - 1!!
      }
      f = find_nth(line1, 0, " ", 1);
      l = find_nth(line1, 0, " ", 2);
      std::string tauid1 = line1.substr(f + 1, l - f - 1);
      f = find_nth(line1, 0, " ", 3);
      l = find_nth(line1, 0, " ", 4);
      std::string mt1 = line1.substr(f + 1, l - f - 1);
      f = 0;
      l = find_nth(line1, 0, " ", 0);
      double ams1 = std::stod(line1.substr(f, l - f));
      
      std::ifstream input2(filename2.c_str());
      std::string line2;
      while (std::getline(input2,line2)){
        int l; int f;
        if(options.onejet_cat_num>1){
          f = find_nth(line2, 0, "!(", 1);
          l = find_nth(line2, 0, ")", 1);
        }
        else{
          f = find_nth(line2, 0, "!(", 0);
          l = find_nth(line2, 0, ")", 0);   
        }
        std::string vbf2 = line2.substr(f + 2, l - f - 2);
        if(vbf1 != vbf2) continue;
        f = find_nth(line2, 0, " ", 1);
        l = find_nth(line2, 0, " ", 2);
        std::string tauid2 = line2.substr(f + 1, l - f - 1);
        if(tauid1 != tauid2) continue;
        f = find_nth(line2, 0, " ", 3);
        l = find_nth(line2, 0, " ", 4);
        std::string mt2 = line2.substr(f + 1, l - f - 1);
        if(mt1 != mt2) continue;
        f = 0;
        l = find_nth(line2, 0, " ", 0);
        double ams2 = std::stod(line2.substr(f, l - f));
        
        double ams_total = sqrt(ams1*ams1 + ams2*ams2);
        if(ams_total == ams_total && !std::isinf(ams_total)){
          f = find_nth(line1, 0, " ", 1);
          l = line1.length();
          std::string output_string1 = line1.substr(f + 1, l - f - 1);
          f = find_nth(line2, 0, " ", 1);
          l = line2.length();
          std::string output_string2 = line2.substr(f + 1, l - f - 1);
          std::string output_line = Form("%f && %s : %s",ams_total,output_string1.c_str(),output_string2.c_str());
          outfile << output_line << "\n";
        }
        
      }
      input2.close();
    }
    
    input1.close();
    outfile.close();
    std::string sortedfile_name = options.dirname + "/combined1JetPlusVBFSorted.txt";
    SortFile(outfile_name, sortedfile_name);
    std::cout << "Finished combining 1Jet and VBF categories.." << std::endl;
  }
  
  if(options.MakePlots){
    std::cout << "Making plots.." << std::endl;
    system_command = "mkdir -p " + options.dirname + "/Plots";
    std::string dir = Form("CutsCombo%u",options.CutLine);
    std::string filename = options.dirname + "/Plots/"+dir;
    system_command = "mkdir -p " + filename;
    system(system_command.c_str());
      
    std::vector<Category> categories;
    std::string mt_string;
    std::string mva_string;
    std::string qcd_string;
    std::string input_name = options.dirname + "/combined1JetPlusVBFSorted.txt";
    std::ifstream input1(input_name.c_str());
    std::string line1;
    unsigned line_count = 1;
    while (std::getline(input1,line1)){
      if(line_count != options.CutLine) continue;  
      system_command = "mkdir -p " + options.dirname + "/Plots";
      std::string dir = Form("CutsCombo%u",line_count);
      std::string filename = options.dirname + "/Plots/"+dir;
      system_command = "mkdir -p " + filename;
      system(system_command.c_str());
      std::ofstream outfile((filename+"/CutsLine.txt").c_str());
      outfile << line1;
      outfile.close();
      
      int f = 0; int l = 0;
      f = find_nth(line1, 0, " ", 1);
      l = find_nth(line1, 0, " ", 2);
      std::string mva = line1.substr(f + 1, l - f - 1);
      f = find_nth(line1, 0, " ", 3);
      l = find_nth(line1, 0, " ", 4);
      std::string mt = line1.substr(f + 1, l - f - 1);
      mt_string=mt;
      
      if(options.channel == "tt"){
        if (mva == "loose"){
          mva_string="--set_alias=\'baseline:(mva_olddm_loose_1>0.5 && mva_olddm_loose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)\'"; 
          qcd_string="--set_alias=\'tt_qcd_norm:(\"mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 && mva_olddm_loose_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)\'";
        } else if (mva == "medium"){
          mva_string="--set_alias=\'baseline:(mva_olddm_medium_1>0.5 && mva_olddm_medium_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)\'";
          qcd_string="--set_alias=\'tt_qcd_norm:(mva_olddm_loose_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_medium_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)\'";
        } else if (mva == "tight"){
          mva_string="--set_alias=\'baseline:(mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)\'";
          qcd_string ="--set_alias=\'tt_qcd_norm:(mva_olddm_medium_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_tight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)\'";
        } else if (mva == "vtight"){
          mva_string="--set_alias=\'baseline:(mva_olddm_vtight_1>0.5 && mva_olddm_vtight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)\'";
          qcd_string="--set_alias=\'tt_qcd_norm:(mva_olddm_tight_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_vtight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && leptonveto==0)\'";
        }
      }
      else{
        qcd_string = " ";
        if (mva == "loose"){
          mva_string="--set_alias=\'baseline:(iso_1<0.1 && mva_olddm_loose_2>0.5 && antiele_2 && antimu_2 && leptonveto==0)\'";
        } else if (mva == "medium"){
          mva_string="--set_alias=\'baseline:(iso_1<0.1 && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && leptonveto==0)\'";
        } else if (mva == "tight"){
          mva_string="--set_alias=\'baseline:(iso_1<0.1 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0)\'";
        } else if (mva == "vtight"){
          mva_string="--set_alias=\'baseline:(iso_1<0.1 && mva_olddm_vtight_2>0.5 && antiele_2 && antimu_2 && leptonveto==0)\'";
        }
      }
      
      std::vector<Cut> cuts;
      std::string exclude;
      
      if(options.vbf_cat_num == 1){
       Category cat;
       cat.name = "VBFInclusive";
       cuts.clear();
       find_cuts(line1, "", ":", 0, 0, cuts, 0, exclude);
       cat.cuts = cuts;
       cat.exclude = exclude;
       categories.push_back(cat);
      } else if(options.vbf_cat_num == 2){
        Category cat;
        cat.name = "VBFLoose";
        cuts.clear();
        find_cuts(line1, "", "!", 0, 0, cuts, 0, exclude);
        cat.cuts = cuts;
        cat.exclude = exclude;
        categories.push_back(cat);
        
        cat.name = "VBFTight";
        cuts.clear();
        find_cuts(line1, "!(", ")", 0, 0, cuts, 0, exclude);
        cat.cuts = cuts;
        cat.exclude = "";
        categories.push_back(cat);
      }
      
      if(options.onejet_cat_num == 1){
       Category cat;
       cat.name = "OneJetInclusive";
       cuts.clear();
       find_cuts(line1, "", "!", 0, 0, cuts, 1, exclude);
       cat.cuts = cuts;
       cat.exclude = exclude;
       categories.push_back(cat);
      } else if(options.onejet_cat_num == 2){
        Category cat;
        cat.name = "OneJetLoose";
        cuts.clear();
        find_cuts(line1, "", "!", 0, 0, cuts, 1, exclude);
        cat.cuts = cuts;
        cat.exclude = exclude;
        categories.push_back(cat);
        
        cat.name = "OneJetTight";
        cuts.clear();
        find_cuts(line1, "!(", ")", 0, 0, cuts, 1, exclude);
        cat.cuts = cuts;
        cat.exclude = "";
        categories.push_back(cat);
      } else if(options.onejet_cat_num == 3){
        Category cat;
        cat.name = "OneJetLoose";
        cuts.clear();
        find_cuts(line1, "", "!", 0, 0, cuts, 1, exclude);
        cat.cuts = cuts;
        cat.exclude = exclude;
        categories.push_back(cat);
        
        cat.name = "OneJetMedium";
        cuts.clear();
        find_cuts(line1, "", "!", 0, 0, cuts, 2, exclude);
        cat.cuts = cuts;
        cat.exclude = exclude;
        categories.push_back(cat);
        
        cat.name = "OneJetTight";
        cuts.clear();
        find_cuts(line1, "!(", ")", 0, 0, cuts, 2, exclude);
        cat.cuts = cuts;
        cat.exclude = "";
        categories.push_back(cat);
      }
      break;
    }
    input1.close();
    
    std::string method = "12";
    if(options.channel == "tt") method = "8";
    std::string back;
    if(options.channel == "et") back = "et_default";
    else if(options.channel == "mt") back = "mt_with_zmm";
    else if(options.channel == "tt") back = "tt_default";
    
    for(unsigned i=0; i<categories.size(); ++i){
      std::string output_dirname = filename + "/"+categories[i].name;
      system_command = "mkdir -p " + output_dirname;
      system(system_command.c_str());
      
      std::string alias_string_all;
      std::string cuts_type_string;
      if(categories[i].cuts[0].type == 0) cuts_type_string = "==";
      else if(categories[i].cuts[0].type == 1) cuts_type_string = "<=";
      else if(categories[i].cuts[0].type == 2) cuts_type_string = ">=";
      alias_string_all = categories[i].cuts[0].name + cuts_type_string + categories[i].cuts[0].value;
      for(unsigned k=1 ; k<categories[i].cuts.size(); ++k){
        if(categories[i].cuts[k].type == 0) cuts_type_string = "==";
        else if(categories[i].cuts[k].type == 1) cuts_type_string = "<=";
        else if(categories[i].cuts[k].type == 2) cuts_type_string = ">=";  
        alias_string_all += " && "+categories[i].cuts[k].name+cuts_type_string+categories[i].cuts[k].value;    
      }
      if(categories[i].exclude != "") alias_string_all += " && "+categories[i].exclude;
      
      std::string bin_string = options.variable+"\'[\'0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200\']\'";
      bool isVBF = categories[i].name.find("VBF") != std::string::npos;
      if (isVBF) {
        bin_string = options.variable+"\'[\'0,20,40,60,80,100,120,140,160,180,200\']\'";
      }
      
      system_command = "./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2016.cfg --channel="+options.channel+" --method="+method+" --var="+bin_string+
      " --cat=variable_cat --x_axis_label=\"M_{#tau#tau} [GeV]\" --blind=true  --x_blind_min=60 --x_blind_max=140 --set_alias=\'sel:("+mt_string+")\' "+mva_string+" "+qcd_string+" --set_alias=\'variable_cat:("+alias_string_all+")\' --background_scheme="+back;
      
      std::cout << system_command << std::endl;

      system(system_command.c_str());
      std::string plot_name = options.variable+"__"+options.channel+"_2016";
      system_command = "mv "+plot_name+"* "+output_dirname+"/.";
      system(system_command.c_str());
      
      for(unsigned j=0 ; j<categories[i].cuts.size(); ++j){
    
        std::string alias_string;
    
        //double scan_cut = 0;
        unsigned count_cuts=0;
        //unsigned scan_cut_type = <categories[i].cuts;
        std::string var = categories[i].cuts[j].name;
        if(var == "n_jets") continue;
        
        for(unsigned k=0 ; k<categories[i].cuts.size(); ++k){
          if(k!=j){
            if(categories[i].cuts[k].type == 0) cuts_type_string = "==";
            else if(categories[i].cuts[k].type == 1) cuts_type_string = "<=";
            else if(categories[i].cuts[k].type == 2) cuts_type_string = ">=";  
            if(count_cuts == 0) alias_string = categories[i].cuts[k].name + cuts_type_string + categories[i].cuts[k].value;
            else alias_string += " && "+categories[i].cuts[k].name+cuts_type_string+categories[i].cuts[k].value;
            count_cuts++;    
          }
        }
        if(categories[i].exclude != "") alias_string += " && "+categories[i].exclude;
        
        system_command = "./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2016.cfg --channel="+options.channel+" --method="+method+" --var="+bin_string+" --cat=variable_cat --x_axis_label=\"M_{#tau#tau} [GeV]\" --blind=true  --x_blind_min=60 --x_blind_max=140 --set_alias=\'sel:("+mt_string+")\' "+mva_string+" "+qcd_string+" --set_alias=\'variable_cat:("+alias_string+")\' --background_scheme="+back;
        
        system(system_command.c_str());
        std::string plot_name = options.variable+"__"+options.channel+"_2016";
        system_command = "mv "+plot_name+".pdf "+output_dirname+"/"+plot_name+"_No"+var+".pdf";
        system(system_command.c_str());
        system_command = "mv "+plot_name+".png "+output_dirname+"/"+plot_name+"_No"+var+".png";
        system(system_command.c_str());
        
        unsigned n_bins=0;
        double max_bin =0;
        
        bin_string = var+"\'[\'0";
        if(var == "jdeta"){
          n_bins = 10;
          max_bin = 10;
        } else if(var == "mjj"){
          n_bins = 12;
          max_bin = 1200;
        } else if(var == "pt_tt"){
          n_bins = 20;
          if (isVBF) n_bins = 10;
          max_bin = 200;
        } else if(var == "pt_2"){
          n_bins = 20;
          if (isVBF) n_bins = 10;
          max_bin = 100;
        } else if(var == "n_bjets"){
          n_bins = 5;
          max_bin = 5;
        } else if(var == "n_jetsingap"){
          n_bins = 5;
          max_bin = 5;
        } else if(var == "met"){
          n_bins = 20;
          max_bin = 100;
        }
        
        for(unsigned k=1; k<=n_bins; k++){
          double bin = k*max_bin/n_bins;
          std::ostringstream  temp;
          temp << bin;
          bin_string += ","+temp.str();
        }
        bin_string += "\']\'";
        
        std::string axis_label = "\""+var+" [GeV]\"";
        if(var == "jdeta" || var == "n_bjets" || var == "n_jetsingap") axis_label = "\""+var+"\"";
        std::ostringstream  max_bin_string;
        max_bin_string << max_bin;
        system_command = "./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2016.cfg --channel="+options.channel+" --method="+method+" --var="+bin_string+" --cat=variable_cat --x_axis_label="+axis_label+" --blind=true  --x_blind_min=0 --x_blind_max="+max_bin_string.str()+" --set_alias=\'sel:("+mt_string+")\' "+mva_string+" "+qcd_string+" --set_alias=\'variable_cat:("+alias_string+")\' --signal_scale=100 --signal_scheme=run2_sm --background_scheme="+back;
        std::cout << system_command << "\n" << std::endl;
//"./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2016.cfg --channel="+options.channel+" --set_alias=\"sel:("+mt_string+")\" "+qcd_string+"  --method="+method+" --cat="+variable_cat+" --var="pt_2(25,0,100)"   --x_axis_label="+axis_label+"  --background_scheme="+back; 
        
        system(system_command.c_str());
        plot_name = var+"__"+options.channel+"_2016";
        system_command = "mv "+plot_name+".pdf "+output_dirname+"/"+plot_name+"_No_"+var+".pdf";
        system(system_command.c_str());
        system_command = "mv "+plot_name+".png "+output_dirname+"/"+plot_name+"_No_"+var+".png";
        system(system_command.c_str());
      }
    }
    line_count++;
    std::cout << "Finished making Plots.." << std::endl;
  }
  
  return 0;
  
}

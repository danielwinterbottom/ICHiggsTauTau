//./bin/CombineCategories --inputDirectory mt_output/2016_09_14_18_20_16/ --OneJetCats 3 --RecreateOutput

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
#include "boost/filesystem.hpp" 

struct ProgramOptions{
  ProgramOptions(){
    dirname        = "";
    vbf_cat_num    = 2;
    onejet_cat_num = 2;
    RecreateOutput = false;
    MakePlots = false;
    NumberOfPlots = 1;
    channel = "mt";
  }

  std::string dirname;
  unsigned vbf_cat_num;
  unsigned onejet_cat_num;
  bool RecreateOutput;
  bool MakePlots;
  unsigned NumberOfPlots;
  std::string channel;
  
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
  
  std::string cut_string = line.substr(f + 1, l - f);
  f = find_nth(cut_string, 0, "!(", 0);
  if(f > -1){
    std::reverse(cut_string.begin(), cut_string.end());
    l = cut_string.length() - find_nth(cut_string, 0, ")", 0);
    std::reverse(cut_string.begin(), cut_string.end());
    exclude = cut_string.substr(f, l - f+1);
  }
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
  
  for(unsigned i = 0; i<count_space; ++i){
    std::string temp;  
    if(start_char == "" && i == 0){
      f = 0;
      l = find_nth(cut_string, 0, " ", i);
      temp = cut_string.substr(f, l - f);
    }
    else{
      f = find_nth(cut_string, 0, " ", i);
      l = find_nth(cut_string, 0, " ", 1 + i);
      temp = cut_string.substr(f + 1, l - f - 1);
    }
    if((temp.find("n_jets") != std::string::npos && temp.find("n_jetsingap") == std::string::npos) || temp == "&&" ) continue;
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
    //cut.value = std::stod(temp.substr(f+1, temp.length() - f));
    cut.value = temp.substr(f+1, temp.length() - f);
    cuts.push_back(cut);
  }
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
    system_command = "cat " + dirname + "/output_* > " + dirname + "/optimizeOutput.txt";
    system(system_command.c_str());
    std::string sorted_name = dirname + "/optimizeOutputSorted.txt";
    SortFile(file_name, sorted_name);
    system_command = "rm " + dirname + "/output_*"; 
    system(system_command.c_str());
    std::cout << "Finished adding outputs." << std::endl;
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
    } else if(arg == "--RecreateOutput") {opt.RecreateOutput  = true;}
    else if(arg == "--MakePlots") {opt.MakePlots  = true;}
    else if(arg == "--NumberOfPlots"){
      if(i+1<argc){i++; opt.NumberOfPlots = atoi(argv[i]);}
    } else if(arg == "--channel"){
      if(i+1<argc){i++; opt.channel = argv[i];}
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
  std::cout << "RecreateOutput         = " << options.RecreateOutput  << std::endl;
  std::cout << "MakePlots              = " << options.MakePlots       << std::endl;
  std::cout << "NumberOfPlots          = " << options.NumberOfPlots   << std::endl;
  std::cout << "channel                = " << options.channel         << std::endl;
  std::cout << "===========================\n"                        << std::endl;
  
  std::string system_command;
  bool file_check;
  
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
    std::string input_name = options.dirname + "/combined1JetPlusVBF.txt";
    std::ifstream input1(input_name.c_str());
    std::string line1;
    unsigned line_count = 1;
    while (std::getline(input1,line1) && line_count <= options.NumberOfPlots){
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
      std::string mva_string;
      std::string qcd_string;
      
      if(options.channel == "tt"){
        if (mva == "loose"){
          mva_string="--set_alias=baseline:(mva_olddm_loose_1>0.5 && mva_olddm_loose_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)"; 
          qcd_string="--set_alias=\"tt_qcd_norm:(\"mva_olddm_vloose_1>0.5 && mva_olddm_vloose_2>0.5 && mva_olddm_loose_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)";
        } else if (mva == "medium"){
          mva_string="--set_alias=baseline:(mva_olddm_medium_1>0.5 && mva_olddm_medium_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)";
          qcd_string="--set_alias=tt_qcd_norm:(mva_olddm_loose_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_medium_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)";
        } else if (mva == "tight"){
          mva_string="--set_alias=baseline:(mva_olddm_tight_1>0.5 && mva_olddm_tight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)";
          qcd_string ="--set_alias=tt_qcd_norm:(mva_olddm_medium_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_tight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)";
        } else if (mva == "vtight"){
          mva_string="--set_alias=baseline:(mva_olddm_vtight_1>0.5 && mva_olddm_vtight_2>0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)";
          qcd_string="--set_alias=tt_qcd_norm:(mva_olddm_tight_1>0.5 && mva_olddm_loose_2>0.5 &&mva_olddm_vtight_2<0.5 && antiele_1 && antimu_1 && antiele_2 && antimu_2 && !leptonveto)";
        }
      }
      else{
        qcd_string = " ";
        if (mva == "loose"){
          mva_string="--set_alias=baseline:(iso_1<0.1 && mva_olddm_loose_2>0.5 && antiele_2 && antimu_2 && !leptonveto)";
        } else if (mva == "medium"){
          mva_string="--set_alias=baseline:(iso_1<0.1 && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && !leptonveto)";
        } else if (mva == "tight"){
          mva_string="--set_alias=baseline:(iso_1<0.1 && mva_olddm_tight_2>0.5 && antiele_2 && antimu_2 && !leptonveto)";
        } else if (mva == "vtight"){
          mva_string="--set_alias=baseline:(iso_1<0.1 && mva_olddm_vtight_2>0.5 && antiele_2 && antimu_2 && !leptonveto)";
        }
      }
      std::string mt_string=mt;
      
      std::vector<Category> categories;
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
        cat.exclude = exclude;
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
        cat.exclude = exclude;
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
        find_cuts(line1, "!(", ")", 0, 0, cuts, 1, exclude);
        cat.cuts = cuts;
        cat.exclude = exclude;
        categories.push_back(cat);
        
        cat.name = "OneJetTight";
        cuts.clear();
        find_cuts(line1, "!(", ")", 0, 0, cuts, 2, exclude);
        cat.cuts = cuts;
        cat.exclude = exclude;
        categories.push_back(cat);
      }
      
      //std::cout << line1 << std::endl;
      //for(unsigned i=0; i< categories.size(); ++i){
      //  std::cout << "Category name: " << categories[i].name << std::endl;
      //  std::cout << "exclude: " << categories[i].exclude << std::endl;
      //  for(unsigned j =0 ; j<categories[i].cuts.size(); ++j){
      //    std::cout << "cut name: " << categories[i].cuts[j].name << std::endl;
      //    std::cout << "cut type: " << categories[i].cuts[j].type << std::endl;
      //    std::cout << "cut value: " << categories[i].cuts[j].value << std::endl;
      //  }
      //}
      for(unsigned i=0; i<categories.size(); ++i){
        std::string output_dirname = filename + "/"+categories[i].name;
        system_command = "mkdir -p " + output_dirname;
        system(system_command.c_str());
        
        for(unsigned j=0 ; j<categories[i].cuts.size(); ++j){
          std::string ams_output_name = output_dirname+"/ams_scan_"+categories[i].cuts[j].name+".txt";
          std::string alias_string;
          unsigned count_cuts=0;
          for(unsigned k=0 ; k<categories[i].cuts.size(); ++k){
            if(k!=j){
              std::string cuts_type_string;
              if(categories[i].cuts[j].type == 0) cuts_type_string = "==";
              else if(categories[i].cuts[j].type == 1) cuts_type_string = "<=";
              else if(categories[i].cuts[j].type == 2) cuts_type_string = ">=";
              if(count_cuts == 0) alias_string = categories[i].cuts[j].name + cuts_type_string + categories[i].cuts[j].value;
              //else alias_string += " && "+categories[i].cuts[j].name+cuts_type_string+categories[i].cuts[j].value;
              count_cuts++;    
            }
          }
          if(categories[i].exclude != "") alias_string += " && "+categories[i].exclude;
          
          system_command = "./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2016.cfg --channel="+options.channel+" --method=8 --var=\"m_vis\"[\"0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250.260,270,280,290,300\"] --cat=variable_cat --blind=true  --x_blind_min=10 --x_blind_max=300 --background_scheme=mt_default --set_alias=\"sel:("+mt_string+")\" "+mva_string+" "+qcd_string+" --set_alias=\"variable_cat:("+alias_string+")\" --supress_output=true --ams_scan_output_name "+ams_output_name;
          
         //system_command = "./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2016.cfg --channel="+options.channel+" --method=8 --var=\"m_vis\"[\"0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250.260,270,280,290,300\"] --cat=variable_cat --x_axis_label=\"M_{#tau#tau} [GeV]\" --blind=true --x_blind_min=30 --x_blind_max=200 --background_scheme=mt_default --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001 --set_alias=\"sel:(\""+mt_string+"\")\" ";
          
          //./bin/HiggsTauTauPlot5 --cfg=scripts/new_plot_sm_2016.cfg --channel=mt --method=8 --var=m_vis[0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240,250.260,270,280,290,300] --cat=variable_cat --x_axis_label=M_{#tau#tau} [GeV] --blind=true --x_blind_min=30 --x_blind_max=200 --background_scheme=mt_with_zmm --extra_pad=0.2 --draw_error_band=true --auto_error_band=0.00001 --set_alias=sel:(mt_1<50) --set_alias=baseline:(iso_1<0.1 && mva_olddm_medium_2>0.5 && antiele_2 && antimu_2 && !leptonveto)   --set_alias=variable_cat:(n_jets>=2 && n_jetsingap==0 && n_bjets==0 && jdeta>=3 && mjj>=400 && pt_tt>=0) --supress_output=true --sOverb_output_name=mt_output/2016_09_14_18_20_16/2Jet/2Cat/tight/outputTemp_medium_mt50_1.txt

          
          std::cout << system_command << "\n\n" <<std::endl;
          //system(system_command.c_str());
        }
      }
      line_count++;
    }
    input1.close();
  }
  
  return 0;
  
}

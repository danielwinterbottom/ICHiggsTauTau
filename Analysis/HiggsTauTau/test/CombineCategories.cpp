//./bin/CombineCategories --inputDirectory mt_output/2016_09_14_18_05_46 --OneJetCats 3 --RecreateOutput

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
    NumberOfPlots = 0;
  }

  std::string dirname;
  unsigned vbf_cat_num;
  unsigned onejet_cat_num;
  bool RecreateOutput;
  bool MakePlots;
  unsigned NumberOfPlots;
  
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
    //std::string c = a.substr(f, l - f - 1);
    l = find_nth(b, 0, " ", 0);
    double d = std::stod(b.substr(f, l - f));
    //std::string d = b.substr(f, l - f - 1);
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
      postprocessOptimizeCategories(options.dirname + "/2Jet/1Cat");
      system_command = "cp " + options.dirname + "/2Jet/1Cat/optimizeOutput.txt" + options.dirname + "/combinedVBF.txt";
      system(system_command.c_str());
      system_command = "cp " + options.dirname + "/2Jet/1Cat/optimizeOutputSorted.txt" + options.dirname + "/combinedVBFSorted.txt";
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
      postprocessOptimizeCategories(options.dirname + "/1Jet/1Cat");
      system_command = "cp " + options.dirname + "/1Jet/1Cat/optimizeOutput.txt" + options.dirname + "/combined1Jet.txt";
      system(system_command.c_str());
      system_command = "cp " + options.dirname + "/1Jet/1Cat/optimizeOutputSorted.txt" + options.dirname + "/combined1JetSorted.txt";
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
      f = find_nth(line1, 0, " ", 5);
      l = line1.find("!(");  
      std::string vbf1 = line1.substr(f + 1, l - f - 5);
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
        f = find_nth(line2, 0, "!(", 1);
        l = find_nth(line2, 0, ")", 1);
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
    unsigned line_count = 0;
    while (std::getline(input1,line1) && line_count <= options.NumberOfPlots){
      line_count++;    
    }
    input1.close();
  }
  
  return 0;
  
}

struct Acceptance{
  double A;
  double A_error;
};

std::pair<double,double> total_calc(std::vector<std::pair<double,double>> a){
  double total=0;
  double total_error=0;
  double sum=0;
  for(unsigned i=0; i<a.size(); ++i){
    double temp = std::fabs(a[i].first);
    double temp_error = std::fabs(a[i].second);
    sum += temp*temp*temp_error*temp_error;
    total = sqrt(total*total+temp*temp);
  }
  
  total_error = sqrt(sum)/total;
  
  return std::make_pair(total,total_error);
}

bool acompare(Acceptance a, Acceptance b) { return a.A < b.A; }

int UncertsMacro2(){

bool is2D = true;
    //bool is2D = false;
bool isGen = false;
//isGen=true;
std::vector<std::string> channels= {"em","et","mt","tt"};
std::vector<std::string> sig = {"gluglu", "vbf"};
std::vector<std::string> sample = { "", "_PythiaFragment_Down", "_PythiaFragment_Up", "_herwigpp" };

gStyle->SetPadBottomMargin(0.2);
gStyle->SetPadGridX(1);
gStyle->SetPadGridY(1);

struct uncert{
  std::string name;
  double value;
  double error;
};

//struct Acceptance{
//  double A;
//  double A_error;
//};


for(unsigned i=0; i< sig.size(); ++i){
    
  std::string outputfile_name = "table_uncerts_"+sig[i]+".txt";
  if(is2D) outputfile_name = "2d_"+outputfile_name;
  if(isGen){
    outputfile_name = "gen_table_uncerts_"+sig[i]+".txt";
    if(is2D) outputfile_name = "2d_"+outputfile_name;    
  }
  std::cout << outputfile_name << std::endl;  
  ofstream outfile (outputfile_name);  
  
  int count_cats = 0;
  for(unsigned j=0; j< channels.size(); ++j){
      
    std::vector<uncert> scale_vec;
    std::vector<uncert> alpha_s_vec;
    std::vector<uncert> pdf_vec;
    std::vector<uncert> parton_shower_vec;
    std::vector<uncert> mc_gen_vec;
    
    std::map<std::string, std::vector<Acceptance>> parton_shower_map;
    std::map<std::string, std::vector<Acceptance>> mc_gen_map;
    
    std::map<std::string, std::pair<double,double>> parton_shower_gen_map;
    std::map<std::string, std::pair<double,double>> parton_shower_tune_map;  
    std::map<std::string, std::pair<double,double>> mc_gen_error_map;
      
    std::string file_name = "theory_output_"+sig[i]+"_"+channels[j]+"/uncertainties__"+sig[i]+"_"+channels[j]+".txt";
    if(is2D) file_name = "theory_output_"+sig[i]+"_"+channels[j]+"/2d_uncertainties__"+sig[i]+"_"+channels[j]+".txt";
    ifstream infile (file_name);
    if(infile.is_open()){
      uncert scale; uncert alpha_s; uncert pdf; std::string cat; double dummy;
      infile.ignore(10000,'\n');
      while ( infile >> cat >> scale.value >> scale.error >> pdf.value >> pdf.error >> dummy >> alpha_s.value >> alpha_s.error){
        if(cat == "baseline" || cat == "inclusive" ) continue;
        std::string label = channels[j]+"_"+cat;
        scale.name = label; alpha_s.name = label; pdf.name = label;
        scale_vec.push_back(scale);
        alpha_s_vec.push_back(alpha_s);
        pdf_vec.push_back(pdf);
        count_cats++;
      }
      infile.close();
    }
    
    for(unsigned k=0; k<sample.size(); ++k){
      std::string file_name = "theory_output_"+sig[i]+"_partonshower/uncertainties_"+sig[i]+"_"+channels[j]+"_"+sample[k]+".txt";
      //std::cout << file_name << std::endl;
      if(is2D) file_name = "theory_output_"+sig[i]+"_partonshower/2d_uncertainties_"+sig[i]+"_"+channels[j]+"_"+sample[k]+".txt";
      
      if(isGen){
        file_name = "theory_output_"+sig[i]+"_partonshower/gen_uncertainties_"+sig[i]+"_"+channels[j]+"_"+sample[k]+".txt";
        if(is2D) file_name = "theory_output_"+sig[i]+"_partonshower/2d_gen_uncertainties_"+sig[i]+"_"+channels[j]+"_"+sample[k]+".txt";   
      }
      ifstream infile (file_name);
      if(infile.is_open()){
        infile.ignore(10000,'\n');
        std::string cat; double A; double A_error;
        while ( infile >> cat >> A >> A_error){
          std::string name = channels[j]+"_"+cat;
          Acceptance temp;
          temp.A = A; temp.A_error = A_error;
          parton_shower_map[name].push_back(temp);
        }
      }
      infile.close();
    }
    
    typedef std::map<std::string, std::vector<double>>::iterator it_type;
    typedef std::map<std::string, std::vector<Acceptance>>::iterator it_type2;
    for(it_type2 iterator = parton_shower_map.begin(); iterator != parton_shower_map.end(); iterator++){
      double Acceptance_default = iterator->second.front().A;
      double Acceptance_herwig = iterator->second.back().A;
      double def_error = iterator->second.front().A_error;
      double herwig_error = iterator->second.back().A_error;
      iterator->second.pop_back();
      std::sort(iterator->second.begin(), iterator->second.end(), acompare);
      double tune = (iterator->second.back().A-iterator->second.front().A)/(2*Acceptance_default);
      double tune_error = sqrt(pow(iterator->second.back().A_error,2) + pow(iterator->second.front().A_error,2))/((iterator->second.back().A-iterator->second.front().A))*tune;
      parton_shower_tune_map[iterator->first] = std::make_pair(tune,tune_error);
      
      double gen = (Acceptance_herwig-Acceptance_default)/Acceptance_default;
      double gen_error = sqrt(pow(def_error,2)+pow(herwig_error,2))/(Acceptance_herwig-Acceptance_default)*gen;
      parton_shower_gen_map[iterator->first] = std::make_pair(std::fabs(gen),gen_error);
      //= std::fabs(Acceptance_herwig-Acceptance_default)/Acceptance_default; 
    }
    
    std::vector<std::string> mc_filnames;
    std::string temp = "theory_output_"+sig[i]+"_partonshower/gen_uncertainties_"+sig[i]+"_"+channels[j]+"_.txt";
    if(is2D) temp = "theory_output_"+sig[i]+"_partonshower/2d_gen_uncertainties_"+sig[i]+"_"+channels[j]+"_.txt";    
    mc_filnames.push_back(temp);
    
    temp = "theory_output_"+sig[i]+"_partonshower/gen_uncertainties_"+sig[i]+"_"+channels[j]+"__amcatnlo.txt";
    if(is2D) temp = "theory_output_"+sig[i]+"_partonshower/2d_gen_uncertainties_"+sig[i]+"_"+channels[j]+"__amcatnlo.txt";
    mc_filnames.push_back(temp);
    for(unsigned k=0; k<mc_filnames.size(); ++k){
        ifstream infile (mc_filnames[k]);
        if(infile.is_open()){
          infile.ignore(10000,'\n');
          std::string cat; double A; double A_error;
          while ( infile >> cat >> A >> A_error){
            std::string name = channels[j]+"_"+cat;
            Acceptance temp;
            temp.A = A; temp.A_error = A_error;
            mc_gen_map[name].push_back(temp);
          }
        }
        infile.close();
    }
    
    for(it_type2 iterator = mc_gen_map.begin(); iterator != mc_gen_map.end(); iterator++){
      double Acceptance_default = iterator->second.front().A;
      double amc_Acceptance = iterator->second.back().A;
      double mc_error = (amc_Acceptance-Acceptance_default)/Acceptance_default;
      double mc_error_error = sqrt(pow(iterator->second.back().A_error,2) + pow(iterator->second.front().A_error,2))/Acceptance_default;
      //std::cout << iterator->first << "  " << mc_error*100 << "  " << std::endl;
      mc_gen_error_map[iterator->first] = std::make_pair(std::fabs(mc_error),mc_error_error);
      //=std::fabs(mc_error);
    
    }
    
    for(unsigned k=0; k < scale_vec.size(); ++k){
       if(scale_vec[k].name.find("inf")!=std::string::npos && channels[j]!="tt") continue;
        //scale, pdf, alpha s, parton showser, parton shower gen (GF vbf only), mc gen (GF only??)
       std::vector<std::pair<double,double>> total;
       total.push_back(std::make_pair(scale_vec[k].value, scale_vec[k].error));
       total.push_back(std::make_pair(pdf_vec[k].value, pdf_vec[k].error));
       total.push_back(std::make_pair(alpha_s_vec[k].value, alpha_s_vec[k].error));
       total.push_back(std::make_pair(parton_shower_tune_map[scale_vec[k].name].first, parton_shower_tune_map[scale_vec[k].name].second));
       //double total = sqrt(pow(scale_vec[k].value,2)+ pow(alpha_s_vec[k].value,2)); //+ pow(pdf_vec[k].value,2) + pow(parton_shower_tune_map[scale_vec[k].name].first,2));
       //double total_error = error_calc(scale_vec[k].value, scale_vec[k].error,alpha_s_vec[k].value,alpha_s_vec[k].error);
       //total_error = error_calc(total,total_error,pdf_vec[k].value,pdf_vec[k].error);
       //total = sqrt(pow(total,2) + pow(pdf_vec[k].value,2));
       //total_error = error_calc(total,total_error,parton_shower_tune_map[scale_vec[k].name].first,parton_shower_tune_map[scale_vec[k].name].second);
       //total = sqrt(pow(total,2) + pow(parton_shower_tune_map[scale_vec[k].name].first,2));
       std::string psg = " - ";
       std::string mcgen = " - ";
       if(sig[i]=="gluglu"){
           mcgen = Form("%.1f $\\pm$ %.1f",mc_gen_error_map[scale_vec[k].name].first*100, mc_gen_error_map[scale_vec[k].name].second*100);
           total.push_back(std::make_pair(mc_gen_error_map[scale_vec[k].name].first, mc_gen_error_map[scale_vec[k].name].second));
           //total_error = error_calc(total,total_error,mc_gen_error_map[scale_vec[k].name].first,mc_gen_error_map[scale_vec[k].name].second);
           //total = sqrt(pow(total,2)+pow(mc_gen_error_map[scale_vec[k].name].first,2));
       }
       if((scale_vec[k].name.find("vbf")!=std::string::npos && sig[i]=="gluglu") || true){
         psg = Form("%.1f $\\pm$ %.1f",parton_shower_gen_map[scale_vec[k].name].first*100,parton_shower_gen_map[scale_vec[k].name].second*100);
         total.push_back(std::make_pair(parton_shower_gen_map[scale_vec[k].name].first, parton_shower_gen_map[scale_vec[k].name].second));
         //total_error = error_calc(total,total_error,parton_shower_gen_map[scale_vec[k].name].first,parton_shower_gen_map[scale_vec[k].name].second);
         //total = sqrt(pow(total,2)+pow(parton_shower_gen_map[scale_vec[k].name].first,2));
       }
       std::string ps = Form("%.1f $\\pm$ %.1f",parton_shower_tune_map[scale_vec[k].name].first*100,parton_shower_tune_map[scale_vec[k].name].second*100);
       std::string name_string = scale_vec[k].name;
       int first_pos = name_string.find("_");
       name_string.replace(name_string.find("_"), 1, "\\_");
       if(name_string.find("_",first_pos+2)!=std::string::npos) name_string.replace(name_string.find("_",first_pos+2), 1, "\\_");
       
       std::pair<double,double> totals=total_calc(total);

       std::string output_line = Form("%s & %.1f $\\pm$ %.1f & %.1f $\\pm$ %.1f & %.1f $\\pm$ %.1f & %s & %s & %s & %.1f $\\pm$ %.1f \\\\",name_string.c_str(),scale_vec[k].value*100,scale_vec[k].error*100,pdf_vec[k].value*100, pdf_vec[k].error*100 , alpha_s_vec[k].value*100, alpha_s_vec[k].error*100, ps.c_str(),psg.c_str(),mcgen.c_str(),totals.first*100,totals.second*100);
       outfile << output_line << std::endl;
       std::cout << output_line << std::endl;
    }
    
  }
  outfile.close();
  
  
  
    //for(unsigned k=0; k<parton_shower_acceptance_samp_vec.size(); ++k){
    //  std::cout << parton_shower_acceptance_samp_vec[k].first << std::endl;
    //  for(unsigned k=0; k<parton_shower_acceptance_samp_vec[k].second.size(); ++k){
    //    std::cout << parton_shower_acceptance_samp_vec[j].second[k].name << "  " << parton_shower_acceptance_samp_vec[j].second[k].A << "  " << parton_shower_acceptance_samp_vec[j].second[k].A_error << std::endl;     
    //  }
    //}
  
}

return 0;
}

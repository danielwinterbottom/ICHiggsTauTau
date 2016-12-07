struct Acceptance{
  double A;
  double A_error;
};

int SignOfLargest(double a, double b){
  int sign = 1;
  if(std::fabs(a)>std::fabs(b)){
    sign = a/std::fabs(a);    
  } else {
    sign = b/std::fabs(b);  
  }
  return sign;  
}

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

int UncertsMacro(){

bool is2D = false;
    //bool is2D = false;
bool isGen = true;
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
    
    if(isGen){
      file_name = "theory_output_"+sig[i]+"_"+channels[j]+"/gen_uncertainties__"+sig[i]+"_"+channels[j]+".txt";
      if(is2D) file_name = "theory_output_"+sig[i]+"_"+channels[j]+"/2d_gen_uncertainties__"+sig[i]+"_"+channels[j]+".txt";    
    }
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
      double tune = (iterator->second[1].A - iterator->second[2].A)/(2*Acceptance_default); 
      //std::cout << tune << std::endl;
      std::sort(iterator->second.begin(), iterator->second.end(), acompare);
      //double tune = (iterator->second.back().A-iterator->second.front().A)/(2*Acceptance_default);
      double tune_error = sqrt( (pow(iterator->second.back().A_error,2) + pow(iterator->second.front().A_error,2))/pow(iterator->second.back().A-iterator->second.front().A,2) + pow(def_error/Acceptance_default,2))*tune;
      tune_error=std::fabs(tune_error);
      parton_shower_tune_map[iterator->first] = std::make_pair(tune,tune_error);
      
      double gen = (Acceptance_herwig-Acceptance_default)/Acceptance_default;
      double gen_error = sqrt((pow(def_error,2)+pow(herwig_error,2))/pow(Acceptance_herwig-Acceptance_default,2) + pow(def_error/Acceptance_default,2))*gen;
      //parton_shower_gen_map[iterator->first] = std::make_pair(std::fabs(gen),std::fabs(gen_error));
      parton_shower_gen_map[iterator->first] = std::make_pair(gen,std::fabs(gen_error));
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
      double def_error = iterator->second.front().A_error;
      double amc_Acceptance = iterator->second.back().A;
      double mc_error = (amc_Acceptance-Acceptance_default)/Acceptance_default;
      double mc_error_error = sqrt( (pow(iterator->second.back().A_error,2) + pow(iterator->second.front().A_error,2))/pow(amc_Acceptance-Acceptance_default,2)  + pow(def_error/Acceptance_default,2) )*mc_error;
      //mc_gen_error_map[iterator->first] = std::make_pair(std::fabs(mc_error),std::fabs(mc_error_error));
      mc_gen_error_map[iterator->first] = std::make_pair(mc_error,std::fabs(mc_error_error));
    
    }
    
    std::map<std::string, std::vector<Acceptance>> updown_gen_map;
    std::vector<std::string> updown_filnames;
    std::string temp2 = "theory_output_gluglu_partonshower/gen_uncertainties_gluglu_mt_.txt";
    //if(is2D) temp = "theory_output_"+sig[i]+"_partonshower/2d_gen_uncertainties_"+sig[i]+"_"+channels[j]+"_.txt";    
    updown_filnames.push_back(temp2);
    temp2 = "theory_output_gluglu_partonshower/up_gen_gen_uncertainties_gluglu_mt_.txt";
    //if(is2D) temp = "theory_output_"+sig[i]+"_partonshower/2d_gen_uncertainties_"+sig[i]+"_"+channels[j]+"_.txt";    
    updown_filnames.push_back(temp2);
    temp2 = "theory_output_gluglu_partonshower/down_gen_up_gen_gen_uncertainties_gluglu_mt_.txt";
    //if(is2D) temp = "theory_output_"+sig[i]+"_partonshower/2d_gen_uncertainties_"+sig[i]+"_"+channels[j]+"__amcatnlo.txt";
    updown_filnames.push_back(temp2);
    for(unsigned k=0; k<updown_filnames.size(); ++k){
        ifstream infile (updown_filnames[k]);
        if(infile.is_open()){
          infile.ignore(10000,'\n');
          std::string cat; double A; double A_error;
          while ( infile >> cat >> A >> A_error){
            std::string name = channels[j]+"_"+cat;
            Acceptance temp;
            temp.A = A; temp.A_error = A_error;
            updown_gen_map[name].push_back(temp);
          }
        }
        infile.close();
    }
    
    for(it_type2 iterator = updown_gen_map.begin(); iterator != updown_gen_map.end(); iterator++){
      double Acceptance_default = iterator->second[0].A;
      double def_error = iterator->second[0].A_error;
      double up_Acceptance = iterator->second[1].A;
      double down_Acceptance = iterator->second[2].A;
      double updown_error = (up_Acceptance-down_Acceptance)/(2*Acceptance_default)*100;
      double updown_error_error = sqrt( (pow(iterator->second[1].A_error,2) + (pow(iterator->second[2].A_error,2)))/pow(up_Acceptance-down_Acceptance,2)  + pow(def_error/Acceptance_default,2) )*updown_error;
      std::cout << iterator->first << ": " << updown_error << " +/- " << updown_error_error << std::endl;
      //mc_gen_error_map[iterator->first] = std::make_pair(mc_error,std::fabs(mc_error_error));
    
    }
    
    for(unsigned k=0; k < scale_vec.size(); ++k){
       if(scale_vec[k].name.find("inf")!=std::string::npos && channels[j]!="tt") continue;
        //scale, pdf, alpha s, parton showser, parton shower gen (GF vbf only), mc gen (GF only??)
       //std::vector<std::pair<double,double>> total;
       //std::vector<std::pair<double,double>> total_noscale;
       //total.push_back(std::make_pair(scale_vec[k].value, scale_vec[k].error));
       //total.push_back(std::make_pair(pdf_vec[k].value, pdf_vec[k].error));
       //total_noscale.push_back(std::make_pair(pdf_vec[k].value, pdf_vec[k].error));
       //total.push_back(std::make_pair(alpha_s_vec[k].value, alpha_s_vec[k].error));
       //total_noscale.push_back(std::make_pair(alpha_s_vec[k].value, alpha_s_vec[k].error));
       //total.push_back(std::make_pair(parton_shower_tune_map[scale_vec[k].name].first, parton_shower_tune_map[scale_vec[k].name].second));
       //total_noscale.push_back(std::make_pair(parton_shower_tune_map[scale_vec[k].name].first, parton_shower_tune_map[scale_vec[k].name].second));

       std::vector<std::pair<double,double>> pdf_total; 
       pdf_total.push_back(std::make_pair(pdf_vec[k].value, pdf_vec[k].error)); 
       pdf_total.push_back(std::make_pair(alpha_s_vec[k].value, alpha_s_vec[k].error));
       
       std::vector<std::pair<double,double>> ps_total;
       ps_total.push_back(std::make_pair(parton_shower_tune_map[scale_vec[k].name].first, parton_shower_tune_map[scale_vec[k].name].second));
       //std::vector<std::pair<double,double>> ps_total;
        
       std::string psg = " - ";
       std::string mcgen = " - ";
       if(sig[i]=="gluglu" || true){
           mcgen = Form("%.1f $\\pm$ %.1f",mc_gen_error_map[scale_vec[k].name].first*100, mc_gen_error_map[scale_vec[k].name].second*100);
           //total.push_back(std::make_pair(mc_gen_error_map[scale_vec[k].name].first, mc_gen_error_map[scale_vec[k].name].second));
           //total_noscale.push_back(std::make_pair(mc_gen_error_map[scale_vec[k].name].first, mc_gen_error_map[scale_vec[k].name].second));

       }
       if((scale_vec[k].name.find("vbf")!=std::string::npos && sig[i]=="gluglu") || true){
         psg = Form("%.1f $\\pm$ %.1f",parton_shower_gen_map[scale_vec[k].name].first*100,parton_shower_gen_map[scale_vec[k].name].second*100);
         //total.push_back(std::make_pair(parton_shower_gen_map[scale_vec[k].name].first, parton_shower_gen_map[scale_vec[k].name].second));
         //total_noscale.push_back(std::make_pair(parton_shower_gen_map[scale_vec[k].name].first, parton_shower_gen_map[scale_vec[k].name].second));
         ps_total.push_back(std::make_pair(parton_shower_gen_map[scale_vec[k].name].first, parton_shower_gen_map[scale_vec[k].name].second));

       }
       std::string ps = Form("%.1f $\\pm$ %.1f",parton_shower_tune_map[scale_vec[k].name].first*100,parton_shower_tune_map[scale_vec[k].name].second*100);
       std::string name_string = scale_vec[k].name;
       int first_pos = name_string.find("_");
       name_string.replace(name_string.find("_"), 1, "\\_");
       if(name_string.find("_",first_pos+2)!=std::string::npos) name_string.replace(name_string.find("_",first_pos+2), 1, "\\_");
       
       int pssign = SignOfLargest(parton_shower_gen_map[scale_vec[k].name].first,parton_shower_tune_map[scale_vec[k].name].first);
       std::pair<double,double> ps_totals=total_calc(ps_total);
       
       int pdfsign = SignOfLargest(pdf_vec[k].value,alpha_s_vec[k].value);
       std::pair<double,double> pdf_totals=total_calc(pdf_total);
       std::string output_line;  std::string output_line2;
       if(sig[i]=="gluglu" || true){
         output_line = Form("%s & %.1f $\\pm$ %.1f & %s & %.1f $\\pm$ %.1f & %.1f $\\pm$ %.1f & %.1f $\\pm$ %.1f & %s & %s  & %.1f $\\pm$ %.1f \\\\",name_string.c_str(),scale_vec[k].value*100,scale_vec[k].error*100,mcgen.c_str(),pdf_vec[k].value*100, pdf_vec[k].error*100 , alpha_s_vec[k].value*100, alpha_s_vec[k].error*100, pdfsign*pdf_totals.first*100,pdf_totals.second*100, ps.c_str(),psg.c_str(),pssign*ps_totals.first*100,ps_totals.second*100);
       }
       else{
         //output_line = Form("%s & %.1f $\\pm$ %.1f & %s & %.1f $\\pm$ %.1f & %.1f $\\pm$ %.1f & %.1f $\\pm$ %.1f & %s & %s  & %.1f $\\pm$ %.1f \\\\",name_string.c_str(),scale_vec[k].value*100,scale_vec[k].error*100,mcgen.c_str(),pdf_vec[k].value*100, pdf_vec[k].error*100 , alpha_s_vec[k].value*100, alpha_s_vec[k].error*100, pdf_total.first*100,pdf_total.second*100, ps.c_str(),psg.c_str(),ps_total.first*100,ps_total.second*100);
       }
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

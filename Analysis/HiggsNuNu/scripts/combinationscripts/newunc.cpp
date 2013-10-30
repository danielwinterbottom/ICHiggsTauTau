int newunc(string inputfile,string type,double outmass){

  if(type=="noeff"){//FOR NO EFFECT ERROR
    std::cout<<"newerror -"<<std::endl;
  }
  else if (type=="asym"){//FOR ASYM ERROR

    std::vector<double> *inmass = new std::vector<double>();
    std::vector<double> *indownerr = new std::vector<double>();
    std::vector<double> *inuperr = new std::vector<double>();
      
    //READ IN INPUT MASS POINT INFO
    std::ifstream inputinfofile( inputfile.c_str());  
    std::string line;    
    while(std::getline(inputinfofile,line)){
      double imass,idownerr,iuperr;
      string itype;
      std::istringstream iss(line);
      iss >> imass >> itype >> idownerr >> iuperr;
      inmass->push_back(imass);
      indownerr->push_back(idownerr);
      inuperr->push_back(iuperr);
    }
  
    //MAKE TGRAPHS
    TGraph* downerr = new TGraph(inmass->size(),&(inmass->at(0)),&(indownerr->at(0)));
    TGraph* uperr = new TGraph(inmass->size(),&(inmass->at(0)),&(inuperr->at(0)));
    
    //GET VALUES FOR NEW MASS POINTS
    double outdownerr = downerr->Eval(outmass);
    double outuperr = uperr->Eval(outmass);
    
    std::cout<<"newerror "<<outdownerr<<"/"<<outuperr<<std::endl;
  }
  else if (type=="sym"){//FOR SYM ERROR
    std::vector<double> *inmass = new std::vector<double>();
    std::vector<double> *inerr = new std::vector<double>();
      
    //READ IN INPUT MASS POINT INFO
    std::ifstream inputinfofile( inputfile.c_str());  
    std::string line;    
    while(std::getline(inputinfofile,line)){
      double imass,ierr;
      string itype;
      std::istringstream iss(line);
      iss >> imass >> itype >> ierr;
      inmass->push_back(imass);
      inerr->push_back(ierr);
    }
  
    //MAKE TGRAPHS
    TGraph* err = new TGraph(inmass->size(),&(inmass->at(0)),&(inerr->at(0)));
    
    //GET VALUES FOR NEW MASS POINTS
    double outerr = err->Eval(outmass);
    
    std::cout<<"newerror "<<outerr<<std::endl;
  }

}

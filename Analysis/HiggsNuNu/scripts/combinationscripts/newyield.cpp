{

  //GET XS AND YIELD INFO ABOUT INPUT MASS POINTS
  std::vector<double> *inmass = new std::vector<double>();
  std::vector<double> *inxs = new std::vector<double>();
  std::vector<double> *inyield = new std::vector<double>();
  std::vector<double> *inlumierr = new std::vector<double>();
  std::vector<double> *inscalejerrdown = new std::vector<double>();
  std::vector<double> *inscalejerrup = new std::vector<double>();
  std::vector<double> *inresjerrdown = new std::vector<double>();
  std::vector<double> *inresjerrup = new std::vector<double>();
  std::vector<double> *inscalemeterrdown = new std::vector<double>();
  std::vector<double> *inscalemeterrup = new std::vector<double>();
  std::vector<double> *inqqbarpdferr = new std::vector<double>();
  std::vector<double> *inqcdqqherr = new std::vector<double>();

  std::ifstream inputinfofile("inputinfo.txt");  
  std::string line;

  while(std::getline(inputinfofile,line)){
    double imass,ixs,iyield;

    std::istringstream iss(line);
    iss >> imass >> ixs >> iyield;
    inmass->push_back(imass);
    inxs->push_back(ixs);
    inyield->push_back(iyield);
  }

  //GET XS INFO ABOUT OUTPUT MASS POINTS
  std::ifstream outputxsfile("outputxsinfo.txt");

  std::vector<double> *outmass = new std::vector<double>();
  std::vector<double> *outxs = new std::vector<double>();
  
  while(std::getline(outputxsfile,line)){
    double imass,ixs;
    std::istringstream iss(line);
    iss >> imass >> ixs;
    outmass->push_back(imass);
    outxs->push_back(ixs);
  }

  //GET YIELD OVER XS
  std::vector<double> *inyieldoverxs = new std::vector<double>();
  for(int i=0;i<inmass->size();i++){
    inyieldoverxs->push_back(inyield->at(i)/inxs->at(i));
    //std::cout << inyield->at(i) <<" "<< inxs->at(i)<<" " << inyieldoverxs->at(i)<<std::endl;
  }

  //MAKE TGRAPHS
  TGraph* yieldoverxs = new TGraph(inmass->size(),&(inmass->at(0)),&(inyieldoverxs->at(0)));
    
  //GET VALUES FOR NEW MASS POINTS
  vector<double> *outyield= new vector<double>();
    
  for(int i=0;i<outmass->size();i++){
    outyield->push_back((yieldoverxs->Eval(outmass->at(i)))*(outxs->at(i)));
  }
  
  for(int i=0;i<outmass->size();i++){
    std::cout<<outmass->at(i)<<std::endl;
    std::cout<<"rate                "<<outyield->at(i)<<std::endl;
  }

  //MAKE PLOTS                                                                                                                                            
  // TCanvas c1;                                                                                                                                             
  // yieldoverxs->SetTitle("Yield/XS");                                                                                                                  
  // yieldoverxs->GetXaxis()->SetTitle("Mass/GeV");                                                                                                      
  // yieldoverxs->GetYaxis()->SetTitle("(Yield/XS)/pb^{-1}");                                                                                            
  // yieldoverxs->SetMarkerColor(4);                                                                                                                     
  // yieldoverxs->SetMarkerStyle(21);                                                                                                                    
  // yieldoverxs->Draw("ALP"); 
}

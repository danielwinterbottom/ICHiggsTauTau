#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"

#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/Utilities.h"


int main(int argc, char* argv[]){


  if (argc != 3) {
    std::cout << " Usage: " << argv[0] 
	      << " <number> <error>"
	      << std::endl;
    return 1;
  }

  double number = 0;
  std::istringstream(argv[1])>>number;
  double err = 0;
  std::istringstream(argv[2])>>err;

  ic::Utilities lTest = ic::Utilities(number,err,false);
  std::cout << "-- Number " << number << " +/- " << err 
	    << " rounded to " << lTest.roundedResult()
	    << std::endl;

  return 0;

}//main

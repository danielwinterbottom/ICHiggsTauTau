#ifndef ICHiggsTauTau_HiggsNuNu_Utilities_h
#define ICHiggsTauTau_HiggsNuNu_Utilities_h

#include <string>
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

namespace ic{

class Utilities {

public:
  Utilities(){};
  ~Utilities(){};

  Utilities(const double aNumber, const double aError, bool aRoundToInt=true);
  Utilities(const double aNumber, const double aErrorPlus, const double aErrorMinus, const bool aRoundToInt=true);

  void calculate();

  double findDecade(const double aErr);

  std::string round(const double aNumber);

  void alignStrings();
  void appendZeros(unsigned int nFinal, unsigned int nInit, std::string & aStr);
  unsigned int nDigitsAfterDot(std::string aStr);


  //std::string findDigit(const double aNumber);

  std::string roundedResult(const bool aPrintError=true);


  inline double rawNumber(){
    return number_;
  };

  inline double rawError(){
    return error_;
  };

  inline double rawErrorPlus(){
    return errorPlus_;
  };

  inline double rawErrorMinus(){
    return errorMinus_;
  };

  inline double roundedNumber(){
    double lTmp;
    std::istringstream(roundedNumber_)>>lTmp;
    return lTmp;
  };

  inline double roundedError(){
    double lTmp;
    std::istringstream(roundedError_)>>lTmp;
    return lTmp;
  };

  inline double roundedErrorPlus(){
    double lTmp;
    std::istringstream(roundedErrorPlus_)>>lTmp;
    return lTmp;
  };

  inline double roundedErrorMinus(){
    double lTmp;
    std::istringstream(roundedErrorMinus_)>>lTmp;
    return lTmp;
  };


private:
  double number_;
  double error_;
  double errorPlus_;
  double errorMinus_;

  double pos_;
  bool roundToInt_;

  //replace by std::vector<std::string> ?
  std::string roundedNumber_;
  std::string roundedError_;
  std::string roundedErrorPlus_;
  std::string roundedErrorMinus_;

  bool asymErrors_;

};

}//namespace

#endif//ICHiggsTauTau_HiggsNuNu_Utilities_h

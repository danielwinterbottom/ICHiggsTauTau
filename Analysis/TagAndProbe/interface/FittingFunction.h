#ifndef ICHiggsTauTau_Analysis_TagAndProbe_FittingFunction_h
#define ICHiggsTauTau_Analysis_TagAndProbe_FittingFunction_h
#include <string>


    int fit(std::string filename, std::string type, bool elec, bool isdata);
    int fitone(std::string filename, std::string type, bool elec, bool isdata, double low, double high, bool isbifurc, bool samemean);


#endif

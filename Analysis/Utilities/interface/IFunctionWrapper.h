

#ifndef UserCode_ICHiggsTauTau_Analysis_Utilities_IFunctionWrapper_h
#define UserCode_ICHiggsTauTau_Analysis_Utilities_IFunctionWrapper_h

#include <vector>
#include <string>

//#include "Rtypes.h"


class IFunctionWrapper
{
    public:
        IFunctionWrapper():m_name("") {};
        IFunctionWrapper(const std::string& name):m_name(name) {};
        virtual ~IFunctionWrapper();

        const std::string& name() const {return m_name;} 
        virtual double value(size_t, const double*) {return 0.;}
        virtual double value(const std::vector<double>&) {return 0.;}

    private:
        std::string m_name;

    //private:
        //ClassDef(IFunctionWrapper, 1); 
};




#endif

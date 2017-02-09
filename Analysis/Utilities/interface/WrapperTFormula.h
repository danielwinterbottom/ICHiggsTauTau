
#ifndef UserCode_ICHiggsTauTau_Analysis_Utilities_WrapperTFormula_h
#define UserCode_ICHiggsTauTau_Analysis_Utilities_WrapperTFormula_h

#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/IFunctionWrapper.h"

#include "TFormula.h"

class WrapperTFormula : public IFunctionWrapper
{

    public:
        WrapperTFormula():IFunctionWrapper() {};
        WrapperTFormula(const TFormula& f, const std::string& name):IFunctionWrapper(name),m_formula(f) {};
        virtual ~WrapperTFormula();

        double value(size_t size, const double* xs) override
        {   (void)size;
            return m_formula.EvalPar(xs);
        }
        double value(const std::vector<double>& xs) override
        {
            return value(xs.size(), xs.data());
        }

    private:
        TFormula m_formula;


    //private:
        //ClassDef(WrapperTFormula,1)
};


#endif

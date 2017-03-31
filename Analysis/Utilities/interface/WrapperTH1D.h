
#ifndef UserCode_ICHiggsTauTau_Analysis_Utilities_WrapperTH1D_h
#define UserCode_ICHiggsTauTau_Analysis_Utilities_WrapperTH1D_h

#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/IFunctionWrapper.h"

#include "TH1D.h"

class WrapperTH1D : public IFunctionWrapper
{

    public:
        WrapperTH1D():IFunctionWrapper() {};
        WrapperTH1D(const TH1D& h, const std::string& name):IFunctionWrapper(name),m_histo(h) {};
        virtual ~WrapperTH1D();

        double value(size_t size, const double* xs) override
        {
            if(size<1) return 0.;
            int bx = m_histo.GetXaxis()->FindBin(xs[0]);
            // Don't use overflow bins
            if(bx>m_histo.GetNbinsX()) bx = m_histo.GetNbinsX();
            else if(bx==0) bx = 1;
            return m_histo.GetBinContent(bx);
        }
        double value(const std::vector<double>& xs) override
        {
            return value(xs.size(), xs.data());
        }

    private:
        TH1D m_histo;


    //private:
        //ClassDef(WrapperTH1D,1)
};


#endif

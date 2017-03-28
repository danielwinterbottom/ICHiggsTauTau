
#ifndef UserCode_ICHiggsTauTau_Analysis_Utilities_WrapperTH2F_h
#define UserCode_ICHiggsTauTau_Analysis_Utilities_WrapperTH2F_h

#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/IFunctionWrapper.h"

#include "TH2F.h"

class WrapperTH2F : public IFunctionWrapper
{

    public:
        WrapperTH2F():IFunctionWrapper() {};
        WrapperTH2F(const TH2F& h, const std::string& name):IFunctionWrapper(name),m_histo(h) {};
        virtual ~WrapperTH2F();

        double value(size_t size, const double* xs) override
        {
            if(size<2) return 0.;
            int bx = m_histo.GetXaxis()->FindBin(xs[0]);
            int by = m_histo.GetYaxis()->FindBin(xs[1]);
            // Don't use overflow bins
            if(bx>m_histo.GetNbinsX()) bx = m_histo.GetNbinsX();
            else if(bx==0) bx = 1;
            if(by>m_histo.GetNbinsY()) by = m_histo.GetNbinsY();
            else if(by==0) by = 1;
            return m_histo.GetBinContent(bx,by);
        }
        double value(const std::vector<double>& xs) override
        {
            return value(xs.size(), xs.data());
        }

    private:
        TH2F m_histo;


    //private:
        //ClassDef(WrapperTH2F,1)
};


#endif

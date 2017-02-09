
#ifndef UserCode_ICHiggsTauTau_Analysis_Utilities_WrapperTH3D_h
#define UserCode_ICHiggsTauTau_Analysis_Utilities_WrapperTH3D_h

#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/IFunctionWrapper.h"

#include "TH3D.h"

class WrapperTH3D : public IFunctionWrapper
{

    public:
        WrapperTH3D():IFunctionWrapper() {};
        WrapperTH3D(const TH3D& h, const std::string& name):IFunctionWrapper(name),m_histo(h) {};
        virtual ~WrapperTH3D();

        double value(size_t size, const double* xs) override
        {
            if(size<3) return 0.;
            int bx = m_histo.GetXaxis()->FindBin(xs[0]);
            int by = m_histo.GetYaxis()->FindBin(xs[1]);
            int bz = m_histo.GetZaxis()->FindBin(xs[2]);
            // Don't use overflow bins
            if(bx>m_histo.GetNbinsX()) bx = m_histo.GetNbinsX();
            else if(bx==0) bx = 1;
            if(by>m_histo.GetNbinsY()) by = m_histo.GetNbinsY();
            else if(by==0) by = 1;
            if(bz>m_histo.GetNbinsZ()) bz = m_histo.GetNbinsZ();
            else if(bz==0) bz = 1;
            return m_histo.GetBinContent(bx,by,bz);
        }
        double value(const std::vector<double>& xs) override
        {
            return value(xs.size(), xs.data());
        }

    private:
        TH3D m_histo;


    //private:
        //ClassDef(WrapperTH3D,1)
};


#endif

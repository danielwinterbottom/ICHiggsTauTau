
#ifndef UserCode_ICHiggsTauTau_Analysis_Utilities_WrapperTGraph_h
#define UserCode_ICHiggsTauTau_Analysis_Utilities_WrapperTGraph_h

#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/IFunctionWrapper.h"

#include "TGraph.h"

class WrapperTGraph : public IFunctionWrapper
{

    public:
        WrapperTGraph():IFunctionWrapper(),m_min(0.),m_max(0.) {};
        WrapperTGraph(const TGraph& g, const std::string& name):IFunctionWrapper(name),m_graph(g) 
        {
            // store x min and max
            double miny, maxy; // temporary variables
            g.ComputeRange(m_min, miny, m_max, maxy);
        };
        virtual ~WrapperTGraph();

        double value(size_t size, const double* xs) override
        {
            double input = xs[0];
            // Avoid out-of-range extrapolation
            if(input>m_max) input = m_max; 
            else if(input<m_min) input = m_min;
            return (size>0 ? m_graph.Eval(xs[0]) : 0.);
        }
        double value(const std::vector<double>& xs) override
        {
            return value(xs.size(), xs.data());
        }

    private:
        TGraph m_graph;
        double m_min, m_max;


    //private:
        //ClassDef(WrapperTGraph,1)
};


#endif

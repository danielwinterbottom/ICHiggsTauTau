
#ifndef ICHiggsTauTau_Analysis_Utilities_FakeFactor_h
#define ICHiggsTauTau_Analysis_Utilities_FakeFactor_h

#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/IFunctionWrapper.h"

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <iostream>

#include "TObject.h"


class FakeFactor : public TObject 
{
    private:
        // some aliases
        using WrapperPtr = IFunctionWrapper*;
        //using WrapperItr = std::vector<WrapperPtr>::iterator;

    public:
        FakeFactor() 
        {
            // Initialize nominal empty tree
            m_nodes.insert( std::make_pair("", std::vector<size_t>()) );
            m_indices.insert( std::make_pair("", std::vector<std::vector<size_t>>()) );
            m_nodeInputs.insert( std::make_pair("", std::vector<std::vector<size_t>>()) );
        };
        FakeFactor(const std::vector<std::string>& inputs) 
        {
            // Store input variable names
            m_inputs = inputs;
            // Initialize nominal empty tree
            m_nodes.insert( std::make_pair("", std::vector<size_t>()) );
            m_indices.insert( std::make_pair("", std::vector<std::vector<size_t>>()) );
            m_nodeInputs.insert( std::make_pair("", std::vector<std::vector<size_t>>()) );
        };
        virtual ~FakeFactor();

        // Retrieving input list
        const std::vector<std::string>& inputs() {return m_inputs;}

        // Retrieving nodes
        const std::vector<WrapperPtr>& nodes() {return m_wrappers;}

        // Retrieving list of systematic sources
        std::vector<std::string> systematics()
        {
            std::vector<std::string> sys;
            for(const auto& sys_node : m_nodes) sys.push_back(sys_node.first);  
            return sys;
        }

        // Retrieving fake factors
        double value(size_t size, const double* xs, const std::string& sys="")
        {
            std::vector<double> vxs(xs, xs+size);
            return value(vxs, sys);
        }
        double value(const std::vector<double>& xs, const std::string& sys="")
        {
            auto sys_nodes = m_nodes.find(sys);
            if(sys_nodes==m_nodes.end())
            {
                std::cout<<"[FakeFactor] ERROR: Non registered systematic "<<sys<<"\n";
                return 1.;
            }
            auto sys_indices = m_indices.find(sys);
            auto sys_inputs = m_nodeInputs.find(sys);
            return value(xs, sys_nodes->second, sys_indices->second, sys_inputs->second, sys_nodes->second.size()-1); // evaluate the root
        }

        // Adding an input name
        void addInput(const std::string& input)
        {
            m_inputs.push_back(input);
        }

        // Manipulating trees
        bool addNode(WrapperPtr fct, 
                size_t sonsSize, const size_t* sons, 
                size_t varsSize, const size_t* vars, 
                const std::string& sys="")
        {
            std::vector<size_t> vsons(sons, sons+sonsSize);
            std::vector<size_t> vvars(vars, vars+varsSize);
            return addNode(fct, vsons, vvars, sys);
        }
        bool addNode(WrapperPtr,
                const std::vector<size_t>&,
                const std::vector<size_t>&,
                const std::string& sys="");

        void registerSystematic(const std::string& name)
        {
            // Initialize empty tree
            m_nodes.insert( std::make_pair(name, std::vector<size_t>()) );
            m_indices.insert( std::make_pair(name, std::vector<std::vector<size_t>>()) );
            m_nodeInputs.insert( std::make_pair(name, std::vector<std::vector<size_t>>()) );

        }

    private:
        double value(const std::vector<double>&,
                std::vector<size_t>&,
                std::vector<std::vector<size_t>>&,
                std::vector<std::vector<size_t>>&,
                size_t);

        std::vector<std::string> m_inputs;

        std::vector<WrapperPtr> m_wrappers; 

        std::map<std::string, std::vector<size_t>> m_nodes;
        std::map<std::string, std::vector<std::vector<size_t>>> m_indices;
        std::map<std::string, std::vector<std::vector<size_t>>> m_nodeInputs;


    private:
        ClassDef(FakeFactor,1)
};


#endif

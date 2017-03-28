
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FakeFactor.h"

#include <iostream>

ClassImp(FakeFactor)

/*****************************************************************/
FakeFactor::~FakeFactor()
/*****************************************************************/
{
    for(auto fct : m_wrappers)
    {
        delete fct;
    }
}


/*****************************************************************/
double FakeFactor::value(const std::vector<double>& xs,
        std::vector<size_t>& nodesVec,
        std::vector<std::vector<size_t>>& indicesVec,
        std::vector<std::vector<size_t>>& inputsVec,
        size_t index)
/*****************************************************************/
{
    // TODO: add xs size check
    const auto& indices = indicesVec[index];
    if(indices.size()>1) // Node
    {
        std::vector<double> values;
        values.reserve(indices.size());
        for(size_t i : indices) values.push_back( value(xs, nodesVec, indicesVec, inputsVec, i) );
        auto& node = m_wrappers.at(nodesVec[index]);
        return node->value(values);
    }
    else // Leaf
    {
        auto& leaf = m_wrappers.at(nodesVec[index]);
        const auto& inputs = inputsVec[index];
        std::vector<double> xssubset;
        xssubset.reserve(inputs.size());
        for(size_t i : inputs) xssubset.push_back( xs[i] );
        return leaf->value(xssubset);
    }
}


/*****************************************************************/
bool FakeFactor::addNode(WrapperPtr fct,
        const std::vector<size_t>& sons,
        const std::vector<size_t>& vars,
        const std::string& sys)
/*****************************************************************/
{
    if(!fct)
    {
        std::cout<<"[FakeFactor] ERROR: Trying to add a nullptr\n";
        return false;
    }
    auto sys_nodes = m_nodes.find(sys);
    if(sys_nodes==m_nodes.end())
    {
        std::cout<<"[FakeFactor] ERROR: Non registered systematic "<<sys<<"\n";
        return false;
    }
    auto sys_indices = m_indices.find(sys);
    auto sys_inputs = m_nodeInputs.find(sys);
    // require that the son indices already exist (avoid cycles)
    for(size_t i : sons) 
    {
        if(i>=sys_nodes->second.size())
        {
            std::cout<<"[FakeFactor] ERROR: Trying to add a node with non-existing sons\n";
            return false;
        }
    }
    // Search if fct has already been inserted
    size_t fctIndex = 0;
    bool found = false;
    for(size_t i=0; i<m_wrappers.size(); i++)
    {
        if(m_wrappers[i]->name()==fct->name())
        {
            fctIndex = i;
            found = true;
            break;
        }
    }
    if(!found) m_wrappers.push_back(fct);
    sys_nodes->second.push_back( found ? fctIndex : m_wrappers.size()-1 );
    sys_indices->second.push_back(sons);
    sys_inputs->second.push_back(vars);
    return true;
}




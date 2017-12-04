
#include "UserCode/ICHiggsTauTau/interface/MadGraphTools.h"


MadGraphTools::MadGraphTools(float mixingAngleOverPiHalf, std::string madgraphProcessDirectory, std::string madgraphParamCard, float alphaS,
bool madGraphSortingHeavyBQuark, bool mg5aMCv2p5OrOlder) :
m_madGraphSortingHeavyBQuark(madGraphSortingHeavyBQuark),
m_mg5aMCv2p5OrOlder(mg5aMCv2p5OrOlder)
{
// initialise interface to Python
if (! Py_IsInitialized())
{
Py_Initialize();
}
PyObject* pyModulePath = PyString_FromString("UserCode.ICHiggsTauTau.reweighting");
PyObject* pyModule = PyImport_Import(pyModulePath);
PyObject* pyModuleDict = PyModule_GetDict(pyModule);
PyObject* pyClass = PyDict_GetItemString(pyModuleDict, "MadGraphTools");
assert(pyClass != nullptr);

// get instance of Python class MadGraphTools
PyObject* pyMixingAngleOverPiHalf = PyFloat_FromDouble(mixingAngleOverPiHalf);
PyObject* pyMadgraphProcessDirectory = PyString_FromString(madgraphProcessDirectory.c_str());
PyObject* pyMadgraphParamCard = PyString_FromString(madgraphParamCard.c_str());
PyObject* pyAlphaS = PyFloat_FromDouble(alphaS);
PyObject* pyMG5aMCv2p5OrOlder = (m_mg5aMCv2p5OrOlder ? Py_True : Py_False);
PyObject* pyArguments = PyTuple_Pack(5, pyMixingAngleOverPiHalf, pyMadgraphProcessDirectory, pyMadgraphParamCard, pyAlphaS, pyMG5aMCv2p5OrOlder);
m_pyMadGraphTools = PyObject_CallObject(pyClass, pyArguments);
PyErr_Print();
assert((m_pyMadGraphTools != nullptr) && PyObject_IsInstance(m_pyMadGraphTools, pyClass));

// clean up; warning: http://stackoverflow.com/a/14678667
Py_DECREF(pyModulePath);
//Py_DECREF(pyModule);
//Py_DECREF(pyModuleDict);
//Py_DECREF(pyClass);
Py_DECREF(pyMixingAngleOverPiHalf);
Py_DECREF(pyMadgraphProcessDirectory);
Py_DECREF(pyMadgraphParamCard);
Py_DECREF(pyAlphaS);
Py_DECREF(pyArguments);
}

MadGraphTools::~MadGraphTools()
{
//Py_DECREF(m_pyMadGraphTools);

if (Py_IsInitialized())
{
Py_Finalize();
}
}

TDatabasePDG* MadGraphTools::GetDatabasePDG(std::string pdgDatabaseFilename)
{
TDatabasePDG* databasePDG = new TDatabasePDG();
boost::algorithm::replace_first(pdgDatabaseFilename, "$ROOTSYS", std::getenv("ROOTSYS"));
databasePDG->ReadPDGTable(pdgDatabaseFilename.c_str());
return databasePDG;
}

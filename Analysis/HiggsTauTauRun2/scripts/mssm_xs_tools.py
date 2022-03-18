from ctypes import cdll
from ctypes import c_bool
from ctypes import c_uint
from ctypes import c_double

class mssm_xs_tools(object):
    """
    Class mssm_xs_tools:
      
    This is a python wrapper class to make the core functionality of
    mssm_xs_tools available also in python. It needs the shared library
    mssm_xs_tools_C.so, which is obtained by compiling opening ROOT
    and typing ".L mssm_xs_tools.C++".
    
    """
    def __init__(self, filename, kINTERPOLATION, verbosity):
        ## pointer to the shared library containing the C wrapper functions
        self.lib = cdll.LoadLibrary('/vols/cms/gu18/CombineCMSSW/CMSSW_10_2_21/lib/slc7_amd64_gcc700/libCombineHarvesterMSSMvsSMRun2Legacy.so')
        ## pointer to the C++ object
        self.obj = self.lib.mssm_xs_tools_new(filename, c_bool(kINTERPOLATION), c_uint(verbosity))
        ## pointer to function mass
        self.mssm_xs_tools_mass = self.lib.mssm_xs_tools_mass
        self.mssm_xs_tools_mass.restype = c_double
        ## pointer to function width
        self.mssm_xs_tools_width = self.lib.mssm_xs_tools_width
        self.mssm_xs_tools_width.restype = c_double
        ## pointer to function coupling
        self.mssm_xs_tools_coupling = self.lib.mssm_xs_tools_coupling
        self.mssm_xs_tools_coupling.restype = c_double
        ## pointer to function br
        self.mssm_xs_tools_br = self.lib.mssm_xs_tools_br
        self.mssm_xs_tools_br.restype = c_double
        ## pointer to function xsec
        self.mssm_xs_tools_xsec = self.lib.mssm_xs_tools_xsec
        self.mssm_xs_tools_xsec.restype = c_double
         
    def mass(self, boson, mA, tanb):
        return self.mssm_xs_tools_mass(self.obj, boson, c_double(mA), c_double(tanb))

    def width(self, boson, mA, tanb):
        return self.mssm_xs_tools_width(self.obj, boson, c_double(mA), c_double(tanb))

    def coupling(self, boson, mA, tanb):
        return self.mssm_xs_tools_coupling(self.obj, boson, c_double(mA), c_double(tanb))

    def br(self, decay, mA, tanb):
        return self.mssm_xs_tools_br(self.obj, decay, c_double(mA), c_double(tanb))

    def xsec(self, mode, mA, tanb):
        return self.mssm_xs_tools_xsec(self.obj, mode, c_double(mA), c_double(tanb))

# and test the whole thing       
mssm = mssm_xs_tools("/vols/cms/gu18/CombineCMSSW/CMSSW_10_2_21/src/CombineHarvester/MSSMvsSMRun2Legacy/data/mh125_13.root", True, 0)
mA   = 2000
tanb =  10
print "m(H)         : ", mssm.mass ("H"                 , mA, tanb)
print "width(H)     : ", mssm.width("H"                 , mA, tanb)
print "br(H->tautau): ", mssm.br   ("H->tautau"         , mA, tanb)
print "br(H->hh)    : ", mssm.br   ("H->hh"             , mA, tanb)
print "---------------------------------------------------------------------------------"
print "m(A)         : ", mssm.mass ("A"                 , mA, tanb)
print "width(A)     : ", mssm.width("A"                 , mA, tanb)
print "gt(A)        : ", mssm.coupling("gt_A"           , mA, tanb)
print "gb(A)        : ", mssm.coupling("gb_A"           , mA, tanb)
print "br(A->tautau): ", mssm.br   ("A->tautau"         , mA, tanb)
print "br(A->Zh)    : ", mssm.br   ("A->Zh"             , mA, tanb)
print "---------------------------------------------------------------------------------"
print "Cross sections are in pb. The uncertainties are absolute uncertainties"
print "with respect to the central value."
print "For gluon fusion they are split into scale and PDF+alphas uncertainties."
print "xsec(gg->h          ):  ", mssm.xsec ("gg->h"                    , mA, tanb)
print "xsec(gg->h,scaleup  ):  ", mssm.xsec ("gg->h::scaleup"           , mA, tanb)
print "xsec(gg->h,scaledown):  ", mssm.xsec ("gg->h::scaledown"         , mA, tanb)
print "xsec(gg->h,pdfasup  ):  ", mssm.xsec ("gg->h::pdfasup"           , mA, tanb)
print "xsec(gg->h,pdfasdown):  ", mssm.xsec ("gg->h::pdfasdown"         , mA, tanb)
print "xsec(gg->H          ):  ", mssm.xsec ("gg->H"                    , mA, tanb)
print "xsec(gg->H,scaleup  ):  ", mssm.xsec ("gg->H::scaleup"           , mA, tanb)
print "xsec(gg->H,scaledown):  ", mssm.xsec ("gg->H::scaledown"         , mA, tanb)
print "xsec(gg->H,pdfasup  ):  ", mssm.xsec ("gg->H::pdfasup"           , mA, tanb)
print "xsec(gg->H,pdfasdown):  ", mssm.xsec ("gg->H::pdfasdown"         , mA, tanb)
print "xsec(bb->H          ):  ", mssm.xsec ("bb->H"                    , mA, tanb)
print "xsec(bb->H,up       ):  ", mssm.xsec ("bb->H::up"                , mA, tanb)
print "xsec(bb->H,down     ):  ", mssm.xsec ("bb->H::down"              , mA, tanb)
print "xsec(pp->Hp         ):  ", mssm.xsec ("pp->Hp"                   , mA, tanb)
print "xsec(pp->Hp,up      ):  ", mssm.xsec ("pp->Hp::up"               , mA, tanb)
print "xsec(pp->Hp,down    ):  ", mssm.xsec ("pp->Hp::down"             , mA, tanb)
#print "---------------------------------------------------------------------------------"


Physics Objects {#physics-objects}
===============

This page is the starting point for information on the physics object classes that are provided, and the CMSSW modules that are used to produce them.

If you want to check the list of properties each object class provides, follow the links below to the object documentation page. If you need to identify how a particular property is extracted or calculated from the corresponding CMSSW object, the easiest way is to go to the object page and click on the *set* method in question. This should contain a link to the source code of the CMSSW producer module where this method is called. Information on the python configuration of each module is found on the producer documentation page.

The ICHiggsTauTau package is designed to work out-of-the-box for a wide range of CMSSW releases. Specifically, it has been developed to meet the following goals:
 - All object classes and CMSSW plugins compile successfully with `scram` in each of the supported CMSSW release series (see the \ref mainpage for details)
 - The core set of CMSSW producer modules (i.e. those listed on this page) will run successfully in a default configuration, provided the relevant input collection is present in the processed dataset.
 - If a producer requires additional input to calculate certain variables, e.g. a Vertex to calculate impact parameters, the loading of these inputs should be optional.
 - Wherever possible, each producer should accept either a `vector<T>` or `edm::RefVector<T>` collection as input. The latter is one of the (many) smart-pointer containers in CMSSW, somewhat equivalent in concept to a vector of pointers, `vector<T *>`. This is achieved with the use of the `edm::View<T>` as the type of the `edm::Handle` used to extract these collections, as it provides a consistent interface into both types of the container. See the section below on filtering, merging and requests for more information on this design.

A default configuration for each module may be found in the file `python/default_producers_cfi.py`.

Because not all object properties are available in all releases (or at least not with the same code interface), the following caveats apply:

  - **The interface to access some property changes between CMSSW releases, or is not present at all in some subset of releases** \n The code to access this property can't be included in the producer, as it would fail to compile in some releases. The solution is to write a separate producer for just this property, and store it in an independent git repository that will be checked-out as a separate CMSSW package. This repository can then have separate branches for different versions of CMSSW. The separate producer will typically add an `edm::ValueMap<T>` to the event, where `T` is the type of the property in question. An option (off by default) may then be added to the main producer to read this map and extract the property values. There are several examples of these workarounds below. These follow a module naming convention of `IC<Object><Property>Calcuator`, a CMSSW package convention of `ICAnalysis/<Property>Calculator` and git repository convention of `ICAnalysis-<Property>Calculator`.
  - **A new method to calculate some property is introduced in a later CMSSW release, but the possibility to use the old method should be maintained.** \n If both the new and the old method are supported in all CMSSW versions then it is fine to just add an option to the main producer to switch between methods. Or, if the new method is only available in later releases, a better option is to create a separate producer in a new package as above. The old method should also be moved to a separate producer. However this may remain within the ICHiggsTauTau package as long as it still compiles in all the supported releases.


[TOC]

Candidate {#obs-candidate}
=========
 - **Definition**: ic::Candidate
 - **CMSSW Producer**: ICCandidateProducer

The ICCandidateProducer can take any collection as input, as long as the objects in that collection inherit from reco::Candidate.

Electron {#obs-electron}
=========
 - **Definition**: ic::Electron
 - **CMSSW Producer**: ICElectronProducer

There are several ic::Electron properties which are not calculated by default in the producer:

 - ic::Electron::r9 \n The method to calculate this variable both changes and fails to compile in some CMSSW releases. See the separate repository [ICAnalysis-ElectronR9Calculator](https://github.com/ajgilbert/ICAnalysis-ElectronR9Calculator) for details. This property will be extracted from the ValueMap specified by the option `inputR9` when the `includeR9` option is set to *True*.
 - ic::Electron::hcal_sum \n The extra inputs required to calculate this variable are not consistently available in the AOD for each CMSSW release series. In anticipation of the method to calculate this variable changing in the `7_X_Y` series this has been moved to a separate producer: ICElectronHcalDepthCalculator within the ICHiggsTauTau package. This property will be extracted from the ValueMap specified by the option `inputHcalSum` when the `includeHcalSum` option is set to *True*.
 - ic::Electron::has_matched_conversion \n In anticipation of the method to calculate this flag changing in future releases, the calculation has moved to a separate producer: ICElectronConversionCalculator within the ICHiggsTauTau package. This property will be extracted from the ValueMap specified by the option `inputConversionMatches` when the `includeConversionMatches` option is set to *True*.
 - ic::Electron::dxy_vertex and ic::Electron::dz_vertex \n These must be calculated with respect to a user-specified reco::Vertex. The first vertex in the collection specified by the `inputVertices` option will be used when the option `includeVertexIP` is set to *True*.
 - ic::Electron::dxy_beamspot \n This must be calculated with respect to a user-specified reco::BeamSpot. The beamspot set by the `inputBeamspot` option will be used when the option `includeBeamspotIP` is set to *True*.
 - **All particle-flow isolation values** \n These must be calculated on-the-fly in a CMSSW job, and may not be relevant for all analyses. The recipes by which these are calculated are strongly CMSSW-version dependent, and often require an update to the release version of various packages. Furthermore, it is common for individual analyses to modify the isolation definition, for example changing veto cones. Please see this page for some example recipes. Two sets of isolation values may be stored, nominally for isolation cones of sizes \f$\Delta R = 0.3\f$ and \f$\Delta R = 0.4\f$. These are enabled via the options `includePFIso03` and `includePFIso04` respectively. The InputTag for each isolation sum must be specified in the `pfIso03` and/or `pfIso04` PSets as relevant.

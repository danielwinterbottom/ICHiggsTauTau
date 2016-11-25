/*
 * HHPID.h
 *
 *  Created on: Jul 17, 2014
 */

#ifndef HHPID_H_
#define HHPID_H_

struct HHPID{
enum pid {
    // leptons, quarks
    e = 1,
    mu = 2,
    tau = 3,
    nu = 4,
    q = 5,
    c = 6,
    b = 7,
    top = 8,

    // SM-bosons
    gamma = 9,
    Z = 10,
    W = 11,
    gluon = 12,
    higgs = 13,

    // underlying event
    UE = 15,

    // MET
    MET = 16,

    // exotic particles
    H = 17,
    h1 = 18,
    h2 = 19,


    tauvis = 20,
    tauinvis = 21,

    undef
};
};

#endif /* HHPID_H_ */

/* 
 * File:   Circuit.h
 * Author: Christopher
 *
 * Created on December 12, 2013, 7:04 PM
 */

#ifndef CIRCUIT_H
#define	CIRCUIT_H

#include <set>
#include "setpSeg.h"

class Segment;

/*
 * Builds a circuit, given one segment in that circuit.
 * Extends the given segment by traversing the ON
 * transistors it is connected to, recursively, until
 * hitting ground and voltage supply. Provides iterators
 * for retrieving all the circuit's segments.
 */
class Circuit {
public:

    Circuit(Segment* extendFrom) {
        extend(extendFrom);
    }

    virtual ~Circuit() {
    }

    bool getValue();

    setpSeg::iterator begin() {
        return this->segs.begin();
    }

    setpSeg::iterator end() {
        return this->segs.end();
    }

private:

    Circuit(const Circuit&) = delete;
    Circuit& operator=(const Circuit&) = delete;

    void extend(Segment* extendFrom);

    setpSeg segs;
};

#endif	/* CIRCUIT_H */

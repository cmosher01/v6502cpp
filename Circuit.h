/* 
 * File:   Circuit.h
 * Author: Christopher
 *
 * Created on December 12, 2013, 7:04 PM
 */

#ifndef CIRCUIT_H
#define	CIRCUIT_H

#include <set>

class Segment;

class Circuit {
public:
    Circuit(Segment* extendFrom);

    virtual ~Circuit() {
    }

    bool getValue();
    std::set<Segment*>::iterator begin() { return this->segs.begin(); }
    std::set<Segment*>::iterator end() { return this->segs.end(); }

private:
    Circuit(const Circuit&);
    Circuit& operator=(const Circuit&);

    void extend(Segment* extendFrom);

    std::set<Segment*> segs;
};

#endif	/* CIRCUIT_H */


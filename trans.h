/* 
 * File:   trans.h
 * Author: cmosher
 *
 * Created on December 10, 2013, 2:37 PM
 */

#ifndef TRANS_H
#define	TRANS_H

#include <string>
#include <vector>
#include <iostream>

class Trans;
class SegmentCache;

class Segment {
public:
    std::string id;
    std::vector<Trans*> gates;
    std::vector<Trans*> c1c2s;

    bool pullup;
    bool pulldown;
    bool on;

    Segment(std::string& id) : id(id) {
    }
};

class Trans {
private:
    Segment* c1;
    Segment* gate;
    Segment* c2;

    bool on;

public:
    Trans(Segment* c1, Segment* gate, Segment* c2) : on(false), c1(c1), gate(gate), c2(c2) {
    }

    virtual ~Trans() {
    }

private:
    Trans(const Trans&);
    Trans& operator=(const Trans&);
};

#endif	/* TRANS_H */

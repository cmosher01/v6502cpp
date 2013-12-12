/* 
 * File:   trans.h
 * Author: cmosher
 *
 * Created on December 10, 2013, 2:37 PM
 */

#ifndef TRANS_H
#define	TRANS_H

#include <string>
#include <set>

class Trans;

class Segment {
public:
    std::string id;
    std::set<Trans*> gates;
    std::set<Trans*> c1c2s;

    bool pullup;
    bool pulldown;
    bool on;

    Segment(std::string& id) : id(id), on(false), pulldown(false), pullup(id[0] == '+') {
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
        c1->c1c2s.insert(this);
        gate->gates.insert(this);
        c2->c1c2s.insert(this);
    }

    virtual ~Trans() {
    }

private:
    Trans(const Trans&);
    Trans& operator=(const Trans&);
};

#endif	/* TRANS_H */

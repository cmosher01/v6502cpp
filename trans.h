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

    bool vss;
    bool vcc;

    Segment(const std::string& id) : id(id), on(false), pulldown(false), pullup(id[0] == '+'), vss(false), vcc(false) {
    }

    void set(bool up) {
        this->pullup = up;
        this->pulldown = !up;
    }

    static unsigned char asByte(Segment* b7, Segment* b6, Segment* b5, Segment* b4, Segment* b3, Segment* b2, Segment* b1, Segment* b0);
    static unsigned short asWord(Segment* b15, Segment* b14, Segment* b13, Segment* b12, Segment* b11, Segment* b10, Segment* b9, Segment* b8, Segment* b7, Segment* b6, Segment* b5, Segment* b4, Segment* b3, Segment* b2, Segment* b1, Segment* b0);
};

class Trans {
public:
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

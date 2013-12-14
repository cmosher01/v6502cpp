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
#include "ptr_less.h"

class Trans;

class Segment {
public:
    std::string id;
    std::set<Trans*> gates;
    std::set<Trans*> c1c2s;

    bool on;
    bool pulldown;
    bool pullup;

    bool vss;
    bool vcc;

    Segment(const std::string& id) : id(id), on(false), pulldown(false), pullup(id[0] == '+'), vss(false), vcc(false) {
    }

    void set(bool up) {
        this->pullup = up;
        this->pulldown = !up;
    }

    static unsigned char asByte(Segment* b7, Segment* b6, Segment* b5, Segment* b4, Segment* b3, Segment* b2, Segment* b1, Segment* b0) {
        return b7->on << 0x7 | b6->on << 0x6 | b5->on << 0x5 | b4->on << 0x4 | b3->on << 0x3 | b2->on << 0x2 | b1->on << 0x1 | b0->on;
    }

    static unsigned short asWord(Segment* bf, Segment* be, Segment* bd, Segment* bc, Segment* bb, Segment* ba, Segment* b9, Segment* b8, Segment* b7, Segment* b6, Segment* b5, Segment* b4, Segment* b3, Segment* b2, Segment* b1, Segment* b0) {
        return bf->on << 0xf | be->on << 0xe | bd->on << 0xd | bc->on << 0xc | bb->on << 0xb | ba->on << 0xa | b9->on << 0x9 | b8->on << 0x8 | b7->on << 0x7 | b6->on << 0x6 | b5->on << 0x5 | b4->on << 0x4 | b3->on << 0x3 | b2->on << 0x2 | b1->on << 0x1 | b0->on;
    }

    bool operator<(const Segment& that) { return this->id < that.id; }
};

typedef std::set<Segment*,ptr_less<Segment>> setpSeg;

class Trans {
public:
    Segment* c1;
    Segment* gate;
    Segment* c2;

    bool on;

public:

    Trans(Segment* c1, Segment* gate, Segment* c2) : c1(c1), gate(gate), c2(c2), on(false) {
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

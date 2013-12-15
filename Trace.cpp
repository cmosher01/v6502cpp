/* 
 * File:   Trace.cpp
 * Author: cmosher
 * 
 * Created on December 12, 2013, 3:39 PM
 */

#include "Trace.h"
#include "SegmentCache.h"
#include "Common.h"
#include <iostream>
#include <iomanip>
#include <cassert>

static void pHex(const unsigned long x, const int width) {
    std::cout << std::setw(width) << std::setfill('0') << std::hex << x << std::dec;
}

static void pHexb(const unsigned char x) {
    pHex(x,2);
}

static void pHexw(const unsigned short x) {
    pHex(x,4);
}

void Trace::dumpSegments() const {
    for (auto sp : this->s) {
        Segment* seg = sp.second.get();
        assert(!(seg->pullup && seg->pulldown));
        if (seg->pullup) {
            std::cout << (seg->on ? "U" : "u");
        } else if (seg->pulldown) {
            std::cout << (seg->on ? "D" : "d");
        } else {
            std::cout << (seg->on ? "F" : "f");
        }
    }
    std::cout << std::endl;
}

void Trace::dumpRegisters() const {
    std::cout << "A";
    pHexb(s.rA());
    std::cout << " X";
    pHexb(s.rX());
    std::cout << " Y";
    pHexb(s.rY());
    std::cout << " ";
    std::cout << (s.c->P7->on ? "N" : "n");
    std::cout << (s.c->P6->on ? "V" : "v");
    std::cout << ".";
    std::cout << (s.c->P4->on ? "B" : "b");
    std::cout << (s.c->P3->on ? "D" : "d");
    std::cout << (s.c->P2->on ? "I" : "i");
    std::cout << (s.c->P1->on ? "Z" : "z");
    std::cout << (s.c->P0->on ? "C" : "c");
    std::cout << " S";
    pHexb(s.rS());
    std::cout << " PC";
    pHexw(s.rPC());
    if (s.c->CLK1OUT->on) {
        std::cout << "  PH1  ";
    }
    if (s.c->CLK2OUT->on) {
        std::cout << "  PH2";
        if (s.c->RW->on) {
            std::cout << " R";
        } else {
            std::cout << " W";
        }
    }
    if (!(s.c->CLK1OUT->on || s.c->CLK2OUT->on)) {
        std::cout << "  PH-  ";
    }
    std::cout << " DB";
    pHexb(s.rData());
    std::cout << " AB";
    pHexw(s.rAddr());
    std::cout << std::endl;
}

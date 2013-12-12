/* 
 * File:   Trace.cpp
 * Author: cmosher
 * 
 * Created on December 12, 2013, 3:39 PM
 */

#include "Trace.h"
#include "SegmentCache.h"
#include <iostream>
#include <iomanip>

static void pHex(const unsigned char x) {
    std::cout << std::setw(2) << std::setfill('0') << std::hex << (unsigned long) x << std::dec;
}

static void pHexw(const unsigned short x) {
    std::cout << std::setw(4) << std::setfill('0') << std::hex << (unsigned long) x << std::dec;
}

void Trace::dumpSegments() {
//    for (int i = 0; i < segs.size(); ++i) {
//        seg& s = segs[i];
//        if (s.pullup) {
//            std::cout << "U";
//        } else if (s.pulldown) {
//            std::cout << "D";
//        } else {
//            std::cout << "f";
//        }
//        if (s->on) {
//            std::cout << "+";
//        } else {
//            std::cout << "-";
//        }
//    }
//    std::cout << std::endl;
}

void Trace::dumpRegisters() {
    std::cout << "A";
    pHex(s.rA());
    std::cout << " X";
    pHex(s.rX());
    std::cout << " Y";
    pHex(s.rY());
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
    pHex(s.rS());
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
    pHex(s.rData());
    std::cout << " AB";
    pHexw(s.rAddr());
    std::cout << std::endl;
}

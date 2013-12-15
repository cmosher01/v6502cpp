/* 
 * File:   Cpu6502.cpp
 * Author: Christopher
 * 
 * Created on December 12, 2013, 10:14 PM
 */

#include "Cpu6502.h"
#include "TransNetwork.h"
#include "trans.h"
#include "addressbus.h"
#include "Trace.h"
#include <iostream>
#include <utility>
#include "StateCalculator.h"
#include "Common.h"



#define TRACEREG 1
//#define TRACESEG 1



Cpu6502::Cpu6502(TransNetwork& transNetwork, AddressBus& addressBus, Trace& trace) : transNetwork(transNetwork), addressBus(addressBus), trace(trace), segs(transNetwork.segs), n(segs.c) {
}

void Cpu6502::setPins(const PinSettings& ps) {
    setpSeg rec;
    for (auto p : ps) {
        p.first->set(p.second);
        rec.insert(p.first);
    }
    StateCalculator::recalc(rec);
}

void Cpu6502::clock(bool phase) {
    setPins(PinSettings{std::make_pair(n->CLK0,phase)});
    rw();

#ifdef TRACEREG
    this->trace.dumpRegisters();
#endif

#ifdef TRACESEG
    this->trace.dumpSegments();
#endif
}

void Cpu6502::rw() {
    // database read/write happens during Clock Phase 2 (only)
    if (n->CLK2OUT->on) {
        readData();
        writeData();
    }
}

void Cpu6502::readData() {
    if (this->transNetwork.segs.c->RW->on) {
        this->transNetwork.segs.setDataSegs(this->addressBus.read(this->transNetwork.segs.rAddr()));

        //???? TODO ???? can this be inside the if block ????
        setpSeg s;
        segs.addDataToRecalc(s);
        StateCalculator::recalc(s);
    }
}

void Cpu6502::writeData() {
    if (!this->transNetwork.segs.c->RW->on) {
        this->addressBus.write(this->transNetwork.segs.rAddr(), this->transNetwork.segs.rData());
    }
}

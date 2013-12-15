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



Cpu6502::Cpu6502(AddressBus& addressBus, Trace& trace, Common& common) : addressBus(addressBus), trace(trace), common(common) {
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
    setPins(PinSettings{std::make_pair(common.CLK0,phase)});
    rw();

#ifdef TRACEREG
    this->trace.dumpRegisters();
#endif

#ifdef TRACESEG
    this->trace.dumpSegments();
#endif
}

void Cpu6502::rw() {
    /* database read/write happens (only) during Clock Phase 2 */
    if (common.CLK2OUT->on) {
        readData();
        writeData();
    }
}

void Cpu6502::readData() {
    if (this->common.RW->on) {
        this->common.setDataSegs(this->addressBus.read(this->common.rAddr()));

        setpSeg s;
        this->common.addDataToRecalc(s);
        StateCalculator::recalc(s);
    }
}

void Cpu6502::writeData() {
    if (!this->common.RW->on) {
        this->addressBus.write(this->common.rAddr(), this->common.rData());
    }
}

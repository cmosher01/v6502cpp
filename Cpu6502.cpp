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


#include "StateCalculator.h"




#define TRACEREG 1
//#define TRACESEG 1
//#define TRACEMEM 1

#ifdef TRACEREG
#define dumpRegs() this->trace.dumpRegisters()
#else
#define dumpRegs()
#endif

#ifdef TRACESEG
#define dumpSegs() dumpSegments()
#else
#define dumpSegs()
#endif






void Cpu6502::powerOn() {
    std::cout << "initial state" << std::endl;
    dumpRegs();
    dumpSegs();



    /*
     * Since we use segs[CLK0].on as our own
     * temporary variable (see "step" method), we
     * need to initialize it here, to "phase one".
     */
    n->CLK0->on = true;



    std::cout << "setting input pins..." << std::endl;
    initPins();

    std::cout << "initial full calculation..." << std::endl;
    recalc(segs.all());
    dumpRegs();
    dumpSegs();
}

void Cpu6502::setSeg(Segment* s, bool on) {
    s->set(on);
}

void Cpu6502::recalc(Segment* s) {
    StateCalculator::recalc(s,n->VSS,n->VCC);
}

void Cpu6502::recalc(std::set<Segment*> s) {
    StateCalculator::recalc(s,n->VSS,n->VCC);
}

void Cpu6502::initPins() {
    // set voltage supply and ground.
    setSeg(n->VCC, true);
    setSeg(n->VSS, false);

    // don't do the set-overflow overriding functionality
    setSeg(n->SO, false);

    // ready to run (i.e., do not do single-stepping of instructions)
    setSeg(n->RDY, true);

    // pull up to indicate that we are not interrupting now
    setSeg(n->IRQ, true);
    setSeg(n->NMI, true);


    /*
     * RES_BAR pin means "not resetting". Since it is a negated pin, pulling it low means "resetting"
     * and pulling it high means "not resetting" or equivalently "running".
     */

    /*
     * RES_BAR false: resetting now (i.e., in power-up now; pull high to begin normal operation)
     * We want to hold RES_BAR low for a while, indicating power-up phase during which the
     * CPU does not start up normal operations yet. The caller can set RES_BAR high (by calling
     * reset) whenever he is ready to start the CPU running.
     */
    setSeg(n->RES, false);
}

void Cpu6502::reset() {
    setSeg(n->RES, true);
    recalc(n->RES);
}

void Cpu6502::tick() {
    step();
    step();
}

void Cpu6502::step() {
    /*
     * We cheat a little bit here: instead of requiring the
     * caller to toggle clock-zero pin, we let him just call
     * "step" and *we* keep track of which phase we are in.
     * To do this, we just use the CLK0 segment value (as
     * a kind of temporary variable), and just toggle it in
     * order to know which phase we are going into.
     * 
     * The real 6502, of course, does not do this.
     */
    const bool nextPhase = !n->CLK0->on;

    clock(nextPhase);
    rw();

    dumpRegs();
    dumpSegs();
}

void Cpu6502::clock(bool phase) {
    setSeg(n->CLK0, phase);
    recalc(n->CLK0);
}

void Cpu6502::rw() {
    // database read/write happens during Clock Phase 2 (only)
    if (n->CLK2OUT->on) {
        readBus();

        std::set<Segment*> s;
        segs.addDataToRecalc(s);
        recalc(s);

        writeBus();
    }
}








void Cpu6502::readBus() {
    if (this->transNetwork.segs.c->RW->on) {
        this->transNetwork.segs.setDataSegs(read(this->transNetwork.segs.rAddr()));
    }
}

void Cpu6502::writeBus() {
    if (!this->transNetwork.segs.c->RW->on) {
        write(this->transNetwork.segs.rAddr(), this->transNetwork.segs.rData());
    }
}






unsigned char Cpu6502::read(unsigned short addr) {
    const unsigned char x = this->addressBus.read(addr);
#ifdef TRACEMEM
    std::cout << "-------------------------------------------------- ";
    pHex(x);
    std::cout << "<";
    pHexw(addr);
    std::cout << std::endl;
#endif
    return x;
}

void Cpu6502::write(unsigned short addr, unsigned char data) {
    this->addressBus.write(addr, data);
#ifdef TRACEMEM
    std::cout << "-------------------------------------------------- ";
    pHex(data);
    std::cout << ">";
    pHexw(addr);
    std::cout << std::endl;
#endif
}

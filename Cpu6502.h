/* 
 * File:   Cpu6502.h
 * Author: Christopher
 *
 * Created on December 12, 2013, 10:14 PM
 */

#ifndef CPU6502_H
#define	CPU6502_H

#include "TransNetwork.h"


class TransNetwork;
class AddressBus;
class Trace;

class Cpu6502 {
public:

    Cpu6502(TransNetwork& transNetwork, AddressBus& addressBus, Trace& trace) : transNetwork(transNetwork), addressBus(addressBus), trace(trace), segs(transNetwork.segs), n(segs.c) {
    }

    virtual ~Cpu6502() {
    }

    void powerOn();
    void tick();
    void reset();

private:
    Cpu6502(const Cpu6502&);
    Cpu6502& operator=(const Cpu6502&);

    void initPins();
    void step();
    void clock(bool phase);
    void rw();
    void readBus();
    void writeBus();
    unsigned char read(unsigned short addr);
    void write(unsigned short addr, unsigned char data);

    static void setSeg(Segment* s, bool on);

    TransNetwork& transNetwork;
    AddressBus& addressBus;

    Trace& trace;

    SegmentCache& segs;
    SegmentCache::Common* n;
};

#endif	/* CPU6502_H */

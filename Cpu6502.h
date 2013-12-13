/* 
 * File:   Cpu6502.h
 * Author: Christopher
 *
 * Created on December 12, 2013, 10:14 PM
 */

#ifndef CPU6502_H
#define	CPU6502_H

class TransNetwork;
class AddressBus;
class Trace;

class Cpu6502 {
public:

    Cpu6502(TransNetwork& transNetwork, AddressBus& addressBus, Trace& trace) : transNetwork(transNetwork), addressBus(addressBus), trace(trace) {
    }

    virtual ~Cpu6502() {
    }

private:
    Cpu6502(const Cpu6502&);
    Cpu6502& operator=(const Cpu6502&);

    void powerOn();
    void initPins();
    void reset();
    void tick();
    void step();
    void clock(bool phase);
    void rw();
    void readBus();
    void writeBus();
    unsigned char read(unsigned short addr);
    void write(unsigned short addr, unsigned char data);

    TransNetwork& transNetwork;
    AddressBus& addressBus;

    Trace& trace;
};

#endif	/* CPU6502_H */

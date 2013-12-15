/* 
 * File:   Cpu6502Helper.h
 * Author: Christopher
 *
 * Created on December 12, 2013, 10:22 PM
 */

#ifndef CPU6502HELPER_H
#define	CPU6502HELPER_H

class Cpu6502;

class Common;

class Cpu6502Helper {
public:

    explicit Cpu6502Helper(Cpu6502& cpu);
    virtual ~Cpu6502Helper();

    void powerOn();
    void tick();
    void reset();

private:

    Cpu6502Helper(const Cpu6502Helper&) = delete;
    Cpu6502Helper& operator=(const Cpu6502Helper&) = delete;

    void step();

    Cpu6502& cpu;
    bool nextPhase;

    Common* n;
};

#endif	/* CPU6502HELPER_H */

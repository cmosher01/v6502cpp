#include "cpu.h"

#include <algorithm>
#include <utility>
#include <iterator>
#include <vector>
#include <set>
#include <map>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "nodes.h"
#include "addressbus.h"







#define TRACEREG 1
//#define TRACESEG 1
//#define TRACEMEM 1

#ifdef TRACEREG
#define dumpRegs() dumpRegisters()
#else
#define dumpRegs()
#endif

#ifdef TRACESEG
#define dumpSegs() dumpSegments()
#else
#define dumpSegs()
#endif







CPU::CPU(AddressBus& addressBus) :
addressBus(addressBus) {
    std::cout << "reading segsonly";
    std::ifstream if_segs("segsonly");
    if (!if_segs.is_open()) {
        std::cerr << "error opening file: segs" << std::endl;
        exit(EXIT_FAILURE);
    }

    int i_seg(0);
    while (if_segs.good()) {
        int i_segin(-1);
        bool b_on(false);
        if_segs >> i_segin >> b_on;
        if (i_segin >= 0) {
            if (i_segin != i_seg++) {
                std::cerr << "error: mismatch in segsonly file near " << i_segin << std::endl;
                exit(EXIT_FAILURE);
            }
            std::cout << ".";
            seg s;
            s.pullup = b_on;
            s.pulldown = false;
            s.on = false;
            segs.push_back(s);
        }
    }
    std::cout << std::endl << "read " << segs.size() << " segs" << std::endl;



    std::cout << "reading trns";
    std::ifstream if_trns("trns");
    if (!if_trns.is_open()) {
        std::cerr << "error opening file: trns" << std::endl;
        exit(EXIT_FAILURE);
    }

    int i_trn(0);
    while (if_trns.good()) {
        std::cout << ".";
        int i_trnin(-1);
        int i_gate, i_c1, i_c2;
        if_trns >> i_trnin >> i_gate >> i_c1 >> i_c2;
        if (i_trnin >= 0) {
            if (i_trnin != i_trn++) {
                std::cerr << "error: mismatch in trns file near " << i_trnin << std::endl;
                exit(EXIT_FAILURE);
            }
            trn t;
            t.gate = i_gate;
            t.c1 = i_c1;
            t.c2 = i_c2;
            t.on = false;
            trns.push_back(t);
        }
    }
    std::cout << std::endl << "read " << trns.size() << " trns" << std::endl;

    for (int i = 0; i != trns.size(); ++i) {
        trn& t = trns[i];
        if (t.c1 == VSS) {
            t.c1 = t.c2;
            t.c2 = VSS;
        } else if (t.c1 == VCC) {
            t.c1 = t.c2;
            t.c2 = VCC;
        }
        segs[t.gate].gates.push_back(i);
        segs[t.c1].c1c2s.push_back(i);
        segs[t.c2].c1c2s.push_back(i);
    }
}

CPU::~CPU() {
}







void CPU::powerOn() {
    std::cout << "initial state" << std::endl;
    dumpRegs();
    dumpSegs();

    std::cout << "recalcAll..." << std::endl;
    recalcAll();
    dumpRegs();
    dumpSegs();



    std::cout << "setting pins..." << std::endl;
    setSeg(VCC, true);
    setSeg(VSS, false);

    setSeg(CLK0, true);
    setSeg(IRQ, true);
    setSeg(RES, false);
    setSeg(NMI, true);
    setSeg(RDY, true);
    setSeg(SO, false);



    std::cout << "recalc all..." << std::endl;
    recalcAll();
    dumpRegs();
    dumpSegs();
}

void CPU::reset() {
    setSeg(RES, true);
    recalc(RES);
}

void CPU::tick() {
    step();
    step();
}

void CPU::step() {
    const bool h = segs[CLK0].on;

    setSeg(CLK0, !h);
    recalc(CLK0);

    if (!h) {
        rw();
    }

    dumpRegs();
    dumpSegs();
}

void CPU::rw() {
    readBus();

    std::set<int> s;
    addDataToRecalc(s);
    recalc(s);

    writeBus();
}








void CPU::readBus() {
    if (segs[RW].on) {
        setDataSegs(mRead(rAddr()));
    }
}

void CPU::writeBus() {
    if (!segs[RW].on) {
        mWrite(rAddr(), rData());
    }
}

void CPU::setDataSegs(const unsigned char data) {
    unsigned char x = data;

    setSeg(DB0, x & 1);
    x >>= 1;
    setSeg(DB1, x & 1);
    x >>= 1;
    setSeg(DB2, x & 1);
    x >>= 1;
    setSeg(DB3, x & 1);
    x >>= 1;
    setSeg(DB4, x & 1);
    x >>= 1;
    setSeg(DB5, x & 1);
    x >>= 1;
    setSeg(DB6, x & 1);
    x >>= 1;
    setSeg(DB7, x & 1);
}

void CPU::addDataToRecalc(std::set<int>& s) {
    s.insert(DB0);
    s.insert(DB1);
    s.insert(DB2);
    s.insert(DB3);
    s.insert(DB4);
    s.insert(DB5);
    s.insert(DB6);
    s.insert(DB7);
}

unsigned char CPU::mRead(unsigned short addr) {
    unsigned char x;
    x = this->addressBus.read(addr);
#ifdef TRACEMEM
    std::cout << "-------------------------------------------------- ";
    pHex(x);
    std::cout << "<";
    pHexw(addr);
    std::cout << std::endl;
#endif
    return x;
}

void CPU::mWrite(unsigned short addr, unsigned char data) {
    this->addressBus.write(addr, data);
#ifdef TRACEMEM
    std::cout << "-------------------------------------------------- ";
    pHex(data);
    std::cout << ">";
    pHexw(addr);
    std::cout << std::endl;
#endif
}





void CPU::setSeg(const int iSeg, const bool up) {
    seg& s = segs[iSeg];

    s.pullup = up;
    s.pulldown = !up;
}







unsigned char CPU::rData() {
    return readByte(DB7, DB6, DB5, DB4, DB3, DB2, DB1, DB0);
}

unsigned short CPU::rAddr() {
    return readWord(AB15, AB14, AB13, AB12, AB11, AB10, AB9, AB8, AB7, AB6, AB5, AB4, AB3, AB2, AB1, AB0);
}

unsigned char CPU::rA() {
    return readByte(A7, A6, A5, A4, A3, A2, A1, A0);
}

unsigned char CPU::rX() {
    return readByte(X7, X6, X5, X4, X3, X2, X1, X0);
}

unsigned char CPU::rY() {
    return readByte(Y7, Y6, Y5, Y4, Y3, Y2, Y1, Y0);
}

unsigned char CPU::rS() {
    return readByte(S7, S6, S5, S4, S3, S2, S1, S0);
}

unsigned short CPU::rPC() {
    return readWord(PCH7, PCH6, PCH5, PCH4, PCH3, PCH2, PCH1, PCH0, PCL7, PCL6, PCL5, PCL4, PCL3, PCL2, PCL1, PCL0);
}



unsigned char CPU::readByte(unsigned int b7, unsigned int b6, unsigned int b5, unsigned int b4, unsigned int b3, unsigned int b2, unsigned int b1, unsigned int b0) {
    return
        segs[b7].on << 7 |
        segs[b6].on << 6 |
        segs[b5].on << 5 |
        segs[b4].on << 4 |
        segs[b3].on << 3 |
        segs[b2].on << 2 |
        segs[b1].on << 1 |
        segs[b0].on;
}

unsigned short CPU::readWord(unsigned int b15, unsigned int b14, unsigned int b13, unsigned int b12, unsigned int b11, unsigned int b10, unsigned int b9, unsigned int b8, unsigned int b7, unsigned int b6, unsigned int b5, unsigned int b4, unsigned int b3, unsigned int b2, unsigned int b1, unsigned int b0) {
    return
        segs[b15].on << 15 |
        segs[b14].on << 14 |
        segs[b13].on << 13 |
        segs[b12].on << 12 |
        segs[b11].on << 11 |
        segs[b10].on << 10 |
        segs[b9].on << 9 |
        segs[b8].on << 8 |
        segs[b7].on << 7 |
        segs[b6].on << 6 |
        segs[b5].on << 5 |
        segs[b4].on << 4 |
        segs[b3].on << 3 |
        segs[b2].on << 2 |
        segs[b1].on << 1 |
        segs[b0].on;
}












void CPU::recalcAll() {
    std::set<int> riSeg;
    for (int iSeg = 0; iSeg < segs.size(); ++iSeg) {
        addRecalc(iSeg,riSeg);
    }
    recalc(riSeg);
}

void CPU::recalc(const int iSeg) {
    std::set<int> riSeg;
    addRecalc(iSeg,riSeg);
    recalc(riSeg);
}

void CPU::recalc(const std::set<int>& riSeg) {
    std::set<int> riSegRecalc(riSeg);
    for (int sane = 0; sane < 100; ++sane) {
        if (!riSegRecalc.size()) {
            return;
        }

        std::set<int> riSegChanged;
        for (std::set<int>::const_iterator iSegRecalc = riSegRecalc.begin(); iSegRecalc != riSegRecalc.end(); ++iSegRecalc) {
            recalcNode(*iSegRecalc, riSegChanged);
        }
        riSegRecalc = riSegChanged;
    }
    std::cerr << "ERROR: reached maximum iteration limit while recalculating CPU state" << std::endl;
}

void CPU::recalcNode(const int iSeg, std::set<int>& riSegChanged) {
    if (!(iSeg == VCC || iSeg == VSS)) {
        std::set<int> riSegGroup;
        addToGroup(iSeg, riSegGroup);
        const bool groupOn = getGroupValue(riSegGroup);

        for (std::set<int>::iterator iSegGroup = riSegGroup.begin(); iSegGroup != riSegGroup.end(); ++iSegGroup) {
            seg& s = segs[*iSegGroup];
            if (s.on != groupOn) {
                s.on = groupOn;
                for (std::vector<int>::iterator iTrnGate = s.gates.begin(); iTrnGate != s.gates.end(); ++iTrnGate) {
                    setTrans(trns[*iTrnGate], s.on, riSegChanged);
                }
            }
        }
    }
}

void CPU::setTrans(trn& t, const bool on, std::set<int>& riSeg) {
    if (t.on != on) {
        t.on = on;
        addRecalc(t.c1, riSeg);
        addRecalc(t.c2, riSeg);
    }
}

void CPU::addRecalc(const int iSeg, std::set<int>& riSeg) {
    if (!(iSeg == VCC || iSeg == VSS)) {
        riSeg.insert(iSeg);
    }
}

/*
 * Adds segment iSeg, and all segments electrically connected to it, to riSeg.
 * This happens recursively, but we don't recurse past ground or voltage supply.
 */
void CPU::addToGroup(int iSeg, std::set<int>& riSeg) {
    const std::pair<std::set<int>::iterator, bool> ret = riSeg.insert(iSeg);
    if (!ret.second) {
        return;
    }
    if (iSeg == VCC || iSeg == VSS) {
        return;
    }

    /*
     * For every ON transistor this seg is connected to via a leg (source or
     * drain), add the seg that's connected to the OTHER leg of the transistor.
     * This is a RECURSIVE addition.
     */
    const seg& s = segs[iSeg];
    for (std::vector<int>::const_iterator iTrn = s.c1c2s.begin(); iTrn != s.c1c2s.end(); ++iTrn) {
        const trn& t = trns[*iTrn];
        if (t.on) {
            if (t.c1 == iSeg) {
                addToGroup(t.c2, riSeg);
            } else if (t.c2 == iSeg) {
                addToGroup(t.c1, riSeg);
            }
        }
    }
}

bool CPU::getGroupValue(const std::set<int>& riSeg) {
    /* If group contain ground, it's OFF. */
    if (riSeg.find(VSS) != riSeg.end()) {
        return false;
    }
    /* If group contain voltage supply, it's ON. */
    if (riSeg.find(VCC) != riSeg.end()) {
        return true;
    }

    for (std::set<int>::const_iterator iSeg = riSeg.begin(); iSeg != riSeg.end(); ++iSeg) {
        const seg& s = segs[*iSeg];
        if (s.pullup) {
            return true;
        }
        if (s.pulldown) {
            return false;
        }
        if (s.on) {
            return true;
        }
    }
    return false;
}











static void pHex(unsigned char x) {
    std::cout << std::setw(2) << std::setfill('0') << std::hex << (unsigned long) x << std::dec;
}

static void pHexw(unsigned short x) {
    std::cout << std::setw(4) << std::setfill('0') << std::hex << (unsigned long) x << std::dec;
}

void CPU::dumpSegments() {
    for (int i = 0; i < segs.size(); ++i) {
        seg& s = segs[i];
        if (s.pullup) {
            std::cout << "U";
        } else if (s.pulldown) {
            std::cout << "D";
        } else {
            std::cout << "f";
        }
        if (s.on) {
            std::cout << "+";
        } else {
            std::cout << "-";
        }
    }
    std::cout << std::endl;
}

void CPU::dumpRegisters() {
    std::cout << "A";
    pHex(rA());
    std::cout << " X";
    pHex(rX());
    std::cout << " Y";
    pHex(rY());
    std::cout << " ";
    std::cout << (segs[P7].on ? "N" : "n");
    std::cout << (segs[P6].on ? "V" : "v");
    std::cout << ".";
    std::cout << (segs[P4].on ? "B" : "b");
    std::cout << (segs[P3].on ? "D" : "d");
    std::cout << (segs[P2].on ? "I" : "i");
    std::cout << (segs[P1].on ? "Z" : "z");
    std::cout << (segs[P0].on ? "C" : "c");
    std::cout << " S";
    pHex(rS());
    std::cout << " PC";
    pHexw(rPC());
    if (segs[CLK1OUT].on) {
        std::cout << "  PH1  ";
    }
    if (segs[CLK2OUT].on) {
        std::cout << "  PH2";
        if (segs[RW].on) {
            std::cout << " R";
        } else {
            std::cout << " W";
        }
    }
    if (!(segs[CLK1OUT].on || segs[CLK2OUT].on)) {
        std::cout << "  PH-  ";
    }
    std::cout << " DB";
    pHex(rData());
    std::cout << " AB";
    pHexw(rAddr());
    std::cout << std::endl;
    //pZP();
}

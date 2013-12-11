#include "cpu.h"

#include <algorithm>
#include <utility>
#include <iterator>
#include <string>
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

static void pHex(const unsigned char x) {
    std::cout << std::setw(2) << std::setfill('0') << std::hex << (unsigned long) x << std::dec;
}

static void pHexw(const unsigned short x) {
    std::cout << std::setw(4) << std::setfill('0') << std::hex << (unsigned long) x << std::dec;
}








static nodes* n;

static std::map<int,std::string> map_i_seg;

static int get_i_seg(const std::string& seg, std::map<std::string,int>& map_seg_i) {
    static int i_segin(0);
    int i_seg = -1;
    if (map_seg_i.find(seg) == map_seg_i.end()) {
        i_seg = i_segin++;
        map_seg_i[seg] = i_seg;
        map_i_seg[i_seg] = seg;
    } else {
        i_seg = map_seg_i[seg];
    }
    return i_seg;
}

CPU::CPU(AddressBus& addressBus) :
addressBus(addressBus) {
    std::cout << "reading transistors";
    std::ifstream if_trans("transistors");
    if (!if_trans.is_open()) {
        std::cerr << "error opening file: transistors" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string seg_c1, seg_gate, seg_c2;
    if_trans >> seg_c1 >> seg_gate >> seg_c2;
    while (if_trans.good()) {
        std::cout << ".";

        trn t;
        t.on = false;
        t.c1 = get_i_seg(seg_c1,map_seg_i);
        t.gate = get_i_seg(seg_gate,map_seg_i);
        t.c2 = get_i_seg(seg_c2,map_seg_i);
        trns.push_back(t);

        if_trans >> seg_c1 >> seg_gate >> seg_c2;
    }
    std::cout << std::endl << "read " << map_seg_i.size() << " segments, " << trns.size() << " transistors" << std::endl;

    for (int i_seg = 0; i_seg < map_seg_i.size(); ++i_seg)
    {
        const std::string id_seg = map_i_seg[i_seg];

        seg s;
        s.id = id_seg;
        s.pullup = id_seg[0] == '+';
        s.pulldown = false;
        s.on = false;
        segs.push_back(s);
    }


    n = new nodes(map_seg_i);

    for (int i = 0; i != trns.size(); ++i) {
        trn& t = trns[i];
        if (t.c1 == n->VSS) {
            t.c1 = t.c2;
            t.c2 = n->VSS;
        } else if (t.c1 == n->VCC) {
            t.c1 = t.c2;
            t.c2 = n->VCC;
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



    /*
     * Since we use segs[CLK0].on as our own
     * temporary variable (see "step" method), we
     * need to initialize it here, to "phase one".
     */
    segs[n->CLK0].on = true;



    std::cout << "setting input pins..." << std::endl;
    initPins();

    std::cout << "initial full calculation..." << std::endl;
    recalcAll();
    dumpRegs();
    dumpSegs();
}

void CPU::initPins() {
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

void CPU::reset() {
    setSeg(n->RES, true);
    recalc(n->RES);
}

void CPU::tick() {
    step();
    step();
}

void CPU::step() {
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
    const bool nextPhase = !segs[n->CLK0].on;

    setSeg(n->CLK0, nextPhase);
    recalc(n->CLK0);

    // database read/write happens during Clock Phase 2 (only)
    if (segs[n->CLK2OUT].on) {
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
    if (segs[n->RW].on) {
        setDataSegs(mRead(rAddr()));
    }
}

void CPU::writeBus() {
    if (!segs[n->RW].on) {
        mWrite(rAddr(), rData());
    }
}

void CPU::setDataSegs(const unsigned char data) {
    unsigned char x = data;

    setSeg(n->DB0, x & 1);
    x >>= 1;
    setSeg(n->DB1, x & 1);
    x >>= 1;
    setSeg(n->DB2, x & 1);
    x >>= 1;
    setSeg(n->DB3, x & 1);
    x >>= 1;
    setSeg(n->DB4, x & 1);
    x >>= 1;
    setSeg(n->DB5, x & 1);
    x >>= 1;
    setSeg(n->DB6, x & 1);
    x >>= 1;
    setSeg(n->DB7, x & 1);
}

void CPU::addDataToRecalc(std::set<int>& s) {
    s.insert(n->DB0);
    s.insert(n->DB1);
    s.insert(n->DB2);
    s.insert(n->DB3);
    s.insert(n->DB4);
    s.insert(n->DB5);
    s.insert(n->DB6);
    s.insert(n->DB7);
}

unsigned char CPU::mRead(unsigned short addr) {
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
    return readByte(n->DB7, n->DB6, n->DB5, n->DB4, n->DB3, n->DB2, n->DB1, n->DB0);
}

unsigned short CPU::rAddr() {
    return readWord(n->AB15, n->AB14, n->AB13, n->AB12, n->AB11, n->AB10, n->AB9, n->AB8, n->AB7, n->AB6, n->AB5, n->AB4, n->AB3, n->AB2, n->AB1, n->AB0);
}

unsigned char CPU::rA() {
    return readByte(n->A7, n->A6, n->A5, n->A4, n->A3, n->A2, n->A1, n->A0);
}

unsigned char CPU::rX() {
    return readByte(n->X7, n->X6, n->X5, n->X4, n->X3, n->X2, n->X1, n->X0);
}

unsigned char CPU::rY() {
    return readByte(n->Y7, n->Y6, n->Y5, n->Y4, n->Y3, n->Y2, n->Y1, n->Y0);
}

unsigned char CPU::rS() {
    return readByte(n->S7, n->S6, n->S5, n->S4, n->S3, n->S2, n->S1, n->S0);
}

unsigned short CPU::rPC() {
    return readWord(n->PCH7, n->PCH6, n->PCH5, n->PCH4, n->PCH3, n->PCH2, n->PCH1, n->PCH0, n->PCL7, n->PCL6, n->PCL5, n->PCL4, n->PCL3, n->PCL2, n->PCL1, n->PCL0);
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

/*
 * Recalculate segment states (on/off), based on the fact that the segments
 * in riSeg have just changed state. Keep track of which other segments are
 * affected, and repeat the process on those segments. Repeat until no more
 * segments change state.
 */
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

/*
 * Gets group of segments currently electrically connected to iSeg,
 * gets what their group value is (or should be), goes through all
 * those segments and sets their "on" value. For all connected gates,
 * turn on/off, and indicate that the segments connected to those
 * transistors' source and drain legs have changed, by adding them
 * to riSegChanged.
 */
void CPU::recalcNode(const int iSeg, std::set<int>& riSegChanged) {
    if (!(iSeg == n->VCC || iSeg == n->VSS)) {
        std::set<int> riSegGroup;
        addToGroup(iSeg, riSegGroup);
        const bool groupOn = getGroupValue(riSegGroup);

        for (std::set<int>::iterator iSegGroup = riSegGroup.begin(); iSegGroup != riSegGroup.end(); ++iSegGroup) {
            seg& s = segs[*iSegGroup];
            if (s.on != groupOn) {
                s.on = groupOn;
                for (std::vector<int>::iterator iTrnGate = s.gates.begin(); iTrnGate != s.gates.end(); ++iTrnGate) {
                    setTrans(trns[*iTrnGate], groupOn, riSegChanged);
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
    if (!(iSeg == n->VCC || iSeg == n->VSS)) {
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
    if (iSeg == n->VCC || iSeg == n->VSS) {
        return;
    }

    /*
     * For every ON transistor this seg is connected to via a leg (source or
     * drain), add the seg that's connected to the OTHER leg of the transistor.
     * This is a RECURSIVE addition.
     * 
     * Also note that, upon system startup, all transistors are initialized
     * to OFF, so at the time of the very first recalcAll call, *no* other
     * segments will be added here.
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

/*
 * Upon system startup, this will return the "pullup" value of
 * each segment (except for VCC and VSS (which will of course be
 * ON and OFF respectively).
 */
bool CPU::getGroupValue(const std::set<int>& riSeg) {
    /* If group contains ground, it's OFF, */
    if (riSeg.find(n->VSS) != riSeg.end()) {
        return false;
    }
    /* otherwise, if group contains voltage supply, it's ON. */
    else if (riSeg.find(n->VCC) != riSeg.end()) {
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
    std::cout << (segs[n->P7].on ? "N" : "n");
    std::cout << (segs[n->P6].on ? "V" : "v");
    std::cout << ".";
    std::cout << (segs[n->P4].on ? "B" : "b");
    std::cout << (segs[n->P3].on ? "D" : "d");
    std::cout << (segs[n->P2].on ? "I" : "i");
    std::cout << (segs[n->P1].on ? "Z" : "z");
    std::cout << (segs[n->P0].on ? "C" : "c");
    std::cout << " S";
    pHex(rS());
    std::cout << " PC";
    pHexw(rPC());
    if (segs[n->CLK1OUT].on) {
        std::cout << "  PH1  ";
    }
    if (segs[n->CLK2OUT].on) {
        std::cout << "  PH2";
        if (segs[n->RW].on) {
            std::cout << " R";
        } else {
            std::cout << " W";
        }
    }
    if (!(segs[n->CLK1OUT].on || segs[n->CLK2OUT].on)) {
        std::cout << "  PH-  ";
    }
    std::cout << " DB";
    pHex(rData());
    std::cout << " AB";
    pHexw(rAddr());
    std::cout << std::endl;
    //pZP();
}

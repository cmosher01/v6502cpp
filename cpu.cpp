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








/* power */
#define VCC sg->isegVCC
#define VSS sg->isegVSS

/* inputs */
#define CLK0 sg->isegCLK0
#define IRQ sg->isegIRQ
#define RES sg->isegRES
#define NMI sg->isegNMI
#define RDY sg->isegRDY
#define SO sg->isegSO

/* data bus (I/O) */
#define DB0 sg->isegDB0
#define DB1 sg->isegDB1
#define DB3 sg->isegDB3
#define DB2 sg->isegDB2
#define DB5 sg->isegDB5
#define DB4 sg->isegDB4
#define DB7 sg->isegDB7
#define DB6 sg->isegDB6

/* address bus (output) */
#define AB0 sg->isegAB0
#define AB1 sg->isegAB1
#define AB2 sg->isegAB2
#define AB3 sg->isegAB3
#define AB4 sg->isegAB4
#define AB5 sg->isegAB5
#define AB6 sg->isegAB6
#define AB7 sg->isegAB7
#define AB8 sg->isegAB8
#define AB9 sg->isegAB9
#define AB12 sg->isegAB12
#define AB13 sg->isegAB13
#define AB10 sg->isegAB10
#define AB11 sg->isegAB11
#define AB14 sg->isegAB14
#define AB15 sg->isegAB15

/* outputs */
#define RW sg->isegRW
#define SYNC sg->isegSYNC
#define CLK1OUT sg->isegCLK1OUT
#define CLK2OUT sg->isegCLK2OUT

/* internal registers */
#define A0 sg->isegA0
#define A1 sg->isegA1
#define A2 sg->isegA2
#define A3 sg->isegA3
#define A4 sg->isegA4
#define A5 sg->isegA5
#define A6 sg->isegA6
#define A7 sg->isegA7

#define X0 sg->isegX0
#define X1 sg->isegX1
#define X2 sg->isegX2
#define X3 sg->isegX3
#define X4 sg->isegX4
#define X5 sg->isegX5
#define X6 sg->isegX6
#define X7 sg->isegX7

#define Y0 sg->isegY0
#define Y1 sg->isegY1
#define Y2 sg->isegY2
#define Y3 sg->isegY3
#define Y4 sg->isegY4
#define Y5 sg->isegY5
#define Y6 sg->isegY6
#define Y7 sg->isegY7

#define PCL0 sg->isegPCL0
#define PCL1 sg->isegPCL1
#define PCL2 sg->isegPCL2
#define PCL3 sg->isegPCL3
#define PCL4 sg->isegPCL4
#define PCL5 sg->isegPCL5
#define PCL6 sg->isegPCL6
#define PCL7 sg->isegPCL7

#define PCH0 sg->isegPCH0
#define PCH1 sg->isegPCH1
#define PCH2 sg->isegPCH2
#define PCH3 sg->isegPCH3
#define PCH4 sg->isegPCH4
#define PCH5 sg->isegPCH5
#define PCH6 sg->isegPCH6
#define PCH7 sg->isegPCH7

#define P0 sg->isegP0
#define P1 sg->isegP1
#define P2 sg->isegP2
#define P3 sg->isegP3
#define P4 sg->isegP4
#define P6 sg->isegP6
#define P7 sg->isegP7

#define S0 sg->isegS0
#define S1 sg->isegS1
#define S2 sg->isegS2
#define S3 sg->isegS3
#define S4 sg->isegS4
#define S5 sg->isegS5
#define S6 sg->isegS6
#define S7 sg->isegS7



static nodes* sg;

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



    const int isegVSS = map_seg_i["-vss"];
    const int isegVCC = map_seg_i["-vcc"];
    for (int i = 0; i != trns.size(); ++i) {
        trn& t = trns[i];
        if (t.c1 == isegVSS) {
            t.c1 = t.c2;
            t.c2 = isegVSS;
        } else if (t.c1 == isegVCC) {
            t.c1 = t.c2;
            t.c2 = isegVCC;
        }
        segs[t.gate].gates.push_back(i);
        segs[t.c1].c1c2s.push_back(i);
        segs[t.c2].c1c2s.push_back(i);
    }

    sg = new nodes(map_seg_i);
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
    segs[CLK0].on = true;



    std::cout << "setting input pins..." << std::endl;
    initPins();

    std::cout << "initial full calculation..." << std::endl;
    recalcAll();
    dumpRegs();
    dumpSegs();
}

void CPU::initPins() {
    // set voltage supply and ground.
    setSeg(VCC, true);
    setSeg(VSS, false);

    // don't do the set-overflow overriding functionality
    setSeg(SO, false);

    // ready to run (i.e., do not do single-stepping of instructions)
    setSeg(RDY, true);

    // pull up to indicate that we are not interrupting now
    setSeg(IRQ, true);
    setSeg(NMI, true);


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
    setSeg(RES, false);
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
    const bool nextPhase = !segs[CLK0].on;

    setSeg(CLK0, nextPhase);
    recalc(CLK0);

    // database read/write happens during Clock Phase 2 (only)
    if (segs[CLK2OUT].on) {
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
    if (!(iSeg == VCC || iSeg == VSS)) {
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
    if (riSeg.find(VSS) != riSeg.end()) {
        return false;
    }
    /* otherwise, if group contains voltage supply, it's ON. */
    else if (riSeg.find(VCC) != riSeg.end()) {
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

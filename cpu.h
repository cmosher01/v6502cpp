#ifndef CPU_H
#define CPU_H

#include <vector>
#include <set>

class AddressBus;

class seg {
public:
    bool pullup;
    bool pulldown;
    bool on;
    std::vector<int> gates;
    std::vector<int> c1c2s;
};

class trn {
public:
    int gate;
    int c1;
    int c2;
    bool on;
};

class CPU {
private:
    std::vector<seg> segs;
    std::vector<trn> trns;
    AddressBus& addressBus;

    void addDataToRecalc(std::set<int>& s);
    void addRecalc(const int iSeg, std::set<int>& riSeg);
    void setTrans(trn& t, const bool on, std::set<int>& riSeg);
    unsigned char readByte(unsigned int b7, unsigned int b6, unsigned int b5, unsigned int b4, unsigned int b3, unsigned int b2, unsigned int b1, unsigned int b0);
    unsigned short readWord(unsigned int b15, unsigned int b14, unsigned int b13, unsigned int b12, unsigned int b11, unsigned int b10, unsigned int b9, unsigned int b8, unsigned int b7, unsigned int b6, unsigned int b5, unsigned int b4, unsigned int b3, unsigned int b2, unsigned int b1, unsigned int b0);
    unsigned char rData();
    unsigned short rAddr();
    unsigned char rA();
    unsigned char rX();
    unsigned char rY();
    unsigned char rS();
    unsigned short rPC();
    void dumpSegments();
    void dumpRegisters();
    bool getGroupValue(const std::set<int>& s);
    void addToGroup(int n, std::set<int>& s);
    void recalcNode(int n, std::set<int>& rcl);
    void recalc(const std::set<int>& s);
    void recalc(int n);
    void recalcAll();
    void setSeg(int iseg, bool up);
    void setHigh(int iseg);
    void setLow(int iseg);
    void setDataSegs(unsigned char data);
    void readBus();
    void writeBus();
    unsigned char mRead(unsigned short addr);
    void mWrite(unsigned short addr, unsigned char data);
    void rw();
    void step();
    void initPins();

public:
    CPU(AddressBus& addressBus);
    ~CPU();

    void powerOn();
    void reset();
    void tick();
};
#endif

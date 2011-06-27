#include <algorithm>
#include <utility>
#include <iterator>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "nodes.h"

#define TRACE 1

class seg {
public:
	bool pullup;
	bool pulldown;
	bool on;
	std::vector<int> gates;
	std::vector<int> c1c2s;
};
std::vector<seg> segs;

class trn {
public:
	int gate;
	int c1;
	int c2;
	bool on;
};
std::vector<trn> trns;

unsigned char readByte(unsigned int b7, unsigned int b6, unsigned int b5, unsigned int b4, unsigned int b3, unsigned int b2, unsigned int b1, unsigned int b0) {
	return
	segs[b7].on<<7 |
	segs[b6].on<<6 |
	segs[b5].on<<5 |
	segs[b4].on<<4 |
	segs[b3].on<<3 |
	segs[b2].on<<2 |
	segs[b1].on<<1 |
	segs[b0].on;
}

unsigned short readWord(unsigned int b15, unsigned int b14, unsigned int b13, unsigned int b12, unsigned int b11, unsigned int b10, unsigned int b9, unsigned int b8, unsigned int b7, unsigned int b6, unsigned int b5, unsigned int b4, unsigned int b3, unsigned int b2, unsigned int b1, unsigned int b0) {
	return
	segs[b15].on<<15 |
	segs[b14].on<<14 |
	segs[b13].on<<13 |
	segs[b12].on<<12 |
	segs[b11].on<<11 |
	segs[b10].on<<10 |
	segs[b9].on<<9 |
	segs[b8].on<<8 |
	segs[b7].on<<7 |
	segs[b6].on<<6 |
	segs[b5].on<<5 |
	segs[b4].on<<4 |
	segs[b3].on<<3 |
	segs[b2].on<<2 |
	segs[b1].on<<1 |
	segs[b0].on;
}

bool isHigh(int iseg) {
	return segs[iseg].on;
}

unsigned char rData() {
	return readByte(DB7,DB6,DB5,DB4,DB3,DB2,DB1,DB0);
}

unsigned short rAddr() {
	return readWord(AB15,AB14,AB13,AB12,AB11,AB10,AB9,AB8,AB7,AB6,AB5,AB4,AB3,AB2,AB1,AB0);
}

unsigned char rA() {
	return readByte(A7,A6,A5,A4,A3,A2,A1,A0);
}

unsigned char rX() {
	return readByte(X7,X6,X5,X4,X3,X2,X1,X0);
}

unsigned char rY() {
	return readByte(Y7,Y6,Y5,Y4,Y3,Y2,Y1,Y0);
}

unsigned char rS() {
	return readByte(S7,S6,S5,S4,S3,S2,S1,S0);
}

unsigned short rPC() {
	return readWord(PCH7,PCH6,PCH5,PCH4,PCH3,PCH2,PCH1,PCH0,PCL7,PCL6,PCL5,PCL4,PCL3,PCL2,PCL1,PCL0);
}

void pHex(unsigned char x) {
	std::cout << std::setw(2) << std::setfill('0') << std::hex << (unsigned long)x << std::dec;
}

void pHexw(unsigned short x) {
	std::cout << std::setw(4) << std::setfill('0') << std::hex << (unsigned long)x << std::dec;
}

void dumpSegs() {
	for (int i = 0; i < segs.size(); ++i) {
		std::cout << i << " ";
		seg& s = segs[i];
		if (s.pullup) {
			std::cout << "+";
		} else if (s.pulldown) {
			std::cout << "-";
		} else {
			std::cout << "0";
		}
		if (s.on) {
			std::cout << "+";
		} else {
			std::cout << "-";
		}
		std::cout << std::endl;
	}
}

unsigned char memory[0x10000];
void init_mem() {
	for (int i = 0; i < 0x10000; ++i) {
		memory[i] = 0;
	}

	memory[0x0200] = 0xEA; // NOP
	memory[0x0201] = 0xEA; // NOP
	memory[0x0202] = 0xA9; // LDA #$5A
	memory[0x0203] = 0x5A; //
	memory[0x0204] = 0x85; // STA $88
	memory[0x0205] = 0x88; //
	memory[0x0206] = 0xA9; // LDA #$23
	memory[0x0207] = 0x23; //
	memory[0x0208] = 0xD0; // BNE 0
	memory[0x0209] = 0xF8; //

	memory[0xFFFC] = 0x02;  // RESET --> $0202
	memory[0xFFFD] = 0x02;

//	memory[0xFF] = 0x68;  // PLA
/*
	memory[0xFF] = 0xFF;
	memory[0xFFFF] = 0xFE;
	memory[0xFEFE] = 0xFD;
	memory[0xFDFD] = 0xFC;
	memory[0xFCFC] = 0xFB;
	memory[0xFBFB] = 0xFA;
	memory[0xFAFA] = 0xF9;
*/
}
void pZP() {
	int a = 0;
	for (int i = 0; i < 16; ++i) {
		pHexw(a);
		std::cout << ": ";
		for (int j = 0; j < 16; ++j) {
			pHex(memory[a++]);
			std::cout << " ";
		}
		std::cout << std::endl;
	}
}

unsigned char mRead(unsigned short addr) {
	unsigned char x;
	x = memory[addr];
#ifdef TRACEMEM
	std::cout << "--------------------------------------------- ";
	pHex(x);
	std::cout << "<";
	pHexw(addr);
	std::cout << std::endl;
#endif
	return x;
}

void mWrite(unsigned short addr, unsigned char data) {
	/* TODO write data to addr in memory */
	memory[addr] = data;
#ifdef TRACEMEM
	std::cout << "--------------------------------------------- ";
	pHex(data);
	std::cout << ">";
	pHexw(addr);
	std::cout << std::endl;
#endif
}

void dumpRegs() {
	std::cout << "A";
	pHex(rA());
	std::cout << " X";
	pHex(rX());
	std::cout << " Y";
	pHex(rY());
	std::cout << " ";
	std::cout << (isHigh(P7) ? "N" : "n");
	std::cout << (isHigh(P6) ? "V" : "v");
	std::cout << ".";
	std::cout << (isHigh(P4) ? "B" : "b");
	std::cout << (isHigh(P3) ? "D" : "d");
	std::cout << (isHigh(P2) ? "I" : "i");
	std::cout << (isHigh(P1) ? "Z" : "z");
	std::cout << (isHigh(P0) ? "C" : "c");
	std::cout << " S";
	pHex(rS());
	std::cout << " PC";
	pHexw(rPC());
	if (isHigh(CLK1OUT)) {
		std::cout << "  PH1  ";
	}
	if (isHigh(CLK2OUT)) {
		std::cout << "  PH2";
		if (isHigh(RW)) {
			std::cout << " R";
		} else {
			std::cout << " W";
		}
	}
	std::cout << " DB";
	pHex(rData());
	std::cout << " AB";
	pHexw(rAddr());
	std::cout << std::endl;
//pZP();
}


void addRecalc(int n, std::set<int>& rcl) {
	if (n==VCC || n==VSS) {
		return;
	}
	rcl.insert(n);
}

void onTrans(trn& t, std::set<int>& rcl) {
	if (t.on) {
		return;
	}
	t.on = true;
	addRecalc(t.c1,rcl);
//	addRecalc(t.c2,rcl); //looks like this is not necessary?
}

void offTrans(trn& t, std::set<int>& rcl) {
	if (!t.on) {
		return;
	}
	t.on = false;
	addRecalc(t.c1,rcl);
	addRecalc(t.c2,rcl);
}

bool getGroupValue(const std::set<int>& s) {
	if (s.find(VSS) != s.end()) {
		return false;
	}
	if (s.find(VCC) != s.end()) {
		return true;
	}
	for (std::set<int>::const_iterator i = s.begin(); i != s.end(); ++i) {
		const seg& s = segs[*i];
		if (s.pullup) { return true; }
		if (s.pulldown) { return false; }
//		if (s.on) { return true; }
	}
	for (std::set<int>::const_iterator i = s.begin(); i != s.end(); ++i) {
		const seg& s = segs[*i];
		if (s.on) { return true; }
	}
	return false;
}

void addToGroup(int n, std::set<int>& s) {
	const std::pair<std::set<int>::iterator,bool> ret = s.insert(n);
	if (!ret.second) {
		return;
	}
	if (n==VCC || n==VSS) {
		return;
	}
//std::cout << "a: " << n << std::endl;
	const seg& sg = segs[n];
	for (std::vector<int>::const_iterator itrn = sg.c1c2s.begin(); itrn != sg.c1c2s.end(); ++itrn) {
		const trn& t = trns[*itrn];
		if (t.on) {
			if (t.c1==n) {
				addToGroup(t.c2,s);
			} else if (t.c2==n) {
				addToGroup(t.c1,s);
			}
		}
	}
}

void recalcNode(int n, std::set<int>& rcl) {
	if (!(n==VCC || n==VSS)) {
		std::set<int> g;
		addToGroup(n,g);
		const bool gval = getGroupValue(g);
//std::cout << "gval: " << gval << " grp size: " << g.size() << std::endl;
		for (std::set<int>::iterator ig = g.begin(); ig != g.end(); ++ig) {
//std::cout << "ig: " << *ig << std::endl;
			seg& seg = segs[*ig];
			if (seg.on != gval) {
//std::cout << "change seg on " << std::endl;
				seg.on = gval;
				for (std::vector<int>::iterator igate = seg.gates.begin(); igate != seg.gates.end(); ++igate) {
					trn& t = trns[*igate];
					if (seg.on) {
						onTrans(t,rcl);
					} else {
						offTrans(t,rcl);
					}
				}
			}
		}
	}
}

void recalc(const std::set<int>& s) {
	std::set<int> list(s);
//	std::set<int> done;
	for (int sane = 0; sane < 100; ++sane) {
//std::cout << "rc: " << list.size() << std::endl;
		if (!list.size()) {
			return;
		}
		std::set<int> rcl;
		for (std::set<int>::const_iterator ilist = list.begin(); ilist != list.end(); ++ilist) {
			recalcNode(*ilist,rcl);
//std::cout << "done recalcNode" << std::endl;
		}
//		done.insert(rcl.begin(),rcl.end());
//		std::set<int> v;
//		std::set_difference(rcl.begin(),rcl.end(),done.begin(),done.end(),std::inserter(v,v.end()));
//		list = v;
//		if (std::equal(list.begin(),list.end(),rcl.begin())) {
//std::cout << "hit stasis" << std::endl;
//			return;
//		}
		list = rcl;
	}
	std::cerr << "ERROR: reached maximum iteration limit while recalculating CPU state" << std::endl;
}

void recalc(int n) {
	std::set<int> s;
	s.insert(n);
	recalc(s);
}

void recalcAll() {
	std::set<int> s;
	for (int i = 0; i < segs.size(); ++i) {
		if (!(i == VCC || i == VSS)) {
			s.insert(i);
		}
	}
	recalc(s);
}




void setSeg(int iseg, bool up) {
	seg& s = segs[iseg];
	s.pullup = up;
	s.pulldown = !up;
}

void setHigh(int iseg) {
	setSeg(iseg,true);
}

void setLow(int iseg) {
	setSeg(iseg,false);
}

void putDataToChip(unsigned char data) {
/*
	std::cout << "d2cpu: ";
	pHex(data);
	std::cout << std::endl;
*/
	unsigned char x = data;

	setSeg(DB0,x&1);
	x >>= 1;
	setSeg(DB1,x&1);
	x >>= 1;
	setSeg(DB2,x&1);
	x >>= 1;
	setSeg(DB3,x&1);
	x >>= 1;
	setSeg(DB4,x&1);
	x >>= 1;
	setSeg(DB5,x&1);
	x >>= 1;
	setSeg(DB6,x&1);
	x >>= 1;
	setSeg(DB7,x&1);
}

void readBus() {
	if (isHigh(RW)) {
		putDataToChip(mRead(rAddr()));
	}
}

void writeBus() {
	if (!isHigh(RW)) {
		mWrite(rAddr(),rData());
	}
}

void addDataToRecalc(std::set<int>& s) {
	s.insert(DB0);
	s.insert(DB1);
	s.insert(DB2);
	s.insert(DB3);
	s.insert(DB4);
	s.insert(DB5);
	s.insert(DB6);
	s.insert(DB7);
}

void rw() {
	readBus();
	std::set<int> s;
	addDataToRecalc(s);
	recalc(s);
	writeBus();
}

void step() {
	if (isHigh(CLK0)) {
		setLow(CLK0);
		recalc(CLK0);
	} else {
		setHigh(CLK0);
		recalc(CLK0);
		rw();
	}
#ifdef TRACE
	dumpRegs();
#endif

}





void init() {
	init_mem();
	std::cout << "initializing CPU..." << std::endl;
//dumpRegs();
//dumpSegs();
	recalcAll();
//dumpSegs();
dumpRegs();

	setHigh(VCC);
	setLow(VSS);

	setHigh(CLK0);
	setHigh(IRQ);
	setLow(RES);
	setHigh(NMI);
	setHigh(RDY);
	setLow(SO);

std::set<int> s;
s.insert(VCC);
s.insert(VSS);
s.insert(CLK0);
s.insert(IRQ);
s.insert(RES);
s.insert(NMI);
s.insert(RDY);
s.insert(SO);
recalc(s);
//dumpSegs();
//	std::cout << "recalc all" << std::endl;
//	recalcAll();
rw();
dumpRegs();
	std::cout << "some power-up pre-reset cycles" << std::endl;
	for (int i(0); i < 10; ++i) {
		step();
	}

	std::cout << "   RESET" << std::endl;
	setHigh(RES);
	recalc(RES);
}





int main(int argc, char *argv[])
{
	std::cout << "reading segsonly";
	std::ifstream if_segs("segsonly");
	if (!if_segs.is_open()) {
		std::cerr << "error opening file: segs" << std::endl;
		return 1;
	}

	int i_seg(0);
	while (if_segs.good()) {
		int i_segin(-1);
		bool b_on(false);
		if_segs >> i_segin >> b_on;
		if (i_segin >= 0) {
			if (i_segin != i_seg++) {
				std::cerr << "error: mismatch in segsonly file near " << i_segin << std::endl;
				return 1;
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
		return 1;
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
				return 1;
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
		if (t.c1==VSS) {
			t.c1 = t.c2;
			t.c2 = VSS;
		} else if (t.c1==VCC) {
			t.c1 = t.c2;
			t.c2 = VCC;
		}
		segs[t.gate].gates.push_back(i);
		segs[t.c1].c1c2s.push_back(i);
		segs[t.c2].c1c2s.push_back(i);
	}
	init();

	std::cout << "running some..." << std::endl;
	for (int i(0); i < 100; ++i) {
		step();
	}
	std::cout << "end" << std::endl;

/*dump chip
	for (std::map<int,seg>::iterator i = segs.begin(); i != segs.end(); ++i) {
		const int n = i->first;
		const seg& s = i->second;
		std::cout << "s" << n << ": " << s.pullup << "  g[";
		for (std::vector<int>::const_iterator ig = s.gates.begin(); ig != s.gates.end(); ++ig) {
			std::cout << *ig << " ";
		}
		std::cout << "]  c[";
		for (std::vector<int>::const_iterator ig = s.c1c2s.begin(); ig != s.c1c2s.end(); ++ig) {
			std::cout << *ig << " ";
		}
		std::cout << "]" << std::endl;
	}
	for (std::map<int,trn>::iterator i = trns.begin(); i != trns.end(); ++i) {
		const int n = (*i).first;
		trn& t = (*i).second;
		std::cout << "t" << n << ": " << t.gate << "->" << t.c1 << "," << t.c2 << std::endl;
	}

*/

	return 0;
}

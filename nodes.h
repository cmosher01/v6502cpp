#ifndef NODES_H
#define NODES_H

#include <string>
#include <map>

class nodes {
public:
/* power */
    int VCC;
    int VSS;

/* inputs */
    int CLK0;
    int IRQ;
    int RES;
    int NMI;
    int RDY;
    int SO;

/* data bus; */
    int DB0;
    int DB1;
    int DB3;
    int DB2;
    int DB5;
    int DB4;
    int DB7;
    int DB6;

/* address bus; */
    int AB0;
    int AB1;
    int AB2;
    int AB3;
    int AB4;
    int AB5;
    int AB6;
    int AB7;
    int AB8;
    int AB9;
    int AB12;
    int AB13;
    int AB10;
    int AB11;
    int AB14;
    int AB15;

/* outputs */
    int RW;
    int SYNC;
    int CLK1OUT;
    int CLK2OUT;

/* internal registers */
    int A0;
    int A1;
    int A2;
    int A3;
    int A4;
    int A5;
    int A6;
    int A7;

    int X0;
    int X1;
    int X2;
    int X3;
    int X4;
    int X5;
    int X6;
    int X7;

    int Y0;
    int Y1;
    int Y2;
    int Y3;
    int Y4;
    int Y5;
    int Y6;
    int Y7;

    int PCL0;
    int PCL1;
    int PCL2;
    int PCL3;
    int PCL4;
    int PCL5;
    int PCL6;
    int PCL7;

    int PCH0;
    int PCH1;
    int PCH2;
    int PCH3;
    int PCH4;
    int PCH5;
    int PCH6;
    int PCH7;

    int P0;
    int P1;
    int P2;
    int P3;
    int P4;
    int P6;
    int P7;

    int S0;
    int S1;
    int S2;
    int S3;
    int S4;
    int S5;
    int S6;
    int S7;

    nodes(std::map<std::string,int>& map_seg_i);
};

#endif

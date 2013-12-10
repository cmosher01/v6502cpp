#include "nodes.h"
#include <string>
#include <map>

nodes::nodes(std::map<std::string,int>& map_seg_i) {
/* power */
    VCC = map_seg_i["+vcc"];
    VSS = map_seg_i["-vss"];

/* inputs */
    CLK0 = map_seg_i["-clk0"];
    IRQ = map_seg_i["-irq"];
    RES = map_seg_i["-res"];
    NMI = map_seg_i["-nmi"];
    RDY = map_seg_i["+rdy"];
    SO = map_seg_i["+s0"];

/* data bus (I/O) */
    DB0 = map_seg_i["-db0"];
    DB1 = map_seg_i["-db1"];
    DB2 = map_seg_i["-db2"];
    DB3 = map_seg_i["-db3"];
    DB4 = map_seg_i["-db4"];
    DB5 = map_seg_i["-db5"];
    DB6 = map_seg_i["-db6"];
    DB7 = map_seg_i["-db7"];

/* address bus (output) */
    AB0 = map_seg_i["-ab0"];
    AB1 = map_seg_i["-ab1"];
    AB2 = map_seg_i["-ab2"];
    AB3 = map_seg_i["-ab3"];
    AB4 = map_seg_i["-ab4"];
    AB5 = map_seg_i["-ab5"];
    AB6 = map_seg_i["-ab6"];
    AB7 = map_seg_i["-ab7"];
    AB8 = map_seg_i["-ab8"];
    AB9 = map_seg_i["-ab9"];
    AB10 = map_seg_i["-ab10"];
    AB11 = map_seg_i["-ab11"];
    AB12 = map_seg_i["-ab12"];
    AB13 = map_seg_i["-ab13"];
    AB14 = map_seg_i["-ab14"];
    AB15 = map_seg_i["-ab15"];

/* outputs */
    RW = map_seg_i["-rw"];
    SYNC = map_seg_i["-sync"];
    CLK1OUT = map_seg_i["-clk1out"];
    CLK2OUT = map_seg_i["-clk2out"];

/* internal registers */
    A0 = map_seg_i["-a0"];
    A1 = map_seg_i["-a1"];
    A2 = map_seg_i["-a2"];
    A3 = map_seg_i["-a3"];
    A4 = map_seg_i["-a4"];
    A5 = map_seg_i["-a5"];
    A6 = map_seg_i["-a6"];
    A7 = map_seg_i["-a7"];

    X0 = map_seg_i["-x0"];
    X1 = map_seg_i["-x1"];
    X2 = map_seg_i["-x2"];
    X3 = map_seg_i["-x3"];
    X4 = map_seg_i["-x4"];
    X5 = map_seg_i["-x5"];
    X6 = map_seg_i["-x6"];
    X7 = map_seg_i["-x7"];

    Y0 = map_seg_i["-y0"];
    Y1 = map_seg_i["-y1"];
    Y2 = map_seg_i["-y2"];
    Y3 = map_seg_i["-y3"];
    Y4 = map_seg_i["-y4"];
    Y5 = map_seg_i["-y5"];
    Y6 = map_seg_i["-y6"];
    Y7 = map_seg_i["-y7"];

    PCL0 = map_seg_i["-pcl0"];
    PCL1 = map_seg_i["-pcl1"];
    PCL2 = map_seg_i["-pcl2"];
    PCL3 = map_seg_i["-pcl3"];
    PCL4 = map_seg_i["-pcl4"];
    PCL5 = map_seg_i["-pcl5"];
    PCL6 = map_seg_i["-pcl6"];
    PCL7 = map_seg_i["-pcl7"];

    PCH0 = map_seg_i["-pch0"];
    PCH1 = map_seg_i["-pch1"];
    PCH2 = map_seg_i["-pch2"];
    PCH3 = map_seg_i["-pch3"];
    PCH4 = map_seg_i["-pch4"];
    PCH5 = map_seg_i["-pch5"];
    PCH6 = map_seg_i["-pch6"];
    PCH7 = map_seg_i["-pch7"];

    P0 = map_seg_i["+Pout0"];
    P1 = map_seg_i["+Pout1"];
    P2 = map_seg_i["+Pout2"];
    P3 = map_seg_i["+Pout3"];
    P4 = map_seg_i["+Pout4"];
    // P5 does not exist in the 6502 chip
    P6 = map_seg_i["+Pout6"];
    P7 = map_seg_i["+Pout7"];

    S0 = map_seg_i["-s0"];
    S1 = map_seg_i["-s1"];
    S2 = map_seg_i["-s2"];
    S3 = map_seg_i["-s3"];
    S4 = map_seg_i["-s4"];
    S5 = map_seg_i["-s5"];
    S6 = map_seg_i["-s6"];
    S7 = map_seg_i["-s7"];
}

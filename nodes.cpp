#include "nodes.h"
#include <string>
#include <map>

nodes::nodes(std::map<std::string,int>& map_seg_i) {
/* power */
    isegVCC = map_seg_i["+vcc"];
    isegVSS = map_seg_i["-vss"];

/* inputs */
    isegCLK0 = map_seg_i["-clk0"];
    isegIRQ = map_seg_i["-irq"];
    isegRES = map_seg_i["-res"];
    isegNMI = map_seg_i["-nmi"];
    isegRDY = map_seg_i["+rdy"];
    isegSO = map_seg_i["+s0"];

/* data bus (I/O) */
    isegDB0 = map_seg_i["-db0"];
    isegDB1 = map_seg_i["-db1"];
    isegDB2 = map_seg_i["-db2"];
    isegDB3 = map_seg_i["-db3"];
    isegDB4 = map_seg_i["-db4"];
    isegDB5 = map_seg_i["-db5"];
    isegDB6 = map_seg_i["-db6"];
    isegDB7 = map_seg_i["-db7"];

/* address bus (output) */
    isegAB0 = map_seg_i["-ab0"];
    isegAB1 = map_seg_i["-ab1"];
    isegAB2 = map_seg_i["-ab2"];
    isegAB3 = map_seg_i["-ab3"];
    isegAB4 = map_seg_i["-ab4"];
    isegAB5 = map_seg_i["-ab5"];
    isegAB6 = map_seg_i["-ab6"];
    isegAB7 = map_seg_i["-ab7"];
    isegAB8 = map_seg_i["-ab8"];
    isegAB9 = map_seg_i["-ab9"];
    isegAB10 = map_seg_i["-ab10"];
    isegAB11 = map_seg_i["-ab11"];
    isegAB12 = map_seg_i["-ab12"];
    isegAB13 = map_seg_i["-ab13"];
    isegAB14 = map_seg_i["-ab14"];
    isegAB15 = map_seg_i["-ab15"];

/* outputs */
    isegRW = map_seg_i["-rw"];
    isegSYNC = map_seg_i["-sync"];
    isegCLK1OUT = map_seg_i["-clk1out"];
    isegCLK2OUT = map_seg_i["-clk2out"];

/* internal registers */
    isegA0 = map_seg_i["-a0"];
    isegA1 = map_seg_i["-a1"];
    isegA2 = map_seg_i["-a2"];
    isegA3 = map_seg_i["-a3"];
    isegA4 = map_seg_i["-a4"];
    isegA5 = map_seg_i["-a5"];
    isegA6 = map_seg_i["-a6"];
    isegA7 = map_seg_i["-a7"];

    isegX0 = map_seg_i["-x0"];
    isegX1 = map_seg_i["-x1"];
    isegX2 = map_seg_i["-x2"];
    isegX3 = map_seg_i["-x3"];
    isegX4 = map_seg_i["-x4"];
    isegX5 = map_seg_i["-x5"];
    isegX6 = map_seg_i["-x6"];
    isegX7 = map_seg_i["-x7"];

    isegY0 = map_seg_i["-y0"];
    isegY1 = map_seg_i["-y1"];
    isegY2 = map_seg_i["-y2"];
    isegY3 = map_seg_i["-y3"];
    isegY4 = map_seg_i["-y4"];
    isegY5 = map_seg_i["-y5"];
    isegY6 = map_seg_i["-y6"];
    isegY7 = map_seg_i["-y7"];

    isegPCL0 = map_seg_i["-pcl0"];
    isegPCL1 = map_seg_i["-pcl1"];
    isegPCL2 = map_seg_i["-pcl2"];
    isegPCL3 = map_seg_i["-pcl3"];
    isegPCL4 = map_seg_i["-pcl4"];
    isegPCL5 = map_seg_i["-pcl5"];
    isegPCL6 = map_seg_i["-pcl6"];
    isegPCL7 = map_seg_i["-pcl7"];

    isegPCH0 = map_seg_i["-pch0"];
    isegPCH1 = map_seg_i["-pch1"];
    isegPCH2 = map_seg_i["-pch2"];
    isegPCH3 = map_seg_i["-pch3"];
    isegPCH4 = map_seg_i["-pch4"];
    isegPCH5 = map_seg_i["-pch5"];
    isegPCH6 = map_seg_i["-pch6"];
    isegPCH7 = map_seg_i["-pch7"];

    isegP0 = map_seg_i["+Pout0"];
    isegP1 = map_seg_i["+Pout1"];
    isegP2 = map_seg_i["+Pout2"];
    isegP3 = map_seg_i["+Pout3"];
    isegP4 = map_seg_i["+Pout4"];
    // P5 does not exist in the 6502 chip
    isegP6 = map_seg_i["+Pout6"];
    isegP7 = map_seg_i["+Pout7"];

    isegS0 = map_seg_i["-s0"];
    isegS1 = map_seg_i["-s1"];
    isegS2 = map_seg_i["-s2"];
    isegS3 = map_seg_i["-s3"];
    isegS4 = map_seg_i["-s4"];
    isegS5 = map_seg_i["-s5"];
    isegS6 = map_seg_i["-s6"];
    isegS7 = map_seg_i["-s7"];
}

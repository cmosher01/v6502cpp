#ifndef NODES_H
#define NODES_H

#include <string>
#include <map>

class nodes {
public:
/* power */
    int isegVCC;
    int isegVSS;

/* inputs */
    int isegCLK0;
    int isegIRQ;
    int isegRES;
    int isegNMI;
    int isegRDY;
    int isegSO;

/* data bus; */
    int isegDB0;
    int isegDB1;
    int isegDB3;
    int isegDB2;
    int isegDB5;
    int isegDB4;
    int isegDB7;
    int isegDB6;

/* address bus; */
    int isegAB0;
    int isegAB1;
    int isegAB2;
    int isegAB3;
    int isegAB4;
    int isegAB5;
    int isegAB6;
    int isegAB7;
    int isegAB8;
    int isegAB9;
    int isegAB12;
    int isegAB13;
    int isegAB10;
    int isegAB11;
    int isegAB14;
    int isegAB15;

/* outputs */
    int isegRW;
    int isegSYNC;
    int isegCLK1OUT;
    int isegCLK2OUT;

/* internal registers */
    int isegA0;
    int isegA1;
    int isegA2;
    int isegA3;
    int isegA4;
    int isegA5;
    int isegA6;
    int isegA7;

    int isegX0;
    int isegX1;
    int isegX2;
    int isegX3;
    int isegX4;
    int isegX5;
    int isegX6;
    int isegX7;

    int isegY0;
    int isegY1;
    int isegY2;
    int isegY3;
    int isegY4;
    int isegY5;
    int isegY6;
    int isegY7;

    int isegPCL0;
    int isegPCL1;
    int isegPCL2;
    int isegPCL3;
    int isegPCL4;
    int isegPCL5;
    int isegPCL6;
    int isegPCL7;

    int isegPCH0;
    int isegPCH1;
    int isegPCH2;
    int isegPCH3;
    int isegPCH4;
    int isegPCH5;
    int isegPCH6;
    int isegPCH7;

    int isegP0;
    int isegP1;
    int isegP2;
    int isegP3;
    int isegP4;
    int isegP6;
    int isegP7;

    int isegS0;
    int isegS1;
    int isegS2;
    int isegS3;
    int isegS4;
    int isegS5;
    int isegS6;
    int isegS7;

    nodes(std::map<std::string,int>& map_seg_i);
};

#endif

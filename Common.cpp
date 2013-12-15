/* 
 * File:   Common.cpp
 * Author: Christopher
 * 
 * Created on December 14, 2013, 8:47 PM
 */

#include "Common.h"
#include "SegmentCache.h"
#include "trans.h"

Common Common::create(const SegmentCache& segs) {
    return Common(
            segs.get("-vss"), segs.get("+vcc"),
            segs.get("-clk0"),
            segs.get("-irq"), segs.get("-res"), segs.get("-nmi"),
            segs.get("+rdy"), segs.get("+so"),
            segs.get("-db0"), segs.get("-db1"), segs.get("-db2"), segs.get("-db3"), segs.get("-db4"), segs.get("-db5"), segs.get("-db6"), segs.get("-db7"),
            segs.get("-ab0"), segs.get("-ab1"), segs.get("-ab2"), segs.get("-ab3"), segs.get("-ab4"), segs.get("-ab5"), segs.get("-ab6"), segs.get("-ab7"),
            segs.get("-ab8"), segs.get("-ab9"), segs.get("-ab10"), segs.get("-ab11"), segs.get("-ab12"), segs.get("-ab13"), segs.get("-ab14"), segs.get("-ab15"),
            segs.get("-rw"), segs.get("-sync"),
            segs.get("-clk1out"), segs.get("-clk2out"),
            segs.get("-a0"), segs.get("-a1"), segs.get("-a2"), segs.get("-a3"), segs.get("-a4"), segs.get("-a5"), segs.get("-a6"), segs.get("-a7"),
            segs.get("-x0"), segs.get("-x1"), segs.get("-x2"), segs.get("-x3"), segs.get("-x4"), segs.get("-x5"), segs.get("-x6"), segs.get("-x7"),
            segs.get("-y0"), segs.get("-y1"), segs.get("-y2"), segs.get("-y3"), segs.get("-y4"), segs.get("-y5"), segs.get("-y6"), segs.get("-y7"),
            segs.get("-pcl0"), segs.get("-pcl1"), segs.get("-pcl2"), segs.get("-pcl3"), segs.get("-pcl4"), segs.get("-pcl5"), segs.get("-pcl6"), segs.get("-pcl7"),
            segs.get("-pch0"), segs.get("-pch1"), segs.get("-pch2"), segs.get("-pch3"), segs.get("-pch4"), segs.get("-pch5"), segs.get("-pch6"), segs.get("-pch7"),
            segs.get("+Pout0"), segs.get("+Pout1"), segs.get("+Pout2"), segs.get("+Pout3"), segs.get("+Pout4"), /*no P5 */segs.get("+Pout6"), segs.get("+Pout7"),
            segs.get("-s0"), segs.get("-s1"), segs.get("-s2"), segs.get("-s3"), segs.get("-s4"), segs.get("-s5"), segs.get("-s6"), segs.get("-s7"));
}

unsigned short Common::rAddr() const {
    return Segment::asWord(this->AB15, this->AB14, this->AB13, this->AB12, this->AB11, this->AB10, this->AB9, this->AB8, this->AB7, this->AB6, this->AB5, this->AB4, this->AB3, this->AB2, this->AB1, this->AB0);
}

unsigned char Common::rData() const {
    return Segment::asByte(this->DB7, this->DB6, this->DB5, this->DB4, this->DB3, this->DB2, this->DB1, this->DB0);
}

unsigned char Common::rA() const {
    return Segment::asByte(this->A7, this->A6, this->A5, this->A4, this->A3, this->A2, this->A1, this->A0);
}

unsigned char Common::rX() const {
    return Segment::asByte(this->X7, this->X6, this->X5, this->X4, this->X3, this->X2, this->X1, this->X0);
}

unsigned char Common::rY() const {
    return Segment::asByte(this->Y7, this->Y6, this->Y5, this->Y4, this->Y3, this->Y2, this->Y1, this->Y0);
}

unsigned char Common::rS() const {
    return Segment::asByte(this->S7, this->S6, this->S5, this->S4, this->S3, this->S2, this->S1, this->S0);
}

unsigned short Common::rPC() const {
    return Segment::asWord(this->PCH7, this->PCH6, this->PCH5, this->PCH4, this->PCH3, this->PCH2, this->PCH1, this->PCH0, this->PCL7, this->PCL6, this->PCL5, this->PCL4, this->PCL3, this->PCL2, this->PCL1, this->PCL0);
}

void Common::setDataSegs(const unsigned char data) {
    unsigned char x = data;

    this->DB0->set(x & 1);
    x >>= 1;
    this->DB1->set(x & 1);
    x >>= 1;
    this->DB2->set(x & 1);
    x >>= 1;
    this->DB3->set(x & 1);
    x >>= 1;
    this->DB4->set(x & 1);
    x >>= 1;
    this->DB5->set(x & 1);
    x >>= 1;
    this->DB6->set(x & 1);
    x >>= 1;
    this->DB7->set(x & 1);
}

void Common::addDataToRecalc(setpSeg& s) const {
    s.insert(this->DB0);
    s.insert(this->DB1);
    s.insert(this->DB2);
    s.insert(this->DB3);
    s.insert(this->DB4);
    s.insert(this->DB5);
    s.insert(this->DB6);
    s.insert(this->DB7);
}

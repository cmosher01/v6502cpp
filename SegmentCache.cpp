/* 
 * File:   SegmentCache.cpp
 * Author: cmosher
 * 
 * Created on December 10, 2013, 9:56 PM
 */

#include "SegmentCache.h"
#include <memory>
#include <string>
#include <map>

Segment* SegmentCache::getOrAdd(const std::string& id) {
    if (this->cache.find(id) == this->cache.end()) {
        this->cache[id] = std::make_shared<Segment>(id);
    }
    return get(id);
}

Segment* SegmentCache::get(const std::string& id) const {
    if (this->cache.find(id) == this->cache.end()) {
        throw "Cannot find segment: " + id;
    }
    return this->cache.at(id).get();
}

void SegmentCache::initCommon() {
    this->c = new Common(
            get("-vss"), get("+vcc"),
            get("-clk0"),
            get("-irq"), get("-res"), get("-nmi"),
            get("+rdy"), get("+so"),
            get("-db0"), get("-db1"), get("-db2"), get("-db3"), get("-db4"), get("-db5"), get("-db6"), get("-db7"),
            get("-ab0"), get("-ab1"), get("-ab2"), get("-ab3"), get("-ab4"), get("-ab5"), get("-ab6"), get("-ab7"),
            get("-ab8"), get("-ab9"), get("-ab10"), get("-ab11"), get("-ab12"), get("-ab13"), get("-ab14"), get("-ab15"),
            get("-rw"), get("-sync"),
            get("-clk1out"), get("-clk2out"),
            get("-a0"), get("-a1"), get("-a2"), get("-a3"), get("-a4"), get("-a5"), get("-a6"), get("-a7"),
            get("-x0"), get("-x1"), get("-x2"), get("-x3"), get("-x4"), get("-x5"), get("-x6"), get("-x7"),
            get("-y0"), get("-y1"), get("-y2"), get("-y3"), get("-y4"), get("-y5"), get("-y6"), get("-y7"),
            get("-pcl0"), get("-pcl1"), get("-pcl2"), get("-pcl3"), get("-pcl4"), get("-pcl5"), get("-pcl6"), get("-pcl7"),
            get("-pch0"), get("-pch1"), get("-pch2"), get("-pch3"), get("-pch4"), get("-pch5"), get("-pch6"), get("-pch7"),
            get("+Pout0"), get("+Pout1"), get("+Pout2"), get("+Pout3"), get("+Pout4"), /*no P5 */get("+Pout6"), get("+Pout7"),
            get("-s0"), get("-s1"), get("-s2"), get("-s3"), get("-s4"), get("-s5"), get("-s6"), get("-s7"));

    this->c->VSS->vss = true;
    this->c->VCC->vcc = true;
}

unsigned char SegmentCache::rData() const {
    return Segment::asByte(this->c->DB7, this->c->DB6, this->c->DB5, this->c->DB4, this->c->DB3, this->c->DB2, this->c->DB1, this->c->DB0);
}

unsigned short SegmentCache::rAddr() const {
    return Segment::asWord(this->c->AB15, this->c->AB14, this->c->AB13, this->c->AB12, this->c->AB11, this->c->AB10, this->c->AB9, this->c->AB8, this->c->AB7, this->c->AB6, this->c->AB5, this->c->AB4, this->c->AB3, this->c->AB2, this->c->AB1, this->c->AB0);
}

unsigned char SegmentCache::rA() const {
    return Segment::asByte(this->c->A7, this->c->A6, this->c->A5, this->c->A4, this->c->A3, this->c->A2, this->c->A1, this->c->A0);
}

unsigned char SegmentCache::rX() const {
    return Segment::asByte(this->c->X7, this->c->X6, this->c->X5, this->c->X4, this->c->X3, this->c->X2, this->c->X1, this->c->X0);
}

unsigned char SegmentCache::rY() const {
    return Segment::asByte(this->c->Y7, this->c->Y6, this->c->Y5, this->c->Y4, this->c->Y3, this->c->Y2, this->c->Y1, this->c->Y0);
}

unsigned char SegmentCache::rS() const {
    return Segment::asByte(this->c->S7, this->c->S6, this->c->S5, this->c->S4, this->c->S3, this->c->S2, this->c->S1, this->c->S0);
}

unsigned short SegmentCache::rPC() const {
    return Segment::asWord(this->c->PCH7, this->c->PCH6, this->c->PCH5, this->c->PCH4, this->c->PCH3, this->c->PCH2, this->c->PCH1, this->c->PCH0, this->c->PCL7, this->c->PCL6, this->c->PCL5, this->c->PCL4, this->c->PCL3, this->c->PCL2, this->c->PCL1, this->c->PCL0);
}

void SegmentCache::setDataSegs(const unsigned char data) {
    unsigned char x = data;

    this->c->DB0->set(x & 1);
    x >>= 1;
    this->c->DB1->set(x & 1);
    x >>= 1;
    this->c->DB2->set(x & 1);
    x >>= 1;
    this->c->DB3->set(x & 1);
    x >>= 1;
    this->c->DB4->set(x & 1);
    x >>= 1;
    this->c->DB5->set(x & 1);
    x >>= 1;
    this->c->DB6->set(x & 1);
    x >>= 1;
    this->c->DB7->set(x & 1);
}

void SegmentCache::addDataToRecalc(setpSeg& s) {
    s.insert(this->c->DB0);
    s.insert(this->c->DB1);
    s.insert(this->c->DB2);
    s.insert(this->c->DB3);
    s.insert(this->c->DB4);
    s.insert(this->c->DB5);
    s.insert(this->c->DB6);
    s.insert(this->c->DB7);
}

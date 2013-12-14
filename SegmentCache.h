/* 
 * File:   SegmentCache.h
 * Author: Christopher
 *
 * Created on December 10, 2013, 9:56 PM
 */

#ifndef SEGMENTCACHE_H
#define	SEGMENTCACHE_H

#include "trans.h"
#include <memory>
#include <string>
#include <map>
#include <set>

class SegmentCache {
public:

    SegmentCache() : c(nullptr) {
    }

    virtual ~SegmentCache() {
        delete this->c;
    }

    Segment* getOrAdd(const std::string& id);
    Segment* get(const std::string& id) const;

    std::set<Segment*> all() {
        std::set<Segment*> s;
        for (auto i : this->cache) {
            s.insert(i.second.get());
        }
        return s;
    }

    class Common {
    public:
        Segment* VSS;
        Segment* VCC;
        Segment* CLK0;
        Segment* IRQ;
        Segment* RES;
        Segment* NMI;
        Segment* RDY;
        Segment* SO;
        Segment* DB0;
        Segment* DB1;
        Segment* DB2;
        Segment* DB3;
        Segment* DB4;
        Segment* DB5;
        Segment* DB6;
        Segment* DB7;
        Segment* AB0;
        Segment* AB1;
        Segment* AB2;
        Segment* AB3;
        Segment* AB4;
        Segment* AB5;
        Segment* AB6;
        Segment* AB7;
        Segment* AB8;
        Segment* AB9;
        Segment* AB10;
        Segment* AB11;
        Segment* AB12;
        Segment* AB13;
        Segment* AB14;
        Segment* AB15;
        Segment* RW;
        Segment* SYNC;
        Segment* CLK1OUT;
        Segment* CLK2OUT;
        Segment* A0;
        Segment* A1;
        Segment* A2;
        Segment* A3;
        Segment* A4;
        Segment* A5;
        Segment* A6;
        Segment* A7;
        Segment* X0;
        Segment* X1;
        Segment* X2;
        Segment* X3;
        Segment* X4;
        Segment* X5;
        Segment* X6;
        Segment* X7;
        Segment* Y0;
        Segment* Y1;
        Segment* Y2;
        Segment* Y3;
        Segment* Y4;
        Segment* Y5;
        Segment* Y6;
        Segment* Y7;
        Segment* PCL0;
        Segment* PCL1;
        Segment* PCL2;
        Segment* PCL3;
        Segment* PCL4;
        Segment* PCL5;
        Segment* PCL6;
        Segment* PCL7;
        Segment* PCH0;
        Segment* PCH1;
        Segment* PCH2;
        Segment* PCH3;
        Segment* PCH4;
        Segment* PCH5;
        Segment* PCH6;
        Segment* PCH7;
        Segment* P0;
        Segment* P1;
        Segment* P2;
        Segment* P3;
        Segment* P4;
        Segment* P6;
        Segment* P7;
        Segment* S0;
        Segment* S1;
        Segment* S2;
        Segment* S3;
        Segment* S4;
        Segment* S5;
        Segment* S6;
        Segment* S7;

        Common(
                Segment* VSS, Segment* VCC,
                Segment* CLK0,
                Segment* IRQ, Segment* RES, Segment* NMI,
                Segment* RDY, Segment* SO,
                Segment* DB0, Segment* DB1, Segment* DB2, Segment* DB3, Segment* DB4, Segment* DB5, Segment* DB6, Segment* DB7,
                Segment* AB0, Segment* AB1, Segment* AB2, Segment* AB3, Segment* AB4, Segment* AB5, Segment* AB6, Segment* AB7,
                Segment* AB8, Segment* AB9, Segment* AB10, Segment* AB11, Segment* AB12, Segment* AB13, Segment* AB14, Segment* AB15,
                Segment* RW, Segment* SYNC,
                Segment* CLK1OUT, Segment* CLK2OUT,
                Segment* A0, Segment* A1, Segment* A2, Segment* A3, Segment* A4, Segment* A5, Segment* A6, Segment* A7,
                Segment* X0, Segment* X1, Segment* X2, Segment* X3, Segment* X4, Segment* X5, Segment* X6, Segment* X7,
                Segment* Y0, Segment* Y1, Segment* Y2, Segment* Y3, Segment* Y4, Segment* Y5, Segment* Y6, Segment* Y7,
                Segment* PCL0, Segment* PCL1, Segment* PCL2, Segment* PCL3, Segment* PCL4, Segment* PCL5, Segment* PCL6, Segment* PCL7,
                Segment* PCH0, Segment* PCH1, Segment* PCH2, Segment* PCH3, Segment* PCH4, Segment* PCH5, Segment* PCH6, Segment* PCH7,
                Segment* P0, Segment* P1, Segment* P2, Segment* P3, Segment* P4, /* no P5 */ Segment* P6, Segment* P7,
                Segment* S0, Segment* S1, Segment* S2, Segment* S3, Segment* S4, Segment* S5, Segment* S6, Segment* S7) :
        VSS(VSS), VCC(VCC),
        CLK0(CLK0),
        IRQ(IRQ), RES(RES), NMI(NMI),
        RDY(RDY), SO(SO),
        DB0(DB0), DB1(DB1), DB2(DB2), DB3(DB3), DB4(DB4), DB5(DB5), DB6(DB6), DB7(DB7),
        AB0(AB0), AB1(AB1), AB2(AB2), AB3(AB3), AB4(AB4), AB5(AB5), AB6(AB6), AB7(AB7),
        AB8(AB8), AB9(AB9), AB10(AB10), AB11(AB11), AB12(AB12), AB13(AB13), AB14(AB14), AB15(AB15),
        RW(RW), SYNC(SYNC),
        CLK1OUT(CLK1OUT), CLK2OUT(CLK2OUT),
        A0(A0), A1(A1), A2(A2), A3(A3), A4(A4), A5(A5), A6(A6), A7(A7),
        X0(X0), X1(X1), X2(X2), X3(X3), X4(X4), X5(X5), X6(X6), X7(X7),
        Y0(Y0), Y1(Y1), Y2(Y2), Y3(Y3), Y4(Y4), Y5(Y5), Y6(Y6), Y7(Y7),
        PCL0(PCL0), PCL1(PCL1), PCL2(PCL2), PCL3(PCL3), PCL4(PCL4), PCL5(PCL5), PCL6(PCL6), PCL7(PCL7),
        PCH0(PCH0), PCH1(PCH1), PCH2(PCH2), PCH3(PCH3), PCH4(PCH4), PCH5(PCH5), PCH6(PCH6), PCH7(PCH7),
        P0(P0), P1(P1), P2(P2), P3(P3), P4(P4), /* no P5 */ P6(P6), P7(P7),
        S0(S0), S1(S1), S2(S2), S3(S3), S4(S4), S5(S5), S6(S6), S7(S7) {
        }
        ~Common() {}

    private:
        Common(const Common&) = delete;
        Common& operator=(const Common&) = delete;
    };

    void initCommon();
    Common* c;

    unsigned char rData() const;
    unsigned short rAddr() const;
    unsigned char rA() const;
    unsigned char rX() const;
    unsigned char rY() const;
    unsigned char rS() const;
    unsigned short rPC() const;

    void setDataSegs(const unsigned char data);
    void addDataToRecalc(std::set<Segment*>& s);

private:
    std::map<const std::string, std::shared_ptr<Segment > > cache;

    SegmentCache(const SegmentCache&) = delete;
    SegmentCache& operator=(const SegmentCache&) = delete;
};

#endif	/* SEGMENTCACHE_H */

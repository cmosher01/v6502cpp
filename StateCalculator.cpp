/* 
 * File:   StateCalculator.cpp
 * Author: Christopher
 * 
 * Created on December 12, 2013, 8:29 PM
 */

#include "StateCalculator.h"
#include "Circuit.h"
#include "trans.h"
#include "cpu.h"
#include <set>

StateCalculator::StateCalculator(Segment* VSS, Segment* VCC) : VSS(VSS), VCC(VCC) {
}

//void StateCalculator::recalcAll() {
//    std::set<int> riSeg;
//    for (int iSeg = 0; iSeg < segs.size(); ++iSeg) {
//        addRecalc(iSeg,riSeg);
//    }
//    recalc(riSeg);
//}

void StateCalculator::recalc(Segment* seg, Segment* VSS, Segment* VCC) {
    std::set<Segment*> rSeg;
    rSeg.insert(seg);
    recalc(rSeg, VSS, VCC);
}

/*
 * Recalculate segment states (on/off), based on the fact that the segments
 * in riSeg have just changed state. Keep track of which other segments are
 * affected, and repeat the process on those segments. Repeat until no more
 * segments change state.
 */
#define SANE (100)

void StateCalculator::recalc(const std::set<Segment*>& segs, Segment* VSS, Segment* VCC) {
    int sanity(0);

    std::set<Segment*> changed(segs);
    while (!changed.empty()) {
        if (++sanity >= SANE) {
            throw "ERROR: reached maximum iteration limit while recalculating CPU state";
        }

        StateCalculator c(VSS, VCC);
        for (auto s : changed) {
            c.recalcNode(s);
        }
        changed = c.getChanged();
    }
}

/*
 * Gets group of segments currently electrically connected to iSeg,
 * gets what their group value is (or should be), goes through all
 * those segments and sets their "on" value. For all connected gates,
 * turn on/off, and indicate that the segments connected to those
 * transistors' source and drain legs have changed, by adding them
 * to riSegChanged.
 */
void StateCalculator::recalcNode(Segment* seg) {
    if (!(seg == this->VSS || seg == this->VCC)) {
        Circuit c(seg, this->VSS, this->VCC);
        for (auto s : c) {
            setSeg(s, c.getValue());
        }
    }
}

void StateCalculator::setSeg(Segment* s, const bool on) {
    if (s->on != on) {
        s->on = on;
        for (auto t : s->gates) {
            setTrans(t, on);
        }
    }
}

void StateCalculator::setTrans(Trans* t, const bool on) {
    if (t->on != on) {
        t->on = on;
        addRecalc(t->c1);
        addRecalc(t->c2);
    }
}

void StateCalculator::addRecalc(Segment* seg) {
    if (!(seg == this->VSS || seg == this->VCC)) {
        this->segs.insert(seg);
    }
}

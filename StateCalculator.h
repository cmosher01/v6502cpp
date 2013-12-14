/* 
 * File:   StateCalculator.h
 * Author: Christopher
 *
 * Created on December 12, 2013, 8:29 PM
 */

#ifndef STATECALCULATOR_H
#define	STATECALCULATOR_H

#include <set>
#include "trans.h"

class StateCalculator {
public:

    static void recalc(const setpSeg& rSeg);
    static void recalc(Segment* seg); // convenience method

private:

    StateCalculator() {
    }

    virtual ~StateCalculator() {
    }

    StateCalculator(const StateCalculator&) = delete;
    StateCalculator& operator=(const StateCalculator&) = delete;

    void recalcNode(Segment* seg);
    void setSeg(Segment* s, const bool on);
    void setTrans(Trans* t, const bool on);
    void addRecalc(Segment* seg);

    setpSeg segs;
};

#endif	/* STATECALCULATOR_H */

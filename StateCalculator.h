/* 
 * File:   StateCalculator.h
 * Author: Christopher
 *
 * Created on December 12, 2013, 8:29 PM
 */

#ifndef STATECALCULATOR_H
#define	STATECALCULATOR_H

#include <set>

class Segment;
class Trans;

class StateCalculator {
public:
    static void recalc(const std::set<Segment*>& rSeg, Segment* VSS, Segment* VCC);
    static void recalc(Segment* seg, Segment* VSS, Segment* VCC);

private:
    std::set<Segment*> segs;
    Segment* VSS;
    Segment* VCC;

    StateCalculator(Segment* VSS, Segment* VCC);

    virtual ~StateCalculator() {
    }

    StateCalculator(const StateCalculator&);
    StateCalculator& operator=(const StateCalculator&);

    void recalcNode(Segment* seg);
    void setSeg(Segment* s, const bool on);
    void setTrans(Trans* t, const bool on);
    void addRecalc(Segment* seg);

    std::set<Segment*> getChanged() {
        return this->segs;
    }
};

#endif	/* STATECALCULATOR_H */

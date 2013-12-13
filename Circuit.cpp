/* 
 * File:   Circuit.cpp
 * Author: Christopher
 * 
 * Created on December 12, 2013, 7:04 PM
 */

#include "Circuit.h"
#include "trans.h"

Circuit::Circuit(Segment* extendFrom, Segment* VSS, Segment* VCC) : VSS(VSS), VCC(VCC) {
    extend(extendFrom);
}

/*
 * Adds segment extendFrom, and all segments electrically connected to it.
 * This happens recursively, but we don't recurse past ground or voltage supply.
 */
void Circuit::extend(Segment* extendFrom) {
    const std::pair < std::set<Segment*>::iterator, bool> ret = this->segs.insert(extendFrom);
    if (!ret.second) {
        return;
    }
    if (extendFrom == this->VCC || extendFrom == this->VSS) {
        return;
    }

    /*
     * For every ON transistor this seg is connected to via a leg (source or
     * drain), add the seg that's connected to the OTHER leg of the transistor.
     * This is a RECURSIVE addition.
     * 
     * Also note that, upon system startup, all transistors are initialized
     * to OFF, so at the time of the very first recalcAll call, *no* other
     * segments will be added here.
     */
    for (std::set<Trans*>::const_iterator iTrn = extendFrom->c1c2s.begin(); iTrn != extendFrom->c1c2s.end(); ++iTrn) {
        const Trans * t(*iTrn);
        if (t->on) {
            if (t->c1 == extendFrom) {
                extend(t->c2);
            } else if (t->c2 == extendFrom) {
                extend(t->c1);
            }
        }
    }
}

/*
 * Upon system startup, this will return the "pullup" value of
 * each segment, except for VCC and VSS (which will of course be
 * ON and OFF respectively).
 */
bool Circuit::getValue() {
    /* If group contains ground, it's OFF, */
    if (contains(this->VSS)) {
        return false;
    }/* otherwise, if group contains voltage supply, it's ON. */
    else if (contains(this->VCC)) {
        return true;
    }



    for (std::set<Segment*>::const_iterator iSeg = this->segs.begin(); iSeg != this->segs.end(); ++iSeg) {
        Segment * s(*iSeg);
        if (s->pullup) {
            return true;
        }
        if (s->pulldown) {
            return false;
        }
        if (s->on) {
            return true;
        }
    }
    return false;
}

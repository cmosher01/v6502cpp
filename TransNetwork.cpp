/* 
 * File:   TransNetwork.cpp
 * Author: cmosher
 * 
 * Created on December 11, 2013, 10:44 AM
 */

#include "TransNetwork.h"
#include "SegmentCache.h"
#include "StateCalculator.h"
#include "trans.h"
#include <iostream>
#include <set>
#include <string>
#include <memory>

TransNetwork::TransNetwork(std::istream& in, SegmentCache& segs) : segs(segs) {
    std::string c1, gate, c2;
    in >> c1 >> gate >> c2;
    while (in.good()) {
        this->transes.insert(std::make_shared<Trans>(this->segs.getOrAdd(c1), this->segs.getOrAdd(gate), this->segs.getOrAdd(c2)));
        in >> c1 >> gate >> c2;
    }

    StateCalculator::recalc(this->segs.all());
}

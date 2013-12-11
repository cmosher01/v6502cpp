/* 
 * File:   TransNetwork.cpp
 * Author: cmosher
 * 
 * Created on December 11, 2013, 10:44 AM
 */

#include "TransNetwork.h"
#include "trans.h"
#include "SegmentCache.h"
#include <iostream>

TransNetwork::TransNetwork(std::istream& readFromHere) {
    SegmentCache segs;

    std::string c1, gate, c2;
    readFromHere >> c1 >> gate >> c2;
    while (readFromHere.good()) {
        Trans trans(segs.getOrAdd(c1), segs.getOrAdd(gate), segs.getOrAdd(c2));
        std::cout << trans;

        readFromHere >> c1 >> gate >> c2;
    }
    std::cout << std::endl << "done" << std::endl;
}

TransNetwork::~TransNetwork() {
}

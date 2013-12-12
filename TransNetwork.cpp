/* 
 * File:   TransNetwork.cpp
 * Author: cmosher
 * 
 * Created on December 11, 2013, 10:44 AM
 */

#include "TransNetwork.h"
#include "trans.h"
#include <memory>
#include <iostream>

TransNetwork::TransNetwork(std::istream& in) {
    std::string c1, gate, c2;
    in >> c1 >> gate >> c2;
    while (in.good()) {
        this->transes.insert(std::make_shared<Trans>(this->segs.getOrAdd(c1), this->segs.getOrAdd(gate), this->segs.getOrAdd(c2)));
        in >> c1 >> gate >> c2;
    }

    this->segs.initCommon();
}

TransNetwork::~TransNetwork() {
}

/* 
 * File:   TransNetwork.h
 * Author: cmosher
 *
 * Created on December 11, 2013, 10:44 AM
 */

#ifndef TRANSNETWORK_H
#define	TRANSNETWORK_H

#include "trans.h"
#include "SegmentCache.h"
#include <istream>

class TransNetwork {
public:
    SegmentCache segs;
    std::set<std::shared_ptr<Trans>> transes;

public:
    TransNetwork(std::istream& readFromHere);
    virtual ~TransNetwork();

private:
    TransNetwork(const TransNetwork&) = delete;
    TransNetwork& operator=(const TransNetwork&) = delete;
};

#endif	/* TRANSNETWORK_H */

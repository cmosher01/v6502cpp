/* 
 * File:   TransNetwork.h
 * Author: cmosher
 *
 * Created on December 11, 2013, 10:44 AM
 */

#ifndef TRANSNETWORK_H
#define	TRANSNETWORK_H

class Trans;
#include "SegmentCache.h"
#include <set>
#include <memory>
#include <istream>

class TransNetwork {
public:

    SegmentCache segs;

    TransNetwork(std::istream& readFromHere);
    virtual ~TransNetwork();

private:

    TransNetwork(const TransNetwork&) = delete;
    TransNetwork& operator=(const TransNetwork&) = delete;

    std::set<std::shared_ptr<Trans >> transes;
};

#endif	/* TRANSNETWORK_H */

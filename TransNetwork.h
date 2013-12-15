/* 
 * File:   TransNetwork.h
 * Author: cmosher
 *
 * Created on December 11, 2013, 10:44 AM
 */

#ifndef TRANSNETWORK_H
#define	TRANSNETWORK_H

class Trans;
#include <set>
#include <memory>
#include <istream>

class SegmentCache;
class Common;

class TransNetwork {
public:

    TransNetwork(std::istream& readFromHere, SegmentCache& segs);
    virtual ~TransNetwork();

private:

    TransNetwork(const TransNetwork&) = delete;
    TransNetwork& operator=(const TransNetwork&) = delete;

    SegmentCache& segs;
    std::set<std::shared_ptr<Trans >> transes;

    friend Common;
};

#endif	/* TRANSNETWORK_H */

/* 
 * File:   TransNetwork.h
 * Author: cmosher
 *
 * Created on December 11, 2013, 10:44 AM
 */

#ifndef TRANSNETWORK_H
#define	TRANSNETWORK_H

#include <istream>
#include <set>
#include <memory>

class SegmentCache;
class Common;
class Trans;

class TransNetwork final {
public:

    TransNetwork(std::istream& readFromHere, SegmentCache& segs);

private:

    TransNetwork(const TransNetwork&) = delete;
    TransNetwork& operator=(const TransNetwork&) = delete;

    SegmentCache& segs;
    std::set<std::shared_ptr<Trans>> transes;

    friend Common;
};

#endif	/* TRANSNETWORK_H */

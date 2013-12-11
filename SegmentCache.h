/* 
 * File:   SegmentCache.h
 * Author: Christopher
 *
 * Created on December 10, 2013, 9:56 PM
 */

#ifndef SEGMENTCACHE_H
#define	SEGMENTCACHE_H

#include "trans.h"
#include <memory>
#include <string>
#include <map>

class SegmentCache {
public:
    SegmentCache() {}
    virtual ~SegmentCache() {}

    Segment* getOrAdd(std::string& id);

private:
    std::map<std::string,std::shared_ptr<Segment>> cache;

    SegmentCache(const SegmentCache&);
    SegmentCache& operator=(const SegmentCache&);
};

#endif	/* SEGMENTCACHE_H */

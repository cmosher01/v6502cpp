/* 
 * File:   SegmentCache.cpp
 * Author: cmosher
 * 
 * Created on December 10, 2013, 9:56 PM
 */

#include "SegmentCache.h"
#include <string>
#include <map>
#include <memory>
#include <algorithm>

Segment* SegmentCache::getOrAdd(std::string& id) {
    if (this->cache.find(id) == this->cache.end()) {
        this->cache[id] = std::make_shared<Segment>(id);
    }
    return this->cache[id].get();
}

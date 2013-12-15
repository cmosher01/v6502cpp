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
#include <set>

class Common;

class SegmentCache {
public:

    SegmentCache() : c(nullptr) {
    }

    virtual ~SegmentCache();

    Segment* getOrAdd(const std::string& id);

    setpSeg all() const {
        setpSeg s;
        for (auto i : this->cache) {
            s.insert(i.second.get());
        }
        return s;
    }


    unsigned char rData() const;
    unsigned short rAddr() const;
    unsigned char rA() const;
    unsigned char rX() const;
    unsigned char rY() const;
    unsigned char rS() const;
    unsigned short rPC() const;

    void setDataSegs(const unsigned char data);
    void addDataToRecalc(setpSeg& s) const;

    std::map<const std::string, std::shared_ptr<Segment > >::const_iterator begin() const {
        return this->cache.begin();
    }

    std::map<const std::string, std::shared_ptr<Segment > >::const_iterator end() const {
        return this->cache.end();
    }

private:
    std::map<const std::string, std::shared_ptr<Segment > > cache;

    SegmentCache(const SegmentCache&) = delete;
    SegmentCache& operator=(const SegmentCache&) = delete;

    Segment* get(const std::string& id) const;

public:
    void initCommon();
    Common* c;
};

#endif	/* SEGMENTCACHE_H */

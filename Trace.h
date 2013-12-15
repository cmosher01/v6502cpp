/* 
 * File:   Trace.h
 * Author: cmosher
 *
 * Created on December 12, 2013, 3:39 PM
 */

#ifndef TRACE_H
#define	TRACE_H

class SegmentCache;
class Common;

class Trace final {
public:

    Trace(const SegmentCache& segs, const Common& common) : segs(segs), common(common) {
    }

    void dumpSegments() const;
    void dumpRegisters() const;

private:

    Trace(const Trace&) = delete;
    Trace& operator=(const Trace&) = delete;

    const SegmentCache& segs;
    const Common& common;
};

#endif	/* TRACE_H */

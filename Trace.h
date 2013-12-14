/* 
 * File:   Trace.h
 * Author: cmosher
 *
 * Created on December 12, 2013, 3:39 PM
 */

#ifndef TRACE_H
#define	TRACE_H

class SegmentCache;

class Trace {
public:
    Trace(const SegmentCache& s) : s(s) {}
    virtual ~Trace() {}

    void dumpSegments() const;
    void dumpRegisters() const;

private:
    Trace(const Trace&) = delete;
    Trace& operator=(const Trace&) = delete;

    const SegmentCache& s;
};

#endif	/* TRACE_H */

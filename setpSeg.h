/* 
 * File:   setpSeg.h
 * Author: Christopher
 *
 * Created on December 15, 2013, 12:16 AM
 */

#ifndef SETPSEG_H
#define	SETPSEG_H

#include "ptr_less.h"
#include <set>
class Segment;

typedef std::set<Segment*,ptr_less<Segment>> setpSeg;

#endif	/* SETPSEG_H */


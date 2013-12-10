/* 
 * File:   trans.h
 * Author: cmosher
 *
 * Created on December 10, 2013, 2:37 PM
 */

#ifndef TRANS_H
#define	TRANS_H

#include <string>
#include <vector>

class Trans;

class Segment {
public:
    std::string id;
    std::vector<Trans*> gates;
    std::vector<Trans*> c1c2s;

    bool pullup;
    bool pulldown;
    bool on;
};

class Trans {
public:
    Segment* c1;
    Segment* gate;
    Segment* c2;

    bool on;
};

#endif	/* TRANS_H */

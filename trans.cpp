/* 
 * File:   Trans.cpp
 * Author: cmosher
 * 
 * Created on December 11, 2013, 10:44 AM
 */

#include "trans.h"

unsigned char Segment::asByte(Segment* b7, Segment* b6, Segment* b5, Segment* b4, Segment* b3, Segment* b2, Segment* b1, Segment* b0) {
    return
        b7->on << 0x7 |
        b6->on << 0x6 |
        b5->on << 0x5 |
        b4->on << 0x4 |
        b3->on << 0x3 |
        b2->on << 0x2 |
        b1->on << 0x1 |
        b0->on;
}

unsigned short Segment::asWord(Segment* bf, Segment* be, Segment* bd, Segment* bc, Segment* bb, Segment* ba, Segment* b9, Segment* b8, Segment* b7, Segment* b6, Segment* b5, Segment* b4, Segment* b3, Segment* b2, Segment* b1, Segment* b0) {
    return
        bf->on << 0xf |
        be->on << 0xe |
        bd->on << 0xd |
        bc->on << 0xc |
        bb->on << 0xb |
        ba->on << 0xa |
        b9->on << 0x9 |
        b8->on << 0x8 |
        b7->on << 0x7 |
        b6->on << 0x6 |
        b5->on << 0x5 |
        b4->on << 0x4 |
        b3->on << 0x3 |
        b2->on << 0x2 |
        b1->on << 0x1 |
        b0->on;
}

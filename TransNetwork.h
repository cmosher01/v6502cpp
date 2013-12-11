/* 
 * File:   TransNetwork.h
 * Author: cmosher
 *
 * Created on December 11, 2013, 10:44 AM
 */

#ifndef TRANSNETWORK_H
#define	TRANSNETWORK_H

#include <istream>

class TransNetwork {
public:
    TransNetwork(std::istream& readFromHere);
    virtual ~TransNetwork();

private:
    TransNetwork(const TransNetwork&);
    TransNetwork& operator=(const TransNetwork&);
};

#endif	/* TRANSNETWORK_H */

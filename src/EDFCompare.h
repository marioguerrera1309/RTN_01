#ifndef __EDF_COMPARE_H
#define __EDF_COMPARE_H


#include <omnetpp.h>

using namespace omnetpp;

class EDFCompare : public cObject{
public:
    virtual int compare (cPacket *a, cPacket *b) const;
};

#endif

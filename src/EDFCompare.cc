#include "EthernetFrame_m.h"

cPacketQueue txQueue;

class EDFCompare : public cCompareFunction {
public:
    int compare(cObject *a, cObject *b) override {
        MyPacket *pa = check_and_cast<MyPacket *>(a);
        MyPacket *pb = check_and_cast<MyPacket *>(b);
        if (pa->getDeadlineAbs() < pb->getDeadlineAbs())
            return -1;
        else if (pa->getDeadlineAbs() > pb->getDeadlineAbs())
            return 1;
        else
            return 0;
    }
};
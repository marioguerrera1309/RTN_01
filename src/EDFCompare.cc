#include "EDFCompare.h"

int EDFCompare::compare(cPacket *a, cPacket*b) {
        DataPacket *daPacket = dynamic_cast<DataPacket *>(a);
        DataPacket *dbPacket = dynamic_cast<DataPacket *>(b);
        simtime_t da = daPacket->getDeadlineAbs();
        simtime_t db = dbPacket->getDeadlineAbs();
        if(da < db) {
            return -1; // a is less than b
        } else if(da > db) {
            return 1; // a is greater than b
        } else {
            return 0; // a is equal to b
        }
    }
Register_Class(EDFCompare);
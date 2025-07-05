#include "RelayUnit.h"

Define_Module(RelayUnit);

void RelayUnit::initialize()
{

}

void RelayUnit::handleMessage(cMessage *msg)
{
    int inIndex = msg->getArrivalGate()->getIndex();
    
    for (int i = 0; i < gateSize("portGatesOut"); i++) {
        if (i != inIndex) {
            send(msg->dup(), "portGatesOut", i);
        }
    }
    delete msg;
}
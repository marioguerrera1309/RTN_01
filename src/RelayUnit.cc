#include "RelayUnit.h"
#include "EthernetFrame_m.h"
Define_Module(RelayUnit);

void RelayUnit::initialize()
{

}

void RelayUnit::handleMessage(cMessage *msg)
{
    int inIndex = msg->getArrivalGate()->getIndex();

    for (int i = 0; i < gateSize("portGatesOut"); i++) {
        if (i != inIndex) {
            EthTransmitReq *req = check_and_cast<EthTransmitReq *>(msg->getControlInfo());
            cMessage *dupMsg = check_and_cast<cMessage *>(msg->dup());
            if (msg->getControlInfo())
                dupMsg->setControlInfo(msg->getControlInfo()->dup());
            send(dupMsg, "portGatesOut", i);
        }
    }
    delete msg;
}
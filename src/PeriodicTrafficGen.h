#ifndef __RTN_01_PERIODICTRAFFICGEN_H_
#define __RTN_01_PERIODICTRAFFICGEN_H_

#include <omnetpp.h>

using namespace omnetpp;
using namespace std;

class PeriodicTrafficGen : public cSimpleModule {
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitPacket();
    virtual void finish() override;
    simtime_t period;
    simtime_t startTime;
    string name;
    unsigned long long payloadSize;
    unsigned int burstSize;
    string destAddr;
    string srcAddr;
    int vlanid;
    simtime_t deadlineRel;
    simtime_t deadlineAbs;
    int frameRicevute;
    int frameInviate;
};

#endif

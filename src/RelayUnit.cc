#include "RelayUnit.h"
#include "EthernetFrame_m.h"
#include <map>

Define_Module(RelayUnit);

void RelayUnit::initialize()
{
    forwardingTable.clear();
}

void RelayUnit::handleMessage(cMessage *msg)
{
    int inPort = msg->getArrivalGate()->getIndex();
    cPacket *pkt = check_and_cast<cPacket *>(msg);

    EthTransmitReq *req = check_and_cast<EthTransmitReq *>(pkt->getControlInfo());
    const char* src = req->getSrc();
    const char* dest = req->getDst();

    //EV << "RelayUnit " << getParentModule()->getName() << " : Pacchetto da " << src << " a " << dest << " ricevuto sulla porta " << inPort << "\n";

    forwardingTable[src] = inPort;

    auto it = forwardingTable.find(dest);
    if (it != forwardingTable.end()) {
        int outPort = it->second;
        if (outPort != inPort) {
            //EV << "Inoltro diretto sulla porta " << outPort << "\n";
            send(pkt, "portGatesOut", outPort);
        } else {
            EV << "Pacchetto tornerebbe alla porta di origine, scartato\n";
            delete pkt;
        }
    } else {
        //EV << "Destinazione sconosciuta. Broadcasting su tutte le porte tranne " << inPort << "\n";
        for (int i = 0; i < gateSize("portGatesOut"); i++) {
            if (i != inPort) {
                cPacket *copy = check_and_cast<cPacket *>(pkt->dup());
                if (pkt->getControlInfo()) {
                    copy->setControlInfo(pkt->getControlInfo()->dup());
                } else {
                    error("Il pacchetto in arrivo non ha un ControlInfo");
                }
                send(copy, "portGatesOut", i);
            }
        }
        delete pkt;
    }
}
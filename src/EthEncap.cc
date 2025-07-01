#include "EthEncap.h"
#include "EthernetFrame_m.h"

Define_Module(EthEncap);

void EthEncap::initialize()
{
    address = par("address").str();
}

void EthEncap::handleMessage(cMessage *msg)
{
    if(msg->getArrivalGate() == gate("upperLayerIn")) {
        /* Gestire il messaggio arrivato dai livelli superiori */
        /* Inoltrarlo alla rete incapsulando il payload in un EthernetFrame */
        cPacket *payload = check_and_cast<cPacket *>(msg);
        EthernetFrame *frame = new EthernetFrame();
        frame->setSrc(address.c_str());
        EthTransmitReq *req = check_and_cast<EthTransmitReq *>(msg->getControlInfo());
        frame->setDst(req->getDst());
        frame->setControlInfo(req->dup());
        frame->encapsulate(payload);
        send(frame, "lowerLayerOut");
        //EV<< "EthEncap:Inviato frame Ethernet con destinazione " << frame->getDst() << " e sorgente " << frame->getSrc() << endl;
        //EV << "EthEncap->ControlInfo: " << payload->getControlInfo() << endl;
    }
    /* Qui gestire il messaggio arrivato dalla rete */
    /* Filtrarlo se non è destinato al nodo altrimenti
     * inviare solo il payload ai livelli superiori */
    else if(msg->getArrivalGate() == gate("lowerLayerIn")) {
        EthernetFrame *frame = dynamic_cast<EthernetFrame *>(msg);
        if(frame->getDst() == address) {
            cPacket *payload = frame->decapsulate();
            EthTransmitReq *req = new EthTransmitReq();
            req->setSrc(frame->getSrc());
            req->setDst(frame->getDst());
            if (payload->getControlInfo() != nullptr)
                delete payload->removeControlInfo();
            payload->setControlInfo(req);
            //EV << "EthEncap:Ricevuto frame Ethernet con destinazione " << frame->getDst() << " e sorgente " << frame->getSrc() << endl;
            send(payload, "upperLayerOut");
            delete frame;
        } else {
            delete frame;
        }
    }
}

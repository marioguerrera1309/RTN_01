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
        EthTransmitReq *req = check_and_cast<EthTransmitReq *>(msg->getControlInfo());
        //EV << "EthEncap: Ricevuto EthTransmitReq con destinazione " << req->getDst() << " e sorgente " << req->getSrc() << " e vlanId " << req->getVlanid() << endl;
        EthernetFrame *frame = new EthernetFrame("EthernetFrame");
        if(req->getVlanid() != -1) {
            EthernetQFrame *qframe = new EthernetQFrame("EthernetQFrame");
            qframe->setVlanid(req->getVlanid());
            qframe->setSrc(req->getSrc());
            qframe->setDst(req->getDst());
            qframe->encapsulate(payload);
            frame = qframe;
            //EV << "EthEncap: Inviato frame EthernetQ con destinazione " << qframe->getDst() << " e sorgente " << qframe->getSrc() << endl;
        } else {
            frame->setSrc(req->getSrc());
            frame->setDst(req->getDst());
            frame->encapsulate(payload);
            //EV<< "EthEncap: Inviato frame Ethernet con destinazione " << frame->getDst() << " e sorgente " << frame->getSrc() << endl;
        }
        send(frame->dup(), "lowerLayerOut");
        delete frame; // Deleting the original frame after sending
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

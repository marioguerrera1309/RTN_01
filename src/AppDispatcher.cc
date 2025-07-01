#include "AppDispatcher.h"
#include "EthernetFrame_m.h"

Define_Module(AppDispatcher);

void AppDispatcher::initialize()
{
    // TODO - Generated method body
}

void AppDispatcher::handleMessage(cMessage *msg)
{
    /*
     * Riceve un pacchetto da un'applicazione,
     * lo inoltra a lowerLayerOut.
     *
     * Riceve un pacchetto dalla rete, lo inoltra
     * a tutti gli upperLayerOut. I gate superiori sono
     * un vettore di gate.
     *
     * Inoltrare ad ogni gate superiore solo il duplicato.
     */
    //messaggio in arrivo dalla rete
    if(msg->getArrivalGate() == gate("lowerLayerIn")) {
        //inoltro a tutti gli upperLayerOut
        EthTransmitReq *req = check_and_cast<EthTransmitReq *>(msg->getControlInfo());
        //EV << "Messaggio in arrivo dalla rete per AppDispatcher->ControlInfo-> src:  " << req->getSrc() << " , dst:" << req->getDst() << endl;
        for (int i = 0; i < gateSize("upperLayerOut"); i++) {
            cPacket *dupMsg = check_and_cast<cPacket *>(msg->dup());
            if (msg->getControlInfo())
                dupMsg->setControlInfo(msg->getControlInfo()->dup());
            else {
                error("Il messaggio in arrivo dalla rete non ha un ControlInfo");
            }
            //EV<< "AppDispatcher: Inoltro a upperLayerOut[" << i << "]" << endl;
            send(dupMsg, "upperLayerOut", i);
        }
        delete msg;
    }
    //messaggio in arrivo da un'applicazione
    else if (strcmp(msg->getArrivalGate()->getBaseName(), "upperLayerIn")==0) {
        // inoltro a lowerLayerOut
        EthTransmitReq *req = check_and_cast<EthTransmitReq *>(msg->getControlInfo());
        //EV << "Messaggio in arrivo da un' app per AppDispatcher->ControlInfo-> src:  " << req->getSrc() << " , dst:" << req->getDst() << endl;
        cPacket *dupMsg = check_and_cast<cPacket *>(msg->dup());
        if (msg->getControlInfo())
            dupMsg->setControlInfo(msg->getControlInfo()->dup());
        else {
            error("Il messaggio in arrivo dall'applicazione non ha un ControlInfo");
        }
        send(dupMsg, "lowerLayerOut");
        delete msg;
    }
}

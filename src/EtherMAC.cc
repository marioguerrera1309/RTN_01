#include "EtherMAC.h"
#include "EthernetFrame_m.h"
#include "AppPackets_m.h"

Define_Module(EtherMAC);

int EDFCompare(cObject *a, cObject *b) {
    EthernetFrame *fa= check_and_cast<EthernetFrame *>(a);
    EthernetFrame *fb= check_and_cast<EthernetFrame *>(b);
    DataPacket *pa = check_and_cast<DataPacket *>(fa->getEncapsulatedPacket());
    DataPacket *pb = check_and_cast<DataPacket *>(fb->getEncapsulatedPacket());
    simtime_t ta = pa->getDeadlineAbs();
    simtime_t tb = pb->getDeadlineAbs();
    if (ta < tb) return -1;
    if (ta > tb) return 1;
    return 0;
}
void EtherMAC::initialize()
{
    txstate = TX_STATE_IDLE;
    rxbuf = nullptr;
    datarate = par("datarate");
    if(strcmp(getParentModule()->getName(),"ethController")==0) {
        sprintf(nomeCoda, "txqueue_%s", getParentModule()->getParentModule()->getName()); 
    } else {
        sprintf(nomeCoda, "txqueue_%s_%d", getParentModule()->getName(), getIndex());
    }
    //EV<< "Inizializzo la coda di trasmissione: " << nomeCoda << endl;
    txqueue = cPacketQueue(nomeCoda, &EDFCompare);
    //txqueue = cPacketQueue(nomeCoda);
    ifgdur = 96.0/(double)datarate;
    cValueArray *vlanArray = check_and_cast<cValueArray*>(par("vlans").objectValue());
    for (int i = 0; i < vlanArray->size(); ++i) {
        vlans.push_back((int)vlanArray->get(i).intValue());
    }
}

void EtherMAC::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()) {
        if(strcmp(msg->getName(), "TxTimer") == 0) {
            delete msg;
            cMessage *ifgtim = new cMessage("IFGTimer");
            scheduleAt(simTime()+ifgdur, ifgtim);
            txstate = TX_STATE_IFG;
        } else if(strcmp(msg->getName(), "IFGTimer") == 0) {
            delete msg;
            startTransmission();
        } else if(strcmp(msg->getName(), "RxTimer") == 0) {
            delete msg;
            if(rxbuf->hasBitError()) {
                EV_DEBUG << "Ricevuta frame errata la rimuovo!" << endl;
                delete rxbuf;
                rxbuf = nullptr;
                return;
            }

            send(rxbuf, "upperLayerOut");
            rxbuf = nullptr;
        }
        return;
    }

    cPacket *pkt = check_and_cast<cPacket *>(msg);
    
    if(pkt->getArrivalGate() == gate("upperLayerIn")) {
        if(vlanFilter(pkt)) {
            //EV << "VlanId non registrato. Pacchetto scartato" << endl;
            delete msg;
            return;
        }
        //EV << "EtherMac->ControlInfo: " << pkt->getControlInfo() << endl;
        /*
        if(txqueue.getLength() > 100) {
            EV << "Coda di trasmissione piena, pacchetto scartato!" << endl;
            delete pkt;
            return;
        }
        */
        txqueue.insert(pkt);
        if(txqueue.getLength() > 1) {
            EV << nomeCoda << " dimensione coda: " << txqueue.getLength() <<endl;
        }
        simsignal_t sig = registerSignal("lenghtQueue");
        emit(sig, txqueue.getLength());
        if(txstate == TX_STATE_IDLE) {
            startTransmission();
        }
        return;
    }

    //Arriva da channelIn
    if(rxbuf != nullptr) {
        error("Non possono esserci due ricezioni contemporanee!");
    }
    rxbuf = pkt;
    simtime_t rxdur = (double)pkt->getBitLength()/(double)datarate;
    cMessage *rxtim = new cMessage("RxTimer");
    scheduleAt(simTime()+rxdur, rxtim);
}

/**
 * Ritorna true se la frame deve essere droppata
 * a causa del vlan id, altrimenti false
 */
bool EtherMAC::vlanFilter(cPacket *pkt) {
    if(vlans.size() == 0) {
        return false;
    }

    EthernetQFrame *qf = dynamic_cast<EthernetQFrame *>(pkt);
    if(qf == nullptr) {
        return false;
    }

    for(int i=0; i<vlans.size(); i++) {
        if(vlans[i] == qf->getVlanid()) {
            return false;
        }
    }

    return true;

}

void EtherMAC::startTransmission() {
    if(txqueue.getLength() == 0) {
        txstate = TX_STATE_IDLE;
        return;
    }
    cPacket *pkt = txqueue.pop();
    //EV << nomeCoda << " dimensione coda: " << txqueue.getLength() <<endl;
    simsignal_t sig = registerSignal("lenghtQueue");
    emit(sig, txqueue.getLength());
    simtime_t txdur = (double)pkt->getBitLength()/(double)datarate;
    //EthTransmitReq *req = check_and_cast<EthTransmitReq *>(pkt->getControlInfo());
    //EV<< "EtherMAC: Inizio trasmissione pacchetto con destinazione " << req->getDst() << endl;
    send(pkt->dup(), "channelOut");
    delete pkt;
    cMessage *txtim = new cMessage("TxTimer");
    scheduleAt(simTime()+txdur, txtim);
    txstate = TX_STATE_TX;
}

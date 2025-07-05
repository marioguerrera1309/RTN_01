#include "PeriodicTrafficGen.h"
#include "AppPackets_m.h"
#include "EthernetFrame_m.h"

Define_Module(PeriodicTrafficGen);

void PeriodicTrafficGen::initialize()
{
    period = par("period");
    startTime = par("startTime");
    name = par("name").str();
    payloadSize = par("payloadSize");
    burstSize = par("burstSize");
    destAddr = par("destAddr").str();
    srcAddr = par("srcAddr").str();
    vlanid = par("vlanid");
    if(startTime > 0) {
        cMessage *timer = new cMessage("TxTimer");
        scheduleAt(startTime, timer);
    }
}

void PeriodicTrafficGen::handleMessage(cMessage *msg)
{
    //EV << "Ricevuto messaggio: " << msg->getName() << endl;
    if(msg->isSelfMessage()) {
        if(strcmp(msg->getName(), "TxTimer") == 0) {
            transmitPacket();
            scheduleAt(simTime()+par("period"), msg);
            return;
        }

        error("E' arrivato un self message non previsto");
    }

    DataPacket *pkt = check_and_cast<DataPacket *>(msg);
    //EthTransmitReq *req = check_and_cast<EthTransmitReq *>(pkt->getControlInfo());
    /*
    if(strcmp(req->getDst(), name.c_str()) != 0) {
        delete pkt;
        return;
    }
    */
    //EV << "EndNode: " << name.c_str() <<" Arrivato pacchetto no. " << pkt->getPktNumber() << ", di " << pkt->getBurstSize() << " questo pacchetto con deadlineAbs " << pkt->getDeadlineAbs() << " è arrivato a " << simTime() << endl;
    simtime_t delay = simTime() - pkt->getGenTime();
    /*if(simTime() > pkt->getDeadlineAbs()) {
        if(pkt->getNumFrammenti() > 0) {
            EV << "EndNode: " << name.c_str() <<" Pacchetto no. " << pkt->getPktNumber() << ", di " << pkt->getBurstSize() << " scaduto di "<< simTime()-pkt->getDeadlineAbs() <<" , deadlineRel: " << pkt->getDeadlineRel() << ", deadlineAbs: " << pkt->getDeadlineAbs() << ", genTime: " << pkt->getGenTime() << ", arrivato a: " << simTime() << ", idFrammento: " << pkt->getIdFrammento() << ", numFrammenti: " << pkt->getNumFrammenti() << endl;
        } else {
            EV << "EndNode: " << name.c_str() <<" Pacchetto no. " << pkt->getPktNumber() << ", di " << pkt->getBurstSize() << " scaduto di "<< simTime()-pkt->getDeadlineAbs() <<" , deadlineRel: " << pkt->getDeadlineRel() << ", deadlineAbs: " << pkt->getDeadlineAbs() << ", genTime: " << pkt->getGenTime() << ", arrivato a: " << simTime() << endl;
        }
    } else {
        if(pkt->getNumFrammenti() > 0) {
            EV << "EndNode: " << name.c_str() <<" Pacchetto no. " << pkt->getPktNumber() << ", di " << pkt->getBurstSize() << " ricevuto in tempo, deadlineRel: " << pkt->getDeadlineRel() << ", deadlineAbs: " << pkt->getDeadlineAbs() << ", genTime: " << pkt->getGenTime() << ", arrivato a: " << simTime() << ", idFrammento: " << pkt->getIdFrammento() << ", numFrammenti: " << pkt->getNumFrammenti() << endl;
        } else {
            EV << "EndNode: " << name.c_str() <<" Pacchetto no. " << pkt->getPktNumber() << ", di " << pkt->getBurstSize() << " ricevuto in tempo, deadlineRel: " << pkt->getDeadlineRel() << ", deadlineAbs: " << pkt->getDeadlineAbs() << ", genTime: " << pkt->getGenTime() << ", arrivato a: " << simTime() << endl;
        }
    }*/
    EV << "EndNode: " << name.c_str() <<" Pacchetto no. " << pkt->getPktNumber() << ", di " << pkt->getBurstSize() << " ricevuto, deadlineRel: " << pkt->getDeadlineRel() << ", deadlineAbs: " << pkt->getDeadlineAbs() << ", genTime: " << pkt->getGenTime() << ", idFrammento: " << pkt->getIdFrammento() << ", numFrammenti: " << pkt->getNumFrammenti() << endl;
    simsignal_t sig = registerSignal("E2EDelay");
    emit(sig, delay);
    if(pkt->getPktNumber() == pkt->getBurstSize()) {
        sig = registerSignal("E2EBurstDelay");
        emit(sig, delay);
    }
    delete pkt;
}
void PeriodicTrafficGen::transmitPacket() {
    DataPacket *pkt = new DataPacket(name.c_str());
    int byteRimasti;
    int numFrammenti;
    if(payloadSize >= 1400) {
        int totFrammenti = payloadSize / 1400;
        if(payloadSize % 1400 != 0) {
            totFrammenti++;
        }
        for(int i=0; i<burstSize; i++) {
            byteRimasti = payloadSize;
            numFrammenti = 0;
            while(byteRimasti > 0) {
                EthTransmitReq *req = new EthTransmitReq();
                pkt->setPktNumber(i+1);
                if(byteRimasti > 1400) {
                    pkt->setByteLength(1400);
                    byteRimasti -= 1400;
                } else {
                    pkt->setByteLength(byteRimasti);
                    byteRimasti = 0;
                }
                pkt->setGenTime(simTime());
                pkt->setBurstSize(burstSize);
                pkt->setDeadlineRel(par("deadlineRel"));
                pkt->setDeadlineAbs(simTime() + pkt->getDeadlineRel());
                pkt->setIdFrammento(numFrammenti + 1);
                pkt->setNumFrammenti(totFrammenti);
                numFrammenti++;
                DataPacket *toSend = pkt->dup();
                req->setSrc(srcAddr.c_str());
                req->setDst(destAddr.c_str());
                req->setVlanid(vlanid);
                //EV << "PeriodicTrafficGen: Inoltro EthTransmitReq con destinazione " << req->getDst() << " e sorgente " << req->getSrc() << " e vlanid "<< req->getVlanid() << " . " << vlanid << endl;
                toSend->setControlInfo(req);
                send(toSend, "lowerLayerOut");
            }
            EV << "totFrammenti(calcolato): " << totFrammenti << ", numFrammenti: " << numFrammenti << endl;
        }

    } else {
        pkt->setByteLength(payloadSize);
        pkt->setGenTime(simTime());
        pkt->setBurstSize(burstSize);
        pkt->setDeadlineRel(par("deadlineRel"));
        pkt->setDeadlineAbs(simTime() + pkt->getDeadlineRel());
        for(int i=0; i<burstSize; i++) {
            DataPacket *toSend = pkt->dup();
            EthTransmitReq *req = new EthTransmitReq();
            req->setSrc(srcAddr.c_str());
            req->setDst(destAddr.c_str());
            req->setVlanid(vlanid);
            toSend->setControlInfo(req);
            //EV << "PeriodicTrafficGen: Inoltro EthTransmitReq con destinazione " << req->getDst() << " e sorgente " << req->getSrc() << " e vlanid "<< req->getVlanid() << " . " << vlanid << endl;
            toSend->setPktNumber(i+1);
            send(toSend, "lowerLayerOut");
        }
    }
    delete pkt;
}

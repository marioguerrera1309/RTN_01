packet DataPacket {
    simtime_t genTime;
    int pktNumber;
    int burstSize;
    simtime_t deadlineRel;
    simtime_t deadlineAbs;
    int idFrammento;
    int numFrammenti;
}
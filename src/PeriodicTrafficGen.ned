package rtn_01;

simple PeriodicTrafficGen
{
    parameters:
        volatile double period @unit(s) = default(0s);
        double startTime @unit(s) = default(0s);
        string name;
        int payloadSize @unit(B) = default(0B);
        int burstSize = default(1);
        string destAddr = default("");
        string srcAddr;
        int vlanid = default(-1);
        double deadlineRel @unit(s);
        @display("i=block/app2");
        @signal[E2EDelay];
	    @statistic[E2EDelay](source=E2EDelay; record=vector, histogram, mean);
    gates:
        input lowerLayerIn;
        output lowerLayerOut;
}

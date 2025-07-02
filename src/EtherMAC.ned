package rtn_01;

simple EtherMAC {
    parameters:
    	@display("i=block/mac");
    	int datarate @unit(bps) = default(1Gbps);
    	object vlans = default([]);
    	@signal[txqueueDim];
	    @statistic[txqueue](source=txqueueDim; record=vector, histogram, mean);
    gates:
        input upperLayerIn;
        output upperLayerOut;
        input channelIn;
        output channelOut;
}

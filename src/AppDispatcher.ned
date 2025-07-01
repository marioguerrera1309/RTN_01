package rtn_01;

simple AppDispatcher
{
    parameters:
    	@display("i=block/classifier");
    gates:
        output upperLayerOut[];
        input upperLayerIn[];
        output lowerLayerOut;
        input lowerLayerIn;
}

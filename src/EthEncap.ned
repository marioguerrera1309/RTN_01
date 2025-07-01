package rtn_01;

simple EthEncap
{
    parameters:
    	@display("i=block/layer");
    	string address; //Indirizzo mac della porta
    gates:
        output lowerLayerOut;
        input lowerLayerIn;
        output upperLayerOut;
        input upperLayerIn;
}

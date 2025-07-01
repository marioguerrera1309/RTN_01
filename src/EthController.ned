//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package rtn_01;

import ned.IdealChannel;


//
// TODO auto-generated type
//
module EthController
{
    @display("bgb=320,280");
    gates:
        output upperLayerOut;
        input upperLayerIn;
        output lowerLayerOut;
        input lowerLayerIn;
    submodules:
        ethEncap: EthEncap {
            @display("p=142,52");
        }
        etherMAC: EtherMAC {
            @display("p=142,165");
        }
    connections:
        ethEncap.upperLayerOut --> IdealChannel --> upperLayerOut;
        upperLayerIn --> IdealChannel --> ethEncap.upperLayerIn;
        etherMAC.upperLayerOut --> IdealChannel --> ethEncap.lowerLayerIn;
        ethEncap.lowerLayerOut --> IdealChannel --> etherMAC.upperLayerIn;
        etherMAC.channelOut --> IdealChannel --> lowerLayerOut;
        lowerLayerIn --> IdealChannel --> etherMAC.channelIn;
}

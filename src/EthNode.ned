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
module EthNode
{
    parameters:
        int n;
        @display("bgb=470,359");
    gates:
        input channelIn;
        output channelOut;
    submodules:
        periodicTrafficGen[n]: PeriodicTrafficGen {
            @display("p=203,53");
        }
        appDispatcher: AppDispatcher {
            @display("p=203,155");
        }
        ethController: EthController {
            @display("p=203,263");
        }
    connections:
        for i=0..n-1 {
            periodicTrafficGen[i].lowerLayerOut --> IdealChannel --> appDispatcher.upperLayerIn++;
        	appDispatcher.upperLayerOut++ --> IdealChannel --> periodicTrafficGen[i].lowerLayerIn;
        }
        appDispatcher.lowerLayerOut --> IdealChannel --> ethController.upperLayerIn;
        ethController.upperLayerOut --> IdealChannel --> appDispatcher.lowerLayerIn;
        channelIn --> IdealChannel --> ethController.lowerLayerIn;
        ethController.lowerLayerOut --> IdealChannel --> channelOut;
}

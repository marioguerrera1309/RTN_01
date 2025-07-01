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

package rtn_01.simulations;

import ned.IdealChannel;
import rtn_01.AppDispatcher;
import rtn_01.EthEncap;
import rtn_01.EthNode;
import rtn_01.EtherMAC;
import rtn_01.EtherSwitch;
import rtn_01.PeriodicTrafficGen;


network Sim1
{
    @display("bgb=2962,1515");
    submodules:
        LD2: EthNode {
            @display("p=208,208");
        }
        SW3: EtherSwitch {
            @display("p=501,418");
        }
        US2: EthNode {
            @display("p=208,418");
        }
        S2: EthNode {
            @display("p=501,208");
        }
        CU: EthNode {
            @display("p=208,629");
        }
        SW2: EtherSwitch {
            @display("p=1385,416");
        }
        TLM: EthNode {
            @display("p=1385,208");
        }
        HU: EthNode {
            @display("p=1163,629");
        }
        RS1: EthNode {
            @display("p=1588,629");
        }
        SW6: EtherSwitch {
            @display("p=2470,414");
        }
        S4: EthNode {
            @display("p=2470,208");
        }
        RC: EthNode {
            @display("p=2765,416");
        }
        US3: EthNode {
            @display("p=2765,208");
        }
        SW4: EtherSwitch {
            @display("p=501,1089");
        }
        CM1: EthNode {
            @display("p=208,874");
        }
        LD1: EthNode {
            @display("p=208,1087");
        }
        US1: EthNode {
            @display("p=208,1338");
        }
        S1: EthNode {
            @display("p=501,1338");
        }
        SW1: EtherSwitch {
            @display("p=1385,1087");
        }
        ME: EthNode {
            @display("p=1163,1338");
        }
        RS2: EthNode {
            @display("p=1586,1338");
        }
        SW5: EtherSwitch {
            @display("p=2470,1085");
        }
        S3: EthNode {
            @display("p=2765,1089");
        }
        US4: EthNode {
            @display("p=2470,1338");
        }
    connections:
        LD2.channelOut --> IdealChannel --> SW3.channelIn[0];
        SW3.channelOut[0] --> IdealChannel --> LD2.channelIn;
        US2.channelOut --> IdealChannel --> SW3.channelIn[1];
        SW3.channelOut[1] --> IdealChannel --> US2.channelIn;
        SW3.channelOut[2] --> IdealChannel --> S2.channelIn;
        S2.channelOut --> IdealChannel --> SW3.channelIn[2];
        SW3.channelOut[3] --> IdealChannel --> CU.channelIn;
        CU.channelOut --> IdealChannel --> SW3.channelIn[3];
        CM1.channelOut --> IdealChannel --> SW4.channelIn[0];
        SW4.channelOut[0] --> IdealChannel --> CM1.channelIn;
        LD1.channelOut --> IdealChannel --> SW4.channelIn[1];
        SW4.channelOut[1] --> IdealChannel --> LD1.channelIn;
        SW4.channelOut[2] --> IdealChannel --> US1.channelIn;
        US1.channelOut --> IdealChannel --> SW4.channelIn[2];
        SW4.channelOut[3] --> IdealChannel --> S1.channelIn;
        S1.channelOut --> IdealChannel --> SW4.channelIn[3];
        SW1.channelOut[0] --> IdealChannel --> ME.channelIn;
        ME.channelOut --> IdealChannel --> SW1.channelIn[0];
        SW1.channelOut[1] --> IdealChannel --> RS2.channelIn;
        RS2.channelOut --> IdealChannel --> SW1.channelIn[1];
        SW5.channelOut[0] --> IdealChannel --> US4.channelIn;
        US4.channelOut --> IdealChannel --> SW5.channelIn[0];
        S3.channelOut --> IdealChannel --> SW5.channelIn[1];
        SW5.channelOut[1] --> IdealChannel --> S3.channelIn;
        S4.channelOut --> IdealChannel --> SW6.channelIn[0];
        SW6.channelOut[0] --> IdealChannel --> S4.channelIn;
        SW6.channelOut[1] --> IdealChannel --> US3.channelIn;
        US3.channelOut --> IdealChannel --> SW6.channelIn[1];
        SW6.channelOut[2] --> IdealChannel --> RC.channelIn;
        RC.channelOut --> IdealChannel --> SW6.channelIn[2];
        TLM.channelOut --> IdealChannel --> SW2.channelIn[0];
        SW2.channelOut[0] --> IdealChannel --> TLM.channelIn;
        HU.channelOut --> IdealChannel --> SW2.channelIn[1];
        SW2.channelOut[1] --> IdealChannel --> HU.channelIn;
        SW2.channelOut[2] --> IdealChannel --> RS1.channelIn;
        RS1.channelOut --> IdealChannel --> SW2.channelIn[2];
        SW3.channelOut[4] --> IdealChannel --> SW4.channelIn[4];
        SW4.channelOut[4] --> IdealChannel --> SW3.channelIn[4];
        SW2.channelOut[3] --> IdealChannel --> SW3.channelIn[5];
        SW3.channelOut[5] --> IdealChannel --> SW2.channelIn[3];
        SW1.channelOut[2] --> IdealChannel --> SW4.channelIn[5];
        SW4.channelOut[5] --> IdealChannel --> SW1.channelIn[2];
        SW5.channelOut[2] --> IdealChannel --> SW1.channelIn[3];
        SW1.channelOut[3] --> IdealChannel --> SW5.channelIn[2];
        SW5.channelOut[3] --> IdealChannel --> SW6.channelIn[3];
        SW6.channelOut[3] --> IdealChannel --> SW5.channelIn[3];
        SW6.channelOut[4] --> IdealChannel --> SW2.channelIn[4];
        SW2.channelOut[4] --> IdealChannel --> SW6.channelIn[4];
        SW2.channelOut[5] --> IdealChannel --> SW1.channelIn[4];
        SW1.channelOut[4] --> IdealChannel --> SW2.channelIn[5];
}

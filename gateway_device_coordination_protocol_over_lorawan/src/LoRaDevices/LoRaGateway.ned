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

package src.LoRaDevices;


simple LoRaGateway
{
    parameters:
        @signal[sent](type="unsigned long");
        @signal[received](type="unsigned long");
        @signal[sentLoRa](type="unsigned long");
        @signal[receivedLoRa](type="unsigned long");
        @signal[sentIp](type="unsigned long");
        @signal[receivedIp](type="unsigned long");
        @signal[lost](type="unsigned long");
        @signal[lostLoRa](type="unsigned long"); 
        @signal[lostIp](type="unsigned long");       
        @signal[sentCount](type="unsigned long");
        @signal[receivedCount](type="unsigned long");
        @signal[sentLoRaCount](type="unsigned long");
        @signal[receivedLoRaCount](type="unsigned long");
        @signal[sentIpCount](type="unsigned long");
        @signal[receivedIpCount](type="unsigned long");
        @signal[lostCount](type="unsigned long");
        @signal[lostLoRaCount](type="unsigned long");
        @signal[lostIpCount](type="unsigned long");
        
        @signal[interference](type="unsigned long");
        @signal[interferencePossible](type="unsigned long");
        @signal[interferenceCount](type="unsigned long");
        @signal[interferencePossibleCount](type="unsigned long");
        
        @signal[rssi](type="int");
        
        @signal[connected](type="unsigned long");
        @signal[cpu](type="double");
        @signal[gpu](type="double");
        @signal[ram](type="double");
        @signal[storage](type="double");
        @signal[networkIn](type="unsigned long");
        @signal[networkOut](type="unsigned long");
        
        @statistic[messageSent](title="messages sent"; source="sent"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceived](title="messages received"; source="received"; record=histogram,vector; interpolationmode=none);
        @statistic[messageSentLoRa](title="messages sent LoRa"; source="sentLoRa"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedLoRa](title="messages received LoRa"; source="receivedLoRa"; record=histogram,vector; interpolationmode=none);
        @statistic[messageSentIp](title="messages sent Ip"; source="sentIp"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedIp](title="messages received Ip"; source="receivedIp"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLost](title="messages lost"; source="lost"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostLoRa](title="messages lost LoRa"; source="lostLoRa"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostIp](title="messages lost Ip"; source="lostIp"; record=histogram,vector; interpolationmode=none);
        @statistic[messageSentCount](title="messages sent count"; source="sentCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedCount](title="messages received count"; source="receivedCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageSentLoRaCount](title="messages sent LoRa count"; source="sentLoRaCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedLoRaCount](title="messages received LoRa count"; source="receivedLoRaCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageSentIpCount](title="messages sent Ip count"; source="sentIpCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedIpCount](title="messages received Ip count"; source="receivedIpCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostCount](title="messages lost count"; source="lostCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostLoRaCount](title="messages lost LoRa count"; source="lostLoRaCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostIpCount](title="messages lost Ip count"; source="lostIpCount"; record=histogram,vector; interpolationmode=none);
        
        @statistic[interference](title="interferences"; source="interference"; record=histogram,vector; interpolationmode=none);
        @statistic[interferencePossible](title="interferences possible"; source="interferencePossible"; record=histogram,vector; interpolationmode=none);
        @statistic[interferenceCount](title="interferences count"; source="interferenceCount"; record=histogram,vector; interpolationmode=none);
        @statistic[interferencePossibleCount](title="interference possible count"; source="interferencePossibleCount"; record=histogram,vector; interpolationmode=none);

		@statistic[rssi](title="RSSI messages"; source="rssi"; record=histogram,vector; interpolationmode=none);
        
        @statistic[connected](title="connected end devices"; source="connected"; record=histogram,vector; interpolationmode=none);
        @statistic[cpu](title="CPU load"; source="cpu"; record=histogram,vector; interpolationmode=none);
        @statistic[gpu](title="GPU load"; source="gpu"; record=histogram,vector; interpolationmode=none);
        @statistic[ram](title="RAM load"; source="ram"; record=histogram,vector; interpolationmode=none);
        @statistic[storage](title="Storage load"; source="storage"; record=histogram,vector; interpolationmode=none);
        @statistic[networkIn](title="network in"; source="networkIn"; record=histogram,vector; interpolationmode=none);
        @statistic[networkOut](title="network out"; source="networkOut"; record=histogram,vector; interpolationmode=none);
        
        @display("i=device/accesspoint;r=5000");
        
        //@signal[gateway-*];//(type=cPacket);
    gates:
        inout lorag[];
        inout ethg[];
}
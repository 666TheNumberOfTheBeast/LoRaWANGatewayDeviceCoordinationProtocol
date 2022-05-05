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
        // Protocol
        @signal[sent](type="unsigned long");
        @signal[received](type="unsigned long");
        @signal[lost](type="unsigned long");
        @signal[invalid](type="unsigned long");
        
        @signal[sentLoRa](type="unsigned long");
        @signal[receivedLoRa](type="unsigned long");
        @signal[lostLoRa](type="unsigned long");
        @signal[invalidLoRa](type="unsigned long");
        
        @signal[sentLoRaGateways](type="unsigned long");
        @signal[receivedLoRaGateways](type="unsigned long");
        @signal[lostLoRaGateways](type="unsigned long");
        
        @signal[sentLoRaDevices](type="unsigned long");
        @signal[receivedLoRaDevices](type="unsigned long");
        @signal[lostLoRaDevices](type="unsigned long");
                
        @signal[sentIp](type="unsigned long");
        @signal[receivedIp](type="unsigned long");
        @signal[lostIp](type="unsigned long");
        @signal[invalidIp](type="unsigned long");
        
        @signal[sentIpGateways](type="unsigned long");
        @signal[receivedIpGateways](type="unsigned long");
        @signal[lostIpGateways](type="unsigned long");
        
        @signal[sentIpServer](type="unsigned long");
        @signal[receivedIpServer](type="unsigned long");
        @signal[lostIpServer](type="unsigned long");
              
        @signal[sentCount](type="unsigned long");
        @signal[receivedCount](type="unsigned long");
        @signal[lostCount](type="unsigned long");
        @signal[invalidCount](type="unsigned long");
        
        @signal[sentLoRaCount](type="unsigned long");
        @signal[receivedLoRaCount](type="unsigned long");
        @signal[lostLoRaCount](type="unsigned long");
        @signal[invalidLoRaCount](type="unsigned long");
        
        @signal[sentLoRaGatewaysCount](type="unsigned long");
        @signal[receivedLoRaGatewaysCount](type="unsigned long");
        @signal[lostLoRaGatewaysCount](type="unsigned long");
        
        @signal[sentLoRaDevicesCount](type="unsigned long");
        @signal[receivedLoRaDevicesCount](type="unsigned long");
        @signal[lostLoRaDevicesCount](type="unsigned long");
        
        @signal[sentIpCount](type="unsigned long");
        @signal[receivedIpCount](type="unsigned long");
        @signal[lostIpCount](type="unsigned long");
        @signal[invalidIpCount](type="unsigned long");
        
        @signal[sentIpGatewaysCount](type="unsigned long");
        @signal[receivedIpGatewaysCount](type="unsigned long");
        @signal[lostIpGatewaysCount](type="unsigned long");
        
        @signal[sentIpServerCount](type="unsigned long");
        @signal[receivedIpServerCount](type="unsigned long");
        @signal[lostIpServerCount](type="unsigned long");
        
        @signal[interference](type="unsigned long");
        @signal[interferencePossible](type="unsigned long");
        @signal[interferenceCount](type="unsigned long");
        @signal[interferencePossibleCount](type="unsigned long");
        
        // Data
        @signal[sentData](type="unsigned long");
        @signal[receivedData](type="unsigned long");
        @signal[lostData](type="unsigned long");
        @signal[invalidData](type="unsigned long");
        
        @signal[sentGatewaysData](type="unsigned long");
        @signal[receivedGatewaysData](type="unsigned long");
        
        @signal[sentServerData](type="unsigned long");
        @signal[receivedServerData](type="unsigned long");
        
        @signal[receivedDevicesData](type="unsigned long");

        @signal[sentDataCount](type="unsigned long");
        @signal[receivedDataCount](type="unsigned long");
        @signal[lostDataCount](type="unsigned long");
        @signal[invalidDataCount](type="unsigned long");
        
        @signal[sentGatewaysDataCount](type="unsigned long");
        @signal[receivedGatewaysDataCount](type="unsigned long");
        
        @signal[sentServerDataCount](type="unsigned long");
        @signal[receivedServerDataCount](type="unsigned long");
        
        @signal[receivedDevicesDataCount](type="unsigned long");
        
        @signal[interferenceData](type="unsigned long");
        @signal[interferencePossibleData](type="unsigned long");
        @signal[interferenceDataCount](type="unsigned long");
        @signal[interferencePossibleDataCount](type="unsigned long");
        
        
        @signal[rssi](type="int");
        
        @signal[connected](type="unsigned long");
        @signal[cpu](type="double");
        @signal[gpu](type="double");
        @signal[ram](type="double");
        @signal[storage](type="double");
        @signal[networkIn](type="unsigned long");
        @signal[networkOut](type="unsigned long");
        
        // Protocol
        @statistic[messageSent](title="messages sent"; source="sent"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceived](title="messages received"; source="received"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLost](title="messages lost"; source="lost"; record=histogram,vector; interpolationmode=none);
        @statistic[messageInvalid](title="messages invalid"; source="invalid"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentLoRa](title="messages sent LoRa"; source="sentLoRa"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedLoRa](title="messages received LoRa"; source="receivedLoRa"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostLoRa](title="messages lost LoRa"; source="lostLoRa"; record=histogram,vector; interpolationmode=none);
        @statistic[messageInvalidLoRa](title="messages invalid LoRa"; source="invalidLoRa"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentLoRaGateways](title="messages sent LoRa gateways"; source="sentLoRaGateways"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedLoRaGateways](title="messages received LoRa gateways"; source="receivedLoRaGateways"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostLoRaGateways](title="messages lost LoRa gateways"; source="lostLoRaGateways"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentLoRaDevices](title="messages sent LoRa devices"; source="sentLoRaDevices"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedLoRaDevices](title="messages received LoRa devices"; source="receivedLoRaDevices"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostLoRaDevices](title="messages lost LoRa devices"; source="lostLoRaDevices"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentIp](title="messages sent Ip"; source="sentIp"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedIp](title="messages received Ip"; source="receivedIp"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostIp](title="messages lost Ip"; source="lostIp"; record=histogram,vector; interpolationmode=none);
        @statistic[messageInvalidIp](title="messages invalid Ip"; source="invalidIp"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentIpGateways](title="messages sent Ip gateways"; source="sentIpGateways"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedIpGateways](title="messages received Ip gateways"; source="receivedIpGateways"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostIpGateways](title="messages lost Ip gateways"; source="lostIpGateways"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentIpServer](title="messages sent Ip server"; source="sentIpServer"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedIpServer](title="messages received Ip server"; source="receivedIpServer"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostIpServer](title="messages lost Ip server"; source="lostIpServer"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentCount](title="messages sent count"; source="sentCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedCount](title="messages received count"; source="receivedCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostCount](title="messages lost count"; source="lostCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageInvalidCount](title="messages invalid count"; source="invalidCount"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentLoRaCount](title="messages sent LoRa count"; source="sentLoRaCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedLoRaCount](title="messages received LoRa count"; source="receivedLoRaCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostLoRaCount](title="messages lost LoRa count"; source="lostLoRaCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageInvalidLoRaCount](title="messages invalid LoRa count"; source="invalidLoRaCount"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentLoRaGatewaysCount](title="messages sent LoRa gateways count"; source="sentLoRaGatewaysCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedLoRaGatewaysCount](title="messages received gateways LoRa count"; source="receivedLoRaGatewaysCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostLoRaGatewaysCount](title="messages lost gateways LoRa count"; source="lostLoRaGatewaysCount"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentLoRaDevicesCount](title="messages sent LoRa devices count"; source="sentLoRaDevicesCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedLoRaDevicesCount](title="messages received LoRa devices count"; source="receivedLoRaDevicesCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostLoRaDevicesCount](title="messages lost LoRa devices count"; source="lostLoRaDevicesCount"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentIpCount](title="messages sent Ip count"; source="sentIpCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedIpCount](title="messages received Ip count"; source="receivedIpCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostIpCount](title="messages lost Ip count"; source="lostIpCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageInvalidIpCount](title="messages invalid Ip count"; source="invalidIpCount"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentIpGatewaysCount](title="messages sent Ip gateways count"; source="sentIpGatewaysCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedIpGatewaysCount](title="messages received Ip gateways count"; source="receivedIpGatewaysCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostIpGatewaysCount](title="messages lost Ip gateways count"; source="lostIpGatewaysCount"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentIpServerCount](title="messages sent Ip server count"; source="sentIpServerCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedIpServerCount](title="messages received Ip server count"; source="receivedIpServerCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostIpServerCount](title="messages lost Ip server count"; source="lostIpServerCount"; record=histogram,vector; interpolationmode=none);
        
        @statistic[interference](title="interferences"; source="interference"; record=histogram,vector; interpolationmode=none);
        @statistic[interferencePossible](title="interferences possible"; source="interferencePossible"; record=histogram,vector; interpolationmode=none);
        @statistic[interferenceCount](title="interferences count"; source="interferenceCount"; record=histogram,vector; interpolationmode=none);
        @statistic[interferencePossibleCount](title="interference possible count"; source="interferencePossibleCount"; record=histogram,vector; interpolationmode=none);

        // Data        
        @statistic[messageSentData](title="messages data sent"; source="sentData"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedData](title="messages data received"; source="receivedData"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostData](title="messages data lost"; source="lostData"; record=histogram,vector; interpolationmode=none);
        @statistic[messageInvalidData](title="messages data invalid"; source="invalidData"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentGatewaysData](title="messages gateways data sent"; source="sentGatewaysData"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedGatewaysData](title="messages gateways data received"; source="receivedGatewaysData"; record=histogram,vector; interpolationmode=none);
        @statistic[messageSentServerData](title="messages server data sent"; source="sentServerData"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedServerData](title="messages server data received"; source="receivedServerData"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedDevicesData](title="messages devices data received"; source="receivedDevicesData"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentDataCount](title="messages sent data count"; source="sentDataCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedDataCount](title="messages received data count"; source="receivedDataCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostDataCount](title="messages lost data count"; source="lostDataCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageInvalidDataCount](title="messages invalid data count"; source="invalidDataCount"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentGatewaysDataCount](title="messages gateways sent data count"; source="sentGatewaysDataCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedGatewaysDataCount](title="messages gateways received data count"; source="receivedGatewaysDataCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageSentServerDataCount](title="messages server sent data count"; source="sentServerDataCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedServerDataCount](title="messages server received data count"; source="receivedServerDataCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedDevicesDataCount](title="messages devices received data count"; source="receivedDevicesDataCount"; record=histogram,vector; interpolationmode=none);
        
        @statistic[interferenceData](title="interferences data"; source="interferenceData"; record=histogram,vector; interpolationmode=none);
        @statistic[interferencePossibleData](title="interferences data possible"; source="interferencePossibleData"; record=histogram,vector; interpolationmode=none);
        @statistic[interferenceDataCount](title="interferences data count"; source="interferenceDataCount"; record=histogram,vector; interpolationmode=none);
        @statistic[interferencePossibleDataCount](title="interference possible data count"; source="interferencePossibleDataCount"; record=histogram,vector; interpolationmode=none);
        
        
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
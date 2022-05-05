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

simple LoRaEndDevice
{
    parameters:
        @signal[sent](type="unsigned long");
        @signal[received](type="unsigned long");
        @signal[lost](type="unsigned long");
        @signal[invalid](type="unsigned long");
        @signal[retransmitted](type="unsigned long");
        
        @signal[sentCount](type="unsigned long");
        @signal[receivedCount](type="unsigned long");
        @signal[lostCount](type="unsigned long");
        @signal[invalidCount](type="unsigned long");
        @signal[retransmittedCount](type="unsigned long");
        
        @signal[interference](type="unsigned long");
        @signal[interferencePossible](type="unsigned long");
        @signal[interferenceCount](type="unsigned long");
        @signal[interferencePossibleCount](type="unsigned long");
        
        @signal[sentData](type="unsigned long");
        @signal[sentDataCount](type="unsigned long");
        
        @statistic[messageSent](title="messages sent"; source="sent"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceived](title="messages received"; source="received"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLost](title="messages lost"; source="lost"; record=histogram,vector; interpolationmode=none);
        @statistic[messageInvalid](title="messages invalid"; source="invalid"; record=histogram,vector; interpolationmode=none);
        @statistic[messageRetransmitted](title="messages retransmitted"; source="retransmitted"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentCount](title="messages sent count"; source="sentCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageReceivedCount](title="messages received count"; source="receivedCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageLostCount](title="messages lost count"; source="lostCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageInvalidCount](title="messages invalid count"; source="invalidCount"; record=histogram,vector; interpolationmode=none);
        @statistic[messageRetransmittedCount](title="messages retransmitted count"; source="retransmittedCount"; record=histogram,vector; interpolationmode=none);
        
        @statistic[interference](title="interferences"; source="interference"; record=histogram,vector; interpolationmode=none);
        @statistic[interferencePossible](title="interferences possible"; source="interferencePossible"; record=histogram,vector; interpolationmode=none);
        @statistic[interferenceCount](title="interferences count"; source="interferenceCount"; record=histogram,vector; interpolationmode=none);
        @statistic[interferencePossibleCount](title="interference possible count"; source="interferencePossibleCount"; record=histogram,vector; interpolationmode=none);
        
        @statistic[messageSentData](title="messages sent data"; source="sentData"; record=histogram,vector; interpolationmode=none);
        @statistic[messageSentDataCount](title="messages sent data count"; source="sentDataCount"; record=histogram,vector; interpolationmode=none);
        
        //@signal[endDevice-*];
        
        //@display("i=device/card;r=3000");
        //@display("i=block/source;r=3000");
        @display("i=misc/building");
    gates:
        inout lorag[];
}
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

package simulations;

//import inet.node.internetcloud.InternetCloud;
//import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth1G;
//import inet.node.ethernet.Eth10M;

import src.LoRaDevices.*;

network LoRaNetworkServerEdgeRealScenarioTest
{
    parameters:
        bool realDeployment = true;
        int region = default(1);
        int nEndDevices = default(0);
        int nGateways = default(0);
        object RSSIs = default([]);
        object spreadingFactors = default([]);
        int bgX = 3000;
        int bgY = 1500;
        @display("bgb=$bgX,$bgY;bgi=background/streetmap,s");
    //types:
        //channel RadioChannel extends ned.DatarateChannel {
        //    double length @unit(m) = 10m;
        //    
        //    delay = replaceUnit(length / 2.97e8, "s");
        //    datarate = 0bps;
        //    ber = 0;
        //    per = 0;
            
            // Show no line
        //    @display("ls=,0");
        //}
    submodules:
        networkServer: LoRaNetworkServer {
            @display("p=1500,200");
        }
        gateways[nGateways]: LoRaGateway {
        }
        endDevices[nEndDevices]: LoRaEndDevice {
        }
        //internetCloud: InternetCloud {
        //    @display("p=374,55");
        //}
        //configurator: Ipv4NetworkConfigurator {
        //    parameters:
        //        assignDisjunctSubnetAddresses = false;
        //        @display("p=237,90");
        //}
    connections allowunconnected:
        //networkServer.pppg++ <--> Eth1G <--> internetCloud.pppg++;

		// Connect gateways to network server
        for i=0..nGateways-1 {
            networkServer.ethg++ <--> Eth1G <--> gateways[i].ethg++;
        }

		// Connect gateways to other gateways
        for i=0..nGateways-1, for j=i+1..nGateways-1 {
            // Over IP
            gateways[i].ethg++ <--> Eth1G <--> gateways[j].ethg++;
            
            // Over LoRa
            //gateways[i].lorag++ <--> Eth1G <--> gateways[j].lorag++;
        }
        
        // Connect end devices to all gateways in this real scenario
        for i=0..nEndDevices-1, for j=0..nGateways-1 {
            // The channel is not important because is overridden at execution time
            endDevices[i].lorag++ <--> Eth1G <--> gateways[j].lorag++;
            //endDevices[i].lorag++ <--> Eth10M <--> gateways[j].lorag++;
            //endDevices[i].lorag++ <--> RadioChannel <--> gateways[j].lorag++;
        }
}
        
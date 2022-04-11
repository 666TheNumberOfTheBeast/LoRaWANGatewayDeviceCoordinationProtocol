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
import inet.node.ethernet.Eth100M;
//import inet.node.ethernet.Eth10M;

import src.LoRaDevices.*;

network LoRaNetworkServerEdgeStochasticScenarioTest
{
    parameters:
        bool realDeployment = false;
        bool fullCoverage = default(false);
        int region = default(1);
        int nEndDevices = default(3);
        int nGateways = default(3);
        //double connectedness = default(0.7); // 0.0<x<1.0
        int bgX = 20000;
        int bgY = 15000;
        //@display("bgb=633,359;bgi=background/streetmap,s;bgu=km");
        //@display("bgb=10000,6000;bgi=background/streetmap,s");
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
            //@display("p=114,55");
            //@display("p=10000,200");
            @display("p=10000,5000");
        }
        gateways[nGateways]: LoRaGateway {
            //@display("p=114,165,r,100");
            //@display("p=3000,1000,ri,2000,2000");
            
            @display("p=5000,1000");
        }
        endDevices[nEndDevices]: LoRaEndDevice {
            //@display("p=114,279,r,25");
            //@display("p=114,279,r,50");
            //@display("p=114,279,r,25");
            
            //@display("p=0,2000,ri,5000,1000");
            
            @display("p=5000,2000");
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
            //networkServer.ethg++ <--> Eth1G <--> gateways[i].ethg++;
            networkServer.ethg++ <--> Eth100M <--> gateways[i].ethg++;
        }
        
		// Connect gateways to other gateways
        for i=0..nGateways-1, for j=i+1..nGateways-1 {
            // Over IP
            //gateways[i].ethg++ <--> Eth1G <--> gateways[j].ethg++;
            gateways[i].ethg++ <--> Eth100M <--> gateways[j].ethg++;
            
            // Over LoRa
            //gateways[i].lorag++ <--> Eth10M <--> gateways[j].lorag++ if uniform(0,1) < connectedness;
        }

        // Connect end devices to a gateway
        //for i=0..nEndDevices-1 {
        //    endDevices[i].lorag++ <--> Eth10M <--> gateways[intuniform(0, nGateways-1)].lorag++;
        //b}

		// Try to connect end devices to multiple gateways
        //for i=0..nEndDevices-1, for j=0..nGateways-1 {
            //endDevices[i].lorag++ <--> RadioChannel <--> gateways[j].lorag++ if uniform(0,1) < connectedness;
            //endDevices[i].lorag++ <--> Eth10M <--> gateways[j].lorag++ if uniform(0,1) < connectedness;
            	//if uniform(0,1) < connectedness && endDevices[i].lorag$i[0] != gateways[j].lorag$o[0];
        //}
        
        
		// Connect end devices to all gateways
		//for i=0..nEndDevices-1, for j=0..nGateways-1 {
            //endDevices[i].lorag++ <--> RadioChannel <--> gateways[j].lorag++;
        //    endDevices[i].lorag++ <--> Eth10M <--> gateways[j].lorag++;
        //}
}

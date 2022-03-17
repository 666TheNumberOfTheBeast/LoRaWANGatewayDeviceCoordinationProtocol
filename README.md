## Gateway-Device Coordination Protocol over LoRaWAN
The number of Internet of Things (IoT) devices is increasing year over year (about 30 billion devices were estimated by 2020 and about 70 billion devices by 2025) and consequently, the data traffic is generated and consumed at the edge of the network infrastructure. 
As this amount of data grows, current cloud-based approaches are becoming increasingly inefficient as massive datasets collected by sensors are moved from the edge to distant machines of the cloud platform.
These large-scale deployments result in a latency that does not fit with time-critical IoT applications requirements, therefore in recent years innovative edge computing and fog computing solutions arose that are able to leverage resources along the path between the end devices and the cloud.
At the same time, stream processing frameworks are become an essential component of an IoT system due to their capability to process huge amounts of data in near real-time both in well-established cloud computing architectures and in more recent edge and fog computing ones.
Here, I propose an edge-based framework over LoRaWAN for processing data close to the source where it is generated.
The location-aware functionality is realized through a Gateway-Device Coordination Protocol able to associate an end device to the best fit available LoRa gateway.
This reduces the data traffic both at the edge and between the edge itself and the core of the network for storing ready-to-use historical data. 
Furthermore, the framework enhances Quality of Service (QoS) by allowing users to submit stream processing tasks to define how specific data streams have to be elaborated and accessing results directly at the edge without the need for a cloud data transfer minimizing latencies.


## Overview
The Gateway-Device Coordination Protocol has been implemented using OMNeT++, a well-known modular simulation software based on C++ for building network simulators.
The three LoRaWAN components involved in the protocol (the network server, the gateway and the end device) are reproduced by corresponding simple modules declared in three separated NED files.

For each module, parameters and gates are defined. 
These are connected in the architecture NED files by defining networks and specifying gates connections.
Once the structure of the simulation model is defined, modules are programmed through related C++ classes.

In this implementation, two different networks have been developed to realize real and stochastic LoRaWAN deployments: one network reproduces real deployments based on values collected in the [LoED dataset](https://zenodo.org/record/4121430) while the other network consists of a random number of end devices and gateways positioned randomly.

Input data for the simulation are specified in an *omnetpp.ini* configuration file and includes the regional parameter (specified through the plan ID denoting where the LoRaWAN network is deployed and corresponding bandwidth, channel frequencies, spreading factors, transmission powers and duty cycle), the number of end devices and the number of gateways.
Additionally, the input parameters for the real deployment network also include the RSSIs and spreading factors collected by gateways and reported in the LoED dataset.

The simulation results are saved in the *simulations/results* directory inside the project directory after the run is complete.

![Example of real deployment from LoED dataset 2020-08-01](/screenshots/real_deployment_example_LoED_20190301.png)


## Features
- Accurate model of LoRa technology (including regional plan IDs and collisions)
- Accurate model of Gateway-Device Coordination Protocol over LoRaWAN (including OTAA)
- Stochastic deployments simulations with *n* end devices and *m* gateways
- Real deployments simulations where all end devices are in the radio range of two gateways
- Statistics on
    - message sent, received, lost and retransmitted by every end device,
    - message sent, received and lost by every gateway (also differentiating between LoRa and IP interfaces and with associated RSSIs for incoming LoRa signals),
    - interferences and possible interferences detected by a gateway,
    - gateway resources state,
    - number of connected end devices to a gateway


## Installation
To get started you need to

1. download and install [OMNeT++](https://doc.omnetpp.org/omnetpp/InstallGuide.pdf)
2. download and install [INET framework](https://inet.omnetpp.org/Installation.html)
3. clone the [Gateway-Device Coordination Protocol]() repository 
4. open OMNeT++ IDE and import the simulation model as a new project into your OMNeT++ workspace (*File* -> *Import* -> *General* -> *Existing project into Workspace*)
4. make sure the imported model has INET framework as a project reference (*Project* -> *Properties* -> *Project References*)
5. build the Gateway-Device Coordination Protocol project (right-click on the project directory and select *Build project*)
6. verify installation by running a simulation scenario located in *simulations* directory (right-click on *omnetpp.ini* and select *Run as* -> *OMNeT++ simulation*).

In particular, the project has been developed using OMNeT++ 6.0 Preview 11 and INET 4.3.

## About me
I'm a student of the MSc of *Engineering in Computer Science* of *Sapienza University of Rome* and this project has been developed as part of my *Master Thesis*. Visit my [LinkedIn profile](https://www.linkedin.com/in/ivan-fardin-304a001a3/) to learn more about me.
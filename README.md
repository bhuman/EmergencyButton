## About The Project

This repository contains everything needed to deploy the emergency button.

It's subdiveded in the following directories:
* EmergencyButtonEmbedded: Contains the software running on the button itself (An arduino pro micro)
* EmergencyButtonServer: The program running on the access point (MIPS architecture) receiving signals from the button and relaying it onto the Network using a UDP broadcast ping
* Schematic: Contains electrical diagrams and 3D models of the button 

> [!note]
> You will need a UNIX-like operating system in order to build this project


## Getting Started

With this project you are able to build the following files:

| File                    | Usage |
| ----------------------- | ----- |
| freetz-image.img        | Freetz-NG image used to flash onto a Fritz!Box 7580 |
| emergency_button_avr    | The program that will be flashed onto the button itself |
| emergency_button_server | The program running on the access point (Fritz!Box 7580) |
| emergency_button_client | A test client built for the hosts architecture used to receive UDP broadcast messages from the server |


### Prerequisites

You will be needing the following dependencies to be able to build this project:

* cmake
* gcc


### Building

1. Clone the repo
   The following will clone this repository into your current directory ($PWD)
   ```sh
   git clone --recurse-submodules --shallow-submodules git@gitlab.informatik.uni-bremen.de:B-Human/Utils/emergencybutton.git
   ```

2. Enter the cloned directory
   ```sh
   cd ./emergencybutton
   ```

2. Choose what you want to build
   You can specify which part of the project you want to build, adding no arguments will build all targets
   ```sh
   ./build.sh
   #./build.sh freetz
   #./build.sh server
   #./build.sh button
   #./build.sh all #(Same as ./build.sh)
   ```

   Running `build.sh` in the projects root directory is equivilant to running 
   the individual `build.sh` scripts in their respective directories and copying the output file into the `output` directory

### Deployment

TODO
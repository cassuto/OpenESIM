# OpenDSIM(Opensource A/D mixed Circuit Simulator)

------
![passing](https://camo.githubusercontent.com/948ddd4d1b81323800104755c8ed392d5410f5e2/68747470733a2f2f696d672e736869656c64732e696f2f62616467652f6275696c642d70617373696e672d677265656e2e737667) ![unstable](https://camo.githubusercontent.com/31a260091d356cea706b3792d242699cfa2c1f04/68747470733a2f2f696d672e736869656c64732e696f2f62616467652f7374617475732d756e737461626c652d6c69676874677265656e2e737667)
</br>
OpenDSIM is A/D circuit simulator designed for analogous and digital circuit validation, which allows you to test a physical circuit in a real-time UI environment with virtual instruments and analysis.
</br>
Based on node admittance analysis and event driven logical model, it simulates linear, non-linear, reactive and even logical blocks, both combined and sequential logic.
There are virtual electronic instruments such as voltage and current probe, plotter, oscilloscope and so on.
</br>
All the electronic models and compnents are flexible and extensible, so more and more new models are easily adding to project tree.
</br>

# Viewing Project Tree

There are mainly three directories of this item:
- opendsim-core          : Core library of OpenDSIM(act as the backend).
- opendsim-frontend      : Console and GUI part
- opendsim-symbols       : Schematic symbols of all devices

# Concepts

Three-layout simulation system:
- LAYOUT-1               : Primitive model
- LAYOUT-2               : Device Package
- LAYOUT-3               : Schematic
</br>
LAYOUT-1: The primitive models is constructed by C/C++ language, which is accessible to node admittance matrix and logic event subsystem.They pay a low-level and important role of the whole simulator.
</br>
LAYOUT-2: The device package is described by one or more primitive models and their physical connections.Maybe extra initial configuration of models should be provided as well.They also deal with what will be shown on schematic and process real-time GUI events sent by user, making a reflection if circuit status is changed. 
</br>
LAYOUT-3: The schematic is what we inputed to computer and what we expected to process. A schematic consisted of devices, configuration, physical nodes (like vertex in graph theory) and connections (like edges).

# How to build

In Windows you should prepare MinGW or CygWin before. Ensure that Qt5.5.1 is installed properly.

    cd to your source root
    ./configure
    make all
    make install
    
    
In Linux, you may need install Qt5.5.1.

# Maintain
We're still in maintenance of this project though currently the item is in alpha stage. A bootstrap script is used to construct building system.
Maintenance-Mode is disabled by default, however, you can enable it if it really needs.

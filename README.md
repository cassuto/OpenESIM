# *OpenDSIM* - A/D circuit simulator)

------

![](https://img.shields.io/badge/opendsim-0.1-blue.svg)

`OpenDSIM` is a circuit simulator designed for analysis and verification of analog and digital circuits, which allows you to investigate physical circuits with virtual instruments in a real-time interactive environment.

Based on node admittance analysis and event-driven logical model, it can simulate linear, non-linear, reactive circuits and digital circuits with delay (both combined and sequential logic).

It supports a variety of virtual electronic instruments such as voltmeters, current probes, plotters, oscilloscopes, etc, which can be directly connected with the circuit and operated in real time.

All electronic models and devices are modular and extensible, so more and more new models are added to this project.

# Features

- Real-time and interactive. Measure, analysis or operate the circuit in real time, just like a virtual laboratory where you can do whatever you want.
- Automatic insertion of AD/DA bridges that process both analogous and digital circuit. Effective digital simulation framework separated from analog part provides higher performance.
- Streamlined low-expense models that help reducing simulation time.
- Unified interfaces of models and devices. Easy to extend.


# Source Tree

| filename | description |
|:--------:|:-----------:|
| `opendsim-core`          | Core library, including utilities, scheme-lisp parser and circuit simulation framework. |
| `opendsim-frontend`      | Console and GUI |
| `opendsim-symbols`       | Device Packages (Schematic symbols) |
| `include`                | C/C++ Header Files |

`opendsim-core` directories:

| filename | description |
|:--------:|:-----------:|
| `circuit` | Simulation framework of circuit object |
| `math` | Mathematical procedures, for example, matrix operators. |
| `models` | Primitive model library |
| `scheme-lisp` | A simple scheme-lisp parser for common purpose. |
| `util` | Utilities |

`opendsim-frontend` directories:

| filename | description |
|:--------:|:-----------:|
| `devices` | Device models library |
| `instruments` | Virtual Instrument library |
| `include` | Private C/C++ Header files restricted to the front-end |

# Concepts

Three-layered simulation system:
- `Layer-1`               : Primitive Model Layer
- `Layer-2`               : Device Package Layer
- `Layer-3`               : Schematic Layer

`Layer-1`: The primitive models is constructed by C/C++ language, which is responsible for maintaining node admittance matrix and logic event subsystem.

`Layer-2`: The device package is described by one or more primitive models and their physical connections and electrical parameters. Device packages also deal with schematic symbols and real-time GUI events. 

`Layer-3`: The schematic is what we inputed to computer and what we expected to process. A schematic consisted of device packages, connections and configurations.

# Build and deploy

1. Requirements:
* Qt v5.5.1
* GCC v4.8 or greater

2. Prepare a bash terminal:

```
	mkdir build && cd build
    ../configure
    make all
    make install
```

Tips: For Windows, you can prepare a Unix-like shell such as MinGW or CygWin that supports GCC toolchain. 

# Maintain

We're still in maintenance of this project though currently the item is in alpha stage. A bootstrap script is used to construct building system.
Maintenance-Mode is disabled by default, however, you can enable it if it really needs.

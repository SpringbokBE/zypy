# Accelerating numerical libraries for Python using FPGAs
E091103 - Master thesis, Computer Science Engineering - Faculty of Engineering and Architecture, UGent

## About
This repository contains all working files for my thesis, in which a drop-in replacement module for NumPy is made that allows for a transparent execution of procedures on the hardware of Xilinx FPGAs. The full thesis and presentation can be found in the `docs` folder.

## Requirements

The hardware synthesis and bare metal software design is done on Windows 10 using:

* `Xilinx® Vitis HLS 2020.2`
* `Xilinx® Vivado 2020.2`
* `Xilinx® Vitis 2020.2`

The ZyPy module is tested on Linux running on the [PYNQ-Z2 board](https://www.tul.com.tw/productspynq-z2.html) using:

* `Python 3.7`
* `Numpy 1.20.2`

## Folder structure

```
docs        // Contains corresponding thesis and presentation.
ip          // Contains exported IP cores (Xilinx® Vitis HLS 2020.2).
unused      // Contains unused files that may still be useful.
vitis       // Contains bare metal Zynq applications (Xilinx® Vitis 2020.2).
vitis_hls   // Contains IP core source (Xilinx® Vitis HLS 2020.2).
vivado      // Contains hardware designs (Xilinx® Vivado 2020.2).
xsa         // Contains the exported hardware designs (Xilinx® Vivado 2020.2).
zypy        // Contains drop-in replacement module for NumPy.
```

## Authors

* **Gerbrand De Laender** (gerbrand.delaender@ugent.be)

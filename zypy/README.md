# ZyPy repository

This repository contains all files that are to be placed on the SD card of the PYNQ-Z2 board.

## Usage

Please open `demo.ipynb` to see how `ZyPy` can be used.

## Folder structure

```
lib         // Contains the build/benchmark files for each of the hardware designs.
overlays    // Contains the .bit/.hwh files for each of the hardware designs.
tools       // Contains some additional files used for benchmarking and profiling.
|- create_drivers_notebook.py // This script can generate most of the boilerplate code to interface with a hardware design from Python.
```
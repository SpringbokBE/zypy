# Vitis source code repository

This repository contains the Vitis source code for the following hardware designs:

- `ufunc_call_f4`
- `ufunc_reduce_all_f4`
- `filter_avg_f4`

## Prerequisites

- [Xilinx Vitis 2020.2](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vitis.html)
- [Microsoft Windows](https://www.microsoft.com/en-us/windows)
- [PYNQ-Z2 board files for Vitis](https://pynq.readthedocs.io/en/v2.6.1/overlay_design_methodology/board_settings.html)

## Recreating the Vitis projects

1. In Vitis, create a new bare metal platform project from the `<BD_NAME>.xsa` file created with Vivado.
2. Copy the `fix_makefiles.py` file into the created `<BD_NAME>_platform` folder.
3. From the `<BD_NAME>_platform` folder, run `python fix_makefiles.py` in a command window to fix the auto-generated makefiles (see [this thread](https://forums.xilinx.com/t5/Embedded-Development-Tools/Vitis-20-2-does-not-build-anything-any-more/m-p/1219975/highlight/true#M57937)).
4. In Vitis, create a new bare metal software project from the platform.
5. Copy the contents of the `<BD_NAME>_src` folder into the `<BD_NAME>/src/` folder.
6. Build the platform and software projects.
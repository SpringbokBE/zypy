# Vivado source code repository

This repository contains the Vivado source code for the following block designs:

- `ufunc_call_f4`
- `ufunc_call_i4`
- `ufunc_reduce_all_f4`
- `ufunc_reduce_all_i4`
- `filter_avg_f4`
- `heat_eqn_f4`

## Prerequisites

- [Xilinx Vivado 2020.2](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vivado-design-tools/2020-2.html)
- [Microsoft Windows](https://www.microsoft.com/en-us/windows)
- [PYNQ-Z2 board files for Vivado](https://pynq.readthedocs.io/en/v2.6.1/overlay_design_methodology/board_settings.html)

Please make sure `../Xilinx/Vivado/2020.2/bin` is added to the PATH environment variable.

## Recreating the Vivado projects

1. Run any of the `<BD_NAME>/build.bat` files to recreate the Vivado project.

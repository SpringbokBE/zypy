# Vitis HLS source code repository

This repository contains the HLS source code for the following IP cores:

- `ufunc_call_f4`
- `ufunc_call_i4`
- `ufunc_reduce_all_f4`
- `ufunc_reduce_all_i4`
- `sad_reduce_all_f4`
- `filter_avg_f4`

## Prerequisites

- [Xilinx Vitis HLS 2020.2](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vitis.html)
- [Microsoft Windows](https://www.microsoft.com/en-us/windows)
- [PYNQ-Z2 board files for Vitis](https://pynq.readthedocs.io/en/v2.6.1/overlay_design_methodology/board_settings.html)

## Recreating the Vitis HLS projects

1. Run any of the `setup_<IP_NAME>.bat` files to recreate the Vitis HLS project.
2. Manually startup Vitis HLS and navigate to the `<IP_NAME>` folder.

## Synthesising, simulating and exporting the IP cores directly

1. Run any of the `build_<IP_NAME>.bat` files.
2. The exported IP core (`export.zip`) can be found in the `<IP_NAME>/solution1/impl/` folder.

If any of the above steps fail, please check the `vitis_hls.log` file for more info.
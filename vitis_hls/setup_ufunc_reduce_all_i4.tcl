#################
# Project setup #
#################

open_project ufunc_reduce_all_i4
set_top ufunc_reduce_all_i4
add_files src/ufunc_reduce_all_i4.cc
add_files src/ufuncs_i4.cc
add_files -tb src/ufunc_reduce_all_i4_tb.cc

open_solution "solution1" -flow_target vivado
set_part {xc7z020clg400-1}
create_clock -period 10 -name default
# csim_design
# csynth_design
# cosim_design
# export_design -rtl verilog -format ip_catalog -vendor "zypy" -library "hls" -version "1.0" -display_name "ufunc_reduce_all_i4"

exit

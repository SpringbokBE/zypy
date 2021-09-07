#################
# Project setup #
#################

open_project ufunc_call_f4
set_top ufunc_call_f4
add_files src/ufunc_call_f4.cc
add_files src/ufuncs_f4.cc
add_files -tb src/ufunc_call_f4_tb.cc

open_solution "solution1" -flow_target vivado
set_part {xc7z020clg400-1}
create_clock -period 10 -name default
# csim_design
# csynth_design
# cosim_design
# export_design -rtl verilog -format ip_catalog -vendor "zypy" -library "hls" -version "1.0" -display_name "ufunc_call_f4"

exit

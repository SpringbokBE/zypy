start_gui

#################
# Project setup #
#################

set workdir [file normalize [file join [file dirname [pwd]] ..]]

create_project heat_eqn_f4 "${workdir}/bd/heat_eqn_f4" -part xc7z020clg400-1
set_property  ip_repo_paths [list [file normalize [file join $workdir ip]]] [current_project]
update_ip_catalog
set_property board_part tul.com.tw:pynq-z2:part0:1.0 [current_project]
create_bd_design "design_1"
update_compile_order -fileset sources_1

#####################
# Initilise Zynq PS #
#####################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.5 processing_system7_0
endgroup
apply_bd_automation -rule xilinx.com:bd_rule:processing_system7 -config {make_external "FIXED_IO, DDR" apply_board_preset "1" Master "Disable" Slave "Disable" }  [get_bd_cells processing_system7_0]
startgroup
set_property -dict [list CONFIG.PCW_USE_S_AXI_HP0 {1} CONFIG.PCW_USE_S_AXI_HP1 {1} CONFIG.PCW_USE_S_AXI_HP2 {1}] [get_bd_cells processing_system7_0]
endgroup

###################
# Initialise DMAs #
###################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dma:7.1 axi_dma_0
endgroup
set_property -dict [list CONFIG.c_m_axi_s2mm_data_width.VALUE_SRC USER] [get_bd_cells axi_dma_0]
set_property -dict [list CONFIG.c_include_sg {0} CONFIG.c_sg_length_width {26} CONFIG.c_sg_include_stscntrl_strm {0} CONFIG.c_m_axi_mm2s_data_width {64} CONFIG.c_include_mm2s_dre {1} CONFIG.c_mm2s_burst_size {256} CONFIG.c_m_axi_s2mm_data_width {64} CONFIG.c_include_s2mm_dre {1} CONFIG.c_s2mm_burst_size {256}] [get_bd_cells axi_dma_0]
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dma:7.1 axi_dma_1
endgroup
set_property -dict [list CONFIG.c_include_sg {0} CONFIG.c_sg_length_width {26} CONFIG.c_sg_include_stscntrl_strm {0} CONFIG.c_m_axi_mm2s_data_width {64} CONFIG.c_include_mm2s_dre {1} CONFIG.c_mm2s_burst_size {256} CONFIG.c_include_s2mm {0}] [get_bd_cells axi_dma_1]
startgroup
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/processing_system7_0/M_AXI_GP0} Slave {/axi_dma_0/S_AXI_LITE} ddr_seg {Auto} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins axi_dma_0/S_AXI_LITE]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/processing_system7_0/M_AXI_GP0} Slave {/axi_dma_1/S_AXI_LITE} ddr_seg {Auto} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins axi_dma_1/S_AXI_LITE]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/axi_dma_0/M_AXI_MM2S} Slave {/processing_system7_0/S_AXI_HP0} ddr_seg {Auto} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins processing_system7_0/S_AXI_HP0]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/axi_dma_1/M_AXI_MM2S} Slave {/processing_system7_0/S_AXI_HP1} ddr_seg {Auto} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins processing_system7_0/S_AXI_HP1]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/axi_dma_0/M_AXI_S2MM} Slave {/processing_system7_0/S_AXI_HP2} ddr_seg {Auto} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins processing_system7_0/S_AXI_HP2]
endgroup

#####################
# Initialise switch #
#####################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axis_switch:1.1 axis_switch_0
endgroup
set_property -dict [list CONFIG.NUM_SI {3} CONFIG.NUM_MI {4} CONFIG.ROUTING_MODE {1} CONFIG.DECODER_REG {1} CONFIG.M00_S01_CONNECTIVITY {0} CONFIG.M00_S02_CONNECTIVITY {0} CONFIG.M01_S01_CONNECTIVITY {0} CONFIG.M01_S02_CONNECTIVITY {0} CONFIG.M02_S00_CONNECTIVITY {0} CONFIG.M02_S02_CONNECTIVITY {0} CONFIG.M03_S00_CONNECTIVITY {0} CONFIG.M03_S01_CONNECTIVITY {0}] [get_bd_cells axis_switch_0]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/processing_system7_0/FCLK_CLK0 (100 MHz)} Clk_slave {Auto} Clk_xbar {/processing_system7_0/FCLK_CLK0 (100 MHz)} Master {/processing_system7_0/M_AXI_GP0} Slave {/axis_switch_0/S_AXI_CTRL} ddr_seg {Auto} intc_ip {/ps7_0_axi_periph} master_apm {0}}  [get_bd_intf_pins axis_switch_0/S_AXI_CTRL]

###########################
# Initialise accelerators #
###########################

startgroup
create_bd_cell -type ip -vlnv zypy:hls:filter_avg_f4:1.0 filter_avg_f4_0
endgroup
startgroup
create_bd_cell -type ip -vlnv zypy:hls:sad_reduce_all_f4:1.0 sad_reduce_all_f4_0
endgroup

######################
# Connect components #
######################

connect_bd_intf_net [get_bd_intf_pins axi_dma_0/M_AXIS_MM2S] [get_bd_intf_pins axis_switch_0/S00_AXIS]
connect_bd_intf_net [get_bd_intf_pins axi_dma_1/M_AXIS_MM2S] [get_bd_intf_pins axis_switch_0/S01_AXIS]
connect_bd_intf_net [get_bd_intf_pins filter_avg_f4_0/out_s] [get_bd_intf_pins axis_switch_0/S02_AXIS]
connect_bd_intf_net [get_bd_intf_pins axis_switch_0/M00_AXIS] [get_bd_intf_pins filter_avg_f4_0/in_s]
connect_bd_intf_net [get_bd_intf_pins axis_switch_0/M01_AXIS] [get_bd_intf_pins sad_reduce_all_f4_0/in1_s]
connect_bd_intf_net [get_bd_intf_pins axis_switch_0/M02_AXIS] [get_bd_intf_pins sad_reduce_all_f4_0/in2_s]
connect_bd_intf_net [get_bd_intf_pins axis_switch_0/M03_AXIS] [get_bd_intf_pins axi_dma_0/S_AXIS_S2MM]
startgroup
apply_bd_automation -rule xilinx.com:bd_rule:clkrst -config { Clk {/processing_system7_0/FCLK_CLK0 (100 MHz)} Freq {100} Ref_Clk0 {} Ref_Clk1 {} Ref_Clk2 {}}  [get_bd_pins axis_switch_0/aclk]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/processing_system7_0/FCLK_CLK0 (100 MHz)} Clk_slave {Auto} Clk_xbar {/processing_system7_0/FCLK_CLK0 (100 MHz)} Master {/processing_system7_0/M_AXI_GP0} Slave {/filter_avg_f4_0/s_axi_control} ddr_seg {Auto} intc_ip {/ps7_0_axi_periph} master_apm {0}}  [get_bd_intf_pins filter_avg_f4_0/s_axi_control]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/processing_system7_0/FCLK_CLK0 (100 MHz)} Clk_slave {Auto} Clk_xbar {/processing_system7_0/FCLK_CLK0 (100 MHz)} Master {/processing_system7_0/M_AXI_GP0} Slave {/sad_reduce_all_f4_0/s_axi_control} ddr_seg {Auto} intc_ip {/ps7_0_axi_periph} master_apm {0}}  [get_bd_intf_pins sad_reduce_all_f4_0/s_axi_control]
endgroup

#####################
# Finalise and save #
#####################

regenerate_bd_layout
validate_bd_design
save_bd_design
make_wrapper -files [get_files "${workdir}/bd/heat_eqn_f4/heat_eqn_f4.srcs/sources_1/bd/design_1/design_1.bd"] -top
add_files -norecurse "${workdir}/bd/heat_eqn_f4/heat_eqn_f4.gen/sources_1/bd/design_1/hdl/design_1_wrapper.v"

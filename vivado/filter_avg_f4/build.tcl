start_gui

#################
# Project setup #
#################

set workdir [file normalize [file join [file dirname [pwd]] ..]]

create_project filter_avg_f4 "${workdir}/bd/filter_avg_f4" -part xc7z020clg400-1
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
set_property -dict [list CONFIG.PCW_USE_S_AXI_HP0 {1} CONFIG.PCW_USE_S_AXI_HP2 {1}] [get_bd_cells processing_system7_0]
endgroup

###################
# Initialise DMAs #
###################

startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_dma:7.1 axi_dma_0
endgroup
set_property -dict [list CONFIG.c_s_axis_s2mm_tdata_width.VALUE_SRC USER CONFIG.c_m_axi_s2mm_data_width.VALUE_SRC USER] [get_bd_cells axi_dma_0]
set_property -dict [list CONFIG.c_include_sg {0} CONFIG.c_sg_length_width {26} CONFIG.c_sg_include_stscntrl_strm {0} CONFIG.c_m_axi_mm2s_data_width {64} CONFIG.c_include_mm2s_dre {1} CONFIG.c_mm2s_burst_size {256} CONFIG.c_m_axi_s2mm_data_width {64} CONFIG.c_include_s2mm_dre {1} CONFIG.c_s2mm_burst_size {256}] [get_bd_cells axi_dma_0]

startgroup
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/processing_system7_0/M_AXI_GP0} Slave {/axi_dma_0/S_AXI_LITE} ddr_seg {Auto} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins axi_dma_0/S_AXI_LITE]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/axi_dma_0/M_AXI_MM2S} Slave {/processing_system7_0/S_AXI_HP0} ddr_seg {Auto} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins processing_system7_0/S_AXI_HP0]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/axi_dma_0/M_AXI_S2MM} Slave {/processing_system7_0/S_AXI_HP2} ddr_seg {Auto} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins processing_system7_0/S_AXI_HP2]
endgroup

##########################
# Initialise accelerator #
##########################

startgroup
create_bd_cell -type ip -vlnv zypy:hls:filter_avg_f4:1.0 filter_avg_f4_0
endgroup
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/processing_system7_0/FCLK_CLK0 (100 MHz)} Clk_slave {Auto} Clk_xbar {/processing_system7_0/FCLK_CLK0 (100 MHz)} Master {/processing_system7_0/M_AXI_GP0} Slave {/filter_avg_f4_0/s_axi_control} ddr_seg {Auto} intc_ip {/ps7_0_axi_periph} master_apm {0}}  [get_bd_intf_pins filter_avg_f4_0/s_axi_control]
connect_bd_intf_net [get_bd_intf_pins axi_dma_0/S_AXIS_S2MM] [get_bd_intf_pins filter_avg_f4_0/out_s]
connect_bd_intf_net [get_bd_intf_pins axi_dma_0/M_AXIS_MM2S] [get_bd_intf_pins filter_avg_f4_0/in_s]

#####################
# ILA for debugging #
#####################

# startgroup
# create_bd_cell -type ip -vlnv xilinx.com:ip:system_ila:1.1 system_ila_0
# endgroup
# set_property -dict [list CONFIG.C_SLOT {2} CONFIG.C_BRAM_CNT {24} CONFIG.C_DATA_DEPTH {2048} CONFIG.C_NUM_MONITOR_SLOTS {3} CONFIG.C_SLOT_1_INTF_TYPE {xilinx.com:interface:axis_rtl:1.0} CONFIG.C_SLOT_2_INTF_TYPE {xilinx.com:interface:axis_rtl:1.0}] [get_bd_cells system_ila_0]
# connect_bd_intf_net [get_bd_intf_pins system_ila_0/SLOT_0_AXI] [get_bd_intf_pins filter_avg_f4_0/s_axi_control]
# connect_bd_intf_net [get_bd_intf_pins system_ila_0/SLOT_1_AXIS] [get_bd_intf_pins filter_avg_f4_0/in_s]
# connect_bd_intf_net [get_bd_intf_pins system_ila_0/SLOT_2_AXIS] [get_bd_intf_pins filter_avg_f4_0/out_s]
# connect_bd_net [get_bd_pins system_ila_0/clk] [get_bd_pins processing_system7_0/FCLK_CLK0]
# connect_bd_net [get_bd_pins system_ila_0/resetn] [get_bd_pins rst_ps7_0_100M/peripheral_aresetn]

#####################
# Finalise and save #
#####################

regenerate_bd_layout
validate_bd_design
save_bd_design
make_wrapper -files [get_files "${workdir}/bd/filter_avg_f4/filter_avg_f4.srcs/sources_1/bd/design_1/design_1.bd"] -top
add_files -norecurse "${workdir}/bd/filter_avg_f4/filter_avg_f4.gen/sources_1/bd/design_1/hdl/design_1_wrapper.v"

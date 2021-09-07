#include "defines.h"

float to_float(unsigned int val) {
    union {
        unsigned int u4;
        float f4;
    } data = {val};

    return data.f4;
}

void filter_avg_f4(unsigned int x1_addr, unsigned int out_addr, unsigned int height, unsigned int width)
{
    init();
    
    filter_avg_f4_0_args_args_w(((height << 0) | (width << 16)));
    filter_avg_f4_0_ctrl_ap_start_rw(0x1);

    axis_switch_0_route_m_to_s(0, 0);
    axis_switch_0_disable_m(1);
    axis_switch_0_disable_m(2);
    axis_switch_0_route_m_to_s(3, 2);
    axis_switch_0_update();

    unsigned int length = height * width * sizeof(float); 

    axi_dma_0_mm2s_dmacr_reset_rw(0x1);
    axi_dma_0_s2mm_dmacr_reset_rw(0x1);
    
    axi_dma_0_mm2s_dmacr_rs_rw(0x1);
    axi_dma_0_mm2s_sa_source_address_rw(x1_addr);
    axi_dma_0_mm2s_length_length_rw(length);
    
    axi_dma_0_s2mm_dmacr_rs_rw(0x1);
    axi_dma_0_s2mm_da_destination_address_rw(out_addr);
    axi_dma_0_s2mm_length_length_rw(length);
    while(!(axi_dma_0_s2mm_dmasr_idle_r()));

    exit();
}

float sad_reduce_all_f4(unsigned int x1_addr, unsigned int x2_addr, unsigned int nbytes)
{
    init();
    
    sad_reduce_all_f4_0_ctrl_ap_start_rw(0x1);

    axis_switch_0_disable_m(0);
    axis_switch_0_route_m_to_s(1, 0);
    axis_switch_0_route_m_to_s(2, 1);
    axis_switch_0_disable_m(3);
    axis_switch_0_update();

    axi_dma_0_mm2s_dmacr_reset_rw(0x1);
    axi_dma_1_mm2s_dmacr_reset_rw(0x1);

    axi_dma_0_mm2s_dmacr_rs_rw(0x1);
    axi_dma_0_mm2s_sa_source_address_rw(x1_addr);
    axi_dma_0_mm2s_length_length_rw(nbytes);

    axi_dma_1_mm2s_dmacr_rs_rw(0x1);
    axi_dma_1_mm2s_sa_source_address_rw(x2_addr);
    axi_dma_1_mm2s_length_length_rw(nbytes);

    while(!(sad_reduce_all_f4_0_ctrl_ap_done_r()));

    float res = to_float(sad_reduce_all_f4_0_ap_return_ap_return_r());
    
    exit();

    return res; 
}

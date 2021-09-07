#include "defines.h"

void filter_avg_f4(
    unsigned int x1_addr,
    unsigned int out_addr,
    unsigned int height,
    unsigned int width)
{
    init();
    
    filter_avg_f4_0_args_args_w(((height << 0) | (width << 16)));
    filter_avg_f4_0_ctrl_ap_start_rw(0x1);

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

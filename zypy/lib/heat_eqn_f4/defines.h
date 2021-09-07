#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/mman.h>

///////////////////
// IP addresses. //
///////////////////

#define axi_dma_0_addr 0x40400000
#define axi_dma_1_addr 0x40410000
#define axis_switch_0_addr 0x43c00000
#define filter_avg_f4_0_addr 0x40000000
#define sad_reduce_all_f4_0_addr 0x40010000

//////////////////////////////////////
// IP register getters and setters. //
//////////////////////////////////////

#define axi_dma_0_mm2s_dmacr_rs_rw(x) x ? (axi_dma_0_vaddr[0 >> 2] = ((x & 0x1) << 0)) : (axi_dma_0_vaddr[0 >> 2] & (0x1 << 0))
#define axi_dma_0_mm2s_dmacr_reset_rw(x) x ? (axi_dma_0_vaddr[0 >> 2] = ((x & 0x1) << 2)) : (axi_dma_0_vaddr[0 >> 2] & (0x1 << 2))
#define axi_dma_0_mm2s_dmacr_keyhole_rw(x) x ? (axi_dma_0_vaddr[0 >> 2] = ((x & 0x1) << 3)) : (axi_dma_0_vaddr[0 >> 2] & (0x1 << 3))
#define axi_dma_0_mm2s_dmacr_cyclic_bd_enable_rw(x) x ? (axi_dma_0_vaddr[0 >> 2] = ((x & 0x1) << 4)) : (axi_dma_0_vaddr[0 >> 2] & (0x1 << 4))
#define axi_dma_0_mm2s_dmacr_ioc_irqen_rw(x) x ? (axi_dma_0_vaddr[0 >> 2] = ((x & 0x1) << 12)) : (axi_dma_0_vaddr[0 >> 2] & (0x1 << 12))
#define axi_dma_0_mm2s_dmacr_dly_irqen_rw(x) x ? (axi_dma_0_vaddr[0 >> 2] = ((x & 0x1) << 13)) : (axi_dma_0_vaddr[0 >> 2] & (0x1 << 13))
#define axi_dma_0_mm2s_dmacr_err_irqen_rw(x) x ? (axi_dma_0_vaddr[0 >> 2] = ((x & 0x1) << 14)) : (axi_dma_0_vaddr[0 >> 2] & (0x1 << 14))
#define axi_dma_0_mm2s_dmacr_irqthreshold_rw(x) x ? (axi_dma_0_vaddr[0 >> 2] = ((x & 0xff) << 16)) : (axi_dma_0_vaddr[0 >> 2] & (0xff << 16))
#define axi_dma_0_mm2s_dmacr_irqdelay_rw(x) x ? (axi_dma_0_vaddr[0 >> 2] = ((x & 0xff) << 24)) : (axi_dma_0_vaddr[0 >> 2] & (0xff << 24))
#define axi_dma_0_mm2s_dmasr_halted_r() (axi_dma_0_vaddr[4 >> 2] & (0x1 << 0))
#define axi_dma_0_mm2s_dmasr_idle_r() (axi_dma_0_vaddr[4 >> 2] & (0x1 << 1))
#define axi_dma_0_mm2s_dmasr_sgincld_r() (axi_dma_0_vaddr[4 >> 2] & (0x1 << 3))
#define axi_dma_0_mm2s_dmasr_dmainterr_r() (axi_dma_0_vaddr[4 >> 2] & (0x1 << 4))
#define axi_dma_0_mm2s_dmasr_dmaslverr_r() (axi_dma_0_vaddr[4 >> 2] & (0x1 << 5))
#define axi_dma_0_mm2s_dmasr_dmadecerr_r() (axi_dma_0_vaddr[4 >> 2] & (0x1 << 6))
#define axi_dma_0_mm2s_dmasr_sginterr_r() (axi_dma_0_vaddr[4 >> 2] & (0x1 << 8))
#define axi_dma_0_mm2s_dmasr_sgslverr_r() (axi_dma_0_vaddr[4 >> 2] & (0x1 << 9))
#define axi_dma_0_mm2s_dmasr_sgdecerr_r() (axi_dma_0_vaddr[4 >> 2] & (0x1 << 10))
#define axi_dma_0_mm2s_dmasr_ioc_irq_rw(x) x ? (axi_dma_0_vaddr[4 >> 2] = ((x & 0x1) << 12)) : (axi_dma_0_vaddr[4 >> 2] & (0x1 << 12))
#define axi_dma_0_mm2s_dmasr_dly_irq_rw(x) x ? (axi_dma_0_vaddr[4 >> 2] = ((x & 0x1) << 13)) : (axi_dma_0_vaddr[4 >> 2] & (0x1 << 13))
#define axi_dma_0_mm2s_dmasr_err_irq_rw(x) x ? (axi_dma_0_vaddr[4 >> 2] = ((x & 0x1) << 14)) : (axi_dma_0_vaddr[4 >> 2] & (0x1 << 14))
#define axi_dma_0_mm2s_dmasr_irqthresholdsts_r() (axi_dma_0_vaddr[4 >> 2] & (0xff << 16))
#define axi_dma_0_mm2s_dmasr_irqdelaysts_r() (axi_dma_0_vaddr[4 >> 2] & (0xff << 24))
#define axi_dma_0_mm2s_curdesc_current_descriptor_pointer_rw(x) x ? (axi_dma_0_vaddr[8 >> 2] = ((x & 0x3ffffff) << 6)) : (axi_dma_0_vaddr[8 >> 2] & (0x3ffffff << 6))
#define axi_dma_0_mm2s_curdesc_msb_current_descriptor_pointer_rw(x) x ? (axi_dma_0_vaddr[12 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_0_vaddr[12 >> 2] & (0xffffffff << 0))
#define axi_dma_0_mm2s_taildesc_tail_descriptor_pointer_rw(x) x ? (axi_dma_0_vaddr[16 >> 2] = ((x & 0x3ffffff) << 6)) : (axi_dma_0_vaddr[16 >> 2] & (0x3ffffff << 6))
#define axi_dma_0_mm2s_taildesc_msb_tail_descriptor_pointer_rw(x) x ? (axi_dma_0_vaddr[20 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_0_vaddr[20 >> 2] & (0xffffffff << 0))
#define axi_dma_0_mm2s_sa_source_address_rw(x) x ? (axi_dma_0_vaddr[24 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_0_vaddr[24 >> 2] & (0xffffffff << 0))
#define axi_dma_0_mm2s_sa_msb_source_address_rw(x) x ? (axi_dma_0_vaddr[28 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_0_vaddr[28 >> 2] & (0xffffffff << 0))
#define axi_dma_0_mm2s_length_length_rw(x) x ? (axi_dma_0_vaddr[40 >> 2] = ((x & 0x3ffffff) << 0)) : (axi_dma_0_vaddr[40 >> 2] & (0x3ffffff << 0))
#define axi_dma_0_sg_ctl_sg_cache_rw(x) x ? (axi_dma_0_vaddr[44 >> 2] = ((x & 0xf) << 0)) : (axi_dma_0_vaddr[44 >> 2] & (0xf << 0))
#define axi_dma_0_sg_ctl_sg_user_rw(x) x ? (axi_dma_0_vaddr[44 >> 2] = ((x & 0xf) << 8)) : (axi_dma_0_vaddr[44 >> 2] & (0xf << 8))
#define axi_dma_0_s2mm_dmacr_rs_rw(x) x ? (axi_dma_0_vaddr[48 >> 2] = ((x & 0x1) << 0)) : (axi_dma_0_vaddr[48 >> 2] & (0x1 << 0))
#define axi_dma_0_s2mm_dmacr_reset_rw(x) x ? (axi_dma_0_vaddr[48 >> 2] = ((x & 0x1) << 2)) : (axi_dma_0_vaddr[48 >> 2] & (0x1 << 2))
#define axi_dma_0_s2mm_dmacr_keyhole_rw(x) x ? (axi_dma_0_vaddr[48 >> 2] = ((x & 0x1) << 3)) : (axi_dma_0_vaddr[48 >> 2] & (0x1 << 3))
#define axi_dma_0_s2mm_dmacr_cyclic_bd_enable_rw(x) x ? (axi_dma_0_vaddr[48 >> 2] = ((x & 0x1) << 4)) : (axi_dma_0_vaddr[48 >> 2] & (0x1 << 4))
#define axi_dma_0_s2mm_dmacr_ioc_irqen_rw(x) x ? (axi_dma_0_vaddr[48 >> 2] = ((x & 0x1) << 12)) : (axi_dma_0_vaddr[48 >> 2] & (0x1 << 12))
#define axi_dma_0_s2mm_dmacr_dly_irqen_rw(x) x ? (axi_dma_0_vaddr[48 >> 2] = ((x & 0x1) << 13)) : (axi_dma_0_vaddr[48 >> 2] & (0x1 << 13))
#define axi_dma_0_s2mm_dmacr_err_irqen_rw(x) x ? (axi_dma_0_vaddr[48 >> 2] = ((x & 0x1) << 14)) : (axi_dma_0_vaddr[48 >> 2] & (0x1 << 14))
#define axi_dma_0_s2mm_dmacr_irqthreshold_rw(x) x ? (axi_dma_0_vaddr[48 >> 2] = ((x & 0xff) << 16)) : (axi_dma_0_vaddr[48 >> 2] & (0xff << 16))
#define axi_dma_0_s2mm_dmacr_irqdelay_rw(x) x ? (axi_dma_0_vaddr[48 >> 2] = ((x & 0xff) << 24)) : (axi_dma_0_vaddr[48 >> 2] & (0xff << 24))
#define axi_dma_0_s2mm_dmasr_halted_r() (axi_dma_0_vaddr[52 >> 2] & (0x1 << 0))
#define axi_dma_0_s2mm_dmasr_idle_r() (axi_dma_0_vaddr[52 >> 2] & (0x1 << 1))
#define axi_dma_0_s2mm_dmasr_sgincld_r() (axi_dma_0_vaddr[52 >> 2] & (0x1 << 3))
#define axi_dma_0_s2mm_dmasr_dmainterr_r() (axi_dma_0_vaddr[52 >> 2] & (0x1 << 4))
#define axi_dma_0_s2mm_dmasr_dmaslverr_r() (axi_dma_0_vaddr[52 >> 2] & (0x1 << 5))
#define axi_dma_0_s2mm_dmasr_dmadecerr_r() (axi_dma_0_vaddr[52 >> 2] & (0x1 << 6))
#define axi_dma_0_s2mm_dmasr_sginterr_r() (axi_dma_0_vaddr[52 >> 2] & (0x1 << 8))
#define axi_dma_0_s2mm_dmasr_sgslverr_r() (axi_dma_0_vaddr[52 >> 2] & (0x1 << 9))
#define axi_dma_0_s2mm_dmasr_sgdecerr_r() (axi_dma_0_vaddr[52 >> 2] & (0x1 << 10))
#define axi_dma_0_s2mm_dmasr_ioc_irq_rw(x) x ? (axi_dma_0_vaddr[52 >> 2] = ((x & 0x1) << 12)) : (axi_dma_0_vaddr[52 >> 2] & (0x1 << 12))
#define axi_dma_0_s2mm_dmasr_dly_irq_rw(x) x ? (axi_dma_0_vaddr[52 >> 2] = ((x & 0x1) << 13)) : (axi_dma_0_vaddr[52 >> 2] & (0x1 << 13))
#define axi_dma_0_s2mm_dmasr_err_irq_rw(x) x ? (axi_dma_0_vaddr[52 >> 2] = ((x & 0x1) << 14)) : (axi_dma_0_vaddr[52 >> 2] & (0x1 << 14))
#define axi_dma_0_s2mm_dmasr_irqthresholdsts_r() (axi_dma_0_vaddr[52 >> 2] & (0xff << 16))
#define axi_dma_0_s2mm_dmasr_irqdelaysts_r() (axi_dma_0_vaddr[52 >> 2] & (0xff << 24))
#define axi_dma_0_s2mm_curdesc_current_descriptor_pointer_rw(x) x ? (axi_dma_0_vaddr[56 >> 2] = ((x & 0x3ffffff) << 6)) : (axi_dma_0_vaddr[56 >> 2] & (0x3ffffff << 6))
#define axi_dma_0_s2mm_curdesc_msb_current_descriptor_pointer_rw(x) x ? (axi_dma_0_vaddr[60 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_0_vaddr[60 >> 2] & (0xffffffff << 0))
#define axi_dma_0_s2mm_taildesc_tail_descriptor_pointer_rw(x) x ? (axi_dma_0_vaddr[64 >> 2] = ((x & 0x3ffffff) << 6)) : (axi_dma_0_vaddr[64 >> 2] & (0x3ffffff << 6))
#define axi_dma_0_s2mm_taildesc_msb_tail_descriptor_pointer_rw(x) x ? (axi_dma_0_vaddr[68 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_0_vaddr[68 >> 2] & (0xffffffff << 0))
#define axi_dma_0_s2mm_da_destination_address_rw(x) x ? (axi_dma_0_vaddr[72 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_0_vaddr[72 >> 2] & (0xffffffff << 0))
#define axi_dma_0_s2mm_da_msb_destination_address_rw(x) x ? (axi_dma_0_vaddr[76 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_0_vaddr[76 >> 2] & (0xffffffff << 0))
#define axi_dma_0_s2mm_length_length_rw(x) x ? (axi_dma_0_vaddr[88 >> 2] = ((x & 0x3ffffff) << 0)) : (axi_dma_0_vaddr[88 >> 2] & (0x3ffffff << 0))
#define axi_dma_1_mm2s_dmacr_rs_rw(x) x ? (axi_dma_1_vaddr[0 >> 2] = ((x & 0x1) << 0)) : (axi_dma_1_vaddr[0 >> 2] & (0x1 << 0))
#define axi_dma_1_mm2s_dmacr_reset_rw(x) x ? (axi_dma_1_vaddr[0 >> 2] = ((x & 0x1) << 2)) : (axi_dma_1_vaddr[0 >> 2] & (0x1 << 2))
#define axi_dma_1_mm2s_dmacr_keyhole_rw(x) x ? (axi_dma_1_vaddr[0 >> 2] = ((x & 0x1) << 3)) : (axi_dma_1_vaddr[0 >> 2] & (0x1 << 3))
#define axi_dma_1_mm2s_dmacr_cyclic_bd_enable_rw(x) x ? (axi_dma_1_vaddr[0 >> 2] = ((x & 0x1) << 4)) : (axi_dma_1_vaddr[0 >> 2] & (0x1 << 4))
#define axi_dma_1_mm2s_dmacr_ioc_irqen_rw(x) x ? (axi_dma_1_vaddr[0 >> 2] = ((x & 0x1) << 12)) : (axi_dma_1_vaddr[0 >> 2] & (0x1 << 12))
#define axi_dma_1_mm2s_dmacr_dly_irqen_rw(x) x ? (axi_dma_1_vaddr[0 >> 2] = ((x & 0x1) << 13)) : (axi_dma_1_vaddr[0 >> 2] & (0x1 << 13))
#define axi_dma_1_mm2s_dmacr_err_irqen_rw(x) x ? (axi_dma_1_vaddr[0 >> 2] = ((x & 0x1) << 14)) : (axi_dma_1_vaddr[0 >> 2] & (0x1 << 14))
#define axi_dma_1_mm2s_dmacr_irqthreshold_rw(x) x ? (axi_dma_1_vaddr[0 >> 2] = ((x & 0xff) << 16)) : (axi_dma_1_vaddr[0 >> 2] & (0xff << 16))
#define axi_dma_1_mm2s_dmacr_irqdelay_rw(x) x ? (axi_dma_1_vaddr[0 >> 2] = ((x & 0xff) << 24)) : (axi_dma_1_vaddr[0 >> 2] & (0xff << 24))
#define axi_dma_1_mm2s_dmasr_halted_r() (axi_dma_1_vaddr[4 >> 2] & (0x1 << 0))
#define axi_dma_1_mm2s_dmasr_idle_r() (axi_dma_1_vaddr[4 >> 2] & (0x1 << 1))
#define axi_dma_1_mm2s_dmasr_sgincld_r() (axi_dma_1_vaddr[4 >> 2] & (0x1 << 3))
#define axi_dma_1_mm2s_dmasr_dmainterr_r() (axi_dma_1_vaddr[4 >> 2] & (0x1 << 4))
#define axi_dma_1_mm2s_dmasr_dmaslverr_r() (axi_dma_1_vaddr[4 >> 2] & (0x1 << 5))
#define axi_dma_1_mm2s_dmasr_dmadecerr_r() (axi_dma_1_vaddr[4 >> 2] & (0x1 << 6))
#define axi_dma_1_mm2s_dmasr_sginterr_r() (axi_dma_1_vaddr[4 >> 2] & (0x1 << 8))
#define axi_dma_1_mm2s_dmasr_sgslverr_r() (axi_dma_1_vaddr[4 >> 2] & (0x1 << 9))
#define axi_dma_1_mm2s_dmasr_sgdecerr_r() (axi_dma_1_vaddr[4 >> 2] & (0x1 << 10))
#define axi_dma_1_mm2s_dmasr_ioc_irq_rw(x) x ? (axi_dma_1_vaddr[4 >> 2] = ((x & 0x1) << 12)) : (axi_dma_1_vaddr[4 >> 2] & (0x1 << 12))
#define axi_dma_1_mm2s_dmasr_dly_irq_rw(x) x ? (axi_dma_1_vaddr[4 >> 2] = ((x & 0x1) << 13)) : (axi_dma_1_vaddr[4 >> 2] & (0x1 << 13))
#define axi_dma_1_mm2s_dmasr_err_irq_rw(x) x ? (axi_dma_1_vaddr[4 >> 2] = ((x & 0x1) << 14)) : (axi_dma_1_vaddr[4 >> 2] & (0x1 << 14))
#define axi_dma_1_mm2s_dmasr_irqthresholdsts_r() (axi_dma_1_vaddr[4 >> 2] & (0xff << 16))
#define axi_dma_1_mm2s_dmasr_irqdelaysts_r() (axi_dma_1_vaddr[4 >> 2] & (0xff << 24))
#define axi_dma_1_mm2s_curdesc_current_descriptor_pointer_rw(x) x ? (axi_dma_1_vaddr[8 >> 2] = ((x & 0x3ffffff) << 6)) : (axi_dma_1_vaddr[8 >> 2] & (0x3ffffff << 6))
#define axi_dma_1_mm2s_curdesc_msb_current_descriptor_pointer_rw(x) x ? (axi_dma_1_vaddr[12 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_1_vaddr[12 >> 2] & (0xffffffff << 0))
#define axi_dma_1_mm2s_taildesc_tail_descriptor_pointer_rw(x) x ? (axi_dma_1_vaddr[16 >> 2] = ((x & 0x3ffffff) << 6)) : (axi_dma_1_vaddr[16 >> 2] & (0x3ffffff << 6))
#define axi_dma_1_mm2s_taildesc_msb_tail_descriptor_pointer_rw(x) x ? (axi_dma_1_vaddr[20 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_1_vaddr[20 >> 2] & (0xffffffff << 0))
#define axi_dma_1_mm2s_sa_source_address_rw(x) x ? (axi_dma_1_vaddr[24 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_1_vaddr[24 >> 2] & (0xffffffff << 0))
#define axi_dma_1_mm2s_sa_msb_source_address_rw(x) x ? (axi_dma_1_vaddr[28 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_1_vaddr[28 >> 2] & (0xffffffff << 0))
#define axi_dma_1_mm2s_length_length_rw(x) x ? (axi_dma_1_vaddr[40 >> 2] = ((x & 0x3ffffff) << 0)) : (axi_dma_1_vaddr[40 >> 2] & (0x3ffffff << 0))
#define axi_dma_1_sg_ctl_sg_cache_rw(x) x ? (axi_dma_1_vaddr[44 >> 2] = ((x & 0xf) << 0)) : (axi_dma_1_vaddr[44 >> 2] & (0xf << 0))
#define axi_dma_1_sg_ctl_sg_user_rw(x) x ? (axi_dma_1_vaddr[44 >> 2] = ((x & 0xf) << 8)) : (axi_dma_1_vaddr[44 >> 2] & (0xf << 8))
#define axi_dma_1_s2mm_dmacr_rs_rw(x) x ? (axi_dma_1_vaddr[48 >> 2] = ((x & 0x1) << 0)) : (axi_dma_1_vaddr[48 >> 2] & (0x1 << 0))
#define axi_dma_1_s2mm_dmacr_reset_rw(x) x ? (axi_dma_1_vaddr[48 >> 2] = ((x & 0x1) << 2)) : (axi_dma_1_vaddr[48 >> 2] & (0x1 << 2))
#define axi_dma_1_s2mm_dmacr_keyhole_rw(x) x ? (axi_dma_1_vaddr[48 >> 2] = ((x & 0x1) << 3)) : (axi_dma_1_vaddr[48 >> 2] & (0x1 << 3))
#define axi_dma_1_s2mm_dmacr_cyclic_bd_enable_rw(x) x ? (axi_dma_1_vaddr[48 >> 2] = ((x & 0x1) << 4)) : (axi_dma_1_vaddr[48 >> 2] & (0x1 << 4))
#define axi_dma_1_s2mm_dmacr_ioc_irqen_rw(x) x ? (axi_dma_1_vaddr[48 >> 2] = ((x & 0x1) << 12)) : (axi_dma_1_vaddr[48 >> 2] & (0x1 << 12))
#define axi_dma_1_s2mm_dmacr_dly_irqen_rw(x) x ? (axi_dma_1_vaddr[48 >> 2] = ((x & 0x1) << 13)) : (axi_dma_1_vaddr[48 >> 2] & (0x1 << 13))
#define axi_dma_1_s2mm_dmacr_err_irqen_rw(x) x ? (axi_dma_1_vaddr[48 >> 2] = ((x & 0x1) << 14)) : (axi_dma_1_vaddr[48 >> 2] & (0x1 << 14))
#define axi_dma_1_s2mm_dmacr_irqthreshold_rw(x) x ? (axi_dma_1_vaddr[48 >> 2] = ((x & 0xff) << 16)) : (axi_dma_1_vaddr[48 >> 2] & (0xff << 16))
#define axi_dma_1_s2mm_dmacr_irqdelay_rw(x) x ? (axi_dma_1_vaddr[48 >> 2] = ((x & 0xff) << 24)) : (axi_dma_1_vaddr[48 >> 2] & (0xff << 24))
#define axi_dma_1_s2mm_dmasr_halted_r() (axi_dma_1_vaddr[52 >> 2] & (0x1 << 0))
#define axi_dma_1_s2mm_dmasr_idle_r() (axi_dma_1_vaddr[52 >> 2] & (0x1 << 1))
#define axi_dma_1_s2mm_dmasr_sgincld_r() (axi_dma_1_vaddr[52 >> 2] & (0x1 << 3))
#define axi_dma_1_s2mm_dmasr_dmainterr_r() (axi_dma_1_vaddr[52 >> 2] & (0x1 << 4))
#define axi_dma_1_s2mm_dmasr_dmaslverr_r() (axi_dma_1_vaddr[52 >> 2] & (0x1 << 5))
#define axi_dma_1_s2mm_dmasr_dmadecerr_r() (axi_dma_1_vaddr[52 >> 2] & (0x1 << 6))
#define axi_dma_1_s2mm_dmasr_sginterr_r() (axi_dma_1_vaddr[52 >> 2] & (0x1 << 8))
#define axi_dma_1_s2mm_dmasr_sgslverr_r() (axi_dma_1_vaddr[52 >> 2] & (0x1 << 9))
#define axi_dma_1_s2mm_dmasr_sgdecerr_r() (axi_dma_1_vaddr[52 >> 2] & (0x1 << 10))
#define axi_dma_1_s2mm_dmasr_ioc_irq_rw(x) x ? (axi_dma_1_vaddr[52 >> 2] = ((x & 0x1) << 12)) : (axi_dma_1_vaddr[52 >> 2] & (0x1 << 12))
#define axi_dma_1_s2mm_dmasr_dly_irq_rw(x) x ? (axi_dma_1_vaddr[52 >> 2] = ((x & 0x1) << 13)) : (axi_dma_1_vaddr[52 >> 2] & (0x1 << 13))
#define axi_dma_1_s2mm_dmasr_err_irq_rw(x) x ? (axi_dma_1_vaddr[52 >> 2] = ((x & 0x1) << 14)) : (axi_dma_1_vaddr[52 >> 2] & (0x1 << 14))
#define axi_dma_1_s2mm_dmasr_irqthresholdsts_r() (axi_dma_1_vaddr[52 >> 2] & (0xff << 16))
#define axi_dma_1_s2mm_dmasr_irqdelaysts_r() (axi_dma_1_vaddr[52 >> 2] & (0xff << 24))
#define axi_dma_1_s2mm_curdesc_current_descriptor_pointer_rw(x) x ? (axi_dma_1_vaddr[56 >> 2] = ((x & 0x3ffffff) << 6)) : (axi_dma_1_vaddr[56 >> 2] & (0x3ffffff << 6))
#define axi_dma_1_s2mm_curdesc_msb_current_descriptor_pointer_rw(x) x ? (axi_dma_1_vaddr[60 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_1_vaddr[60 >> 2] & (0xffffffff << 0))
#define axi_dma_1_s2mm_taildesc_tail_descriptor_pointer_rw(x) x ? (axi_dma_1_vaddr[64 >> 2] = ((x & 0x3ffffff) << 6)) : (axi_dma_1_vaddr[64 >> 2] & (0x3ffffff << 6))
#define axi_dma_1_s2mm_taildesc_msb_tail_descriptor_pointer_rw(x) x ? (axi_dma_1_vaddr[68 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_1_vaddr[68 >> 2] & (0xffffffff << 0))
#define axi_dma_1_s2mm_da_destination_address_rw(x) x ? (axi_dma_1_vaddr[72 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_1_vaddr[72 >> 2] & (0xffffffff << 0))
#define axi_dma_1_s2mm_da_msb_destination_address_rw(x) x ? (axi_dma_1_vaddr[76 >> 2] = ((x & 0xffffffff) << 0)) : (axi_dma_1_vaddr[76 >> 2] & (0xffffffff << 0))
#define axi_dma_1_s2mm_length_length_rw(x) x ? (axi_dma_1_vaddr[88 >> 2] = ((x & 0x3ffffff) << 0)) : (axi_dma_1_vaddr[88 >> 2] & (0x3ffffff << 0))
#define filter_avg_f4_0_ctrl_ap_start_rw(x) x ? (filter_avg_f4_0_vaddr[0 >> 2] = ((x & 0x1) << 0)) : (filter_avg_f4_0_vaddr[0 >> 2] & (0x1 << 0))
#define filter_avg_f4_0_ctrl_ap_done_r() (filter_avg_f4_0_vaddr[0 >> 2] & (0x1 << 1))
#define filter_avg_f4_0_ctrl_ap_idle_r() (filter_avg_f4_0_vaddr[0 >> 2] & (0x1 << 2))
#define filter_avg_f4_0_ctrl_ap_ready_r() (filter_avg_f4_0_vaddr[0 >> 2] & (0x1 << 3))
#define filter_avg_f4_0_ctrl_reserved_1_r() (filter_avg_f4_0_vaddr[0 >> 2] & (0x7 << 4))
#define filter_avg_f4_0_ctrl_auto_restart_rw(x) x ? (filter_avg_f4_0_vaddr[0 >> 2] = ((x & 0x1) << 7)) : (filter_avg_f4_0_vaddr[0 >> 2] & (0x1 << 7))
#define filter_avg_f4_0_ctrl_reserved_2_r() (filter_avg_f4_0_vaddr[0 >> 2] & (0xffffff << 8))
#define filter_avg_f4_0_gier_enable_rw(x) x ? (filter_avg_f4_0_vaddr[4 >> 2] = ((x & 0x1) << 0)) : (filter_avg_f4_0_vaddr[4 >> 2] & (0x1 << 0))
#define filter_avg_f4_0_gier_reserved_r() (filter_avg_f4_0_vaddr[4 >> 2] & (0x7fffffff << 1))
#define filter_avg_f4_0_ip_ier_chan0_int_en_rw(x) x ? (filter_avg_f4_0_vaddr[8 >> 2] = ((x & 0x1) << 0)) : (filter_avg_f4_0_vaddr[8 >> 2] & (0x1 << 0))
#define filter_avg_f4_0_ip_ier_chan1_int_en_rw(x) x ? (filter_avg_f4_0_vaddr[8 >> 2] = ((x & 0x1) << 1)) : (filter_avg_f4_0_vaddr[8 >> 2] & (0x1 << 1))
#define filter_avg_f4_0_ip_ier_reserved_r() (filter_avg_f4_0_vaddr[8 >> 2] & (0x3fffffff << 2))
#define filter_avg_f4_0_ip_isr_chan0_int_st_r() (filter_avg_f4_0_vaddr[12 >> 2] & (0x1 << 0))
#define filter_avg_f4_0_ip_isr_chan1_int_st_r() (filter_avg_f4_0_vaddr[12 >> 2] & (0x1 << 1))
#define filter_avg_f4_0_ip_isr_reserved_r() (filter_avg_f4_0_vaddr[12 >> 2] & (0x3fffffff << 2))
#define filter_avg_f4_0_args_args_w(x) (filter_avg_f4_0_vaddr[16 >> 2] = ((x & 0xffffffff) << 0))
#define sad_reduce_all_f4_0_ctrl_ap_start_rw(x) x ? (sad_reduce_all_f4_0_vaddr[0 >> 2] = ((x & 0x1) << 0)) : (sad_reduce_all_f4_0_vaddr[0 >> 2] & (0x1 << 0))
#define sad_reduce_all_f4_0_ctrl_ap_done_r() (sad_reduce_all_f4_0_vaddr[0 >> 2] & (0x1 << 1))
#define sad_reduce_all_f4_0_ctrl_ap_idle_r() (sad_reduce_all_f4_0_vaddr[0 >> 2] & (0x1 << 2))
#define sad_reduce_all_f4_0_ctrl_ap_ready_r() (sad_reduce_all_f4_0_vaddr[0 >> 2] & (0x1 << 3))
#define sad_reduce_all_f4_0_ctrl_reserved_1_r() (sad_reduce_all_f4_0_vaddr[0 >> 2] & (0x7 << 4))
#define sad_reduce_all_f4_0_ctrl_auto_restart_rw(x) x ? (sad_reduce_all_f4_0_vaddr[0 >> 2] = ((x & 0x1) << 7)) : (sad_reduce_all_f4_0_vaddr[0 >> 2] & (0x1 << 7))
#define sad_reduce_all_f4_0_ctrl_reserved_2_r() (sad_reduce_all_f4_0_vaddr[0 >> 2] & (0xffffff << 8))
#define sad_reduce_all_f4_0_gier_enable_rw(x) x ? (sad_reduce_all_f4_0_vaddr[4 >> 2] = ((x & 0x1) << 0)) : (sad_reduce_all_f4_0_vaddr[4 >> 2] & (0x1 << 0))
#define sad_reduce_all_f4_0_gier_reserved_r() (sad_reduce_all_f4_0_vaddr[4 >> 2] & (0x7fffffff << 1))
#define sad_reduce_all_f4_0_ip_ier_chan0_int_en_rw(x) x ? (sad_reduce_all_f4_0_vaddr[8 >> 2] = ((x & 0x1) << 0)) : (sad_reduce_all_f4_0_vaddr[8 >> 2] & (0x1 << 0))
#define sad_reduce_all_f4_0_ip_ier_chan1_int_en_rw(x) x ? (sad_reduce_all_f4_0_vaddr[8 >> 2] = ((x & 0x1) << 1)) : (sad_reduce_all_f4_0_vaddr[8 >> 2] & (0x1 << 1))
#define sad_reduce_all_f4_0_ip_ier_reserved_r() (sad_reduce_all_f4_0_vaddr[8 >> 2] & (0x3fffffff << 2))
#define sad_reduce_all_f4_0_ip_isr_chan0_int_st_r() (sad_reduce_all_f4_0_vaddr[12 >> 2] & (0x1 << 0))
#define sad_reduce_all_f4_0_ip_isr_chan1_int_st_r() (sad_reduce_all_f4_0_vaddr[12 >> 2] & (0x1 << 1))
#define sad_reduce_all_f4_0_ip_isr_reserved_r() (sad_reduce_all_f4_0_vaddr[12 >> 2] & (0x3fffffff << 2))
#define sad_reduce_all_f4_0_ap_return_ap_return_r() (sad_reduce_all_f4_0_vaddr[16 >> 2] & (0xffffffff << 0))
#define axis_switch_0_disable_m(m) (axis_switch_0_vaddr[(0x40 + 4 * m) >> 2] = 0x80000000)
#define axis_switch_0_route_m_to_s(m, s) (axis_switch_0_vaddr[(0x40 + 4 * m) >> 2] = s)
#define axis_switch_0_update(m) (axis_switch_0_vaddr[0x00 >> 2] |= 0x02)

////////////////////////////////////////
// Physical to virtual memory mapping //
////////////////////////////////////////

#define init()\
    int fd = open("/dev/mem", O_RDWR | O_SYNC);\
    volatile unsigned int *axi_dma_0_vaddr = (volatile unsigned int *) mmap(NULL, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40400000);\
    volatile unsigned int *axi_dma_1_vaddr = (volatile unsigned int *) mmap(NULL, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40410000);\
    volatile unsigned int *axis_switch_0_vaddr = (volatile unsigned int *) mmap(NULL, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x43C00000);\
    volatile unsigned int *filter_avg_f4_0_vaddr = (volatile unsigned int *) mmap(NULL, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40000000);\
    volatile unsigned int *sad_reduce_all_f4_0_vaddr = (volatile unsigned int *) mmap(NULL, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40010000);\
    close(fd);

#define exit()\
    munmap((unsigned int *) axi_dma_0_vaddr, 65535);\
    munmap((unsigned int *) axi_dma_1_vaddr, 65535);\
    munmap((unsigned int *) axis_switch_0_vaddr, 65535);\
    munmap((unsigned int *) filter_avg_f4_0_vaddr, 65535);\
    munmap((unsigned int *) sad_reduce_all_f4_0_vaddr, 65535);\

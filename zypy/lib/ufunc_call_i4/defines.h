#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/mman.h>

///////////////////
// IP addresses. //
///////////////////

#define axi_dma_0_addr 0x40400000
#define axi_dma_1_addr 0x40410000
#define ufunc_call_i4_0_addr 0x40000000

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
#define ufunc_call_i4_0_ctrl_ap_start_rw(x) x ? (ufunc_call_i4_0_vaddr[0 >> 2] = ((x & 0x1) << 0)) : (ufunc_call_i4_0_vaddr[0 >> 2] & (0x1 << 0))
#define ufunc_call_i4_0_ctrl_ap_done_r() (ufunc_call_i4_0_vaddr[0 >> 2] & (0x1 << 1))
#define ufunc_call_i4_0_ctrl_ap_idle_r() (ufunc_call_i4_0_vaddr[0 >> 2] & (0x1 << 2))
#define ufunc_call_i4_0_ctrl_ap_ready_r() (ufunc_call_i4_0_vaddr[0 >> 2] & (0x1 << 3))
#define ufunc_call_i4_0_ctrl_reserved_1_r() (ufunc_call_i4_0_vaddr[0 >> 2] & (0x7 << 4))
#define ufunc_call_i4_0_ctrl_auto_restart_rw(x) x ? (ufunc_call_i4_0_vaddr[0 >> 2] = ((x & 0x1) << 7)) : (ufunc_call_i4_0_vaddr[0 >> 2] & (0x1 << 7))
#define ufunc_call_i4_0_ctrl_reserved_2_r() (ufunc_call_i4_0_vaddr[0 >> 2] & (0xffffff << 8))
#define ufunc_call_i4_0_gier_enable_rw(x) x ? (ufunc_call_i4_0_vaddr[4 >> 2] = ((x & 0x1) << 0)) : (ufunc_call_i4_0_vaddr[4 >> 2] & (0x1 << 0))
#define ufunc_call_i4_0_gier_reserved_r() (ufunc_call_i4_0_vaddr[4 >> 2] & (0x7fffffff << 1))
#define ufunc_call_i4_0_ip_ier_chan0_int_en_rw(x) x ? (ufunc_call_i4_0_vaddr[8 >> 2] = ((x & 0x1) << 0)) : (ufunc_call_i4_0_vaddr[8 >> 2] & (0x1 << 0))
#define ufunc_call_i4_0_ip_ier_chan1_int_en_rw(x) x ? (ufunc_call_i4_0_vaddr[8 >> 2] = ((x & 0x1) << 1)) : (ufunc_call_i4_0_vaddr[8 >> 2] & (0x1 << 1))
#define ufunc_call_i4_0_ip_ier_reserved_r() (ufunc_call_i4_0_vaddr[8 >> 2] & (0x3fffffff << 2))
#define ufunc_call_i4_0_ip_isr_chan0_int_st_r() (ufunc_call_i4_0_vaddr[12 >> 2] & (0x1 << 0))
#define ufunc_call_i4_0_ip_isr_chan1_int_st_r() (ufunc_call_i4_0_vaddr[12 >> 2] & (0x1 << 1))
#define ufunc_call_i4_0_ip_isr_reserved_r() (ufunc_call_i4_0_vaddr[12 >> 2] & (0x3fffffff << 2))
#define ufunc_call_i4_0_args_args_w(x) (ufunc_call_i4_0_vaddr[16 >> 2] = ((x & 0xff) << 0))
#define ufunc_call_i4_0_args_reserved_r() (ufunc_call_i4_0_vaddr[16 >> 2] & (0xffffff << 8))

////////////////////////////////////////
// Physical to virtual memory mapping //
////////////////////////////////////////

#define init()\
    int fd = open("/dev/mem", O_RDWR | O_SYNC);\
    volatile unsigned int *axi_dma_0_vaddr = (volatile unsigned int *) mmap(NULL, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40400000);\
    volatile unsigned int *axi_dma_1_vaddr = (volatile unsigned int *) mmap(NULL, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40410000);\
    volatile unsigned int *ufunc_call_i4_0_vaddr = (volatile unsigned int *) mmap(NULL, 65535, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x40000000);\
    close(fd);

#define exit()\
    munmap((unsigned int *) axi_dma_0_vaddr, 65535);\
    munmap((unsigned int *) axi_dma_1_vaddr, 65535);\
    munmap((unsigned int *) ufunc_call_i4_0_vaddr, 65535);\

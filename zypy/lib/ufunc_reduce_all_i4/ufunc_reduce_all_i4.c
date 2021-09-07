#include "defines.h"

#define NONE 0
#define ADD 1
#define MULTIPLY 2
#define SQUARE 3
#define EXP 4
#define LOG 5
#define ABSDIFF 6
#define SUBTRACT 7
#define TRUE_DIVIDE 8
#define SIN 9
#define SQRT 10
#define ARCTAN 11
#define SINH 12

float to_int(unsigned int val) {
    union {
        unsigned int u4;
        int i4;
    } data = {val};

    return data.i4;
}

float add_reduce_all_i4(unsigned int x1_addr, unsigned int nbytes)
{
    init();

    ufunc_reduce_all_i4_0_args_args_w(((ADD << 0) | (NONE << 8) | (NONE << 16)));
    ufunc_reduce_all_i4_0_ctrl_ap_start_rw(0x1);

    axi_dma_0_mm2s_dmacr_reset_rw(0x1);

    axi_dma_0_mm2s_dmacr_rs_rw(0x1);
    axi_dma_0_mm2s_sa_source_address_rw(x1_addr);
    axi_dma_0_mm2s_length_length_rw(nbytes);

    while(!(ufunc_reduce_all_i4_0_ctrl_ap_done_r()));

    float res = to_int(ufunc_reduce_all_i4_0_ap_return_ap_return_r());

    exit();

    return res;
}

float multiply_reduce_all_i4(unsigned int x1_addr, unsigned int nbytes)
{
    init();

    ufunc_reduce_all_i4_0_args_args_w(((MULTIPLY << 0) | (NONE << 8) | (NONE << 16)));
    ufunc_reduce_all_i4_0_ctrl_ap_start_rw(0x1);

    axi_dma_0_mm2s_dmacr_reset_rw(0x1);

    axi_dma_0_mm2s_dmacr_rs_rw(0x1);
    axi_dma_0_mm2s_sa_source_address_rw(x1_addr);
    axi_dma_0_mm2s_length_length_rw(nbytes);

    while(!(ufunc_reduce_all_i4_0_ctrl_ap_done_r()));

    float res = to_int(ufunc_reduce_all_i4_0_ap_return_ap_return_r());

    exit();

    return res;
}

float dot_i4(unsigned int x1_addr, unsigned int x2_addr, unsigned int nbytes)
{
    init();

    ufunc_reduce_all_i4_0_args_args_w(((ADD << 0) | (MULTIPLY << 8) | (NONE << 16)));
    ufunc_reduce_all_i4_0_ctrl_ap_start_rw(0x1);

    axi_dma_0_mm2s_dmacr_reset_rw(0x1);
    axi_dma_1_mm2s_dmacr_reset_rw(0x1);

    axi_dma_0_mm2s_dmacr_rs_rw(0x1);
    axi_dma_0_mm2s_sa_source_address_rw(x1_addr);
    axi_dma_0_mm2s_length_length_rw(nbytes);

    axi_dma_1_mm2s_dmacr_rs_rw(0x1);
    axi_dma_1_mm2s_sa_source_address_rw(x2_addr);
    axi_dma_1_mm2s_length_length_rw(nbytes);

    while(!(ufunc_reduce_all_i4_0_ctrl_ap_done_r()));

    float res = to_int(ufunc_reduce_all_i4_0_ap_return_ap_return_r());

    exit();

    return res;
}

float sum_of_squares_i4(unsigned int x1_addr, unsigned int nbytes)
{
    init();

    ufunc_reduce_all_i4_0_args_args_w(((ADD << 0) | (SQUARE << 8) | (NONE << 16)));
    ufunc_reduce_all_i4_0_ctrl_ap_start_rw(0x1);

    axi_dma_0_mm2s_dmacr_reset_rw(0x1);

    axi_dma_0_mm2s_dmacr_rs_rw(0x1);
    axi_dma_0_mm2s_sa_source_address_rw(x1_addr);
    axi_dma_0_mm2s_length_length_rw(nbytes);

    while(!(ufunc_reduce_all_i4_0_ctrl_ap_done_r()));

    float res = to_int(ufunc_reduce_all_i4_0_ap_return_ap_return_r());

    exit();

    return res;
}

float sad_i4(unsigned int x1_addr, unsigned int x2_addr, unsigned int nbytes)
{
    init();

    ufunc_reduce_all_i4_0_args_args_w(((ADD << 0) | (ABSDIFF << 8) | (NONE << 16)));
    ufunc_reduce_all_i4_0_ctrl_ap_start_rw(0x1);

    axi_dma_0_mm2s_dmacr_reset_rw(0x1);
    axi_dma_1_mm2s_dmacr_reset_rw(0x1);

    axi_dma_0_mm2s_dmacr_rs_rw(0x1);
    axi_dma_0_mm2s_sa_source_address_rw(x1_addr);
    axi_dma_0_mm2s_length_length_rw(nbytes);

    axi_dma_1_mm2s_dmacr_rs_rw(0x1);
    axi_dma_1_mm2s_sa_source_address_rw(x2_addr);
    axi_dma_1_mm2s_length_length_rw(nbytes);

    while(!(ufunc_reduce_all_i4_0_ctrl_ap_done_r()));

    float res = to_int(ufunc_reduce_all_i4_0_ap_return_ap_return_r());

    exit();

    return res;
}

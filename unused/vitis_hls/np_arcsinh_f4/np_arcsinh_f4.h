/**
 *  @file    np_arcsinh_f4.h
 *  @author  Gerbrand De Laender
 *  @date    15/04/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Inverse hyperbolic sine element-wise.
 *  Single precision (f4) implementation.
 *
 */

#pragma once

#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define TYPE_FLOAT
typedef float np_t;
typedef unsigned int len_t;
typedef ap_axiu<8 * sizeof(np_t), 1, 1, 1> channel_t;
typedef hls::stream<channel_t> stream_t;

void np_arcsinh_f4_hw(stream_t &X1, stream_t &OUT, len_t stream_len);
void np_arcsinh_f4_sw(np_t *x1, np_t *out, len_t len);

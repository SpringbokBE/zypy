/**
 *  @file    np_add_subtract_multiply_divide_f4.h
 *  @author  Gerbrand De Laender
 *  @date    20/04/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
 *  Element-wise addition, subtraction, multiplication and division.
 *  Single precision (f4) implementation.
 *
 */

#pragma once

#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define TYPE_FLOAT
typedef float np_t;
typedef unsigned int len_t;
typedef unsigned int sel_t;
typedef ap_axiu<8 * sizeof(np_t), 1, 1, 1> channel_t;
typedef hls::stream<channel_t> stream_t;

void np_add_subtract_multiply_divide_f4_hw(stream_t &X1, stream_t &X2, stream_t &OUT, len_t stream_len, sel_t sel);
void np_add_subtract_multiply_divide_f4_sw(np_t *x1, np_t *x2, np_t *out, len_t len, sel_t sel);

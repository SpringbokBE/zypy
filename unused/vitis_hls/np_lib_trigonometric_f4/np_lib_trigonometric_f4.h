/**
 *  @file    np_lib_trigonometric_f4.h
 *  @author  Gerbrand De Laender
 *  @date    19/04/2021
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis
 *
 *  @section DESCRIPTION
 *
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

void sin_f4_hw(stream_t &x1s, stream_t &outs);
void sin_f4_sw(np_t *x1a, np_t *outa, len_t len);

void cos_f4_hw(stream_t &x1s, stream_t &outs);
void cos_f4_sw(np_t *x1a, np_t *outa, len_t len);

void np_lib_trigonometric_f4_hw(stream_t &x1s, stream_t &x2s, stream_t &outs, sel_t sel);
void np_lib_trigonometric_f4_sw(np_t *x1a, np_t *x2a, np_t *outa, len_t len, sel_t sel);

/**
 *  @file    gravity.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  IP that is able to calculate the gravitational force between two
 *  objects, given their masses and distance. Illustrates the usage of
 *  the AXI Lite interface in HLS.
 *
 */

float gravity(float m1, float m2, float d) {
	#pragma HLS INTERFACE s_axilite port=m1 bundle=CRTLS
	#pragma HLS INTERFACE s_axilite port=m2 bundle=CRTLS
	#pragma HLS INTERFACE s_axilite port=d bundle=CRTLS
	#pragma HLS INTERFACE s_axilite port=return bundle=CRTLS

	float d_squared = d ? d * d : 0.000001f;
	return 10.0f * (m1 * m2) / d_squared;;
}

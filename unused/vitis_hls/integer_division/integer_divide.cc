/**
 *  @file    integer_divide.cc
 *  @author  Gerbrand De Laender
 *  @date    17/12/2020
 *  @version 1.0
 *
 *  @brief   E091103, Master thesis, HLS testing
 *
 *  @section DESCRIPTION
 *
 *  IP that allows to do an integer division with remainder on 8-bit numbers.
 *
 */

void integer_divide(unsigned char N, unsigned char D, unsigned char *Q,
		unsigned char *R) {
	if (D != 0) {
		*Q = *R = 0;

		for (int i = 7; i >= 0; i--) {
			*R <<= 1;
			*R |= (N >> i) & 1;

			if (*R >= D) {
				*R -= D;
				*Q |= (1 << i);
			}
		}
	}
}

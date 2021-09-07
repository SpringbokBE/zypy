/**
 * Header file for ufuncs_f4.cc.
 */

#pragma once

////////////////////////////////////////////////////////////////////////////////

// Definition of all supported operations.

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

// Is the supported operation binary?

const static bool is_binary[] = {
	false, 	// NONE
	true, 	// ADD
	true, 	// MULTIPLY
	false, 	// SQUARE
	false, 	// EXP
	false, 	// LOG
	true, 	// ABSDIF
	true,	// SUBTRACT
	true,	// TRUE_DIVIDE
	false,	// SIN
	false,	// SQRT
	false,	// ARCTAN
	false	// SINH
};

// Does the supported operation return two values?

const static bool returns_pair[] = {
	false, 	// NONE
	false, 	// ADD
	false, 	// MULTIPLY
	false, 	// SQUARE
	false, 	// EXP
	false, 	// LOG
	false, 	// ABSDIF
	false,	// SUBTRACT
	false,	// TRUE_DIVIDE
	false,	// SIN
	false,	// SQRT
	false,	// ARCTAN
	false,	// SINH
	true	// Prevent optimizing away output ports
};

////////////////////////////////////////////////////////////////////////////////

// Custom type to hold two integers, used as input and output of the ufuncs.

typedef struct {
	int first;
	int second;
} int2;

////////////////////////////////////////////////////////////////////////////////

int2 add(int2 in);
int2 multiply(int2 in);
int2 square(int2 in);
int2 exp(int2 in);
int2 log(int2 in);
int2 absdiff(int2 in);
int2 subtract(int2 in);
int2 true_divide(int2 in);
int2 sin(int2 in);
int2 sqrt(int2 in);
int2 arctan(int2 in);
int2 sinh(int2 in);

////////////////////////////////////////////////////////////////////////////////

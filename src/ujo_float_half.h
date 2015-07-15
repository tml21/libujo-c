#ifndef _HFLOAT_H_
#define _HFLOAT_H_

#include <stdint.h>
#include <stddef.h>

typedef uint16_t float16_t;

float16_t float_to_half(float f);

float half_to_float(float16_t h);

//void generatetables_float_to_half();

//void generatetables_half_to_float();

void printBits(const size_t  size, void const * const ptr);

unsigned int convertmantissa(unsigned int i);

#endif

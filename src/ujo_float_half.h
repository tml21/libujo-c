#ifndef _UJO_FLOAT__HALF_H_
#define _UJO_FLOAT__HALF_H_

#include <stdint.h>
#include <stddef.h>

#include "ujo_types.h"

BEGIN_C_DECLS

float16_t float_to_half(float f);

float half_to_float(float16_t h);

//unsigned int convertmantissa(unsigned int i);

//void printBits(const size_t  size, void const * const ptr);

//void generatetables_float_to_half();

//void generatetables_half_to_float();

END_C_DECLS

#endif

/* 
 *  LibUjo:  An UJO binaray data object notation library.
 *  Copyright (c) 2015 by wobe-systems GmbH
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *  
 *  You may find a copy of the license under this software is released
 *  at COPYING file. This is LGPL software: you are welcome to develop
 *  proprietary applications using this library without any royalty or
 *  fee but returning back any change, improvement or addition in the
 *  form of source code, project image, documentation patches, etc.
 *
 *  Homepage:
 *    http://www.libujo.org
 *
 *  For professional support contact us:
 *
 *    wobe-systems GmbH
 *    support@libujo.org
 *
 *  Contributers:
 *		Created on: 02.07.2015
 *			Author: akristmann
 */

#ifndef _UJO_FLOAT_H_
#define _UJO_FLOAT_H_

#define FLOAT16_EXPONENT_MASK 0x7C00
#define FLOAT16_MANTISSA_MASK 0x3FF

#include <stddef.h>
#include "ujo_types.h"
#include "ujo_int.h"

typedef uint16_t float16_t;
typedef float float32_t;
typedef double float64_t;

BEGIN_C_DECLS

float16_t float_to_half(float32_t f);

float32_t half_to_float(float16_t h);

int isinf_float16(uint16_t x);

int isnan_float16(uint16_t x);

END_C_DECLS

#endif

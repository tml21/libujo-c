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
 */

#ifndef __UJO_TYPES_H__
#define __UJO_TYPES_H__

#include "ujo_decl.h"
#include "ujo_int_types.h"

/**
 * @brief error type
 */
typedef uint32_t ujoError; 

typedef uint8_t ujoByte;


/**
 * @brief Bool definition for the UJO library. This type built on top
 * of <b>int</b> is used along with \ref ujoFalse and \ref ujoTrue
 * to model those API functions and attributes that returns or receive
 * a boolean state. 
 */
typedef uint8_t ujoBool;

/** 
 * @brief Common definition to have false (\ref ujoFalse) value (which is defined to 0 integer value).
 */
#define ujoFalse ((ujoBool)0)
/** 
 * @brief Common definition to have true (\ref ujoTrue) value (which is defined to 1 integer value).
 */
#define ujoTrue  ((ujoBool)1)

/**
 * @brief access type.
 * @ingroup ujo_reader
 */
typedef enum { 
	/** file access */
	UJO_FILE    = 0x100, 
	/** memory access */
	UJO_MEMORY  = 0x101,
	/** stream access */
	UJO_STREAM  = 0x102, 
} ujoAccessType; 

/**
 * @brief UJO type id
 */
typedef uint8_t ujoTypeId;

typedef float float32_t;

typedef double float64_t;

/**
 * @brief UJO datetime
 */
typedef struct {
	int16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t millisecond;
} ujoDateTime;



#endif

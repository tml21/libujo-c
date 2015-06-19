/* 
 *  LibUjo:  An UJO binaray data object notation library.
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

#ifndef __UJO_CONSTANTS_H__
#define __UJO_CONSTANTS_H__

#include "ujo_int_types.h"

/**
 * @brief API version number
 */
#define UJO_API_VERSION 101 // 1.1

/**
 * @brief library version
 */
#define UJO_VERSION 901 // 0.9.1

#define UJO_DEFAULT_BUFSIZE 4096


// ujo magic
static const char       *UJO_MAGIC = "\x5F\x55\x4A\x4F"; 

// data format version
#define  UJO_DATA_VERSION   0x0001

// compression types
#define UJO_COMPRESS_NONE   ((uint8_t)0x00)

/** 
 * \addtogroup ujo_element_types
 * @{
 */
#define  UJO_TERMINATOR     ((uint8_t)0x00)


// ujo atomic types
#define UJO_TYPE_FLOAT64         ((uint8_t)0x01)
#define UJO_TYPE_FLOAT32         ((uint8_t)0x02)
#define UJO_TYPE_FLOAT16         ((uint8_t)0x03)
#define UJO_TYPE_STRING          ((uint8_t)0x04)

#define UJO_TYPE_INT64           ((uint8_t)0x05)
#define UJO_TYPE_INT32           ((uint8_t)0x06)
#define UJO_TYPE_INT16           ((uint8_t)0x07)
#define UJO_TYPE_INT8            ((uint8_t)0x08)

#define UJO_TYPE_UINT64     ((uint8_t)0x09)
#define UJO_TYPE_UINT32     ((uint8_t)0x0A)
#define UJO_TYPE_UINT16     ((uint8_t)0x0B)
#define UJO_TYPE_UINT8      ((uint8_t)0x0C)

#define UJO_TYPE_BOOL       ((uint8_t)0x0D)
#define UJO_TYPE_BIN        ((uint8_t)0x0E)
#define UJO_TYPE_NONE       ((uint8_t)0x0F)

#define UJO_TYPE_UX_TIME    ((uint8_t)0x10)
#define UJO_TYPE_DATE       ((uint8_t)0x11)
#define UJO_TYPE_TIME       ((uint8_t)0x12)
#define UJO_TYPE_TIMESTAMP  ((uint8_t)0x13)

//ujo container types
#define UJO_TYPE_LIST       ((uint8_t)0x30)
#define UJO_TYPE_MAP        ((uint8_t)0x31)
#define UJO_TYPE_TABLE      ((uint8_t)0x32)

// ujo string subtypes
#define UJO_SUB_STRING_C        ((uint8_t)0x00)
#define UJO_SUB_STRING_U8       ((uint8_t)0x01)
#define UJO_SUB_STRING_U16      ((uint8_t)0x02)
#define UJO_SUB_STRING_U32      ((uint8_t)0x03)

// ujo binary subtypes
#define UJO_SUB_BINARY_GENERIC   ((uint8_t)0x00)
#define UJO_SUB_BINARY_UJO       ((uint8_t)0x01)


 /* @} */


#endif
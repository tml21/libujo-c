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
 *      Created on: 31.07.2015
 *          Author: akristmann
 */

#ifndef _UJO_ENDIAN_H_
#define _UJO_ENDIAN_H_


#include "stdio.h"
#include "ujo_types.h"
#include "ujo_config.h"

BEGIN_C_DECLS

#define UJO_LITTLE_ENDIAN 1234
#define UJO_BIG_ENDIAN 4321

#if UJO_BYTE_ORDER == UJO_LITTLE_ENDIAN
# define UJO_UINT16_SWAP(x) (x)
# define UJO_UINT32_SWAP(x) (x)
# define UJO_UINT64_SWAP(x) (x)
#elif UJO_BYTE_ORDER == UJO_BIG_ENDIAN
# define UJO_UINT16_SWAP(x) _ujo_uint16_swap(x)
# define UJO_UINT32_SWAP(x) _ujo_uint32_swap(x)
# define UJO_UINT64_SWAP(x) _ujo_uint64_swap(x)
#else
# error "endianness unknown"
#endif


static __inline uint16_t _ujo_uint16_swap (uint16_t value)
{
    return ((value & 0x00FF) << 8) |
           ((value & 0xFF00) >> 8);
}

static __inline uint32_t _ujo_uint32_swap (uint32_t value)
{
    return ((value & 0x000000FFU) << 24) |
           ((value & 0x0000FF00U) <<  8) |
           ((value & 0x00FF0000U) >>  8) |
           ((value & 0xFF000000U) >> 24);
}

static __inline uint64_t _ujo_uint64_swap (uint64_t value)
{
    return ((value & 0x00000000000000FFULL) << 56) |
           ((value & 0x000000000000FF00ULL) << 40) |
           ((value & 0x0000000000FF0000ULL) << 24) |
           ((value & 0x00000000FF000000ULL) <<  8) |
           ((value & 0x000000FF00000000ULL) >>  8) |
           ((value & 0x0000FF0000000000ULL) >> 24) |
           ((value & 0x00FF000000000000ULL) >> 40) |
           ((value & 0xFF00000000000000ULL) >> 56);
}


END_C_DECLS


#endif // _UJO_ENDIAN_H_














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

#ifndef __UJO_READER_H__
#define __UJO_READER_H__

#include "ujo_decl.h"
#include "ujo_types.h"


/**
 * @brief UJO reader object.
 */
typedef struct _ujo_reader ujo_reader;

/**
 * @brief UJO data element.
 */
typedef struct _ujo_element ujo_element;

/**
 * @brief On element found.
 * @ingroup ujo_reader_callbacks
 *
 * A callback function to signal an ujo element while parsing a buffer or file.
 *
 * @param element UJO data element.
 * @param data    A pointer to custom data. 
 *
 * @return UJO error code or UJO_SUCCESS
 */
typedef ujoError (*ujoOnElementFunc)(ujo_element* element, ujoPointer data);


BEGIN_C_DECLS

/** 
 * \addtogroup ujo_reader
 * @{
 */

	ujoError ujo_new_reader(ujoAccessType type, ujo_reader** r);
	ujoError ujo_free_reader(ujo_reader* r);

	ujoError ujo_reader_get_type(ujo_reader* r, ujoAccessType* type);

	ujoError ujo_reader_set_on_element(ujo_reader* r, ujoOnElementFunc f, ujoPointer data);

	ujoError ujo_reader_set_buffer(ujo_reader* r, ujoByte* buffer, size_t bytes);

	ujoError ujo_reader_parse(ujo_reader* r);

	ujoError ujo_reader_get_first(ujo_reader *r, ujo_element** v, ujoBool *eod);
	ujoError ujo_reader_get_next(ujo_reader *r, ujo_element** v, ujoBool *eod);

/* @} */

/** 
 * \addtogroup ujo_element
 * @{
 */
	ujoError ujo_free_element(ujo_element* e);

	ujoError ujo_element_get_int8(ujo_element* e, int8_t* value);
	ujoError ujo_element_get_int16(ujo_element* e, int16_t* value);
	ujoError ujo_element_get_int32(ujo_element* e, int32_t* value);
	ujoError ujo_element_get_int64(ujo_element* e, int64_t* value);

	ujoError ujo_element_get_uint8(ujo_element* e, uint8_t* value);
	ujoError ujo_element_get_uint16(ujo_element* e, uint16_t* value);
	ujoError ujo_element_get_uint32(ujo_element* e, uint32_t* value);
	ujoError ujo_element_get_uint64(ujo_element* e, uint64_t* value);

	ujoError ujo_element_get_float16(ujo_element* e, float32_t* value);
	ujoError ujo_element_get_float32(ujo_element* e, float32_t* value);
	ujoError ujo_element_get_float64(ujo_element* e, float64_t* value);

	ujoError ujo_element_get_bool(ujo_element* e, ujoBool* value);

	ujoError ujo_element_get_type(ujo_element* e, ujoTypeId* value);

	ujoError ujo_element_get_uxtime(ujo_element* e, int64_t* value);
	ujoError ujo_element_get_date(ujo_element* e, ujoDateTime* value);
	ujoError ujo_element_get_time(ujo_element* e, ujoDateTime* value);
	ujoError ujo_element_get_timestamp(ujo_element* e, ujoDateTime* value);

	ujoError ujo_element_get_string_type(ujo_element* e, ujoTypeId* value);
	ujoError ujo_element_get_string_c(ujo_element* e, char** s, uint32_t* n);
	ujoError ujo_element_get_string_u8(ujo_element* e, uint8_t** s, uint32_t* n);
	ujoError ujo_element_get_string_u16(ujo_element* e, uint16_t** s, uint32_t* n);
	ujoError ujo_element_get_string_u32(ujo_element* e, uint32_t** s, uint32_t* n);

	ujoError ujo_element_get_binary(ujo_element* e, uint8_t* t, uint8_t** d, uint32_t* n);

/* @} */

/** 
@cond INTERNAL_DOCS
*/
	void _ujo_reader_buffer_read(ujo_reader* r, void* sequence, size_t bytes);

/**
@endcond
*/


END_C_DECLS

#endif
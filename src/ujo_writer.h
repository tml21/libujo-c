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

#ifndef __UJO_WRITER_H__
#define __UJO_WRITER_H__

#include "ujo.h"
#include "ujo_types.h"


typedef struct _ujo_writer ujo_writer;

BEGIN_C_DECLS

/** 
 * \addtogroup ujo_writer
 * @{
 */

	ujoError ujo_new_memory_writer(ujo_writer** w);
	ujoError ujo_new_file_writer(ujo_writer** w, wchar_t* filename);

	ujoError ujo_free_writer(ujo_writer* w);

	ujoError ujo_writer_get_type(ujo_writer* w, ujoAccessType* type);

	ujoError ujo_writer_get_buffer(ujo_writer *w, ujoByte** buffer, size_t *bytes);

	// list methods
	ujoError ujo_writer_list_open(ujo_writer* w);
	ujoError ujo_writer_list_close(ujo_writer* w);

	// map methods
	ujoError ujo_writer_map_open(ujo_writer* w);
	ujoError ujo_writer_map_close(ujo_writer* w);

	// table methods
	ujoError ujo_writer_table_open(ujo_writer* w);
	ujoError ujo_writer_table_end_columns(ujo_writer* w);
	ujoError ujo_writer_table_close(ujo_writer* w);

	// atomic type write methods
	// int types
	ujoError ujo_writer_add_int64(ujo_writer* w, int64_t value);
	ujoError ujo_writer_add_int32(ujo_writer* w, int32_t value);
	ujoError ujo_writer_add_int16(ujo_writer* w, int16_t value);
	ujoError ujo_writer_add_int8(ujo_writer* w, int8_t value);

	ujoError ujo_writer_add_uint64(ujo_writer* w, uint64_t value);
	ujoError ujo_writer_add_uint32(ujo_writer* w, uint32_t value);
	ujoError ujo_writer_add_uint16(ujo_writer* w, uint16_t value);
	ujoError ujo_writer_add_uint8(ujo_writer* w, uint8_t value);

	// float types
	ujoError ujo_writer_add_float16(ujo_writer* w, float32_t value);
	ujoError ujo_writer_add_float32(ujo_writer* w, float32_t value);
	ujoError ujo_writer_add_float64(ujo_writer* w, float64_t value);

	// none,null types
	ujoError ujo_writer_add_none(ujo_writer* w);
	ujoError ujo_writer_add_null(ujo_writer* w, ujoTypeId type);

	// date and time
	ujoError ujo_writer_add_uxtime(ujo_writer* w, int64_t t);
	ujoError ujo_writer_add_date(ujo_writer* w,  const ujoDateTime dt);
	ujoError ujo_writer_add_time(ujo_writer* w,  const ujoDateTime dt);
	ujoError ujo_writer_add_timestamp(ujo_writer* w,  const ujoDateTime dt);


	// boolean
	ujoError ujo_writer_add_bool(ujo_writer* w, ujoBool value);

	// string
	ujoError ujo_writer_add_string_c(ujo_writer* w, const char* s);
	ujoError ujo_writer_add_string_u8(ujo_writer* w, const uint8_t* s, size_t n);
	ujoError ujo_writer_add_string_u16(ujo_writer* w, const uint16_t* s, size_t n);
	ujoError ujo_writer_add_string_u32(ujo_writer* w, const uint32_t* s, size_t n);

	// binary
	ujoError ujo_writer_add_binary(ujo_writer* w, uint8_t t, const uint8_t* d, uint32_t n);


	/* internal methods: don't use them in applications. */

	/** 
	@cond INTERNAL_DOCS
	*/
	void _ujo_writer_buffer_put(ujo_writer* w, const void* sequence, size_t bytes);
	void _ujo_writer_buffer_set_uint8(ujo_writer* w, uint8_t value); 
	void _ujo_writer_buffer_set_uint16(ujo_writer* w, uint16_t value); 

	/**
	@endcond
	*/

/* @} */

END_C_DECLS

#endif

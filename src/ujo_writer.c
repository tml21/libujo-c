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

/* global includes */
#include "ujo_writer.h"
#include "ujo_log.h"
#include "ujo_stack.h"
#include "ujo_constants.h"
#include "ujo_macros.h"
#include "ujo_state.h"
#include "ujo_float.h"
#include "ujo_endian.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>


/** 
@cond INTERNAL_DOCS
*/

struct _ujo_writer {
	ujoAccessType	type;
	ujoStack*		state_stack;
	ujo_state*		state;
    
	// memory writer
	size_t			buffersize;
	ujoByte*		buffer;
	size_t			bytes;

	// file writer
	FILE*           file;
};

static __inline ujoError _ujo_new_writer(ujo_writer** w)
{
	ujo_writer*     newhdl;

	newhdl = (ujo_writer*)ujo_new(ujo_writer, 1); 
	report_error(newhdl, "allocation failed", UJO_ERR_ALLOCATION);
	
	newhdl->state = (ujo_state*)ujo_new(ujo_state, 1);
	newhdl->state->state = STATE_ROOT;

	/* initialize stack */
	newhdl->state_stack = ujo_new_stack(ujo_free);

	*w = newhdl;

	return UJO_SUCCESS;
};

/** 
@endcond
*/

/** 
 * \addtogroup ujo_writer
 * @{
 */


/**
 * @brief Create a new memory writer.
 *
 * The writer object provides functions to create UJO data
 * in a memory buffer.
 * 
 * @param w    reference to a writer
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_free_writer
 */
ujoError ujo_new_memory_writer(ujo_writer** w) 
{
	ujo_writer*     newhdl;
	ujoError        err;

	return_on_err(_ujo_new_writer(&newhdl));

	newhdl->type = UJO_MEMORY; 

	newhdl->bytes = 0;
	newhdl->buffer = ujo_new(ujoByte, UJO_DEFAULT_BUFSIZE);
	newhdl->buffersize = UJO_DEFAULT_BUFSIZE;

	return_on_err(_ujo_writer_put(newhdl, UJO_MAGIC, strlen(UJO_MAGIC)));
	return_on_err(_ujo_writer_put_uint16(newhdl, UJO_DATA_VERSION));
	return_on_err(_ujo_writer_put_uint8(newhdl, UJO_COMPRESS_NONE));

	*w = newhdl;

	return UJO_SUCCESS;
}

/**
 * @brief Create a new file writer.
 *
 * The writer object provides functions to create an UJO data
 * file.
 * 
 * @param w         reference to a writer
 * @param filename  path of the file
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_free_writer
 */
ujoError ujo_new_file_writer(ujo_writer** w, const char* filename)
{
	ujo_writer*     newhdl;
	ujoError        err;
	FILE*           filehandle;

	filehandle = fopen(filename, "wb"); 
    report_error(filehandle != NULL, "cannot open file", UJO_ERR_FILE);	

	return_on_err(_ujo_new_writer(&newhdl));

	newhdl->type = UJO_FILE; 
	newhdl-> file = filehandle;

	return_on_err(_ujo_writer_put(newhdl, UJO_MAGIC, strlen(UJO_MAGIC)));
	return_on_err(_ujo_writer_put_uint16(newhdl, UJO_DATA_VERSION));
	return_on_err(_ujo_writer_put_uint8(newhdl, UJO_COMPRESS_NONE));

	*w = newhdl;

	return UJO_SUCCESS;
};

/**
 * @brief Dispose a writer object.
 *
 * If a writer object is not needed anymore is has to be
 * disposed to release allocated memory.
 * 
 * @param w    ujo writer handle
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_new_writer
 */
ujoError ujo_free_writer(ujo_writer* w) 
{
	report_error(w, "invalid writer handle", UJO_ERR_INVALID_DATA);	
	
	ujo_free_stack(w->state_stack);
	ujo_free(w->state);
	
	switch(w->type) {
	case UJO_MEMORY:
		ujo_free(w->buffer);
		break;
	case UJO_FILE:
		fclose(w->file);
		break;
	}

	ujo_free(w);

	return UJO_SUCCESS;
}

/**
 * @brief Get the type of a writer object.
 *
 * The type property defines how the data is handled.
 *
 * @param w    ujo writer handle
 * @param type reference to the writer type
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_new_writer
 */
ujoError ujo_writer_get_type(ujo_writer* w, ujoAccessType* type)
{
	report_error(w, "invalid writer handle", UJO_ERR_INVALID_DATA);	
	*type = w->type;
	return UJO_SUCCESS;
}


/**
 * @brief Access the writer memory buffer.
 *
 * The writer memory buffer can be accessed directly as long as the
 * writer intance exists. The buffer is 
 * deallocated if the memory object id disposed. 
 *
 * @param w    ujo writer handle
 * @param buffer reference to a pointer indicating the start of the buffer.
 * @param bytes reference to the size of the buffer.
 *
 * @return UJO error code or UJO_SUCCESS
 */
ujoError ujo_writer_get_buffer(ujo_writer* w, ujoByte** buffer, size_t *bytes)
{
	report_error(w, "invalid writer handle", UJO_ERR_INVALID_DATA);		
	
	*buffer = w->buffer;
	*bytes  = w->bytes;

	return UJO_SUCCESS;
};


/**
 * @brief Open a list.
 *
 * A list is a container with a sequence of ordered items. Each item can
 * be an atomic type like float, int, string, etc. or a container.
 * After openeing a list the values can be added by subsequent calls to 
 * the write functions of appropriate data.
 *
 * @param w    ujo writer handle
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_list_close
 */
ujoError ujo_writer_list_open(ujo_writer* w)
{
	ujoError err;

	report_error(ujo_state_allow_container(w->state->state),"list not allowed", UJO_ERR_TYPE_MISPLACED);

	w->state = ujo_state_next(STATE_LIST, w->state, w->state_stack);
	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_LIST));

	return UJO_SUCCESS;
};

/**
 * @brief Close a list.
 *
 * Close a list of an ordered sequence of values. If the list was the top level container,
 * closing it will close the document as well. No additional values can be added.
 *
 * @param w    ujo writer handle
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_list_open
 */
ujoError ujo_writer_list_close(ujo_writer* w)
{
	ujoError err;

	report_error(w->state->state==STATE_LIST,"close list not allowed", UJO_ERR_INVALID_OBJECT);
	
	w->state = ujo_state_prev(w->state, w->state_stack);
	return_on_err(_ujo_writer_put_uint8(w, UJO_TERMINATOR));

	w->state = ujo_state_switch(CONTAINER_CLOSED, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Open a map.
 *
 * A map is a sequence of key value pairs, where key and value can be an 
 * atomic type like float, int, string, etc. The value can be a container 
 * as well. 
 * After openeing a map the keys and values can be added by subsequent calls to 
 * the write functions of appropriate data. 
 *
 * \code{.c}
 *   ujo_writer_map_open(ujow);
 *		ujo_writer_add_int64(ujow, 42); // key
 *		ujo_writer_add_int16(ujow, 102); // value
 *		ujo_writer_add_int8(ujow, 21); // key
 *		ujo_writer_add_float32(ujow, 3.14); // value
 *   ujo_writer_map_close(ujow);
 * \endcode
 *
 * @param w    ujo writer handle
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_map_close
 */
ujoError ujo_writer_map_open(ujo_writer* w)
{
	ujoError err;

	report_error(ujo_state_allow_container(w->state->state),"map not allowed", UJO_ERR_TYPE_MISPLACED);

	w->state = ujo_state_next(STATE_DICT_KEY, w->state, w->state_stack);
	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_MAP));

	return UJO_SUCCESS;
};

/**
 * @brief Close a map.
 *
 * Close a map. If the map was the top level container,
 * closing it will close the document as well. No additional values can be added.
 *
 * @param w    ujo writer handle
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_map_open
 */
ujoError ujo_writer_map_close(ujo_writer* w)
{
	ujoError err;

	report_error(w->state->state==STATE_DICT_KEY,"close map not allowed", UJO_ERR_INVALID_OBJECT);
	
	w->state = ujo_state_prev(w->state, w->state_stack);
	return_on_err(_ujo_writer_put_uint8(w, UJO_TERMINATOR));

    w->state = ujo_state_switch(CONTAINER_CLOSED, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a 64bit signed integer.
 *
 * A 64bit signed integer is an atomic UJO type.
 *
 * @param w    ujo writer handle
 * @param value 64bit signed integer value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_int32, ujo_writer_add_int16, ujo_writer_add_int8,
 * ujo_writer_add_uint64, ujo_writer_add_uint32, ujo_writer_add_uint16, ujo_writer_add_uint8
 */
ujoError ujo_writer_add_int64(ujo_writer* w, int64_t value)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_INT64));
	value = (int64_t) UJO_UINT64_SWAP(value);
	return_on_err(_ujo_writer_put(w, &value, sizeof(int64_t)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a 32bit signed integer.
 *
 * A 32bit signed integer is an atomic UJO type.
 *
 * @param w    ujo writer handle
 * @param value 32bit signed integer value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_int64, ujo_writer_add_int16, ujo_writer_add_int8,
 * ujo_writer_add_uint64, ujo_writer_add_uint32, ujo_writer_add_uint16, ujo_writer_add_uint8
 */
ujoError ujo_writer_add_int32(ujo_writer* w, int32_t value)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_INT32));
	value = (int32_t) UJO_UINT32_SWAP(value);
	return_on_err(_ujo_writer_put(w, &value, sizeof(int32_t)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a 16bit signed integer.
 *
 * A 16bit signed integer is an atomic UJO type.
 *
 * @param w    ujo writer handle
 * @param value 16bit signed integer value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_int64, ujo_writer_add_int32, ujo_writer_add_int8,
 * ujo_writer_add_uint64, ujo_writer_add_uint32, ujo_writer_add_uint16, ujo_writer_add_uint8
 */
ujoError ujo_writer_add_int16(ujo_writer* w, int16_t value)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_INT16));
	value = (int16_t) UJO_UINT16_SWAP(value);
	return_on_err(_ujo_writer_put(w, &value, sizeof(int16_t)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a 8bit signed integer.
 *
 * A 8bit signed integer is an atomic UJO type.
 *
 * @param w    ujo writer handle
 * @param value 8bit signed integer value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_int64, ujo_writer_add_int32, ujo_writer_add_int16,
 * ujo_writer_add_uint64, ujo_writer_add_uint32, ujo_writer_add_uint16, ujo_writer_add_uint8
 */
ujoError ujo_writer_add_int8(ujo_writer* w, int8_t value)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_INT8));
	return_on_err(_ujo_writer_put(w, &value, sizeof(int8_t)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write None value.
 *
 * A None value means no data and no type. It is used to indicate empty values.
 * A Null value is an atomic UJO type.
 *
 * @param w    ujo writer handle
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_null
 */
ujoError ujo_writer_add_none(ujo_writer* w)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);
	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_NONE));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write Null value.
 *
 * A Null value means no data but known type. The type indicates how the
 * empty value is handled by the application. For example a string null can be
 * interpreted as "" and an integer null as 0. If an application generates 
 * input forms automatically, the type can be used to select a proper widget.
 * A Null value is an atomic UJO type.
 *
 * @param w    ujo writer handle
 * @param type type of the null value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_null
 */
ujoError ujo_writer_add_null(ujo_writer* w, ujoTypeId type)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, type & 0x80));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a 16bit float value.
 *
 * A 16bit float value, also known as half precision,
 * is an atomic UJO type. C does not support half precision. The 
 * function converts the value internally. The main reason to use
 * half precision is to store float values in only 2 octets. If for example
 * in 3D graphics datasets man float values are required.
 *
 * @param w    ujo writer handle
 * @param value 32bit float value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_float64, ujo_writer_add_float32
 */
ujoError ujo_writer_add_float16(ujo_writer* w, float32_t value)
{
	ujoError err;
	float16_t hValue = float_to_half(value);

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	if (isnan_float16(hValue) != 0)
	{
		report_error(0,"value is NaN", UJO_ERR_INVALID_DATA);
	}
	else if (isinf_float16(hValue) != 0)
	{
		report_error(0,"value is out of range", UJO_ERR_INVALID_DATA);
	}

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_FLOAT16));
	hValue = (float16_t) UJO_UINT16_SWAP(hValue);
	return_on_err(_ujo_writer_put(w, &hValue, sizeof(float16_t)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a 32bit float value.
 *
 * A 32bit float value, also known as single precision,
 * is an atomic UJO type.
 *
 * @param w    ujo writer handle
 * @param value 32bit float value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_float64, ujo_writer_add_float16
 */
ujoError ujo_writer_add_float32(ujo_writer* w, float32_t value)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_FLOAT32));
	value = (float32_t) UJO_FLOAT32_SWAP(value);
	return_on_err(_ujo_writer_put(w, &value, sizeof(float32_t)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a 64bit float value.
 *
 * A 64bit float value, also known as double precision,
 * is an atomic UJO type.
 *
 * @param w    ujo writer handle
 * @param value 64bit float value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_float32, ujo_writer_add_float16
 */
ujoError ujo_writer_add_float64(ujo_writer* w, float64_t value)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_FLOAT64));
	value = (float64_t) UJO_FLOAT64_SWAP(value);
	return_on_err(_ujo_writer_put(w, &value, sizeof(float64_t)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a boolean value.
 *
 * A boolean value is expressed as ujoTrue or ujoFalse.
 *
 * @param w    ujo writer handle
 * @param value boolean value (ujoTrue, ujoFalse)
 *
 * @return UJO error code or UJO_SUCCESS
 */
ujoError ujo_writer_add_bool(ujo_writer* w, ujoBool value)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_BOOL));
	return_on_err(_ujo_writer_put(w, &value, sizeof(ujoBool)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a 64bit unsigned integer.
 *
 * A 64bit unsigned integer is an atomic UJO type.
 *
 * @param w    ujo writer handle
 * @param value 64bit unsigned integer value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_int64, ujo_writer_add_int32, ujo_writer_add_int16, ujo_writer_add_int8,
 * ujo_writer_add_uint32, ujo_writer_add_uint16, ujo_writer_add_uint8
 */
ujoError ujo_writer_add_uint64(ujo_writer* w, uint64_t value)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_UINT64));
	value = (uint64_t) UJO_UINT64_SWAP(value);
	return_on_err(_ujo_writer_put(w, &value, sizeof(uint64_t)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a 32bit unsigned integer.
 *
 * A 32bit unsigned integer is an atomic UJO type.
 *
 * @param w    ujo writer handle
 * @param value 32bit unsigned integer value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_int64, ujo_writer_add_int32, ujo_writer_add_int16, ujo_writer_add_int8,
 * ujo_writer_add_uint64, ujo_writer_add_uint16, ujo_writer_add_uint8
 */
ujoError ujo_writer_add_uint32(ujo_writer* w, uint32_t value)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_UINT32));
	value = (uint32_t) UJO_UINT32_SWAP(value);
	return_on_err(_ujo_writer_put(w, &value, sizeof(uint32_t)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a 16bit unsigned integer.
 *
 * A 16bit unsigned integer is an atomic UJO type.
 *
 * @param w    ujo writer handle
 * @param value 16bit unsigned integer value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_int64, ujo_writer_add_int32, ujo_writer_add_int16, ujo_writer_add_int8,
 * ujo_writer_add_uint64, ujo_writer_add_uint32, ujo_writer_add_uint8
 */
ujoError ujo_writer_add_uint16(ujo_writer* w, uint16_t value)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_UINT16));
	value = (uint16_t) UJO_UINT16_SWAP(value);
	return_on_err(_ujo_writer_put(w, &value, sizeof(uint16_t)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a 8bit unsigned integer.
 *
 * A 8bit unsigned integer is an atomic UJO type.
 *
 * @param w    ujo writer handle
 * @param value 8bit unsigned integer value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_int64, ujo_writer_add_int32, ujo_writer_add_int16, ujo_writer_add_int8,
 * ujo_writer_add_uint64, ujo_writer_add_uint32, ujo_writer_add_uint16
 */
ujoError ujo_writer_add_uint8(ujo_writer* w, uint8_t value)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_UINT8));
	return_on_err(_ujo_writer_put(w, &value, sizeof(uint8_t)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a Unix date/time.
 *
 * A Unix date/time value is a 64bit signed integer value expressing the 
 * seconds since the epoch. This particular point in time is 
 * 00:00:00 Coordinated Universal Time (UTC), Thursday, 1 January 1970.
 * A Unix time is an atomic UJO value.
 *
 * @param w    ujo writer handle
 * @param t    64bit signed integer expressing the unix time
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_date, ujo_writer_add_time, ujo_writer_add_timestamp
 */
ujoError ujo_writer_add_uxtime(ujo_writer* w, int64_t t)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_UX_TIME));
	t = (int64_t) UJO_UINT64_SWAP(t);
	return_on_err(_ujo_writer_put(w, &t, sizeof(int64_t)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a date structure.
 *
 * The date is expressed by a structure of year, month and day as integer value.
 * A date is an atomic UJO value.
 *
 * @param w    ujo writer handle
 * @param dt   structure with date 
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_uxtime, ujo_writer_add_time, ujo_writer_add_timestamp
 */
ujoError ujo_writer_add_date(ujo_writer* w, const ujoDateTime dt)
{
	ujoError err;
	int16_t i16_year;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_DATE));
	i16_year = (int16_t) UJO_UINT16_SWAP(dt.year);
	return_on_err(_ujo_writer_put(w, &i16_year, sizeof(int16_t)));
	return_on_err(_ujo_writer_put(w, &dt.month, sizeof(uint8_t)));
	return_on_err(_ujo_writer_put(w, &dt.day, sizeof(uint8_t)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a time structure.
 *
 * The time is expressed by a structure of hour, minute and second as integer values.
 * The hour is 00-23.
 * A time is an atomic UJO value.
 *
 * @param w    ujo writer handle
 * @param dt   structure with time 
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_uxtime, ujo_writer_add_date, ujo_writer_add_timestamp
 */
ujoError ujo_writer_add_time(ujo_writer* w, const ujoDateTime dt)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_TIME));
	return_on_err(_ujo_writer_put(w, &dt.hour, sizeof(uint8_t)));
	return_on_err(_ujo_writer_put(w, &dt.minute, sizeof(uint8_t)));
	return_on_err(_ujo_writer_put(w, &dt.second, sizeof(uint8_t)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a timestamp structure.
 *
 * The timestamp is expressed by a structure of year, month and day, 
 * hour, minute, second and millisecond as integer values.
 * The hour is 00-23.
 * A timestamp is an atomic UJO value.
 *
 * @param w    ujo writer handle
 * @param dt   structure with timestamp 
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_uxtime, ujo_writer_add_date, ujo_writer_add_time
 */
ujoError ujo_writer_add_timestamp(ujo_writer* w, const ujoDateTime dt)
{
	ujoError err;
	int16_t i16_temp;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_TIMESTAMP));

	i16_temp = (int16_t) UJO_UINT16_SWAP(dt.year);
	return_on_err(_ujo_writer_put(w, &i16_temp, sizeof(int16_t)));
	return_on_err(_ujo_writer_put(w, &dt.month, sizeof(uint8_t)));
	return_on_err(_ujo_writer_put(w, &dt.day, sizeof(uint8_t)));

	return_on_err(_ujo_writer_put(w, &dt.hour, sizeof(uint8_t)));
	return_on_err(_ujo_writer_put(w, &dt.minute, sizeof(uint8_t)));
	return_on_err(_ujo_writer_put(w, &dt.second, sizeof(uint8_t)));

	i16_temp = (int16_t) UJO_UINT16_SWAP(dt.millisecond);
	return_on_err(_ujo_writer_put(w, &dt.millisecond, sizeof(uint16_t)));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a \\x00 terminated c string;
 *
 * A c string is terminated by \\x00. Only the last octed is allowed to be 
 * \\x00. 
 *
 * @param w    ujo writer handle
 * @param s    array with string
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_string_u8, ujo_writer_add_string_u16, ujo_writer_add_string_u32,
 * ujo_element_get_string_c
 */
ujoError ujo_writer_add_string_c(ujo_writer* w, const char* s)
{
	ujoError err;
	uint32_t units = (uint32_t)(strlen(s)+1);

	report_error(ujo_state_allow_string(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_STRING));
	return_on_err(_ujo_writer_put_uint8(w, UJO_SUB_STRING_C));

	return_on_err(_ujo_writer_put(w, &units, sizeof(uint32_t)));
	return_on_err(_ujo_writer_put(w, s, units));

	w->state = ujo_state_switch(STRING_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a unicode UTF-8 string.
 *
 * A unicode string is not terminated by \\x00 and all octets are part of
 * the string. The number of octets is not the number of characters because a single
 * character can use multiple bytes. The size n is the number of octets to write.
 *
 * @param w    ujo writer handle
 * @param s    array with string
 * @param n    number of octets
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_string_c, ujo_writer_add_string_u16, ujo_writer_add_string_u32,
 * ujo_element_get_string_u8
 */
ujoError ujo_writer_add_string_u8(ujo_writer* w, const uint8_t* s, size_t n)
{
	ujoError err;
	uint32_t units = (uint32_t)n;

	report_error(ujo_state_allow_string(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_STRING));
	return_on_err(_ujo_writer_put_uint8(w, UJO_SUB_STRING_U8));

	return_on_err(_ujo_writer_put(w, &units, sizeof(uint32_t)));
	return_on_err(_ujo_writer_put(w, s, n));

	w->state = ujo_state_switch(STRING_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a unicode UTF-16 string.
 *
 * The smalles unit of a unicode UTF-16 string is 16 bit. To add a unicode UTF-16 string
 * it is passed as an array of 16 bit unsigned integer. The size n is the number of units.
 *
 * @param w    ujo writer handle
 * @param s    array with string
 * @param n    number of units
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_string_c, ujo_writer_add_string_u8, ujo_writer_add_string_u32,
 * ujo_element_get_string_u16
 */
ujoError ujo_writer_add_string_u16(ujo_writer* w, const uint16_t* s, size_t n)
{
	ujoError err;
	uint32_t units = (uint32_t)n;

	report_error(ujo_state_allow_string(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_STRING));
	return_on_err(_ujo_writer_put_uint8(w, UJO_SUB_STRING_U16));

	return_on_err(_ujo_writer_put(w, &units, sizeof(uint32_t)));
	return_on_err(_ujo_writer_put(w, s, n*sizeof(uint16_t)));

	w->state = ujo_state_switch(STRING_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write a unicode UTF-32 string.
 *
 * The smalles unit of a unicode UTF-32 string is 32 bit. To add a unicode UTF-32 string
 * it is passed as an array of 32 bit unsigned integer. The size n is the number of units.
 *
 * @param w    ujo writer handle
 * @param s    array with string
 * @param n    number of units
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_string_c, ujo_writer_add_string_u8, ujo_writer_add_string_u16,
 * ujo_element_get_string_u32
 */
ujoError ujo_writer_add_string_u32(ujo_writer* w, const uint32_t* s, size_t n)
{
	ujoError err;
	uint32_t units = (uint32_t)n;

	report_error(ujo_state_allow_string(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_STRING));
	return_on_err(_ujo_writer_put_uint8(w, UJO_SUB_STRING_U32));

	return_on_err(_ujo_writer_put(w, &units, sizeof(uint32_t)));
	return_on_err(_ujo_writer_put(w, s, n*sizeof(uint32_t)));

	w->state = ujo_state_switch(STRING_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Write binary data.
 *
 * Binary data is an array of unsigned 8 bit integer values. The type for generic binary
 * is \\x00. UJO documents can be embedded into other UJO documents as binary data. For
 * automatic conversion into other formats they can be marked with type \\x01 as UJO documents.
 * The types from \\x80 to \\xFF are reserved for application specific types. All other types
 * may be specified in later versions of UJO.
 *
 * @param w    ujo writer handle
 * @param t    binary type
 * @param d    pointer to binary data
 * @param n    number of octets
 *
 * @return UJO error code or UJO_SUCCESS
 */
ujoError ujo_writer_add_binary(ujo_writer* w, uint8_t t, const uint8_t* d, uint32_t n)
{
	ujoError err;

	report_error(ujo_state_allow_atomic(w->state->state),"value not allowed", UJO_ERR_TYPE_MISPLACED);

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_BIN));
	return_on_err(_ujo_writer_put_uint8(w, t));

	return_on_err(_ujo_writer_put(w, &n, sizeof(uint32_t)));
	return_on_err(_ujo_writer_put(w, d, n));

	w->state = ujo_state_switch(ATOMIC_FOUND, w->state, w->state_stack);

	return UJO_SUCCESS;
};

/**
 * @brief Open a table.
 *
 * A table is a combination of a list of columns and a table body with data fields.
 * After opening a table, columns must be added first. There has to be at least one column
 * to create a valid table. To add columns the string writing functions
 * are used. To mark the
 * end of the column set a call to ujo_writer_table_end_columns() is required.
 * Depending on how many columns are created for the table, values can be added. You can
 * simply close the table right after creating the columns with ujo_writer_table_close() or 
 * add values of your choice. Unlike a database table, UJO tables contain variant values.
 * You can store containers like lists, maps or other tables as well as atomic type values
 * in a table cell. 
 * The number of values to be added to the table has to be a multiple of the number of columns or 0.
 * 
 * @param w    ujo writer handle
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_table_end_columns, ujo_writer_table_close
 */
ujoError ujo_writer_table_open(ujo_writer* w)
{
	ujoError err;

	report_error(ujo_state_allow_container(w->state->state),"table not allowed", UJO_ERR_TYPE_MISPLACED);

	w->state = ujo_state_next(STATE_TABLE_COLUMNS, w->state, w->state_stack);
	
	w->state->table.columns = 0;
	w->state->table.column  = 0;

	return_on_err(_ujo_writer_put_uint8(w, UJO_TYPE_TABLE));

	return UJO_SUCCESS;
};

/**
 * @brief End of table columns. 
 *
 * After opening a table with ujo_writer_table_open(), table rows have to be added.
 * A call to ujo_writer_table_end_columns() marks the end of the set of columns.
 *
 * @param w    ujo writer handle
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_table_open, ujo_writer_table_close
 */
ujoError ujo_writer_table_end_columns(ujo_writer* w)
{
	ujoError err;

	report_error(w->state->state==STATE_TABLE_COLUMNS,"close table columns not allowed", UJO_ERR_INVALID_OBJECT);
	report_error(w->state->table.columns > 0,"minimum column count mismatch", UJO_ERR_INVALID_OBJECT);
	
	return_on_err(_ujo_writer_put_uint8(w, UJO_TERMINATOR));

    w->state->state = STATE_TABLE_VALUES;

	return UJO_SUCCESS;
};

/**
 * @brief Close a table. 
 *
 * Close a table container.
 *
 * @param w    ujo writer handle
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_table_end_columns, ujo_writer_table_open
 */
ujoError ujo_writer_table_close(ujo_writer* w)
{
	report_error(w->state->state==STATE_TABLE_VALUES,"close table not allowed", UJO_ERR_INVALID_OBJECT);
	report_error(w->state->table.column == 0,"unbalanced table row", UJO_ERR_INVALID_OBJECT);
	
	w->state = ujo_state_prev(w->state, w->state_stack);
	_ujo_writer_put_uint8(w, UJO_TERMINATOR);

    w->state = ujo_state_switch(CONTAINER_CLOSED, w->state, w->state_stack);

	return UJO_SUCCESS;
};


/** 
@cond INTERNAL_DOCS
*/

static __inline ujoError _ujo_writer_put_memory(ujo_writer* w, const void* sequence, size_t bytes) 
{
	uint64_t   totalbytes;
	size_t     newbufsize;
	ujoByte    *temp;

	totalbytes = w->bytes+bytes;

	/* resize buffer */
	if (totalbytes > w->buffersize) {
		/* reallocate buffer */
		newbufsize = w->buffersize + ((bytes / UJO_DEFAULT_BUFSIZE)+1)*UJO_DEFAULT_BUFSIZE;
		temp = w->buffer;
		w->buffer = (ujoByte*)realloc(w->buffer, newbufsize);
		if (w->buffer == NULL) {
			w->buffer = temp;
			report_error(ujoFalse, "resize buffer failed", UJO_ERR_ALLOCATION);
		} /* end if */
		w->buffersize = newbufsize;
	}

	/* write sequence to buffer */
	memcpy(w->buffer+w->bytes, sequence, bytes);
	w->bytes += bytes;

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_writer_put_file(ujo_writer* w, const void* sequence, size_t bytes) 
{
	/* write sequence to file */

	report_error(fwrite(sequence, 1, bytes, w->file) == bytes,
		"write to file failed", UJO_ERR_FILE);

	return UJO_SUCCESS;
};


ujoError _ujo_writer_put(ujo_writer* w, const void* sequence, size_t bytes)
{
	ujoError err = UJO_SUCCESS;

	switch(w->type)
	{
	case UJO_MEMORY:
		err = _ujo_writer_put_memory(w, sequence, bytes);
		break;
	case UJO_FILE:
		err = _ujo_writer_put_file(w, sequence, bytes);
	}
	return err;
}

ujoError _ujo_writer_put_uint8(ujo_writer* w, uint8_t value) 
{
	return _ujo_writer_put(w, &value, sizeof(uint8_t));
};

ujoError _ujo_writer_put_uint16(ujo_writer* w, uint16_t value)
{
	return _ujo_writer_put(w, &value, sizeof(uint16_t));
};


/**
@endcond
*/

/* @} */


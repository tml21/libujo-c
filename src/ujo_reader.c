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

#include "ujo_reader.h"
#include "ujo_stack.h"
#include "ujo_int.h"
#include "ujo_errors.h"
#include "ujo_macros.h"
#include "ujo_log.h"
#include "string.h"
#include "ujo_constants.h"
#include "ujo_state.h"
#include "ujo_float.h"
#include "ujo_endian.h"

/** 
@cond INTERNAL_DOCS
*/
struct _ujo_reader {
	ujoAccessType	type;
	ujoStack*		state_stack;
	ujo_state*		state;

	// header
	struct  {
		char            magic[4];
		uint16_t        version;
		uint8_t         compression;
	} header;

	// memory reader
	size_t			buffersize;
	ujoByte*		buffer;
	size_t			parsed;

	// file reader
	FILE*           file;

	ujoOnElementFunc  onElement;
	ujoPointer        onElementData;
};

struct _ujo_element {
	ujoTypeId type;
	union {
		int8_t  int8val;
		int16_t int16val;
		int32_t int32val;
		int64_t int64val;

		uint8_t  uint8val;
		uint16_t uint16val;
		uint32_t uint32val;
		uint64_t uint64val;

		float16_t float16val;
		float32_t float32val;
		float64_t float64val;

		ujoBool boolval;

		ujoDateTime datetime;
		int64_t  uxtime;
		
		struct {
			ujoTypeId type;
			union {
				char* c_string;
				uint8_t* u8_string;
				uint16_t* u16_string;
				uint32_t* u32_string;
			};
			uint32_t n;
		} string;
		
		struct {
			ujoTypeId type;
			uint8_t*  data;
			uint32_t  n;
		} binary;
	};
};

/** 
@endcond
*/

/** 
@cond INTERNAL_DOCS
*/

static __inline ujoError _ujo_new_reader(ujo_reader** r)
{
	ujo_reader*  newr;

	newr = (ujo_reader*)ujo_new(ujo_reader, 1); 

	report_error(newr, "allocation failed", UJO_ERR_ALLOCATION);
	
	newr->state = (ujo_state*)ujo_new(ujo_state,1);
	newr->state->state = STATE_ROOT;

	/* initialize stack */
	newr->state_stack = ujo_new_stack(ujo_free);

	*r = newr;

	return UJO_SUCCESS;
}

/**
@endcond
*/

/**
 * @brief Create a new memory reader.
 *
 * The reader object provides functions to traverse UJO data
 * from a memory buffer.
 * 
 * @param r    reference to a reader
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_free_reader
 */
ujoError ujo_new_memory_reader(ujo_reader** r)
{
	ujoError err;
	ujo_reader*  newr;

	err = _ujo_new_reader(&newr);
	if (err != UJO_SUCCESS)
	{
		return err;
	};
	
	newr->type = UJO_MEMORY;
	*r = newr;

	return UJO_SUCCESS;
};

/**
 * @brief Create a new file reader.
 *
 * The reader object provides functions to traverse UJO data
 * from a file.
 * 
 * @param r         reference to a reader
 * @param filename  full path of the UJO file
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_free_reader
 */ujoError ujo_new_file_reader(ujo_reader** r, const char* filename)
{
	ujoError     err;
	ujo_reader*  newr;
	FILE*        filehandle;

	filehandle = fopen(filename, "rb"); 
    report_error(filehandle != NULL, "cannot open file", UJO_ERR_FILE);	
	return_on_err(_ujo_new_reader(&newr));
	
	newr->type = UJO_FILE;
	newr->file = filehandle;

	*r = newr;

	return UJO_SUCCESS;
}


/**
 * @brief Set onElement callback.
 *
 * If an element is found by the reader while scanning UJO data
 * it is passed to the onElement callback function to processed.
 * 
 * @param r    reader object
 * @param f    callback function or NULL to remove a previously assigned function
 * @param data a pointer to custom data or NULL;
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_reader_parse
 */
ujoError ujo_reader_set_on_element(ujo_reader* r, ujoOnElementFunc f, ujoPointer data)
{
	if (f)
	{
		r->onElement = f;
		r->onElementData = data;
	}
	else
	{
		r->onElement = NULL;
		r->onElementData = NULL;
	}
	return UJO_SUCCESS;
};

/**
 * @brief Dispose UJO reader.
 *
 * If the reader object is not used anymore my the application, it can be disposed. 
 * The allocated memory is released.
 * 
 * @param r    reader object
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_new_reader
 */
ujoError ujo_free_reader(ujo_reader* r)
{
	report_error(r, "invalid handle", UJO_ERR_INVALID_DATA);	
	
	ujo_free_stack(r->state_stack);	
	ujo_free(r->state);
	
	switch(r->type) {
	case UJO_MEMORY:
		ujo_free(r->buffer);
		break;
	case UJO_FILE:
		fclose(r->file);
		break;
	default:
		break;
	}
	
	ujo_free(r);

	return UJO_SUCCESS;
};

/**
 * @brief Get the type of a reader object.
 *
 * The type property defines how the data is handled.
 *
 * @param r    ujo reader handle
 * @param type reference to the reader type
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_new_reader
 */
ujoError ujo_reader_get_type(ujo_reader* r, ujoAccessType* type)
{
	report_error(r, "invalid handle", UJO_ERR_INVALID_DATA);	
	*type = r->type;
	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_header(ujo_reader *r)
{
	ujoError err;

    // get magic code
	return_on_err(_ujo_reader_get_data(r,&(r->header.magic[0]), 4));
	report_error(strncmp(UJO_MAGIC, (const char*)r->header.magic,4) == 0, "no ujo header found", UJO_ERR_INVALID_DATA);

	// get version
	return_on_err(_ujo_reader_get_data(r,&(r->header.version), sizeof(uint16_t)));
	r->header.version = UJO_UINT16_SWAP(r->header.version);
	report_error(r->header.version == 1, "unsupported UJO version", UJO_ERR_INVALID_DATA);

	// compression
	return_on_err(_ujo_reader_get_data(r,&(r->header.compression), sizeof(uint8_t))); // always \x00

	return UJO_SUCCESS;
}

static __inline ujoError _ujo_reader_open_list(ujo_reader *r, /*@unused@*/ ujo_element *v)
{
	r->state = ujo_state_next(STATE_LIST, r->state, r->state_stack);
	return UJO_SUCCESS;
}

static __inline ujoError _ujo_reader_open_map(ujo_reader *r, /*@unused@*/ ujo_element *v)
{
	r->state = ujo_state_next(STATE_DICT_KEY, r->state, r->state_stack);
	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_open_table(ujo_reader *r, /*@unused@*/ ujo_element *v)
{
	r->state = ujo_state_next(STATE_TABLE_COLUMNS, r->state, r->state_stack);
	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_int64(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->int64val), sizeof(int64_t)));
	v->int64val = (int64_t) UJO_UINT64_SWAP(v->int64val);
	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_int32(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->int32val), sizeof(int32_t)));
	v->int32val = (int32_t) UJO_UINT32_SWAP(v->int32val);
	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
}

static __inline ujoError _ujo_reader_parse_int16(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->int16val), sizeof(int16_t)));
	v->int16val = (int16_t) UJO_UINT16_SWAP(v->int16val);
	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_int8(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->int8val), sizeof(int8_t)));
	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_uint64(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->uint64val), sizeof(uint64_t)));
	v->uint64val = (uint64_t) UJO_UINT64_SWAP(v->uint64val);
	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_uint32(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->uint32val), sizeof(uint32_t)));
	v->uint32val = (uint32_t) UJO_UINT32_SWAP(v->uint32val);
	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
}

static __inline ujoError _ujo_reader_parse_uint16(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->uint16val), sizeof(uint16_t)));
	v->uint16val = (uint16_t) UJO_UINT16_SWAP(v->uint16val);
	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_uint8(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->uint8val), sizeof(uint8_t)));
	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_float16(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->float16val), sizeof(float16_t)));
	v->float16val = UJO_UINT16_SWAP(v->float16val);
	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_float32(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->float32val), sizeof(float32_t)));
	v->float32val = UJO_FLOAT32_SWAP(v->float32val);
	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_float64(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->float64val), sizeof(float64_t)));
	v->float64val = UJO_FLOAT64_SWAP(v->float64val);
	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_bool(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->boolval), sizeof(ujoBool)));
	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_none(ujo_reader *r, /*@unused@*/ ujo_element *v)
{
	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_close_container(ujo_reader *r, /*@unused@*/ ujo_element *v)
{
	r->state = ujo_state_prev(r->state, r->state_stack);
    r->state = ujo_state_switch(CONTAINER_CLOSED, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_uxtime(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->uxtime), sizeof(int64_t)));
	v->uxtime = UJO_UINT64_SWAP(v->uxtime);
	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_date(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->datetime.year), sizeof(int16_t)));
	v->datetime.year = (int16_t) UJO_UINT16_SWAP(v->datetime.year);
	return_on_err(_ujo_reader_get_data(r,&(v->datetime.month), sizeof(uint8_t)));
	return_on_err(_ujo_reader_get_data(r,&(v->datetime.day), sizeof(uint8_t)));

	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_time(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->datetime.hour), sizeof(uint8_t)));
	return_on_err(_ujo_reader_get_data(r,&(v->datetime.minute), sizeof(uint8_t)));
	return_on_err(_ujo_reader_get_data(r,&(v->datetime.second), sizeof(uint8_t)));

	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_timestamp(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r,&(v->datetime.year), sizeof(int16_t)));
	v->datetime.year = (int16_t) UJO_UINT16_SWAP(v->datetime.year);
	return_on_err(_ujo_reader_get_data(r,&(v->datetime.month), sizeof(uint8_t)));
	return_on_err(_ujo_reader_get_data(r,&(v->datetime.day), sizeof(uint8_t)));

	return_on_err(_ujo_reader_get_data(r,&(v->datetime.hour), sizeof(uint8_t)));
	return_on_err(_ujo_reader_get_data(r,&(v->datetime.minute), sizeof(uint8_t)));
	return_on_err(_ujo_reader_get_data(r,&(v->datetime.second), sizeof(uint8_t)));

	return_on_err(_ujo_reader_get_data(r,&(v->datetime.millisecond), sizeof(uint16_t)));
	v->datetime.millisecond = (int16_t) UJO_UINT16_SWAP(v->datetime.millisecond);

	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_string(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r, &v->string.type, sizeof(ujoTypeId)));
	return_on_err(_ujo_reader_get_data(r,&v->string.n, sizeof(uint32_t)));
	switch (v->string.type)
	{
	case UJO_SUB_STRING_C:
		v->string.c_string = (char*)ujo_calloc(v->string.n, sizeof(char));
		report_error(v->string.c_string, "allocation failed", UJO_ERR_ALLOCATION);
		return_on_err(_ujo_reader_get_data(r,v->string.c_string, v->string.n*sizeof(char)));
		break;
	case UJO_SUB_STRING_U8:
		v->string.u8_string = (uint8_t*)ujo_calloc(v->string.n, sizeof(uint8_t));
		report_error(v->string.u8_string, "allocation failed", UJO_ERR_ALLOCATION);
		return_on_err(_ujo_reader_get_data(r,v->string.u8_string, v->string.n*sizeof(uint8_t)));
		break;
	case UJO_SUB_STRING_U16:
		v->string.u16_string = (uint16_t*)ujo_calloc(v->string.n, sizeof(uint16_t));
		report_error(v->string.u16_string, "allocation failed", UJO_ERR_ALLOCATION);
		return_on_err(_ujo_reader_get_data(r,v->string.u16_string, v->string.n*sizeof(uint16_t)));
		break;
	case UJO_SUB_STRING_U32:
		v->string.u32_string = (uint32_t*)ujo_calloc(v->string.n, sizeof(uint32_t));
		report_error(v->string.u32_string, "allocation failed", UJO_ERR_ALLOCATION);
		return_on_err(_ujo_reader_get_data(r,v->string.u32_string, v->string.n*sizeof(uint32_t)));
		break;
	default:
		report_error(0, "invalid string subtype", UJO_ERR_INVALID_DATA);
	}

	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_parse_binary(ujo_reader *r, ujo_element *v)
{
	ujoError err;

	return_on_err(_ujo_reader_get_data(r, &v->binary.type, sizeof(ujoTypeId)));
	return_on_err(_ujo_reader_get_data(r,&v->binary.n, sizeof(uint32_t)));

	v->binary.data = (uint8_t*)ujo_calloc(v->binary.n, 1);
	report_error(v->binary.data, "allocation failed", UJO_ERR_ALLOCATION);
	return_on_err(_ujo_reader_get_data(r,v->binary.data, v->binary.n));

	r->state = ujo_state_switch(ATOMIC_FOUND, r->state, r->state_stack);

	return UJO_SUCCESS;
};

static __inline ujoError _ujo_reader_get_memory_data(ujo_reader* r, void* sequence, size_t bytes) 
{
	ujoByte *cursor = r->buffer + r->parsed;
	memcpy(sequence,cursor,bytes);
	r->parsed += bytes;

	return UJO_SUCCESS;
}

static __inline ujoError _ujo_reader_get_file_data(ujo_reader* r, void* sequence, size_t bytes) 
{
	report_error(fread(sequence, 1, bytes, r->file) == bytes,
		"read from file failed", UJO_ERR_FILE);

	return UJO_SUCCESS;
}

ujoError _ujo_reader_get_data(ujo_reader* r, void* sequence, size_t bytes) 
{
	ujoError err = UJO_SUCCESS;

	switch(r->type)
	{
	case UJO_MEMORY:
		err = _ujo_reader_get_memory_data(r, sequence, bytes);
		break;
	case UJO_FILE:
		err = _ujo_reader_get_file_data(r, sequence, bytes);
		break;
	default:
		break;
	}
	return err;
}

/**
 * @brief Assign a buffer to the reader.
 *
 * A memory reader operated on a buffer containing an UJO document as
 * a sequence of octets. This functions copies the content of an existing
 * buffer to the reader buffer to prepare the scanning.
 *
 * @param r    ujo reader handle
 * @param buffer a pointer to an UJO document in memory
 * @param bytes the number of octets in the buffer
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_reader_parse, ujo_reader_get_first, ujo_reader_get_next
 */
ujoError ujo_reader_set_buffer(ujo_reader *r, ujoByte* buffer, size_t bytes)
{	
	report_error(r, "invalid handle", UJO_ERR_INVALID_DATA);	
	report_error(buffer, "invalid buffer", UJO_ERR_INVALID_DATA);	


	r->buffer = ujo_new(ujoByte, bytes);
	r->buffersize = bytes;
	r->parsed = 0;
	ujo_clear_stack(r->state_stack);

	r->state->state = STATE_ROOT;

	memcpy(r->buffer, buffer, bytes);

	return UJO_SUCCESS;
};

/**
 * @brief Parse an UJO document.
 *
 * Parsing an UJO document means scanning for elements from first to last.
 * Each element is passed to the onElement callback function.
 *
 * @param r    ujo reader handle
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_reader_set_buffer, ujo_reader_get_first, ujo_reader_get_next
 */
ujoError ujo_reader_parse(ujo_reader *r) 
{
	ujoError  err;
	ujo_element* ujoval;
	ujoBool   eod;

	err = ujo_reader_get_first(r, &ujoval, &eod);
	if (err != UJO_SUCCESS) return err;

	while (!eod)
	{
		if (r->onElement) {
			err = r->onElement(ujoval, r->onElementData);
		}
		ujo_free(ujoval);
		if (err != UJO_SUCCESS) return err;

		err = ujo_reader_get_next(r, &ujoval, &eod);
		if (err != UJO_SUCCESS) return err;
		
	}
	return UJO_SUCCESS;
};

/**
 * @brief Get the first UJO element.
 *
 * The header of the UJO document is parsed and the first element
 * found is returned. The first element of an UJO document has to be
 * a marker to open a container type like list, map or table.
 *
 * @param r    ujo reader handle
 * @param v    reference to an UJO element
 * @param eod  reference to a boolean value to indicate end of document
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_reader_set_buffer, ujo_reader_parse, ujo_reader_get_next
 */
ujoError ujo_reader_get_first(ujo_reader *r, ujo_element** v, ujoBool *eod)
{
	ujoError err;
	
	*v = NULL;

    err = _ujo_reader_parse_header(r);
	if (err != UJO_SUCCESS) return err;
	
	return ujo_reader_get_next(r,v,eod);
};

/**
 * @brief Get the next UJO element.
 *
 * To use this function a previous call of ujo_reader_get_first() is needed
 * to initialize the scanning.
 *
 * @param r    ujo reader handle
 * @param v    reference to an UJO element
 * @param eod  reference to a boolean value to indicate end of document
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_reader_set_buffer, ujo_reader_parse, ujo_reader_get_first
 */
ujoError ujo_reader_get_next(ujo_reader *r, ujo_element** v, ujoBool *eod)
{
	ujoError   err;
    ujo_element* value;

	*v   = NULL;

	/* signal end of data, if document is closed */
	if (r->state->state == STATE_CLOSED) 
	{
		*eod = ujoTrue;
		return UJO_SUCCESS;
	} 
	else
	{
		*eod = ujoFalse;
	}

	value = ujo_new(ujo_element,1);
	return_on_err(_ujo_reader_get_data(r,&(value->type), sizeof(uint8_t)));

	switch(value->type)
	{
	case UJO_TYPE_LIST: 
		err = _ujo_reader_open_list(r, value); break;
	case UJO_TYPE_MAP: 
		err = _ujo_reader_open_map(r, value); break;
	case UJO_TYPE_TABLE: 
		err = _ujo_reader_open_table(r, value); break;
	case UJO_TERMINATOR: 
		if (r->state->state == STATE_TABLE_COLUMNS)
		{
			r->state->state = STATE_TABLE_VALUES;
			err = UJO_SUCCESS;
		} else
			err = _ujo_reader_close_container(r, value); 
		break;
	case UJO_TYPE_INT64: 
		err = _ujo_reader_parse_int64(r, value); break;
	case UJO_TYPE_INT32: 
		err = _ujo_reader_parse_int32(r, value); break;
	case UJO_TYPE_INT16: 
		err = _ujo_reader_parse_int16(r, value); break;
	case UJO_TYPE_INT8: 
		err = _ujo_reader_parse_int8(r, value); break;
	case UJO_TYPE_UINT64: 
		err = _ujo_reader_parse_uint64(r, value); break;
	case UJO_TYPE_UINT32: 
		err = _ujo_reader_parse_uint32(r, value); break;
	case UJO_TYPE_UINT16: 
		err = _ujo_reader_parse_uint16(r, value); break;
	case UJO_TYPE_UINT8: 
		err = _ujo_reader_parse_uint8(r, value); break;
	case UJO_TYPE_FLOAT16:
		err = _ujo_reader_parse_float16(r, value); break;
	case UJO_TYPE_FLOAT32: 
		err = _ujo_reader_parse_float32(r, value); break;
	case UJO_TYPE_FLOAT64: 
		err = _ujo_reader_parse_float64(r, value); break;
	case UJO_TYPE_BOOL: 
		err = _ujo_reader_parse_bool(r, value); break;
	case UJO_TYPE_NONE: 
		err = _ujo_reader_parse_none(r, value); break;
	case UJO_TYPE_UX_TIME: 
		err = _ujo_reader_parse_uxtime(r, value); break;
	case UJO_TYPE_DATE: 
		err = _ujo_reader_parse_date(r, value); break;
	case UJO_TYPE_TIME: 
		err = _ujo_reader_parse_time(r, value); break;
	case UJO_TYPE_TIMESTAMP: 
		err = _ujo_reader_parse_timestamp(r, value); break;
	case UJO_TYPE_STRING: 
		err = _ujo_reader_parse_string(r, value); break;
	case UJO_TYPE_BIN: 
		err = _ujo_reader_parse_binary(r, value); break;
	default:  
		err = UJO_ERR_INVALID_DATA;
	}

	if (err == UJO_SUCCESS)
	{
		*v = value;
		return UJO_SUCCESS;
	}
	else
	{
		ujo_free_element(value);
		report_error(0, "failed to get next value", err);
	}
};


/**
 * @brief Dispose an UJO element.
 *
 * If elements are fetched from the document using ujo_reader_get_first()
 * and ujo_reader_get_next(), each element has to be disposed 
 * to release the allocated memory. The ujo_reader_parse() method 
 * automatically disposes the element after the handler function returns.
 *
 * @param e    ujo element handle
 *
 * @return UJO error code or UJO_SUCCESS
 */
ujoError ujo_free_element(ujo_element* e)
{
	switch (e->type)
	{
	case UJO_TYPE_STRING:
		switch (e->string.type)
		{
		case UJO_SUB_STRING_C:
			ujo_free(e->string.c_string);
			break;
		case UJO_SUB_STRING_U8:
			ujo_free(e->string.u8_string);
			break;
		case UJO_SUB_STRING_U16:
			ujo_free(e->string.u16_string);
			break;
		case UJO_SUB_STRING_U32:
			ujo_free(e->string.u32_string);
			break;
		}; break;
	case UJO_TYPE_BIN:
		ujo_free(e->binary.data);
	};
	ujo_free(e);
	return UJO_SUCCESS;
};

/**
 * @brief Get a 8bit signed integer value.
 *
 * If the element is of UJO_TYPE_INT8, this function is used
 * to retrieve the value.
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 */
ujoError ujo_element_get_int8(ujo_element* e, int8_t* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_INT8, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->int8val;
	return UJO_SUCCESS;
};

/**
 * @brief Get a 16bit signed integer value.
 *
 * If the element is of UJO_TYPE_INT16, this function is used
 * to retrieve the value.
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 */
ujoError ujo_element_get_int16(ujo_element* e, int16_t* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_INT16, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->int16val;
	return UJO_SUCCESS;
};

/**
 * @brief Get a 32bit signed integer value.
 *
 * If the element is of UJO_TYPE_INT32, this function is used
 * to retrieve the value.
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 */
ujoError ujo_element_get_int32(ujo_element* e, int32_t* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_INT32, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->int32val;
	return UJO_SUCCESS;
};

/**
 * @brief Get a 64bit signed integer value.
 *
 * If the element is of UJO_TYPE_INT64, this function is used
 * to retrieve the value.
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 */
ujoError ujo_element_get_int64(ujo_element* e, int64_t* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_INT64, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->int64val;
	return UJO_SUCCESS;
};

/**
 * @brief Get a 8bit unsigned integer value.
 *
 * If the element is of UJO_TYPE_UINT8, this function is used
 * to retrieve the value.
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 */
ujoError ujo_element_get_uint8(ujo_element* e, uint8_t* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_UINT8, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->uint8val;
	return UJO_SUCCESS;
};

/**
 * @brief Get a 16bit unsigned integer value.
 *
 * If the element is of UJO_TYPE_UINT16, this function is used
 * to retrieve the value.
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 */
ujoError ujo_element_get_uint16(ujo_element* e, uint16_t* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_UINT16, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->uint16val;
	return UJO_SUCCESS;
};

/**
 * @brief Get 32bit unsigned integer value.
 *
 * If the element is of UJO_TYPE_UINT32, this function is used
 * to retrieve the value.
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 */
ujoError ujo_element_get_uint32(ujo_element* e, uint32_t* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_UINT32, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->uint32val;
	return UJO_SUCCESS;
};

/**
 * @brief Get a 64bit unsigned integer value.
 *
 * If the element is of UJO_TYPE_UINT64, this function is used
 * to retrieve the value.
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 */
ujoError ujo_element_get_uint64(ujo_element* e, uint64_t* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_UINT64, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->uint64val;
	return UJO_SUCCESS;
};

/**
 * @brief Get a 16bit float value (half precision)
 *
 * If the element is of UJO_TYPE_FLOAT16, this function is used
 * to retrieve the value. C does not support half precision values.
 * The function converts to single precision automatically. 
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_float16
 */
ujoError ujo_element_get_float16(ujo_element* e, float32_t* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_FLOAT16, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = half_to_float(e->float16val);
	return UJO_SUCCESS;
};

/**
 * @brief Get a 32bit float value (single precision)
 *
 * If the element is of UJO_TYPE_FLOAT32, this function is used
 * to retrieve the value.
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_float32
 */
ujoError ujo_element_get_float32(ujo_element* e, float32_t* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_FLOAT32, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->float32val;
	return UJO_SUCCESS;
};

/**
 * @brief Get a 64bit float value (double precision)
 *
 * If the element is of UJO_TYPE_FLOAT64, this function is used
 * to retrieve the value.
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_float64
 */
ujoError ujo_element_get_float64(ujo_element* e, float64_t* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_FLOAT64, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->float64val;
	return UJO_SUCCESS;
};

/**
 * @brief Get a boolean value
 *
 * If the element is of UJO_TYPE_BOOL, this function is used
 * to retrieve the value.
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 */
ujoError ujo_element_get_bool(ujo_element* e, ujoBool* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_BOOL, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->boolval;
	return UJO_SUCCESS;
};


/**
 * @brief Get the element type
 *
 * The application needs to check the element type to call
 * the proper function to retrieve
 *
 * @param e    ujo element handle
 * @param value   reference to a variable to hold the type id
 *
 * @return UJO error code or UJO_SUCCESS
 */
ujoError ujo_element_get_type(ujo_element* e, ujoTypeId* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);

	*value = e->type;
	return UJO_SUCCESS;
};

/**
 * @brief Get a unix time.
 *
 * If the element is of UJO_TYPE_UX_TIME, this function is used
 * to retrieve the value.
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_uxtime
 */
ujoError ujo_element_get_uxtime(ujo_element* e, int64_t* value) 
{
    report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_UX_TIME, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->uxtime;
	return UJO_SUCCESS;
};
	
/**
 * @brief Get a date structure.
 *
 * If the element is of UJO_TYPE_DATE, this function is used
 * to retrieve the value.
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_date
 */
ujoError ujo_element_get_date(ujo_element* e, ujoDateTime* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_DATE, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->datetime;
	return UJO_SUCCESS;
};

/**
 * @brief Get a time structure.
 *
 * If the element is of UJO_TYPE_TIME, this function is used
 * to retrieve the value.
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_time
 */
ujoError ujo_element_get_time(ujo_element* e, ujoDateTime* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_TIME, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->datetime;
	return UJO_SUCCESS;
};

/**
 * @brief Get a timestamp structure.
 *
 * If the element is of UJO_TYPE_TIMESTAMP, this function is used
 * to retrieve the value.
 *
 * @param e    ujo element handle
 * @param value    reference to a variable to hold the value
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_timestamp
 */
ujoError ujo_element_get_timestamp(ujo_element* e, ujoDateTime* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_TIMESTAMP, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->datetime;
	return UJO_SUCCESS;
};

/**
 * @brief Get a \\x00 terminated c string.
 *
 * If the element is of UJO_TYPE_STRING with subtype UJO_SUB_STRING_C,
 * this function is used to retrieve the value. The size n is not the
 * number of characters, it is the total number of octets including the 
 * terminaring \\x00.
 * The buffer returned by this function is managed by the element
 * and will be released if the element is disposed.
 *
 * @param e    ujo element handle
 * @param s    reference to a buffer
 * @param n    number of octets including teminating \\x00
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_string_c
 */
ujoError ujo_element_get_string_c(ujo_element* e, char** s, uint32_t* n)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_STRING, "element type mismatch", UJO_ERR_INVALID_DATA);
	report_error(e->string.type == UJO_SUB_STRING_C, "string type mismatch", UJO_ERR_INVALID_DATA);

	*s = e->string.c_string;
	*n = e->string.n;
	
	return UJO_SUCCESS;
};

/**
 * @brief Get a unicode UTF-8 string.
 *
 * If the element is of UJO_TYPE_STRING with subtype UJO_SUB_STRING_U8,
 * this function is used to retrieve the value. The size n is not the
 * number of characters, it is the total number of units. A unit of an UTF-8
 * string is 8 bit.
 * The buffer returned by this function is managed by the element
 * and will be released if the element is disposed.
 *
 * @param e    ujo element handle
 * @param s    reference to a buffer
 * @param n    number of units
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_string_u8
 */
ujoError ujo_element_get_string_u8(ujo_element* e, uint8_t** s, uint32_t* n)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_STRING, "element type mismatch", UJO_ERR_INVALID_DATA);
	report_error(e->string.type == UJO_SUB_STRING_U8, "string type mismatch", UJO_ERR_INVALID_DATA);

	*s = e->string.u8_string;
	*n = e->string.n;
	
	return UJO_SUCCESS;
};

/**
 * @brief Get a unicode UTF-16 string.
 *
 * If the element is of UJO_TYPE_STRING with subtype UJO_SUB_STRING_U16,
 * this function is used to retrieve the value. The size n is not the
 * number of characters, it is the total number of units. A unit of an UTF-16
 * string is 16 bit.
 * The buffer returned by this function is managed by the element
 * and will be released if the elemenet is disposed.
 *
 * @param e    ujo element handle
 * @param s    reference to a buffer
 * @param n    number of units
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_string_u16
 */
ujoError ujo_element_get_string_u16(ujo_element* e, uint16_t** s, uint32_t* n)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_STRING, "element type mismatch", UJO_ERR_INVALID_DATA);
	report_error(e->string.type == UJO_SUB_STRING_U16, "string type mismatch", UJO_ERR_INVALID_DATA);

	*s = e->string.u16_string;
	*n = e->string.n;
	
	return UJO_SUCCESS;
};

/**
 * @brief Get a unicode UTF-32 string.
 *
 * If the element is of UJO_TYPE_STRING with subtype UJO_SUB_STRING_U32,
 * this function is used to retrieve the value. The size n is not the
 * number of characters, it is the total number of units. A unit of an UTF-32
 * string is 32 bit.
 * The buffer returned by this function is managed by the element
 * and will be released if the element is disposed.
 *
 * @param e    ujo element handle
 * @param s    reference to a buffer
 * @param n    number of units
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_string_u32
 */
ujoError ujo_element_get_string_u32(ujo_element* e, uint32_t** s, uint32_t* n)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_STRING, "element type mismatch", UJO_ERR_INVALID_DATA);
	report_error(e->string.type == UJO_SUB_STRING_U32, "string type mismatch", UJO_ERR_INVALID_DATA);

	*s = e->string.u32_string;
	*n = e->string.n;
	
	return UJO_SUCCESS;
};

/**
 * @brief Get binary data.
 *
 * If the element is of UJO_TYPE_BIN,
 * this function is used to retrieve the value. 
 * The buffer returned by this function is managed by the element
 * and will be released if the element is disposed.
 *
 * @param e    ujo element handle
 * @param t    binary type
 * @param d    reference to a an array of unsigned 8 bit integer
 * @param n    number of octets
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_writer_add_binary
 */
ujoError ujo_element_get_binary(ujo_element* e, uint8_t* t, uint8_t** d, uint32_t* n)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_BIN, "element type mismatch", UJO_ERR_INVALID_DATA);

	*d = e->binary.data;
	*n = e->binary.n;
	*t = e->binary.type;
	
	return UJO_SUCCESS;
};

/**
 * @brief Get string type.
 *
 * UJO supports c string, UTF-8, UTF-16 amd UTF-32 strings.
 * This functions returns the type of string.
 *
 * @param e      ujo element handle
 * @param value  string type
 *
 * @return UJO error code or UJO_SUCCESS
 * @sa ujo_element_get_string_u32, ujo_element_get_string_u16, ujo_element_get_string_u8, ujo_element_get_string_c
 */
ujoError ujo_element_get_string_type(ujo_element* e, ujoTypeId* value)
{
	report_error(e, "invalid handle", UJO_ERR_INVALID_DATA);
	report_error(e->type == UJO_TYPE_STRING, "element type mismatch", UJO_ERR_INVALID_DATA);

	*value = e->string.type;
	
	return UJO_SUCCESS;
};



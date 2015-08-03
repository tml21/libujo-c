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
 */

#include "ujo_tests.h"
#include "testujo_helper.h"
#include <stdio.h>

/**
 * test02: reading and writing integer types
 */
ujoBool test02()
{
	ujo_writer*		ujow;
	ujo_reader*		ujor;
	ujo_element*	element;

	ujoError		err = UJO_SUCCESS;

	ujoByte*		data;
	size_t		datasize;
	ujoBool			eod;
	ujoTypeId		type;

	int64_t         int64val; 
	int32_t         int32val;
	int16_t         int16val;
	int8_t          int8val;

	uint64_t         uint64val; 
	uint32_t         uint32val;
	uint16_t         uint16val;
	uint8_t          uint8val;

//		printf("**saved data size %i\n",datasize);
//		printf("**actual data size %i\n",get_write_buffer_size(ujow));

	err = ujo_new_memory_writer(&ujow);
	print_return_ujo_err(err,"ujo_new_writer"); 

	
	// create a list as top level container
	err = ujo_writer_list_open(ujow);
	print_return_ujo_err(err,"ujo_writer_list_open"); 
	

	    // int64 max
		datasize = get_write_buffer_size(ujow);
		err = ujo_writer_add_int64(ujow, INT64_MAX);
		print_return_ujo_err(err,"ujo_writer_add_int64 (max)"); 
		print_return_expr_fail(datasize+9 == get_write_buffer_size(ujow),
			"atomic type size unexpected (int64)");

	    // int64 min
		err = ujo_writer_add_int64(ujow, INT64_MIN);
		print_return_ujo_err(err,"ujo_writer_add_int64 (min)"); 

	    // int32 max
		datasize = get_write_buffer_size(ujow);
		err = ujo_writer_add_int32(ujow, INT32_MAX);
		print_return_ujo_err(err,"ujo_writer_add_int32 (max)"); 
		print_return_expr_fail(datasize+5 == get_write_buffer_size(ujow),
			"atomic type size unexpected (int32)"); 

	    // int32 min
		err = ujo_writer_add_int32(ujow, INT32_MIN);
		print_return_ujo_err(err,"ujo_writer_add_int32 (min)"); 

	    // int16 max
		datasize = get_write_buffer_size(ujow);
		err = ujo_writer_add_int16(ujow, INT16_MAX);
		print_return_ujo_err(err,"ujo_writer_add_int16 (max)"); 
		print_return_expr_fail(datasize+3 == get_write_buffer_size(ujow),
			"atomic type size unexpected (int16)"); 

	    // int16 min
		err = ujo_writer_add_int16(ujow, INT16_MIN);
		print_return_ujo_err(err,"ujo_writer_add_int16 (min)"); 

	    // int8 max
		datasize = get_write_buffer_size(ujow);
		err = ujo_writer_add_int8(ujow, INT8_MAX);
		print_return_ujo_err(err,"ujo_writer_add_int8 (max)"); 
		print_return_expr_fail(datasize+2 == get_write_buffer_size(ujow),
			"atomic type size unexpected (int8)"); 

	    // int8 min
		err = ujo_writer_add_int8(ujow, INT8_MIN);
		print_return_ujo_err(err,"ujo_writer_add_int8 (min)"); 

	    // uint64 max
		datasize = get_write_buffer_size(ujow);
		err = ujo_writer_add_uint64(ujow, UINT64_MAX);
		print_return_ujo_err(err,"ujo_writer_add_uint64 (max)"); 
		print_return_expr_fail(datasize+9 == get_write_buffer_size(ujow),
			"atomic type size unexpected (int64)");

	    // uint32 max
		datasize = get_write_buffer_size(ujow);
		err = ujo_writer_add_uint32(ujow, UINT32_MAX);
		print_return_ujo_err(err,"ujo_writer_add_uint32 (max)"); 
		print_return_expr_fail(datasize+5 == get_write_buffer_size(ujow),
			"atomic type size unexpected (int32)");

	    // uint16 max
		datasize = get_write_buffer_size(ujow);
		err = ujo_writer_add_uint16(ujow, UINT16_MAX);
		print_return_ujo_err(err,"ujo_writer_add_uint16 (max)"); 
		print_return_expr_fail(datasize+3 == get_write_buffer_size(ujow),
			"atomic type size unexpected (int16)");

	    // uint8 max
		datasize = get_write_buffer_size(ujow);
		err = ujo_writer_add_uint8(ujow, UINT8_MAX);
		print_return_ujo_err(err,"ujo_writer_add_uint8 (max)"); 
		print_return_expr_fail(datasize+2 == get_write_buffer_size(ujow),
			"atomic type size unexpected (int8)");

	err = ujo_writer_list_close(ujow);
	print_return_ujo_err(err,"ujo_writer_list_close"); 

	/* get the buffer and print */
	err = ujo_writer_get_buffer(ujow, &data, &datasize);
	print_return_ujo_err(err,"ujo_writer_get_buffer"); 

	print_buffer(data, datasize);

	/* ---> test the reader */
	err = ujo_new_memory_reader(&ujor);
	print_return_ujo_err(err,"ujo_new_reader"); 

		err = ujo_reader_set_buffer(ujor, data, datasize);
		print_return_ujo_err(err,"ujo_reader_set_buffer"); 

		err = ujo_reader_get_first(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first");

		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type");
		print_return_expr_fail(type == UJO_TYPE_LIST,
			"top level container is not a list"); 

		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element (1)");

		// read int64 max
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first int64 (max)");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type");
		print_return_expr_fail(type == UJO_TYPE_INT64,
			"type not expected int64 (max)"); 
		err = ujo_element_get_int64(element, &int64val);
   		print_return_ujo_err(err,"ujo_element_get_int64 (max)");
		print_return_expr_fail(int64val == INT64_MAX,
			"unexpected value int64 (max)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element int64 (max)");

		// read int64 min
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first int64 (min)");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type int64 (min)");
		print_return_expr_fail(type == UJO_TYPE_INT64,
			"type not expected int64 (min)"); 
		err = ujo_element_get_int64(element, &int64val);
   		print_return_ujo_err(err,"ujo_element_get_int64 (min)");
		print_return_expr_fail(int64val == INT64_MIN, "unexpected value int64 (min)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element int64 (min)");

		// read int32 max
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first int32 (max)");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type");
		print_return_expr_fail(type == UJO_TYPE_INT32,
			"type not expected int32 (max)"); 
		err = ujo_element_get_int32(element, &int32val);
   		print_return_ujo_err(err,"ujo_element_get_int32 (max)");
		print_return_expr_fail(int32val == INT32_MAX,
			"unexpected value int32 (max)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element int32 (max)");

		// read int32 min
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first int32 (min)");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type int32 (min)");
		print_return_expr_fail(type == UJO_TYPE_INT32,
			"type not expected int32 (min)"); 
		err = ujo_element_get_int32(element, &int32val);
   		print_return_ujo_err(err,"ujo_element_get_int32 (min)");
		print_return_expr_fail(int32val == INT32_MIN, "unexpected value int32 (min)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element int32 (min)");

		// read int16 max
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first int16 (max)");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type");
		print_return_expr_fail(type == UJO_TYPE_INT16,
			"type not expected int16 (max)"); 
		err = ujo_element_get_int16(element, &int16val);
   		print_return_ujo_err(err,"ujo_element_get_int16 (max)");
		print_return_expr_fail(int16val == INT16_MAX,
			"unexpected value int16 (max)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element int16 (max)");

		// read int16 min
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first int16 (min)");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type int16 (min)");
		print_return_expr_fail(type == UJO_TYPE_INT16,
			"type not expected int16 (min)"); 
		err = ujo_element_get_int16(element, &int16val);
   		print_return_ujo_err(err,"ujo_element_get_int16 (min)");
		print_return_expr_fail(int16val == INT16_MIN, "unexpected value int16 (min)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element int16 (min)");

		// read int8 max
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first int8 (max)");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type");
		print_return_expr_fail(type == UJO_TYPE_INT8,
			"type not expected int8 (max)"); 
		err = ujo_element_get_int8(element, &int8val);
   		print_return_ujo_err(err,"ujo_element_get_int8 (max)");
		print_return_expr_fail(int8val == INT8_MAX,
			"unexpected value int8 (max)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element int8 (max)");

		// read int8 min
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first int8 (min)");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type int8 (min)");
		print_return_expr_fail(type == UJO_TYPE_INT8,
			"type not expected int8 (min)"); 
		err = ujo_element_get_int8(element, &int8val);
   		print_return_ujo_err(err,"ujo_element_get_int8 (min)");
		print_return_expr_fail(int8val == INT8_MIN, "unexpected value int8 (min)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element int8 (min)");

		// read uint64 max
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first uint64 (max)");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type");
		print_return_expr_fail(type == UJO_TYPE_UINT64,
			"type not expected uint64 (max)"); 
		err = ujo_element_get_uint64(element, &uint64val);
   		print_return_ujo_err(err,"ujo_element_get_uint64 (max)");
		print_return_expr_fail(uint64val == UINT64_MAX,
			"unexpected value uint64 (max)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element uint64 (max)");

		// read uint32 max
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first uint32 (max)");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type");
		print_return_expr_fail(type == UJO_TYPE_UINT32,
			"type not expected uint32 (max)"); 
		err = ujo_element_get_uint32(element, &uint32val);
   		print_return_ujo_err(err,"ujo_element_get_uint32 (max)");
		print_return_expr_fail(uint32val == UINT32_MAX,
			"unexpected value uint32 (max)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element uint32 (max)");

		// read uint16 max
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first uint16 (max)");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type");
		print_return_expr_fail(type == UJO_TYPE_UINT16,
			"type not expected uint16 (max)"); 
		err = ujo_element_get_uint16(element, &uint16val);
   		print_return_ujo_err(err,"ujo_element_get_uint16 (max)");
		print_return_expr_fail(uint16val == UINT16_MAX,
			"unexpected value uint16 (max)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element uint16 (max)");

		// read uint8 max
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first uint8 (max)");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type");
		print_return_expr_fail(type == UJO_TYPE_UINT8,
			"type not expected uint8 (max)"); 
		err = ujo_element_get_uint8(element, &uint8val);
   		print_return_ujo_err(err,"ujo_element_get_uint8 (max)");
		print_return_expr_fail(uint8val == UINT8_MAX,
			"unexpected value uint8 (max)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element uint8 (max)");

	// read the last element
	err = ujo_reader_get_next(ujor, &element, &eod);
	print_return_ujo_err(err,"ujo_reader_get_first (end)");
	err = ujo_element_get_type(element, &type);
	print_return_ujo_err(err,"ujo_element_get_type (end)");
	print_return_expr_fail(type == UJO_TERMINATOR,
		"type not expected terminator (end)"); 
	err = ujo_free_element(element);
	print_return_ujo_err(err,"ujo_free_element (end)");

	err = ujo_free_reader(ujor);
	print_return_ujo_err(err,"ujo_free_reader"); 

	err = ujo_free_writer(ujow);
	print_return_ujo_err(err,"ujo_free_writer"); 

	return ujoTrue;

}

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

#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include "ujo.h"

#define VERSION "0.9.1"
#define TEST_CSTR "Maik Wojcieszak@tml-software.com\n"

#define print_return_ujo_err(err, message) \
if (err != UJO_SUCCESS) {printf(#message":error %i\n", err); return ujoFalse;}

#define print_return_expr_fail(exp, message) \
if (!(exp)) {printf(#message"\n"); return ujoFalse;}

/**
 * print a buffer content to the console
 */
void print_buffer(unsigned char *buffer, uint64_t size) 
{
	uint64_t cur = 0;
	uint64_t i;

    while (cur < size)
	{
		printf("%04llx ", cur);
		for (i=cur; i<cur+16; i++)
		{
			if (i >= size) {
				printf("   ");
			} 
			else
			{
				printf("%02x ", buffer[i]);
			}
		}
		for (i=cur; i<cur+16; i++)
		{
			if (i >= size) break;
			if ((buffer[i] <= 127) && (buffer[i] >= 32)) { 
				printf("%c", buffer[i]);
			} else {
				printf(".");
			}
		}
		printf("\n");
		cur += 16;
	}

	printf("%lli bytes printed\n", size);
}

void bin_to_str(unsigned char *inbuffer, char* outbuffer, uint64_t size)
{
	uint64_t i;
	char conv[10];

	for (i=0; i<size; i++)
	{
		sprintf(conv, "%02x ", inbuffer[i]);
		memcpy(outbuffer+i*2, conv, 2);
	}
	outbuffer[size*2] = 0x00;
};
// ---------------------------- helper functions ---------------------------
size_t get_write_buffer_size(ujo_writer* w)
{
	ujoByte*		data;
	size_t		datasize;
	ujoError		err = UJO_SUCCESS;

	err = ujo_writer_get_buffer(w, &data, &datasize);
	if (err != UJO_SUCCESS) {
		printf("write_buffer_size:ujo_writer_get_buffer failed with %i\n", err);
		return 0;
	}
	return datasize;
};

uint8_t* get_pseudo_bin(uint32_t n)
{
	uint8_t* buffer = (uint8_t*)calloc(n,1);
	uint8_t  value = 0;
	uint32_t index;

	for (index = 0; index < n; index++) {
		buffer[index] = value;
		if (value == 255) 
			value = 0;
		else
			value += 1;
	}

	return buffer;

}
// --------------------------- begin tests ---------------------------------

/**
 * test01: library test
 */
ujoBool test01()
{
	ujo_writer *ujow;
	ujoError   err = UJO_SUCCESS;
	ujoByte    *data;
	size_t     datasize;
	ujo_element* element;
	ujoBool    eod;
	ujoTypeId   type;

	ujo_reader *ujor;

	err = ujo_new_writer(UJO_MEMORY, &ujow);
	print_return_ujo_err(err,"ujo_new_writer"); 

	
	// list of int

	err = ujo_writer_list_open(ujow);
	print_return_ujo_err(err,"ujo_writer_list_open"); 
 
	err = ujo_writer_add_int64(ujow, 12345678);
	print_return_ujo_err(err,"ujo_writer_add_int64");

	err = ujo_writer_add_float16(ujow, -9909.13414);
	print_return_ujo_err(err,"ujo_writer_add_float16");

	err = ujo_writer_add_float32(ujow, (float32_t)3.14);
	print_return_ujo_err(err,"ujo_writer_add_float32"); 

	err = ujo_writer_add_float64(ujow, 9.81);
	print_return_ujo_err(err,"ujo_writer_add_int64"); 
	
	    err = ujo_writer_map_open(ujow);
		print_return_ujo_err(err,"ujo_writer_list_open"); 
 
		// key
		err = ujo_writer_add_int64(ujow, 42);
		print_return_ujo_err(err,"ujo_writer_add_int64"); 

		// value
		err = ujo_writer_add_int16(ujow, 102);
		print_return_ujo_err(err,"ujo_writer_add_int64"); 

		// key 
		err = ujo_writer_add_int8(ujow, 21);
		print_return_ujo_err(err,"ujo_writer_add_int8"); 

		// value
		err = ujo_writer_list_open(ujow);
		print_return_ujo_err(err,"ujo_writer_list_open"); 

		err = ujo_writer_add_bool(ujow, ujoTrue);
		print_return_ujo_err(err,"ujo_writer_add_bool"); 

		err = ujo_writer_add_none(ujow);
		print_return_ujo_err(err,"ujo_writer_add_none"); 

		err = ujo_writer_list_close(ujow);
		print_return_ujo_err(err,"ujo_writer_list_close"); 

		err = ujo_writer_map_close(ujow);
		print_return_ujo_err(err,"ujo_writer_list_close"); 

	err = ujo_writer_list_close(ujow);
	print_return_ujo_err(err,"ujo_writer_list_close"); 

	/* get the buffer and print */
	err = ujo_writer_get_buffer(ujow, &data, &datasize);
	print_return_ujo_err(err,"ujo_writer_get_buffer"); 

	print_buffer(data, datasize);


	/* test the reader */
	err = ujo_new_reader(UJO_MEMORY, &ujor);
	print_return_ujo_err(err,"ujo_new_reader"); 

		err = ujo_reader_set_buffer(ujor, data, datasize);
		print_return_ujo_err(err,"ujo_reader_set_buffer"); 

		err = ujo_reader_get_first(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first");

		while (!eod)
		{
			err = ujo_element_get_type(element, &type);
			print_return_ujo_err(err,"ujo_element_get_type");

			switch(type)
			{
			case UJO_TERMINATOR:
				printf("-- container closed\n");
				break;
			case UJO_TYPE_INT64:
				printf("int64 found\n");
				break;
			case UJO_TYPE_INT32:
				printf("int32 found\n");
				break;
			case UJO_TYPE_INT16:
				printf("int16 found\n");
				break;
			case UJO_TYPE_INT8:
				printf("int8 found\n");
				break;
			case UJO_TYPE_NONE:
				printf("None found\n");
				break;
			case UJO_TYPE_FLOAT16:
				printf("float16 found\n");
				break;
			case UJO_TYPE_FLOAT32:
				printf("float32 found\n");
				break;
			case UJO_TYPE_FLOAT64:
				printf("float64 found\n");
				break;
			case UJO_TYPE_BOOL:
				printf("bool found\n");
				break;

			case UJO_TYPE_LIST:
				printf("-- list found\n");
				break;
			case UJO_TYPE_MAP:
				printf("-- map found\n");
				break;
			}

			err = ujo_free_element(element);
			print_return_ujo_err(err,"ujo_free_element");

			err = ujo_reader_get_next(ujor, &element, &eod);
			print_return_ujo_err(err,"ujo_reader_get_first");
		}
 
	err = ujo_free_reader(ujor);
	print_return_ujo_err(err,"ujo_free_reader"); 

	err = ujo_free_writer(ujow);
	print_return_ujo_err(err,"ujo_free_writer"); 

	return ujoTrue;
};

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

	err = ujo_new_writer(UJO_MEMORY, &ujow);
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
	err = ujo_new_reader(UJO_MEMORY, &ujor);
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

/**
 * test03: reading and writing date types
 */
ujoBool test03()
{
	ujo_writer*		ujow;
	ujo_reader*		ujor;
	ujo_element*	element;

	ujoError		err = UJO_SUCCESS;

	ujoByte*		data;
	size_t		datasize;
	ujoBool			eod;
	ujoTypeId		type;

	int64_t         uxtime;
	ujoDateTime	    dt;

	err = ujo_new_writer(UJO_MEMORY, &ujow);
	print_return_ujo_err(err,"ujo_new_writer"); 

	
	// create a list as top level container
	err = ujo_writer_list_open(ujow);
	print_return_ujo_err(err,"ujo_writer_list_open"); 

	    // uxtime
		datasize = get_write_buffer_size(ujow);
		err = ujo_writer_add_uxtime(ujow, INT64_MAX);
		print_return_ujo_err(err,"ujo_writer_add_uxtime"); 
		print_return_expr_fail(datasize+9 == get_write_buffer_size(ujow),
			"atomic type size unexpected (uxtime)");

	    // date
		dt.year = 1971;
		dt.month = 8;
		dt.day   = 23;

		datasize = get_write_buffer_size(ujow);
		err = ujo_writer_add_date(ujow, dt);
		print_return_ujo_err(err,"ujo_writer_add_date"); 
		print_return_expr_fail(datasize+5 == get_write_buffer_size(ujow),
			"atomic type size unexpected (date)");

	    // time
		dt.hour    = 23;
		dt.minute  = 45;
		dt.second  = 12;

		datasize = get_write_buffer_size(ujow);
		err = ujo_writer_add_time(ujow, dt);
		print_return_ujo_err(err,"ujo_writer_add_time"); 
		print_return_expr_fail(datasize+4 == get_write_buffer_size(ujow),
			"atomic type size unexpected (time)");

	    // timestamp
		dt.year = 2013;
		dt.month = 7;
		dt.day   = 24;

		dt.hour    = 16;
		dt.minute  = 12;
		dt.second  = 40;

		dt.millisecond = 340;

		datasize = get_write_buffer_size(ujow);
		err = ujo_writer_add_timestamp(ujow, dt);
		print_return_ujo_err(err,"ujo_writer_add_timestamp"); 
		print_return_expr_fail(datasize+10 == get_write_buffer_size(ujow),
			"atomic type size unexpected (timestamp)");

	err = ujo_writer_list_close(ujow);
	print_return_ujo_err(err,"ujo_writer_list_close"); 

	/* get the buffer and print */
	err = ujo_writer_get_buffer(ujow, &data, &datasize);
	print_return_ujo_err(err,"ujo_writer_get_buffer"); 

	print_buffer(data, datasize);

	/* ---> test the reader */
	err = ujo_new_reader(UJO_MEMORY, &ujor);
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

		// read uxtime
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first uxtime");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type");
		print_return_expr_fail(type == UJO_TYPE_UX_TIME,
			"type not expected uxtime"); 
		err = ujo_element_get_uxtime(element, &uxtime);
   		print_return_ujo_err(err,"ujo_element_get_uxtime");
		print_return_expr_fail(uxtime == INT64_MAX,
			"unexpected value int64 (max)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element uxtime");

		// read date
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first date");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type");
		print_return_expr_fail(type == UJO_TYPE_DATE,
			"type not expected date"); 
		err = ujo_element_get_date(element, &dt);
   		print_return_ujo_err(err,"ujo_element_get_date");
		print_return_expr_fail(dt.year == 1971,
			"unexpected value date (year)"); 
		print_return_expr_fail(dt.month == 8,
			"unexpected value date (month)"); 
		print_return_expr_fail(dt.day == 23,
			"unexpected value date (day)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element uxtime");

		// read time
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first date");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type");
		print_return_expr_fail(type == UJO_TYPE_TIME,
			"type not expected time"); 
		err = ujo_element_get_time(element, &dt);
   		print_return_ujo_err(err,"ujo_element_get_time");
		print_return_expr_fail(dt.hour == 23,
			"unexpected value time (hour)"); 
		print_return_expr_fail(dt.minute == 45,
			"unexpected value time (minute)"); 
		print_return_expr_fail(dt.second == 12,
			"unexpected value time (second)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element uxtime");

		// read timestamp
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first timestamp");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type");
		print_return_expr_fail(type == UJO_TYPE_TIMESTAMP,
			"type not expected timestamp"); 
		err = ujo_element_get_timestamp(element, &dt);
   		print_return_ujo_err(err,"ujo_element_get_timestamp");

		print_return_expr_fail(dt.year == 2013,
			"unexpected value timestamp (year)"); 
		print_return_expr_fail(dt.month == 7,
			"unexpected value timestamp (month)"); 
		print_return_expr_fail(dt.day == 24,
			"unexpected value timestamp (day)"); 
		print_return_expr_fail(dt.hour == 16,
			"unexpected value timestamp (hour)"); 
		print_return_expr_fail(dt.minute == 12,
			"unexpected value timestamp (minute)"); 
		print_return_expr_fail(dt.second == 40,
			"unexpected value timestamp (second)"); 
		print_return_expr_fail(dt.millisecond == 340,
			"unexpected value timestamp (second)"); 
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element uxtime");

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

/**
 * test02: basic string type access
 */
ujoBool test04()
{
	ujo_writer*		ujow;
	ujo_reader*		ujor;
	ujo_element*	element;

	ujoError		err = UJO_SUCCESS;

	ujoByte*		data;
	size_t		datasize;
	ujoBool			eod;
	ujoTypeId		type;

	char*           cs = "Maik Wojcieszak@tml-software.com";
	char*           strval;
	uint32_t        strsize;

	err = ujo_new_writer(UJO_MEMORY, &ujow);
	print_return_ujo_err(err,"ujo_new_writer"); 

	
	// create a list as top level container
	err = ujo_writer_list_open(ujow);
	print_return_ujo_err(err,"ujo_writer_list_open"); 

	    // c_string
		datasize = get_write_buffer_size(ujow);
		err = ujo_writer_add_string_c(ujow, TEST_CSTR);
		print_return_ujo_err(err,"ujo_writer_add_string_c"); 

	err = ujo_writer_list_close(ujow);
	print_return_ujo_err(err,"ujo_writer_list_close"); 

	/* get the buffer and print */
	err = ujo_writer_get_buffer(ujow, &data, &datasize);
	print_return_ujo_err(err,"ujo_writer_get_buffer"); 

	print_buffer(data, datasize);

	/* ---> test the reader */
	err = ujo_new_reader(UJO_MEMORY, &ujor);
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

		// read c string
		err = ujo_reader_get_next(ujor, &element, &eod);
		print_return_ujo_err(err,"ujo_reader_get_first string_c");
		err = ujo_element_get_type(element, &type);
   		print_return_ujo_err(err,"ujo_element_get_type string_c");
		print_return_expr_fail(type == UJO_TYPE_STRING,
			"type not expected string_c"); 
		
		err = ujo_element_get_string_c(element, &strval, &strsize);
   		print_return_ujo_err(err,"ujo_element_get_string_c");
		print_return_expr_fail(strcmp(TEST_CSTR, strval) == 0, "unexpected value string_c");
		printf("%s\n",strval);
		err = ujo_free_element(element);
		print_return_ujo_err(err,"ujo_free_element int64 string_c");

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

/**
 * myOnElement: Callback method for ujo_reader_set_on_element
 */
ujoError myOnElement (ujo_element *element, ujoPointer data){
  ujoBool    bVal;
  int64_t i64Val;
  int32_t i32Val;
  int16_t i16Val;
  int8_t   i8Val;
  float64_t f64Val;
  float32_t f32Val;
  char*     cstrVal;
  uint8_t*  bindata;
  uint32_t  n;
  ujoTypeId type;
  ujoTypeId stype;

	ujoError err = ujo_element_get_type(element, &type);
	print_return_ujo_err(err,"ujo_element_get_type");

	switch(type)
	{
			case UJO_TERMINATOR:
				printf("-- container closed\n");
				break;
			case UJO_TYPE_INT64:
        i64Val = -1;
			  err = ujo_element_get_int64(element, &i64Val);
			  print_return_ujo_err(err,"ujo_element_get_int64");
        printf("int64 \"%lld\"found\n", i64Val);
				break;
			case UJO_TYPE_INT32:
        i32Val = -1;
			  err = ujo_element_get_int32(element, &i32Val);
			  print_return_ujo_err(err,"ujo_element_get_int32");
        printf("int32 \"%d\"found\n", i32Val);
				break;
			case UJO_TYPE_INT16:
        i16Val = -1;
			  err = ujo_element_get_int16(element, &i16Val);
			  print_return_ujo_err(err,"ujo_element_get_int16");
        printf("int16 \"%hd\"found\n", i16Val);
				break;
			case UJO_TYPE_INT8:
        i8Val = -1;
			  err = ujo_element_get_int8(element, &i8Val);
			  print_return_ujo_err(err,"ujo_element_get_int8");
        printf("int8 \"%d\"found\n", i8Val);
				break;
			case UJO_TYPE_NONE:
				printf("None found\n");
				break;
			case UJO_TYPE_FLOAT32:
        f32Val = -1.0;
			  err = ujo_element_get_float32(element, &f32Val);
			  print_return_ujo_err(err,"ujo_element_get_float32");
        printf("float32 \"%lf\"found\n", f32Val);
				break;
			case UJO_TYPE_FLOAT64:
        f64Val = -1.0;
			  err = ujo_element_get_float64(element, &f64Val);
			  print_return_ujo_err(err,"ujo_element_get_float64");
        printf("float64 \"%lf\"found\n", f64Val);
				break;
			case UJO_TYPE_BOOL:
        bVal = ujoFalse;
			  err = ujo_element_get_bool(element, &bVal);
			  print_return_ujo_err(err,"ujo_element_get_bool");
        printf("bool \"%d\"found\n", bVal == ujoTrue);
				break;
			case UJO_TYPE_STRING:
			  err = ujo_element_get_string_type(element, &stype);
			  print_return_ujo_err(err,"ujo_element_get_string_type");
			  if (stype == UJO_SUB_STRING_C)
			  {	
				err = ujo_element_get_string_c(element, &cstrVal, &n);
				print_return_ujo_err(err,"ujo_element_get_string_c");
				printf("string \"%s\"found\n", cstrVal);
			  } else
				printf("string found\n");
			  break;

			case UJO_TYPE_BIN:
			  err = ujo_element_get_binary(element, &stype, &bindata, &n);
			  print_return_ujo_err(err,"ujo_element_get_binary");
  			  printf("binary with \"%d\" bytes found\n", n);
			  break;

			case UJO_TYPE_LIST:
        printf("List found\n");
				break;
			case UJO_TYPE_MAP:
        printf("Map found\n");
				break;
			case UJO_TYPE_TABLE:
        printf("Table found\n");
				break;
	}
  return UJO_SUCCESS;
};

/**
 * test04: test parse function with callback
 */
ujoBool test05()
{
	ujo_writer *ujow;
	ujoError   err = UJO_SUCCESS;
	ujoByte    *data;
	size_t   datasize;

	ujo_reader *ujor;
  int      tab;
  
  tab = 0;

	err = ujo_new_writer(UJO_MEMORY, &ujow);
	print_return_ujo_err(err,"ujo_new_writer"); 

	
	// list of int

	err = ujo_writer_list_open(ujow);
	print_return_ujo_err(err,"ujo_writer_list_open"); 
 
	err = ujo_writer_add_int64(ujow, 12345678);
	print_return_ujo_err(err,"ujo_writer_add_int64"); 

	err = ujo_writer_add_float32(ujow, (float32_t) 3.14);
	print_return_ujo_err(err,"ujo_writer_add_float32"); 

	err = ujo_writer_add_float64(ujow, 9.81);
	print_return_ujo_err(err,"ujo_writer_add_int64"); 
	
	  err = ujo_writer_map_open(ujow);
		print_return_ujo_err(err,"ujo_writer_list_open"); 
 
		// key
		err = ujo_writer_add_int64(ujow, 42);
		print_return_ujo_err(err,"ujo_writer_add_int64"); 

		// value
		err = ujo_writer_add_int16(ujow, 102);
		print_return_ujo_err(err,"ujo_writer_add_int64"); 

		// key 
		err = ujo_writer_add_int8(ujow, 21);
		print_return_ujo_err(err,"ujo_writer_add_int8"); 

		// value
		err = ujo_writer_list_open(ujow);
		print_return_ujo_err(err,"ujo_writer_list_open"); 

		err = ujo_writer_add_bool(ujow, ujoTrue);
		print_return_ujo_err(err,"ujo_writer_add_bool"); 

		err = ujo_writer_add_none(ujow);
		print_return_ujo_err(err,"ujo_writer_add_none"); 

		err = ujo_writer_list_close(ujow);
		print_return_ujo_err(err,"ujo_writer_list_close"); 

		err = ujo_writer_map_close(ujow);
		print_return_ujo_err(err,"ujo_writer_list_close"); 

	err = ujo_writer_list_close(ujow);
	print_return_ujo_err(err,"ujo_writer_list_close"); 

	/* get the buffer and print */
	err = ujo_writer_get_buffer(ujow, &data, &datasize);
	print_return_ujo_err(err,"ujo_writer_get_buffer"); 

	print_buffer(data, datasize);


	/* test the reader */
	err = ujo_new_reader(UJO_MEMORY, &ujor);
	print_return_ujo_err(err,"ujo_new_reader"); 

    err = ujo_reader_set_buffer(ujor, data, datasize);
	print_return_ujo_err(err,"ujo_reader_set_buffer"); 

    err = ujo_reader_set_on_element(ujor, myOnElement, ujor);
	print_return_ujo_err(err,"ujo_reader_set_on_element"); 

    err = ujo_reader_parse(ujor);
	print_return_ujo_err(err,"ujo_reader_parse"); 

 	err = ujo_free_reader(ujor);
	print_return_ujo_err(err,"ujo_free_reader"); 

	err = ujo_free_writer(ujow);
	print_return_ujo_err(err,"ujo_free_writer"); 

	return ujoTrue;
};

/**
 * test06: basic table test
 */
ujoBool test06()
{
	ujo_writer *ujow;
	ujoError   err = UJO_SUCCESS;
	ujoByte    *data;
	size_t   datasize;

	ujo_reader *ujor;

	err = ujo_new_writer(UJO_MEMORY, &ujow);
	print_return_ujo_err(err,"ujo_new_writer"); 

	
	// list of int

	err = ujo_writer_table_open(ujow);
	print_return_ujo_err(err,"ujo_writer_table_open"); 

	err = ujo_writer_add_string_c(ujow, "Name");
	print_return_ujo_err(err,"ujo_writer_add_string_c"); 

	err = ujo_writer_add_string_c(ujow, "Age");
	print_return_ujo_err(err,"ujo_writer_add_string_c"); 

	err = ujo_writer_add_string_c(ujow, "Phone");
	print_return_ujo_err(err,"ujo_writer_add_string_c"); 

	err = ujo_writer_table_end_columns(ujow);
	print_return_ujo_err(err,"ujo_writer_table_end_columns"); 

	// add firsst row
	err = ujo_writer_add_string_c(ujow,"Maik");
	print_return_ujo_err(err,"ujo_writer_add_string_c"); 

	err = ujo_writer_add_int8(ujow,23);
	print_return_ujo_err(err,"ujo_writer_add_int8"); 

	err = ujo_writer_add_string_c(ujow,"+49 0431 1234");
	print_return_ujo_err(err,"ujo_writer_add_string_c"); 

	// add second row
	err = ujo_writer_add_string_c(ujow,"Stefan");
	print_return_ujo_err(err,"ujo_writer_add_string_c"); 

	err = ujo_writer_add_int8(ujow,36);
	print_return_ujo_err(err,"ujo_writer_add_int8"); 


	// add a list of phone numbers
	err = ujo_writer_list_open(ujow);
	print_return_ujo_err(err,"ujo_writer_list_open"); 
 
	err = ujo_writer_add_string_c(ujow,"+49 0431 1234");
	print_return_ujo_err(err,"ujo_writer_add_string_c"); 

	err = ujo_writer_add_string_c(ujow,"+49 127 87917");
	print_return_ujo_err(err,"ujo_writer_add_string_c"); 

	err = ujo_writer_add_string_c(ujow,"+49 3456789");
	print_return_ujo_err(err,"ujo_writer_add_string_c"); 

	err = ujo_writer_list_close(ujow);
	print_return_ujo_err(err,"ujo_writer_list_close"); 

	// close table and document
	err = ujo_writer_table_close(ujow);
	print_return_ujo_err(err,"ujo_writer_table_close"); 

	/* get the buffer and print */
	err = ujo_writer_get_buffer(ujow, &data, &datasize);
	print_return_ujo_err(err,"ujo_writer_get_buffer"); 

	print_buffer(data, datasize);

	/* test the reader */
	err = ujo_new_reader(UJO_MEMORY, &ujor);
	print_return_ujo_err(err,"ujo_new_reader"); 

    err = ujo_reader_set_buffer(ujor, data, datasize);
	print_return_ujo_err(err,"ujo_reader_set_buffer"); 

    err = ujo_reader_set_on_element(ujor, myOnElement, ujor);
	print_return_ujo_err(err,"ujo_reader_set_on_element"); 

    err = ujo_reader_parse(ujor);
	print_return_ujo_err(err,"ujo_reader_parse"); 

 	err = ujo_free_reader(ujor);
	print_return_ujo_err(err,"ujo_free_reader"); 

	err = ujo_free_writer(ujow);
	print_return_ujo_err(err,"ujo_free_writer"); 

	return ujoTrue;
};

/**
 * test07: basic binary test
 */
ujoBool test07()
{
	ujo_writer *ujow;
	ujoError   err = UJO_SUCCESS;
	ujoByte    *data;
	size_t   datasize;

	uint8_t* bindata;

	ujo_reader *ujor;

	err = ujo_new_writer(UJO_MEMORY, &ujow);
	print_return_ujo_err(err,"ujo_new_writer"); 
	

	err = ujo_writer_list_open(ujow);
	print_return_ujo_err(err,"ujo_writer_list_open"); 

	// list of int
	bindata = get_pseudo_bin(1024);

	err = ujo_writer_add_binary(ujow, UJO_SUB_BINARY_GENERIC, bindata, 1024);
	print_return_ujo_err(err,"ujo_writer_add_binary"); 

	free(bindata);


	// close table and document
	err = ujo_writer_list_close(ujow);
	print_return_ujo_err(err,"ujo_writer_table_close"); 

	/* get the buffer and print */
	err = ujo_writer_get_buffer(ujow, &data, &datasize);
	print_return_ujo_err(err,"ujo_writer_get_buffer"); 

	print_buffer(data, datasize);

	/* test the reader */
	err = ujo_new_reader(UJO_MEMORY, &ujor);
	print_return_ujo_err(err,"ujo_new_reader"); 

    err = ujo_reader_set_buffer(ujor, data, datasize);
	print_return_ujo_err(err,"ujo_reader_set_buffer"); 

    err = ujo_reader_set_on_element(ujor, myOnElement, ujor);
	print_return_ujo_err(err,"ujo_reader_set_on_element"); 

    err = ujo_reader_parse(ujor);
	print_return_ujo_err(err,"ujo_reader_parse"); 

 	err = ujo_free_reader(ujor);
	print_return_ujo_err(err,"ujo_free_reader"); 

	err = ujo_free_writer(ujow);
	print_return_ujo_err(err,"ujo_free_writer"); 

	return ujoTrue;
};

/**
 * test08: format integrity
 */
ujoBool test08()
{
	ujo_writer *ujow;
	ujoError   err = UJO_SUCCESS;
	ujoByte    *data;
	size_t   datasize;

	char* binstring;

	// float integrity --------------------------------------------------------
	err = ujo_new_writer(UJO_MEMORY, &ujow);
	print_return_ujo_err(err,"ujo_new_writer"); 
	
	err = ujo_writer_list_open(ujow);
	print_return_ujo_err(err,"ujo_writer_list_open"); 

	err = ujo_writer_add_float32(ujow, 3.14);
	print_return_ujo_err(err,"ujo_writer_add_float32"); 

	err = ujo_writer_add_float64(ujow, 3.14);
	print_return_ujo_err(err,"ujo_writer_add_float64"); 

	err = ujo_writer_list_close(ujow);
	print_return_ujo_err(err,"ujo_writer_table_close"); 


	err = ujo_writer_get_buffer(ujow, &data, &datasize);
	print_return_ujo_err(err,"ujo_writer_get_buffer"); 

	print_buffer(data, datasize);

	binstring = (char*)calloc(datasize*2+1, 1);
	bin_to_str(data, binstring, datasize);

	printf("%s\n", binstring);
	print_return_expr_fail(strcmp("5f554a4f0100003002c3f54840011f85eb51b81e094000",binstring) == 0,"float integrity failed");
	free(binstring);

	err = ujo_free_writer(ujow);
	print_return_ujo_err(err,"ujo_free_writer"); 

	// int integrity -------------------------------------------------------------
	err = ujo_new_writer(UJO_MEMORY, &ujow);
	print_return_ujo_err(err,"ujo_new_writer"); 
	
	err = ujo_writer_list_open(ujow);
	print_return_ujo_err(err,"ujo_writer_list_open"); 

	err = ujo_writer_add_int8(ujow, 130);
	print_return_ujo_err(err,"ujo_writer_add_int8"); 

	err = ujo_writer_add_int16(ujow, 1024);
	print_return_ujo_err(err,"ujo_writer_add_int16"); 

	err = ujo_writer_add_int32(ujow, 12345678);
	print_return_ujo_err(err,"ujo_writer_add_int32"); 

	err = ujo_writer_add_int64(ujow, 12345678);
	print_return_ujo_err(err,"ujo_writer_add_int64"); 

	err = ujo_writer_list_close(ujow);
	print_return_ujo_err(err,"ujo_writer_table_close"); 

	err = ujo_writer_get_buffer(ujow, &data, &datasize);
	print_return_ujo_err(err,"ujo_writer_get_buffer"); 

	print_buffer(data, datasize);

	binstring = (char*)calloc(datasize*2+1, 1);
	bin_to_str(data, binstring, datasize);

	printf("%s\n", binstring);
	print_return_expr_fail(strcmp("5f554a4f010000300882070004064e61bc00054e61bc000000000000",binstring) == 0,"int integrity failed");
	free(binstring);

	err = ujo_free_writer(ujow);
	print_return_ujo_err(err,"ujo_free_writer"); 

	return ujoTrue;
};

/**
 * Run a test and return success or failure.
 */
ujoBool run_test(int no)
{
	switch(no) {
	case 1: 
		if (test01()) {
			printf ("Test 01: library test [   OK   ]\n");
		}else {
			printf ("Test 01: library test [ FAILED ]\n");
			return ujoFalse;
		}; break;
		
	case 2: 
		if (test02()) {
			printf ("Test 02: reading and writing integer types [   OK   ]\n");
		}else {
			printf ("Test 02: reading and writing integer types [ FAILED ]\n");
			return ujoFalse;
		}; break;
	case 3: 
		if (test03()) {
			printf ("Test 03: reading and writing date/time types [   OK   ]\n");
		}else {
			printf ("Test 03: reading and writing date/time types [ FAILED ]\n");
			return ujoFalse;
		}; break;
	case 4: 
		if (test04()) {
			printf ("Test 04: basic string type access [   OK   ]\n");
		}else {
			printf ("Test 04: basic string type access [ FAILED ]\n");
			return ujoFalse;
		}; break;
	case 5: 
		if (test05()) {
			printf ("Test 05: test parse function with callback [   OK   ]\n");
		}else {
			printf ("Test 05: test parse function with callback [ FAILED ]\n");
			return ujoFalse;
		}; break;
	case 6: 
		if (test06()) {
			printf ("Test 06: basic table test [   OK   ]\n");
		}else {
			printf ("Test 06: basic table test [ FAILED ]\n");
			return ujoFalse;
		}; break;
	case 7: 
		if (test07()) {
			printf ("Test 07: basic binary test [   OK   ]\n");
		}else {
			printf ("Test 07: basic binary test [ FAILED ]\n");
			return ujoFalse;
		}; break;
	case 8: 
		if (test08()) {
			printf ("Test 08: format integrity [   OK   ]\n");
		}else {
			printf ("Test 08: format integrity [ FAILED ]\n");
			return ujoFalse;
		}; break;
	default:
		printf ("Test with no %d not found! [ FAILED ]\n", no);
		return ujoFalse;
	}
	return ujoTrue;
};

// --------------------------- end tests ---------------------------------

/**
 * Main function.
 */
int main(int argc, char **argv)
{
	int      testno;

	printf ("** LibUJO: UJO Data Object Notation (regression test).\n");
	printf ("** Copyright (C) 2015 wobe-systems GmbH\n**\n");
	printf ("** UJO regression tests: version=%s\n**\n",
		VERSION);
	printf ("** To gather information about time performance you can use:\n**\n");
	printf ("**     >> time ./testujo\n**\n");
	printf ("** To gather information about memory consumed (and leaks) use:\n**\n");
	printf ("**     >> libtool --mode=execute valgrind --leak-check=yes --error-limit=no ./testujo\n**\n");

	if (argc == 2) {
		if(sscanf(argv[1], "%d", &testno) == EOF) {
		fprintf(stderr, "WARNING: Incorrect value for device\n");
		return -1;
		}
		if (!run_test(testno)) {
			return -1;
		}
	}
	else {
		for (testno = 1; testno <= 8; testno++)
		{
			if (!run_test(testno)) {
			return -1;
			}
		}
	}
}


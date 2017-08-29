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

#include "testujo_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

};

/**
 * myOnElement: Callback method for ujo_reader_set_on_element
 */
ujoError myOnElement (ujo_element *element, /*@unused@*/ ujoPointer data){
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
            case UJO_TYPE_FLOAT16:
        f32Val = -1.0;
              err = ujo_element_get_float16(element, &f32Val);
              print_return_ujo_err(err,"ujo_element_get_float16");
        printf("float16 \"%lf\"found\n", f32Val);
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


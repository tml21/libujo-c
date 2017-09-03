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
 * basic string type access
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

	err = ujo_new_memory_writer(&ujow);
	print_return_ujo_err(err,"ujo_new_writer"); 

	
	// create a list as top level container
	err = ujo_writer_list_open(ujow);
	print_return_ujo_err(err,"ujo_writer_list_open"); 

	    // c_string
		datasize = get_write_buffer_size(ujow);
		err = ujo_writer_add_string_c(ujow, TEST_CSTR, sizeof(TEST_CSTR));
		print_return_ujo_err(err,"ujo_writer_add_string_c"); 

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
};

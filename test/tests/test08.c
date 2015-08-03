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
#include <stdlib.h>

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
	err = ujo_new_memory_writer(&ujow);
	print_return_ujo_err(err,"ujo_new_memory_writer"); 
	
	err = ujo_writer_list_open(ujow);
	print_return_ujo_err(err,"ujo_writer_list_open"); 

	err = ujo_writer_add_float16(ujow, 4.13);
	print_return_ujo_err(err,"ujo_writer_add_float16");

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
	print_return_expr_fail(strcmp("5f554a4f0100003003214402c3f54840011f85eb51b81e094000",binstring) == 0,"float integrity failed");
	free(binstring);

	err = ujo_free_writer(ujow);
	print_return_ujo_err(err,"ujo_free_writer"); 

	// int integrity -------------------------------------------------------------
	err = ujo_new_memory_writer(&ujow);
	print_return_ujo_err(err,"ujo_new_memory_writer"); 
	
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
	print_return_ujo_err(err,"ujo_writer_list_close"); 

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

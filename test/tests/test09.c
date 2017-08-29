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
 * test09: test file access
 */
ujoBool test09()
{
	ujo_writer *ujow;
	ujo_reader *ujor;

	ujoError   err = UJO_SUCCESS;
	
	// writer
	err = ujo_new_file_writer(&ujow, "./test09.ujo");
	print_return_ujo_err(err,"ujo_new_memory_writer"); 
	
	err = ujo_writer_list_open(ujow);
	print_return_ujo_err(err,"ujo_writer_list_open"); 

	err = ujo_writer_add_float16(ujow, (float32_t)24.135);
	print_return_ujo_err(err,"ujo_writer_add_float16");

	err = ujo_writer_add_float32(ujow, (float32_t)3.14);
	print_return_ujo_err(err,"ujo_writer_add_float32"); 

	err = ujo_writer_add_float64(ujow, 3.14);
	print_return_ujo_err(err,"ujo_writer_add_float64"); 

	err = ujo_writer_list_close(ujow);
	print_return_ujo_err(err,"ujo_writer_list_close"); 

	err = ujo_free_writer(ujow);
	print_return_ujo_err(err,"ujo_free_writer"); 

	// reader
	err = ujo_new_file_reader(&ujor, "./test09.ujo");
	print_return_ujo_err(err,"ujo_new_file_reader"); 

    err = ujo_reader_set_on_element(ujor, myOnElement, ujor);
	print_return_ujo_err(err,"ujo_reader_set_on_element"); 

    err = ujo_reader_parse(ujor);
	print_return_ujo_err(err,"ujo_reader_parse"); 

	err = ujo_free_reader(ujor);
	print_return_ujo_err(err,"ujo_free_reader"); 



	return ujoTrue;
};

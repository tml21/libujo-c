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
 * basic binary test
 */
ujoBool test07()
{
	ujo_writer *ujow;
	ujoError   err = UJO_SUCCESS;
	ujoByte    *data;
	size_t   datasize;

	uint8_t* bindata;

	ujo_reader *ujor;

	err = ujo_new_memory_writer(&ujow);
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
	err = ujo_new_memory_reader(&ujor);
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


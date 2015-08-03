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

	err = ujo_new_memory_writer(&ujow);
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
};

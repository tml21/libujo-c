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

#ifndef __TESTUJO_HELPER_H__
#define __TESTUJO_HELPER_H__

#include "ujo.h"
#include <string.h>

#define VERSION "0.9.1"
#define TEST_CSTR "Maik Wojcieszak@tml-software.com\n"

#define print_return_ujo_err(err, message) \
if (err != UJO_SUCCESS) {printf(#message":error %i\n", err); return ujoFalse;}

#define print_return_expr_fail(exp, message) \
if (!(exp)) {printf(#message"\n"); return ujoFalse;}

/**
 * print a buffer content to the console
 */
void print_buffer(ujoByte* buffer, uint64_t size);

ujoError bin_to_str(ujoByte* inbuffer, char* outbuffer, uint64_t size);

size_t get_write_buffer_size(ujo_writer* w);

uint8_t* get_pseudo_bin(uint32_t n);

/**
 * myOnElement: Callback method for ujo_reader_set_on_element
 */
ujoError myOnElement (ujo_element *element, ujoPointer data);

#endif
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

#ifndef __UJO_TESTS_H__
#define __UJO_TESTS_H__

#include "ujo.h"

/**
 * test01: library test
 */
ujoBool test01();

/**
 * test02: reading and writing integer types
 */
ujoBool test02();

/**
 * test03: reading and writing date types
 */
ujoBool test03();

/**
 * basic string type access
 */
ujoBool test04();

/**
 * test parse function with callback
 */
ujoBool test05();

/**
 * basic table test
 */
ujoBool test06();

/**
 * basic binary test
 */
ujoBool test07();

/**
 * test08: format integrity
 */
ujoBool test08();

/**
 * test09: test file access
 */
ujoBool test09();

#endif
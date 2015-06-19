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

/* global includes */
#include "ujo.h"
#include "ujo_constants.h"


/**
 * \defgroup ujo_module General library functions.
 *
 * This module contains library function to get or set general information about
 * the library as well as initialization and deinitialization methods.
 */

/**
 * \defgroup ujo_writer UJO Writer: Write ujo data.
 *
 * The writer object creates a new document and writes the data to destination 
 * depending on the writer type.
 */

/**
 * \defgroup ujo_reader UJO Reader: Traverse UJO data.
 *
 * The reader object object is used to traverse UJO data.
 */

/**
 * \defgroup ujo_reader_callbacks UJO Reader: callbacks
 * \ingroup ujo_reader
 *
 * The reader object object is used to traverse UJO data.
 */

/**
 * \defgroup ujo_element UJO Element: access UJO data.
 * 
 * An UJO element is a value or a marker in an UJO document.
 */

/**
 * \defgroup ujo_element_types Identifier values of element types.
 * \ingroup ujo_element
 */


/**
 * @brief Get library version.
 *
 * The library version and API version are returned.
 * 
 * @param libversion library version reference
 * @param apiversion API version reference
 */
void ujo_get_version(uint32_t* libversion, uint32_t* apiversion){
	*libversion = UJO_VERSION;
	*apiversion = UJO_API_VERSION;
};


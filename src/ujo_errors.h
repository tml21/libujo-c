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

#ifndef __UJO_ERRORS_H__
#define __UJO_ERRORS_H__

// ujo error definitions
#define UJO_SUCCESS                     0       // operation successful
#define UJO_ERR_UNKNOWN                 5501    // unknown error
#define UJO_ERR_ALLOCATION              5502    // memory allocation failed
#define UJO_ERR_INVALID_DATA            5503    // a parameter is invalid
#define UJO_ERR_TYPE_MISPLACED          5504    // a type is not allowed in a specifc location
#define UJO_ERR_INVALID_OBJECT          5505    // an operation can only be performed on specific objects
#define UJO_ERR_NOT_IMPLEMENTED         5506    // the feature is not yet implemented


#define report_error(expr,message,ecode) \
	if (!(expr)) {UJO_LOG(#message":expression failed %s: error (%d)", #expr, ecode); return ecode;}

/** 
 * @brief Allows to check a condition and return if it is not meet.
 * 
 * @param expr The expresion to check.
 */
#define return_if_fail(expr, message) \
if (!(expr)) {UJO_LOG(#message":expression failed %s", expr); return;}

/** 
 * @brief Allows to check a condition and return the given value if it
 * is not meet.
 * 
 * @param expr The expresion to check.
 * @param val The value to return if the expression is not meet.
 */
#define return_val_if_fail(expr, message, val) \
if (!(expr)) {UJO_LOG(#message":expression failed %s", expr); return val;}


#endif
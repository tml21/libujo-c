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

#ifndef __UJO_DECL_H__
#define __UJO_DECL_H__

#include "ujo_int.h"


/** 
 * @internal
 *
 * C++ support declarations borrowed from the libtool webpage. Thanks
 * you guys for this information. 
 *
 * BEGIN_C_DECLS should be used at the beginning of your declarations,
 * so that C++ compilers don't mangle their names.  Use END_C_DECLS at
 * the end of C declarations.
 */
#undef BEGIN_C_DECLS
#undef END_C_DECLS
#ifdef __cplusplus
# define BEGIN_C_DECLS extern "C" {
# define END_C_DECLS }
#else
# define BEGIN_C_DECLS /* empty */
# define END_C_DECLS /* empty */
#endif

// Check windows
#if _WIN32 || _WIN64
   #if _WIN64
     #define ENV64BIT
  #else
    #define ENV32BIT
  #endif
#endif

// Check GCC
#if __GNUC__
  #if __x86_64__ || __ppc64__
    #define ENV64BIT
  #else
    #define ENV32BIT
  #endif
#endif

/**
 * @brief object handle type
 *
 * Handles are defined as integer not as pointer to simplify usage of wrapper generators.
 */
#if defined(ENV32BIT)
	// 32-bit pointer
	typedef uint32_t ujoHandle;
#elif defined(ENV64BIT)
	// 64-bit pointer
	typedef uint64_t ujoHandle;
#else
	#error "Must define either ENV32BIT or ENV64BIT"
#endif

/** 
 * @brief Pointer to any structure definition. It should be required
 * to use this definition, however, some platforms doesn't support the
 * <b>void *</b> making it necessary to use the <b>char *</b>
 * definition as a general way to represent references.
 */
typedef void * ujoPointer;

#define UJO_NULL_POINTER 0

BEGIN_C_DECLS

	ujoPointer  ujo_calloc(size_t count, size_t size);

	ujoPointer  ujo_realloc(ujoPointer ref, size_t size);

	void        ujo_free(ujoPointer ref);

END_C_DECLS


#endif

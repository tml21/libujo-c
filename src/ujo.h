/* 
 *  LibUjo:  An UJO binaray data object notation library.
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

#ifndef __UJO_H__
#define __UJO_H__

#include "ujo_decl.h"
#include "ujo_errors.h"
#include "ujo_writer.h"
#include "ujo_reader.h"
#include "ujo_constants.h"

BEGIN_C_DECLS

/** 
 * \addtogroup ujo_module
 * @{
 */

 void ujo_get_version(uint32_t* libversion, uint32_t* apiversion);

/* @} */

END_C_DECLS

#endif

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

#ifndef __UJO_STATE_H__
#define __UJO_STATE_H__

#include "ujo_decl.h"
#include "ujo_stack.h"

/* UJO writer states */
typedef enum {
	STATE_ROOT,
	STATE_LIST,
	STATE_DICT_KEY,
	STATE_DICT_VALUE,
	STATE_CLOSED,
	STATE_TABLE_COLUMNS,
	STATE_TABLE_VALUES
} ujoDocState;

/* UJO writer state structure */
typedef struct {
	ujoDocState state;
	struct {
		uint32_t columns;
		uint32_t column;
	} table;
} ujo_state;

/**
 * @brief State changing document ewvents.
 */
typedef enum { 
	ATOMIC_FOUND,
	CONTAINER_CLOSED,
	STRING_FOUND
} ujoDocEvent;

ujoBool ujo_state_allow_atomic(ujoDocState s);
ujoBool ujo_state_allow_container(ujoDocState s);
ujoBool ujo_state_allow_string(ujoDocState s);


ujo_state* ujo_state_next(ujoDocState s, ujo_state* c, ujoStack *stack);
ujo_state* ujo_state_prev(ujo_state* o, ujoStack *stack);

ujo_state* ujo_state_switch(ujoDocEvent e, ujo_state* s, ujoStack *stack);

#endif
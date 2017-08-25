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

#include "ujo_state.h"
#include "ujo_macros.h"

ujoBool ujo_state_allow_atomic(ujoDocState s)
{
	return (ujoBool)(s == STATE_DICT_KEY ||
		    s == STATE_DICT_VALUE ||
			s == STATE_LIST ||
			s == STATE_TABLE_VALUES);
};

ujoBool ujo_state_allow_container(ujoDocState s)
{
	return (ujoBool)(s == STATE_ROOT ||
		    s == STATE_DICT_VALUE ||
			s == STATE_LIST ||
			s == STATE_TABLE_VALUES);
};

ujoBool ujo_state_allow_string(ujoDocState s)
{
	return (ujoBool)(ujo_state_allow_atomic(s) ||
		    s == STATE_TABLE_COLUMNS);
};

ujo_state* ujo_state_next(ujoDocState s, ujo_state* c, ujoStack *stack)
{
	ujo_state *pstate = ujo_new(ujo_state, 1);
	pstate->state = s;

	ujo_stack_push(stack, c);
	return pstate;
};

ujo_state* ujo_state_prev(ujo_state* o, ujoStack *stack)
{
	ujo_state* pstate = (ujo_state*)ujo_stack_pop(stack);

	ujo_free(o);
	
	/* if we are in root again, the document is closed */
	if (pstate->state == STATE_ROOT) {
		pstate->state = STATE_CLOSED;
	}
	return pstate;
};

ujo_state* ujo_state_switch(ujoDocEvent e, ujo_state* s, ujoStack *stack) 
{
	switch (e) {
	  case ATOMIC_FOUND: 
		switch (s->state) {
                  case STATE_DICT_VALUE: 
					   return ujo_state_prev(s, stack);
                       break;
                  case STATE_DICT_KEY: 
					   return ujo_state_next(STATE_DICT_VALUE, s, stack); 
                       break;
				  case STATE_TABLE_VALUES:
					   s->table.column += 1;
					   if (s->table.column >= s->table.columns)
					     s->table.column = 0;
					   break;
				  default:
					   break;
		}; 
        break;
	  case STRING_FOUND: 
		switch (s->state) {
                  case STATE_DICT_VALUE: 
					   return ujo_state_prev(s, stack);
                       break;
                  case STATE_DICT_KEY: 
					   return ujo_state_next(STATE_DICT_VALUE, s, stack); 
                       break;
				  case STATE_TABLE_COLUMNS:
					   s->table.columns += 1;
					   break;
				  case STATE_TABLE_VALUES:
					   s->table.column += 1;
					   if (s->table.column >= s->table.columns)
					     s->table.column = 0;
					   break;
				  default:
					   break;
		}; 
        break;
	  case CONTAINER_CLOSED: 
		       switch (s->state) {
			   case STATE_DICT_VALUE: 
				   return ujo_state_prev(s, stack); 
				   break;
			   case STATE_TABLE_VALUES:
				   s->table.column += 1;
				   if (s->table.column >= s->table.columns)
				     s->table.column = 0;
				   break;
			   default:
				   break;
			   };
               break;
	}
	return s;
};

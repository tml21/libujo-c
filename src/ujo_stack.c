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

#include "ujo_stack.h"
#include "ujo_errors.h"
#include "ujo_log.h"
#include "ujo_macros.h"

/** 
@cond INTERNAL_DOCS
*/

struct _ujoStack {
	ujoPointer     *stack;
	int            size;
	int            items;
	ujoDestroyFunc destroy;
};

ujoStack* ujo_new_stack(ujoDestroyFunc destroy_data)
{
	ujoStack *stack;

	stack = ujo_new(ujoStack, 1);
	if (stack == NULL)
		return NULL;
	stack->destroy = destroy_data;

	return stack;
};

ujoBool ujo_stack_is_empty(ujoStack* stack) 
{
	return_val_if_fail(stack,"invalid pointer",ujoFalse);

	return (ujoBool)(stack->items == 0);
}

void ujo_clear_stack(ujoStack* stack)
{
	return_if_fail(stack,"invalid pointer");
	if (stack->destroy) {
		while(! ujo_stack_is_empty(stack)) {
			/* destroy data */
			stack->destroy(ujo_stack_pop(stack));
		} /* end while */
	} /* end if */
};

void ujo_free_stack(ujoStack* stack)
{
	return_if_fail(stack,"invalid pointer");

	ujo_clear_stack(stack);

	/* free stack array */
	if (stack->stack) ujo_free(stack->stack);

	/* free stack */
	ujo_free(stack);

	return;
};

void ujo_stack_push(ujoStack* stack, ujoPointer data)
{
	ujoPointer *temp;

	return_if_fail(stack,"invalid stack pointer");
	return_if_fail(stack,"invalid data pointer");

	if (stack->size == stack->items) {
		if (!stack->stack) {
			stack->stack = ujo_new(ujoPointer, 1);
			return_if_fail(stack->stack, "stack not allocated");
		} else {
			temp = stack->stack;
			stack->stack = (ujoPointer*)realloc(stack->stack, sizeof(ujoPointer)*(stack->size +1));
			if (stack->stack == NULL) {
				stack->stack = temp;
				return_if_fail(ujoFalse, "resize stack failed");
			} /* end if */
		} /* end if */
		stack->size++;
	}

	stack->stack[stack->items] = data;

	stack->items++;

	return;
};

ujoPointer ujo_stack_pop(ujoStack* stack)
{
	ujoPointer pointer;

	return_val_if_fail(stack, "invalid stack", NULL);
	
	if (ujo_stack_is_empty(stack)) {
		return NULL;
	}

	stack->items--;
	pointer = stack->stack[stack->items];

	return pointer;
};

/** 
@endcond
*/

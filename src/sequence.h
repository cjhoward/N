/*
 * Copyright (C) 2020  Christopher J. Howard
 *
 * This file is part of nterpreter.
 *
 * nterpreter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * nterpreter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with nterpreter.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <stddef.h>
#include "bignum.h"

/// Sequence element type, implemented as a circular doubly linked list node.
typedef struct element_t
{
	/// Pointer to the previous element in the sequence.
	struct element_t* previous;
	
	/// Pointer to the next element in the sequence.
	struct element_t* next;
	
	/// Value of this element.
	bignum_t value;
	
} element_t;

/// Returns the total number of elements in a sequence.
size_t count_elements(element_t* head);

/// Appends an element to a sequence
element_t* append_sequence(element_t* head, bignum_t value);

/// Erases the last element of a sequence, if not a singleton.
size_t truncate_sequence(element_t* head);

/// Deallocates all elements in a sequence.
void free_sequence(element_t* head);

#endif // SEQUENCE_H

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

/// Sequence element type, implemented as a doubly linked list node.
typedef struct element_t
{
	/// Pointer to the previous element in the sequence.
	struct element_t* previous;
	
	/// Pointer to the next element in the sequence.
	struct element_t* next;
	
	/// Value of this element.
	bignum_t value;
	
} element_t;

/// Returns a pointer to the first element in a sequence.
element_t* find_first_element(element_t* element);

/// Returns a pointer to the last element in a sequence.
element_t* find_last_element(element_t* element);

/// Returns the total number of elements in a sequence.
size_t count_elements(element_t* element);

/// Returns the index of an element in the sequence.
size_t find_element_index(element_t* element);

/// Inserts an element into a sequence, following the given element.
element_t* append_element(element_t* element);

/// Inserts an element into a sequence, preceding the given element.
element_t* prepend_element(element_t* element);

/// Erases all elements preceding the given element.
void erase_preceding_elements(element_t* element);

/// Erases all elements following the given element.
void erase_succeeding_elements(element_t* element);

/// Erases all elements in a sequence.
void free_sequence(element_t* element);

/// Returns the number of matching elements of two sequences.
size_t compare_sequences(element_t* a, element_t* b);

#endif // SEQUENCE_H

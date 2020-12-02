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

#include "sequence.h"
#include <stdlib.h>

size_t count_elements(element_t* head)
{
	if (!head)
		return 0;
	
	size_t count = 1;
	for (element_t* element = head->next; element != head; element = element->next)
		++count;
	
	return count;
}

element_t* append_sequence(element_t* head, bignum_t value)
{
	element_t* tail = malloc(sizeof(element_t));
	tail->value = value;
	
	if (head)
	{
		head->previous->next = tail;
		tail->previous = head->previous;
		tail->next = head;
		head->previous = tail;
	}
	else
	{
		tail->next = tail;
		tail->previous = tail;
	}
	
	return tail;
}

size_t truncate_sequence(element_t* head)
{
	if (head->previous == head)
		return 0;
	
	element_t* tail = head->previous;
	tail->previous->next = head;
	head->previous = tail->previous;
	free(tail);

	return 1;
}

void free_sequence(element_t* head)
{
	if (!head)
		return;
	
	element_t* tail = head->previous;
	element_t* previous;
	
	while (tail != head)
	{
		previous = tail->previous;
		free(tail);
		tail = previous;
	}
	free(head);
}

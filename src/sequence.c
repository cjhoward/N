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

element_t* find_first_element(element_t* element)
{
	if (element)
		for (; element->previous; element = element->previous);
	return element;
}

element_t* find_last_element(element_t* element)
{
	if (element)
		for (; element->next; element = element->next);
	return element;
}

size_t count_elements(element_t* element)
{
	size_t count = 0;
	
	if (element)
	{
		element = find_first_element(element);
		for (count = 1; element->next; element = element->next)
			++count;
	}
	
	return count;
}

size_t find_element_index(element_t* element)
{
	size_t index = 0;
	
	if (element)
	{
		for (element = element->previous; element; element = element->previous)
			++index;
	}
	
	return index;
}

element_t* append_element(element_t* element)
{
	element_t* new_element = calloc(1, sizeof(element_t));
	
	if (element)
	{
		if (element->next)
		{	
			element->next->previous = new_element;
			new_element->next = element->next;
		}
		
		element->next = new_element;
		new_element->previous = element;
	}
	
	return new_element;
}

element_t* prepend_element(element_t* element)
{
	element_t* new_element = calloc(1, sizeof(element_t));
	
	if (element)
	{
		if (element->previous)
		{	
			element->previous->next = new_element;
			new_element->previous = element->previous;
		}
		
		element->previous = new_element;
		new_element->next = element;
	}
	
	return new_element;
}

void erase_preceding_elements(element_t* element)
{
	if (element)
	{
		element_t* previous = element->previous;
		element->previous = 0;
		
		while (previous)
		{
			element = previous->previous;
			free(previous);
			previous = element;
		}
	}
}

void erase_succeeding_elements(element_t* element)
{
	if (element)
	{
		element_t* next = element->next;
		element->next = 0;
		
		while (next)
		{
			element = next->next;
			free(next);
			next = element;
		}
	}
}

void free_sequence(element_t* element)
{
	element_t* next;
	element = find_first_element(element);
	
	while (element)
	{
		next = element->next;
		free(element);
		element = next;
	}
}

size_t compare_sequences(element_t* a, element_t* b)
{
	size_t count = 0;
	if (a && b)
	{
		while (a->value == b->value)
		{
			++count;
			if (a->next && a->next == b->next)
			{
				a = a->next;
				b = b->next;
			}
		}
	}
	return count;
}

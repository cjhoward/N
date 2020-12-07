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

#include "interpret.h"
#include <stdlib.h>

void n_interpret(const char* source, element_t** sequence)
{
	// No code or sequence pointer provided, abort
	if (!source || !*source || !sequence)
		return;
	
	// Get a pointer to the first element in the sequence
	element_t* head = *sequence;
	
	// If input sequence is empty, create a zero singleton
	if (!head)
		head = append_sequence(0, 0);
	
	// Count number of elements in the sequence;
	size_t element_count = count_elements(head);
	
	// Count number of instructions
	size_t instruction_count = strlen(source);
	
	// Count max loop depth
	size_t loop_depth = 0;
	size_t max_loop_depth = 0;
	for (const char* c = source; *c; ++c)
	{
		if (*c == '[')
			max_loop_depth += (++loop_depth > max_loop_depth);
		else if (*c == ']')
			loop_depth -= !!loop_depth;
	}
	
	// Allocate loop stacks
	loop_depth = 0;
	bignum_t* loop_counters = calloc(max_loop_depth + 1, sizeof(bignum_t));
	size_t* loop_headers = malloc((max_loop_depth + 1) * (sizeof(size_t)));
	size_t skipped_loops = 0;
	
	for (size_t ip = 0; ip < instruction_count; ++ip)
	{
		char i = source[ip];
		
		if (!skipped_loops)
		{
			switch (i)
			{
				case '+':
					++head->value;
					break;
				
				case '-':
					head->value -= !!head->value;
					break;
				
				case '>':
					head = head->previous;
					break;
				
				case '<':
					head = head->next;
					break;
				
				case '[':
					if (head->value)
					{
						++loop_depth;
						loop_counters[loop_depth] = head->value;
						loop_headers[loop_depth] = ip;
					}
					else
					{
						++skipped_loops;
					}
					break;
				
				case ']':
					if (loop_depth)
					{
						if (--loop_counters[loop_depth])
							ip = loop_headers[loop_depth];
						else
							--loop_depth;
					}
					break;
				
				case ':':
					append_sequence(head, head->value);
					++element_count;
					break;
				
				case '|':
					element_count -= truncate_sequence(head);
					break;
				
				case '#':
					head->value = element_count;
					break;
			}
		}
		else
		{
			if (i == '[')
				++skipped_loops;
			else if (i == ']')
				--skipped_loops;
		}
	}
	
	// Free loop stacks
	free(loop_headers);
	free(loop_counters);
	
	// Redirect sequence pointer to first element
	*sequence = head;
}

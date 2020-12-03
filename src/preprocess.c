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

#include "preprocess.h"
#include <stdlib.h>

void n_preprocess(char** source)
{
	char* operators, *c;
	size_t length = 0;
	
	// Remove all comments and non-operator characters from source
	for (c = *source; *c != '\0'; ++c)
	{
		switch (*c)
		{
			case '+':
			case '-':
			case '>':
			case '<':
			case '[':
			case ']':
			case ':':
			case '|':
			case '#':
				++length;
				break;
			
			case ';':
				do *c++ = ' ';
				while (*c && *c != '\n');
				--c;
				break;
			
			default:
				*c = ' ';
				break;
		}
	}
	
	// Copy operators into new buffer
	operators = malloc(length + 1);
	operators[length] = '\0';
	for (c = *source; *c != '\0'; ++c)
	{
		switch (*c)
		{
			case ' ':
				break;
			
			default:
				*(operators++) = *c;
				break;
		}
	}
	operators -= length;
	
	// Free source buffer
	free(*source);
	
	// Set source buffer to operator buffer
	*source = operators;
}

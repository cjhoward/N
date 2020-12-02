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

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sequence.h"

#define ERROR_ARGC 1
#define ERROR_FOPEN 2
#define ERROR_FREAD 3

/// Preprocesses an (N) program source, removing comments and non-operators.
void preprocess(char** source);

/// Interprets an (N) program and returns the resulting sequence.
element_t* interpret(const char* source, element_t* head);

/// Writes a sequence to a file stream in text mode, with space-delimeted textual elements.
void write_sequence_text(FILE* file, element_t* head);

/// Writes a sequence to a file stream in binary mode, with element values translated to binary.
void write_sequence_binary(FILE* file, element_t* head);

/// Prints the usage string.
void usage();

int main(int argc, char* argv[])
{
	int write_binary = 0;
	FILE* output_file = stdout;
	int output_argc = 0;
	
	if (argc < 2)
	{
		usage();
		return ERROR_ARGC;
	}
	
	// Open source file
	FILE* source_file = fopen(argv[1], "rb");
	if (!source_file)
	{
		printf("Failed to open source file \"%s\"\n", argv[1]);
		return ERROR_FOPEN;
	}
	
	// Allocate source buffer
	fseek(source_file, 0, SEEK_END);
	size_t source_size = ftell(source_file);
	rewind(source_file);
	char* source = malloc(source_size + 1);
	source[source_size] = '\0';
	
	// Read source file into source buffer then close source file
	size_t read_bytes = fread(source, 1, source_size, source_file);
	fclose(source_file);
	
	if (read_bytes != source_size)
	{
		printf("Failed to read source file \"%s\"\n", argv[1]);
		free(source);
		return ERROR_FREAD;
	}
	
	// Read options
	for (int i = 2; i < argc; ++i)
	{
		if (!strcmp(argv[i], "-b") || !strcmp(argv[i], "--binary"))
			write_binary = 1;
		else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output"))
		{
			if (++i < argc)
			{
				// Flag output file argc, so it won't be interpreted as a sequence element
				output_argc = i;
				
				// Open output file
				output_file = fopen(argv[i], "wb");
				if (!output_file)
				{
					printf("Failed to open output file \"%s\"\n", argv[i]);
					free(source);
					return ERROR_FOPEN;
				}
			}
		}
	}
	
	// Read sequence elements from argv
	element_t* tail = 0;
	for (int i = 2; i < argc; ++i)
	{
		if (i == output_argc)
			continue;
		
		bignum_t value = 0;
		if (sscanf(argv[i], "%" SCNu64, &value) == 1)
		{
			tail = append_sequence(tail, value);
		}
	}
	
	// Find head of sequence
	element_t* head = 0;
	if (!tail)
		head = append_sequence(0, 0);
	else
		head = tail->next;
	
	// Preprocess source code
	preprocess(&source);
	
	// Interpret program
	head = interpret(source, head);
	
	// Write sequence to file stream
	if (write_binary)
		write_sequence_binary(output_file, head);
	else
		write_sequence_text(output_file, head);
	
	// Close output file
	if (output_file != stdout)
		fclose(output_file);
	
	// Free source buffer
	free(source);
	
	// Free sequence
	free_sequence(head);
	
	return EXIT_SUCCESS;
}

void preprocess(char** source)
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
					do
						*c++ = ' ';
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

element_t* interpret(const char* source, element_t* head)
{
	if (!*source)
		return head;
	
	if (!head)
		head = append_sequence(0, 0);
	
	size_t loop_depth = 0, max_loop_depth, skipped_loops = 0;
	
	// Count number of instructions
	size_t instruction_count = strlen(source);
	
	// Count max loop depth
	loop_depth = 0;
	max_loop_depth = 0;
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
	size_t cardinality = count_elements(head);
	
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
					++cardinality;
					break;
				
				case '|':
					cardinality -= truncate_sequence(head);
					break;
				
				case '#':
					head->value = cardinality;
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
	
	return head;
}

void write_sequence_text(FILE* file, element_t* head)
{
	if (!head)
		return;
	
	element_t* element = head;
	do
	{
		fprintf(file, "%" PRIu64, element->value);
		
		element = element->next;
		if (element != head)
			fprintf(file, " ");	
	}
	while (element != head);
}

void write_sequence_binary(FILE* file, element_t* head)
{
	if (!head)
		return;
	
	element_t* element = head;
	do
	{
		if (element->value <= UINT8_MAX)
			fwrite(&element->value, sizeof(uint8_t), 1, file);
		else if (element->value <= UINT16_MAX)
			fwrite(&element->value, sizeof(uint16_t), 1, file);
		else if (element->value <= UINT32_MAX)
			fwrite(&element->value, sizeof(uint32_t), 1, file);
		else
			fwrite(&element->value, sizeof(uint64_t), 1, file);
		
		element = element->next;
	}
	while (element != head);
}

void usage()
{
	printf("Usage: n <source file> [-o output file] [options] [first element] ... [nth element]\n");
}

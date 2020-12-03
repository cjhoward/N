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
#include "preprocess.h"
#include "sequence.h"

#define ERROR_ARGC 1
#define ERROR_FOPEN 2
#define ERROR_FREAD 3

#define MODE_NUMBERS 0
#define MODE_BYTES 1

/// Interprets an (N) program and returns the resulting sequence.
element_t* interpret(const char* source, element_t* head);

/// Writes a sequence to a file stream in text mode, with space-delimeted numbers.
void write_sequence_numbers(FILE* file, element_t* head);

/// Writes a sequence to a file stream in binary mode, with element values translated to bytes.
void write_sequence_bytes(FILE* file, element_t* head);

/// Prints the usage string.
void usage();

int main(int argc, char* argv[])
{
	int input_mode = MODE_NUMBERS;
	int output_mode = MODE_NUMBERS;
	int first_element_arg = -1;
	
	FILE* output_file = stdout;
	
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
		if (!strcmp(argv[i], "-ob") || !strcmp(argv[i], "--output-bytes"))
			output_mode = MODE_BYTES;
		else if (!strcmp(argv[i], "-on") || !strcmp(argv[i], "--output-numbers"))
			output_mode = MODE_NUMBERS;
		else if (!strcmp(argv[i], "-ib") || !strcmp(argv[i], "--input-bytes"))
			input_mode = MODE_BYTES;
		else if (!strcmp(argv[i], "-in") || !strcmp(argv[i], "--input-numbers"))
			input_mode = MODE_NUMBERS;
		else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output"))
		{
			if (++i < argc)
			{
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
		else if (first_element_arg < 0)
		{
			first_element_arg = i;
		}
	}
	
	// Read sequence elements from argv
	element_t* head = 0;
	if (first_element_arg > 0)
	{
		for (int i = argc - 1; i >= first_element_arg; --i)
		{
			if (input_mode == MODE_NUMBERS)
			{
				bignum_t value = 0;
				if (sscanf(argv[i], "%" SCNu64, &value) == 1)
					head = append_sequence(head, value);
			}
			else
			{
				for (int j = strlen(argv[i]) - 1; j >= 0; --j)
					head = append_sequence(head, (bignum_t)argv[i][j]);
			}
		}
	}
	
	// Create zero singleton if no initial sequence was given
	if (!head)
		head = append_sequence(0, 0);
	
	// Preprocess source code
	n_preprocess(&source);
	
	// Interpret program
	head = interpret(source, head);
	
	// Write sequence to file stream
	if (output_mode == MODE_BYTES)
		write_sequence_bytes(output_file, head);
	else
		write_sequence_numbers(output_file, head);
	
	// Close output file
	if (output_file != stdout)
		fclose(output_file);
	
	// Free source buffer
	free(source);
	
	// Free sequence
	free_sequence(head);
	
	return EXIT_SUCCESS;
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

void write_sequence_numbers(FILE* file, element_t* head)
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

void write_sequence_bytes(FILE* file, element_t* head)
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
	printf("Usage: n <source file> [options] [first element] ... [nth element]\n");
}

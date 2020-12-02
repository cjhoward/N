/*
 * Copyright (C) 2020  Christopher J. Howard
 *
 * This file is part of bin2n.
 *
 * bin2n is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * bin2n is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bin2n.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bin2n.h"

#define ERROR_ARGC 1
#define ERROR_INPUT 2
#define ERROR_OUTPUT 3
#define ERROR_FWRITE 4

int main(int argc, char* argv[])
{
	FILE* input, *output;
	unsigned char index;
	int write_success = 1;
	size_t input_size;
	const char op_cons = ':';
	const char op_lshift = '<';
	
	if (argc != 2 && argc != 3)
	{
		printf("Usage: bin2n <input file> [output file]\n");
		return ERROR_ARGC;
	}
	
	// Open input file
	input = fopen(argv[1], "rb");
	if (!input)
	{
		printf("Failed to open input file \"%s\"\n", argv[1]);
		return ERROR_INPUT;
	}
	
	// Determine size of input file
	fseek(input, 0, SEEK_END);
	input_size = ftell(input);
	rewind(input);
	
	// Open output file or stdout
	output = (argc == 3) ? fopen(argv[2], "wb") : stdout;
	if (!output)
	{
		printf("Failed to open output file \"%s\"", argv[2]);
		return ERROR_OUTPUT;
	}
	
	// Write clear sequence to output
	write_success = (fwrite(bin2n_clear_sequence, 1, strlen(bin2n_clear_sequence), output) == strlen(bin2n_clear_sequence));
	
	// Write allocation operations
	for (size_t i = 1; (i < input_size) && write_success; ++i)
		write_success = (fwrite(&op_cons, 1, 1, output) == 1);
	
	// Translate input bytes to operations, then write to output
	while (write_success && fread(&index, 1, 1, input))
	{
		write_success = (fwrite(bin2n_operations[index], 1, bin2n_lengths[index], output) == bin2n_lengths[index]);
		if (write_success)
			write_success = (fwrite(&op_lshift, 1, 1, output) == 1);
	}
	
	// Close input and output file streams
	fclose(input);
	if (argc == 3)
		fclose(output);
	
	if (!write_success)
	{
		printf("Failed to write data to output\n");
		return ERROR_FWRITE;
	}
	
	return EXIT_SUCCESS;
}

/*
 * Copyright (C) 2020  Christopher J. Howard
 *
 * This file is part of n2c.
 *
 * n2c is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * n2c is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with n2c.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preprocess.h"

const char* bootstrap_header = "#include <inttypes.h>\n#include <stdlib.h>\n#in"
"clude <stdio.h>\ntypedef uint64_t n;typedef struct e{struct e*l,*r;n v;}e;e*a("
"e*h,n v){e*t=malloc(sizeof(e));t->v=v;if(h){t->l=h->l;t->r=h;h->l->r=t;h->l=t;"
"}else t->l=t->r=t;return t;}void x(e*h){if(h->l!=h){e*t=h->l;t->l->r=h;h->l=t-"
">l;free(t);}}void main(int c,char**s){e* h=0,*o;n v;for(;--c;)if(sscanf(s[c],"
"\"%llu\",&v))h=a(h,v);if(!h)h=a(h,0);c=1;";

const char* bootstrap_footer = "o=h;do{printf(\"%llu\",o->v);o=o->r;if(o!=h)put"
"char(32);}while(o!=h);}";

const char* op_inc = "++h->v;";
const char* op_dec = "--h->v;";
const char* op_car = "h->v=c;";
const char* op_rsh = "h=h->l;";
const char* op_lsh = "h=h->r;";
const char* op_app = "a(h,h->v);";
const char* op_trn = "x(h);";
const char* op_lst = "for(int i=h->v;i;--i){";
const char* op_lsp = "}";

// "h->v+=0xFFFFFFFFFFFFFFFFULL;"
const size_t max_opcode_length = 28;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Usage: n2c <source file>");
		return 1;
	}
	
	// Open (N) source file
	FILE* n_source_file = fopen(argv[1], "rb");
	if (!n_source_file)
	{
		printf("Failed to open (N) source file \"%s\"\n", argv[1]);
		return 1;
	}
	
	// Read (N) source into buffer
	fseek(n_source_file, 0, SEEK_END);
	size_t n_source_length = ftell(n_source_file);
	rewind(n_source_file);
	char* n_source = malloc(n_source_length + 1);
	n_source[n_source_length] = '\0';
	if (fread(n_source, 1, n_source_length, n_source_file) != n_source_length)
	{
		printf("Failed to read (N) source file \"%s\"\n", argv[1]);
		return 1;
	}
	fclose(n_source_file);
	
	// Preprocess (N) source
	n_preprocess(&n_source);
	n_source_length = strlen(n_source);
	
	// Allocate C source buffer
	size_t bootstrap_length = strlen(bootstrap_header) + strlen(bootstrap_footer);
	char* c_source = malloc(bootstrap_length + n_source_length * max_opcode_length + 1);
	*c_source = '\0';
	
	// Copy bootstrap header into C source buffer
	strcat(c_source, bootstrap_header);
	
	// Translate opcodes from (N) to C
	for (char* c = n_source; *c; ++c)
	{
		switch (*c)
		{
			case '+':
				strcat(c_source, op_inc);
				break;
			
			case '-':
				strcat(c_source, op_dec);
				break;
			
			case '#':
				strcat(c_source, op_car);
				break;
			
			case '>':
				strcat(c_source, op_rsh);
				break;
			
			case '<':
				strcat(c_source, op_lsh);
				break;
			
			case ':':
				strcat(c_source, op_app);
				break;
			
			case '|':
				strcat(c_source, op_trn);
				break;
				
			case '[':
				strcat(c_source, op_lst);
				break;
			
			case ']':
				strcat(c_source, op_lsp);
				break;
		}
	}
	
	// Copy bootstrap footer into C source buffer
	strcat(c_source, bootstrap_footer);
	
	// Print C source to stdout
	fputs(c_source, stdout);
	
	// Deallocate buffers
	free(n_source);
	free(c_source);
	
	return EXIT_SUCCESS;
}

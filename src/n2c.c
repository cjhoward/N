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
#include "bignum.h"
#include "preprocess.h"

const char* bootstrap_header = "#include <inttypes.h>\n#include <stdlib.h>\n#in"
"clude <stdio.h>\ntypedef uint64_t n;typedef struct e{struct e*l,*r;n v;}e;e*a("
"e*h,n v){e*t=malloc(sizeof(e));t->v=v;if(h){t->l=h->l;t->r=h;h->l->r=t;h->l=t;"
"}else t->l=t->r=t;return t;}int x(e*h){if(h->l!=h){e*t=h->l;t->l->r=h;h->l=t->"
"l;free(t);return 1;}return 0;}int main(int m,char**s){e* h=0,*o;n v,c=0;for(;-"
"-m;)if(sscanf(s[m],\"%llu\",&v)){h=a(h,v);++c;}if(!h){h=a(h,0);c=1;}";

const char* bootstrap_footer = "o=h;do{printf(\"%llu\",o->v);o=o->r;if(o!=h)put"
"char(32);}while(o!=h);return 0;}";

const char* op_inc = "++h->v;";
const char* op_dec = "h->v-=!!h->v;";
const char* op_car = "h->v=c;";
const char* op_rsh = "h=h->l;";
const char* op_lsh = "h=h->r;";
const char* op_app = "a(h,h->v);";
const char* op_trn = "c-=x(h);";
const char* op_lst = "for(int i=h->v;i;--i){";
const char* op_lsp = "}";
const char* op_add = "h->v+=0x%llXULL;";
const char* op_sub = "h->v=(h->v>0x%llXULL)?h->v-0x%llXULL:0;";

// Max length opcode: "h->v=(h->v>0xFFFFFFFFFFFFFFFFULL)?h->v-0xFFFFFFFFFFFFFFFFULL:0;"
const size_t max_opcode_length = 64;

int main(int argc, char* argv[])
{
	if (argc != 2 && argc != 3)
	{
		printf("Usage: n2c <input file> [output file]");
		return 1;
	}
	
	// Open (N) source file
	FILE* n_source_file = fopen(argv[1], "rb");
	if (!n_source_file)
	{
		printf("Failed to open input file \"%s\"\n", argv[1]);
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
		printf("Failed to read input file \"%s\"\n", argv[1]);
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
	bignum_t consecutive_incs = 0;
	bignum_t consecutive_decs = 0;
	for (char* c = n_source; *c; ++c)
	{
		switch (*c)
		{
			case '+':
				++consecutive_incs;
				break;
			
			case '-':
				++consecutive_decs;
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
		
		if (consecutive_incs && *(c + 1) != '+')
		{
			if (consecutive_incs == 1)
				strcat(c_source, op_inc);
			else
				sprintf(c_source + strlen(c_source), op_add, consecutive_incs);
			consecutive_incs = 0;
		}
		else if (consecutive_decs && *(c + 1) != '-')
		{
			if (consecutive_decs == 1)
				strcat(c_source, op_dec);
			else
				sprintf(c_source + strlen(c_source), op_sub, consecutive_decs, consecutive_decs);
			consecutive_decs = 0;
		}
	}
	
	// Copy bootstrap footer into C source buffer
	strcat(c_source, bootstrap_footer);
	
	// Open output file, if any
	FILE* c_source_file = stdout;
	if (argc == 3)
		c_source_file = fopen(argv[2], "wb");
	
	// Print C source to output file
	if (c_source_file)
		if (fputs(c_source, c_source_file) < 0)
			if (argc == 3)
				printf("Failed to write to output file \"%s\"\n", argv[2]);
	
	// Close output file, if any
	if (c_source_file != stdout)
		fclose(c_source_file);
	
	// Deallocate buffers
	free(n_source);
	free(c_source);
	
	return EXIT_SUCCESS;
}

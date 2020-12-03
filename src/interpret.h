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

#ifndef N_INTERPRET_H
#define N_INTERPRET_H

#include "sequence.h"

/**
 * Interprets an (N) program, transforming the input sequence.
 *
 * @param sequence Reference to the pointer to the first element in the input sequence.
 */
void n_interpret(const char* source, element_t** sequence);

#endif // N_INTERPRET_H

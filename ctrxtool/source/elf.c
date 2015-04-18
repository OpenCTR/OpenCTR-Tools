/*
 * elf.c - ELF functions
 * 
 * Copyright 2015, Alex Shaw <alex.shaw.as@gmail.com>
 * 
 * This file is part of OpenCTR.
 * 
 * OpenCTR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OpenCTR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with OpenCTR.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_LIBELF_H
#include "libelf.h"
#endif

#ifdef HAVE_GELF_H
#include "gelf.h"
#endif

#include "endian.h"
#include "utils.h"
#include "elf.h"

struct _ELF {
	Elf32_Ehdr* ehdr;
	Elf32_Shdr* sections;
	unsigned int sections_size;
	const char* sections_names;
};

